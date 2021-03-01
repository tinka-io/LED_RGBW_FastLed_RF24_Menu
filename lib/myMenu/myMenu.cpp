#include "myMenu.h"
#include "Stripe.h"

#include <Arduino.h>
#include <EEPROM.h>

// Dispaly
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Encoder Input
#define CLK 3
#define DT 4
#define SW 2
#define PWR 5

// static Variable
mval_type myMenu::mval[max_mval];
int myMenu::pos = 0;
int myMenu::max_pos = max_mval;
bool myMenu::user_input = false;

void myMenu::setup()
{
    Serial.println("Setup myMenu");

    Wire.setClock(1000000);

    // Encoder Input
    pinMode(PWR, OUTPUT);
    digitalWrite(PWR, HIGH);
    pinMode(CLK, INPUT);
    pinMode(DT, INPUT_PULLUP);
    pinMode(SW, INPUT);

    pinMode(LED_BUILTIN, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(CLK), handleEncoder, FALLING); // Envoder
    attachInterrupt(digitalPinToInterrupt(SW), handleButton, FALLING); // Button
    //attachInterrupt(digitalPinToInterrupt(DT), new_Encoder_val, RISING);

    // Display Output
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.setTextColor(SSD1306_WHITE);
    display.clearDisplay();

    //limits first myMenu
    mval[0].min = 0;
    mval[0].max = 1;

    // Start Massage
    display.clearDisplay();
    display.setTextSize(1); // Draw 2X-scale text
    display.setCursor(0, 0);
    display.println("Nora, alles gute zum Geburtstag! Hoffent-\nlich begleite ich dich eine lange weile!");
    display.display();
}

void myMenu::loop()
{
    //get_Button_val();
    //getEncoder();

    if (user_input)
    {
        user_input = false;
        Serial.println("pos: " + String(pos) + " val: " + String(mval[pos].v));
        draw_menus();
    }
}

void myMenu::draw_menus()
{
    display.clearDisplay();

    switch (mval[0].v)
    {
    case 1:
        set_mval_limits_bpm();
        draw_bpm();
        break;
    default: // also 0
        set_mval_limits_static_color();
        draw_static_color();
    }

    display.display();
}

void myMenu::set_mval_limits_static_color()
{
    max_pos = 5;
    for (int i = 1; i < max_mval; i++)
    {
        mval[i].min = 0;
        mval[i].max = 255;
    }
}

void myMenu::draw_static_color()
{
    const int size = 1;
    display.setTextSize(size);

    display.setCursor(0, 0 * size * 8);
    display.println("Static Color");

    display.setCursor(00, 1 * size * 8);
    display.println("R: " + String(mval[1].v));
    display.setCursor(64, 1 * size * 8);
    display.println("G: " + String(mval[2].v));
    display.setCursor(00, 2 * size * 8);
    display.println("B: " + String(mval[3].v));
    display.setCursor(64, 2 * size * 8);
    display.println("W: " + String(mval[4].v));
}

void myMenu::set_mval_limits_bpm()
{
    max_pos = max_mval;

    // BPM
    mval[1].min = 0;
    mval[1].max = 1000;

    // F Color
    mval[2].min = 0;
    mval[2].max = maxColor;

    // B Color
    mval[3].min = 0;
    mval[3].max = maxColor;

    // BlackBeat
    mval[4].min = 0;
    mval[4].max = 16;

    // Strobe
    mval[5].min = 0;
    mval[5].max = 16;

    // Glitter
    mval[6].min = 0;
    mval[6].max = 100;

    // Direction
    mval[7].min = 0;
    mval[7].max = 1;
}

void myMenu::draw_bpm()
{
    const int size = 1;
    display.setTextSize(size);

    display.setCursor(0, 0 * size * 8);
    display.println("Pro: Test");

    display.setCursor(64, 0 * size * 8);
    display.println("BPM: " + String(mval[1].v));

    display.setCursor(00, 1 * size * 8);
    display.println("FCo: " + String(mval[2].v));
    display.setCursor(64, 1 * size * 8);
    display.println("BCo: " + String(mval[3].v));

    display.setCursor(00, 2 * size * 8);
    display.println("BBl: " + String(mval[4].v));
    display.setCursor(64, 2 * size * 8);
    display.println("Str: " + String(mval[5].v));

    display.setCursor(00, 3 * size * 8);
    display.println("Gli: " + String(mval[6].v));
    display.setCursor(64, 3 * size * 8);
    display.println("Dir: " + String(mval[7].v));
}

void myMenu::get_Button_val()
{
    static double last = millis();
    if (last + 250 < millis())
    {
        if (!digitalRead(SW))
        {
            last = millis();
            user_input = true;

            Serial.println("Button Pressed! " + String(pos) + " >= " + String(max_mval));
            if (pos + 1 >= max_pos)
            {
                pos = 0;
            }
            else
            {
                pos++;
            }
        }
    }
}

void myMenu::new_Encoder_val(int step)
{
    // detect fast Rotation
    static double last = 0;
    double time = millis() - last;
    last = millis();
    if (time < 10)
    {
        step = step * 10;
    }

    // calculate Value
    user_input = true;
    int newval = mval[pos].v + step;
    if (newval > mval[pos].max)
    {
        newval = mval[pos].max;
    }
    if (newval < mval[pos].min)
    {
        newval = mval[pos].min;
    }
    mval[pos].v = newval;
}

void myMenu::handleEncoder()
{
    cli();

    bool led = digitalRead(LED_BUILTIN);
    digitalWrite(LED_BUILTIN, !led);

    int val = 0;
    byte reading = PIND & B00011000;

    if (reading == B00011000)
    {
        val = 1;
    }
    else if (reading == B00001000)
    {
        val = -1;
    }

    new_Encoder_val(val);
    //draw_menus();

    user_input = true;
    
    sei();
}

void myMenu::handleButton()
{
    cli();

    //bool led = digitalRead(LED_BUILTIN);
    //digitalWrite(LED_BUILTIN, !led);

    if (pos + 1 >= max_pos)
    {
        pos = 0;
    }
    else
    {
        pos++;
    }

    //draw_menus();
    sei();
}
/*
volatile byte aFlag = 0;
volatile byte bFlag = 0;
volatile byte reading = 0;
void myMenu::PinB()
{
    cli();                //stop interrupts happening before we read pin values
    reading = PIND & 0xC; //read all eight pin values then strip away all but pinA and pinB's values
    if (reading == B00001100 && bFlag)
    { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
        new_Encoder_val(-1);
        bFlag = 0; //reset flags for the next turn
        aFlag = 0; //reset flags for the next turn
    }
    else if (reading == B00001000){
        aFlag = 1; //signal that we're expecting pinA to signal the transition to detent from free rotation
        sei();         //restart interrupts
    }
}
*/
