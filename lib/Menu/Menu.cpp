#include "Menu.h"
#include "Stripe.h"

#include <EEPROM.h>

// Dispaly
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)
#define DISPLAY_PWR_PIN 4

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include <Encoder.h>
Encoder encoder(DT_PIN, CLK_PIN);

int Setting::get_value()
{
    return value;
}

void Setting::set_value(int _diff)
{
    int new_value = value + _diff;
    value = constrain(new_value, min, max);
}

void Setting::reset()
{
    value = start_value;
}

void Menu::setup(String welcome_message)
{
    Serial.println("Setup Menu");

    setup_display(welcome_message);
    setup_encoder();
}

void Menu::loop()
{
    if (get_Encoder_Rotation() == 0 || get_Encoder_Button() == 0)
    {
        draw();
    }
}

void Menu::setup_display(String welcome_message)
{
    Serial.println("Menu -> Setup Display");

    Wire.setClock(1000000);

    pinMode(DISPLAY_PWR_PIN, OUTPUT);
    digitalWrite(DISPLAY_PWR_PIN, HIGH);

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(front_size);

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(welcome_message);
    display.display();
}

void Menu::setup_encoder()
{
    Serial.println("Menu -> Setup Encoder");

    pinMode(ENCODER_PWR_PIN, OUTPUT);
    digitalWrite(ENCODER_PWR_PIN, HIGH);

    pinMode(SW_PIN, INPUT_PULLUP);
    encoder.write(0);
}

// TODO: move to main or helper function module
int Menu::get_Encoder_Rotation()
{
    // Get Encoder Position
    long Steps = encoder.read();

    // The Encoder have 80 Steps on 20 Clicks
    static int Clicks;
    if (Steps != 0 && Steps % 4 == 0)
    {
        // devide Steps / 4 to get Clicks
        Clicks = Steps / 4;

        // detect Fast rotations and go faster with +-5 Clicks
        static unsigned long lastClick = 0;
        if (lastClick + 40 > millis())
        {
            Clicks = Clicks * 10;
        }
        lastClick = millis();

        // change Value in Menu
        set_value(Clicks);

        // reset Encoder to 0
        encoder.write(0);

        return 0;
    }

    return -1;
}

// TODO: move to main or helper function module
int Menu::get_Encoder_Button()
{
    // read Button with Debonce Time
    static double last = millis();
    if (last + 250 < millis() && !digitalRead(SW_PIN))
    {
        last = millis();

        curser_position++;
        if (curser_position > pages[page_index].used_settings)
        {
            curser_position = 0;
        }

        return 0;
    }
    return -1;
}

void Menu::set_value(int value)
{
    if (curser_position == 0)
    {
        page_index = constrain(value, 0, used_pages-1);
        Serial.println(page_index);
    }
    else
    {
        pages[page_index].settings[curser_position-1].set_value(value);
    }
}

void Menu::draw()
{
    display.clearDisplay();

    display.setCursor(0, 0);
    display.println(pages[page_index].title);

    for (int i = 0; i < pages[page_index].used_settings; i++)
    {
        Setting *s = &pages[page_index].settings[i];
        display.setCursor(s->position.x, 1 * s->position.row * line_height);
        display.println(s->name + ": " + String(s->value));
    }

    display.display();
}
