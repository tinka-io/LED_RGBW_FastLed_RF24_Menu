#ifndef RF24_reciver_H
#define RF24_reciver_H

#include <SPI.h>
#include "RF24.h"

// RF24 Pins    CE, CSN
// nano          9, 10
// mega         49, 48
// teensy 3.2   15, 16

#define CEpin 9
#define CSNpin 10
#define debug_level 0

class RF24reciver
{
public:
    void setup();
    float get_timestamp();

private:
    RF24 radio = RF24(CEpin, CSNpin); 
    float timestamp = 0.0;
};

#endif