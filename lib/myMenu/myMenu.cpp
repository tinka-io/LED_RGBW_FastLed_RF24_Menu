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
#define display_pwr 4

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Encoder Input
#define micro

#include <Encoder.h>
#ifdef nano
Encoder encoder(2, 3);
#define SW 4
#define PWR 5
#endif

#ifdef micro
Encoder encoder(1, 0);
#define SW 7
#define encoder_pwr 6
#endif

int Setting::get_value() {
  return value
}

void Setting::set_value(int _diff) {
  int new_value = value + _diff;
  value = constrain(new_value, min, max);
}

void Setting::reset() {
  value = default;
}

void Menu::setup_display() {
  pinMode(display_pwr, OUTPUT);
  digitalWrite(display_pwr, HIGH);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setCursor(0, 0);
  display.println("Laries super Licht\nmade from Tinka Vey");
  display.display();
}

void Menu::draw(Setting settings[];) {
  const int size = 1;
  int line_height = SCREEN_HEIGHT / sizeof(settings);

  display.setCursor(0, 0 * size * 8);
  display.println(title);

  for (i = 0; i < sizeof(settings); i++) {
    display.setCursor(settings[i].position.x, 1 * size * line_height);
    display.println(settings[i].name + ": " + Stringsettings[i].value));
  }

}

void Menu::setup()
{
    Serial.println("Setup myMenu");

    Wire.setClock(1000000);

    pinMode(6, OUTPUT);
    digitalWrite(6, LOW);

    // Encoder Input
    pinMode(encoder_pwr, OUTPUT);
    digitalWrite(encoder_pwr, HIGH);
    pinMode(SW, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);
    encoder.write(0);
}

// TODO: move to main or helper function module
int myMenu::get_Encoder_Clicks()
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
        if (lastClick + 25 > millis())
        {
            Clicks = Clicks * 10;
        }
        lastClick = millis();

        // change Value in Menu
        set_MenuVal_min_max(Clicks);

        // reset Encoder to 0
        encoder.write(0);

        return 0;
    }

    return -1;
}

// TODO: move to main or helper function module
int myMenu::get_Button_val()
{
    static double last = millis();
    // read Button Pin with Debonce Time
    if (last + 250 < millis() && !digitalRead(SW))
    {
        last = millis();

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
