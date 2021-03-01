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
#include <Encoder.h>
Encoder encoder(2, 3);
#define SW 4
#define PWR 5

void myMenu::setup()
{
    Serial.println("Setup myMenu");

    Wire.setClock(1000000);

    pinMode(6, OUTPUT);
    digitalWrite(6, LOW);

    // Encoder Input
    pinMode(PWR, OUTPUT);
    digitalWrite(PWR, HIGH);
    pinMode(SW, INPUT_PULLUP);

    pinMode(LED_BUILTIN, OUTPUT);

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
    if (!get_Encoder_Clicks() || !get_Button_val())
    {
        draw_menus();
    }
}

int myMenu::set_MenuVal_min_max(int Clicks)
{
    // Check if new Value is between min max
    int newval = mval[pos].v + Clicks;
    if (newval > mval[pos].max)
    {
        newval = mval[pos].max;
    }
    if (newval < mval[pos].min)
    {
        newval = mval[pos].min;
    }

    // set new Value
    mval[pos].v = newval;

    return newval;
}

int myMenu::get_Encoder_Clicks()
{
    // Get Encoder Position
    long Steps = encoder.read();

    // The Encoder have 80 Steps on 20 Clicks
    static int Clicks;
    if (Steps != 0 && Steps % 4 == 0)
    {
        Serial.print(Steps);

        // devide Steps / 4 to get Clicks
        Clicks = Steps / 4;

        // detect Fast rotations and go faster with +-5 Clicks
        static unsigned long lastClick = 0;
        if (lastClick + 50 > millis())
        {
            Clicks = Clicks * 10;
            Serial.print(" *10");
        }
        lastClick = millis();

        Serial.println();

        // change Value in Menu
        set_MenuVal_min_max(Clicks);

        // reset Encoder to 0
        encoder.write(0);

        return 0;
    }

    return -1;
}

int myMenu::get_Button_val()
{
    static double last = millis();
    // read Button Pin with Debonce Time
    if (last + 250 < millis() && !digitalRead(SW))
    {
        last = millis();

        Serial.println("Button Pressed! " + String(pos) + " >= " + String(max_mval));

        // itterate menu Curser Position
        if (pos + 1 >= max_pos)
        {
            pos = 0;
        }
        else
        {
            pos++;
        }

        return 0;
    }

    return -1;
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