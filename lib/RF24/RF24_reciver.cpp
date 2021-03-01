#include "RF24_reciver.hpp"
#include "printf.h"

const uint8_t address[][6] = {"TV_TX", "TV_RX"};

void RF24reciver::setup()
{
    // initialize the transceiver on the SPI bus
    if (!radio.begin())
    {
        Serial.println(F("RF214 radio hardware is not responding!!"));
        while (1)
        {
        } // hold in infinite loop
    }
    Serial.println("RF24 radio hardware is ready");

    radio.setPALevel(RF24_PA_LOW);           // _MIN _LOW _HIGH _MAX
    radio.setPayloadSize(sizeof(timestamp)); // float datatype occupies 4 bytes

    radio.openReadingPipe(1, address[0]); // using pipe 1
    radio.startListening();               // put radio in RX mode

    timestamp = 0;

    // For debugging info
#if debug_level > 1
    printf_begin();             // needed only once for printing details
    radio.printDetails();       // (smaller) function that prints raw register values
    radio.printPrettyDetails(); // (larger) function that prints human readable data
#endif
}

float RF24reciver::get_timestamp()
{
    uint8_t pipe;
    if (radio.available(&pipe))
    {                                           // is there a payload? get the pipe number that recieved it
        uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
        radio.read(&timestamp, bytes);          // fetch payload from FIFO
#if debug_level > 2
        Serial.print(F("Received "));
        Serial.print(bytes); // print the size of the payload
        Serial.print(F(" bytes on pipe "));
        Serial.print(pipe); // print the pipe number
        Serial.print(F(": "));
        Serial.println(timestamp); // print the payload's value
#endif
    }

    return timestamp;
}
