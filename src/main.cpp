#include <Arduino.h>
#include "Stripe.h"
#include "Menu.h"
#include "RF24_reciver.hpp"

Stripe stripe = Stripe();

#define noschlitten
#ifdef schlitten
#define swpin 2
#endif

#define wmenu
#ifdef wmenu
Menu menu;
#endif

#define notimestamp
#ifdef timestamp
RF24reciver rf = RF24reciver();
#endif

void fill_menu()
{
  static const int max_pages = 1;
  static MenuPage pages[max_pages];

  pages[0] = MenuPage("static color", 4);
  for (int i = 0; i < 4; i++)
  {
    Setting *s = &pages[0].settings[i];
    s->min = 0;
    s->max = 255;
    s->value = 0;
  }
  pages[0].settings[0].name = "red";
  pages[0].settings[0].position.row = 1;
  pages[0].settings[0].position.x = 0;

  pages[0].settings[1].name = "green";
  pages[0].settings[1].position.row = 1;
  pages[0].settings[1].position.x = 64;

  pages[0].settings[2].name = "blue";
  pages[0].settings[2].position.row = 2;
  pages[0].settings[2].position.x = 0;

  pages[0].settings[3].name = "white";
  pages[0].settings[3].position.row = 2;
  pages[0].settings[3].position.x = 64;

  menu = Menu(pages, max_pages);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Welcome to Tinka's LED RGBW FastLed RF24 Menu");

  stripe.setup();

#ifdef schlitten
  pinMode(swpin, INPUT_PULLUP);
#endif

#ifdef wmenu
  fill_menu();
  menu.setup("This menu is desiged\nby Luci and Tinka");
#endif

#ifdef timestamp
  rf.setup();
#endif
}

#ifdef wmenu
void loop()
{
  menu.loop();
  MenuPage *p;
  CRGBW color;
  switch (menu.page_index)
  {
  case 0:
    p = (menu.pages+menu.page_index);
    color = CRGBW(p->settings[0].value, 
                        p->settings[1].value,
                        p->settings[2].value, 
                        p->settings[3].value);
    stripe.loop_color(color, 0);
    break;
  case 1:
    p = (menu.pages+menu.page_index);
    stripe.loop_control(p->settings[0].value,
                        p->settings[1].value,
                        p->settings[2].value,
                        p->settings[3].value,
                        p->settings[4].value,
                        p->settings[5].value,
                        p->settings[6].value);
    break;
  }
}
#endif

#ifdef schlitten
void loop()
{
  static int s = 0;
  static unsigned long ptime = 0;
  if (!digitalRead(swpin) && ptime + 500 < millis())
  {
    ptime = millis();
    s++;
  }

  switch (s)
  {
  case 0:
    stripe.loop_color(stripe.dred, 32);
    break;
  case 1:
    stripe.loop_control(27, stripe.cyan, stripe.red, 3, 8, 0, false, true);
    break;
  case 2:
    stripe.loop_control(240, stripe.cyan, stripe.black, 1, 16, 0, false, true);
    break;
  default:
    s = 0;
  }
}
#endif

#ifdef timestamp
void loop()
{
  float o = -1;
  float ts = rf.get_timestamp();
  static float lts = -1;
  if (lts + 1 <= ts || (0 < ts && ts < 1) || ts == -2)
  {
    Serial.println(ts);
    lts = ts;
  }

  if (ts < 24.5)
  {
    stripe.loop_color(stripe.black, 0);
  }
  else if (ts < 30.5)
  {
    stripe.reset_BPM(1);
    int g = map(ts, 24.5, 30.5, 255, 0);
    int b = map(ts, 24.5, 32.5, 100, 0);
    stripe.loop_control(27, CRGBW(0, g, b, 0), stripe.black, 0, 0, 0, false);
  }
  else if (ts < 32)
  {
    stripe.loop_color(stripe.black, 0);
  }
  else if (ts < 42)
  {
    stripe.reset_BPM(2);
    stripe.loop_sinelon(27 * 8, stripe.cyan, 8);
  }
  else if (ts < 55)
  {
    stripe.reset_BPM(3);
    u8 bpm = map(ts, 44, 55, 27 * 8, 27);
    stripe.loop_sinelon(bpm, stripe.green, 8);
  }
  else if (ts < 56)
  {
    stripe.loop_fade_out(1000);
  }
  else if (ts < 94)
  {
    stripe.reset_BPM(4);
    stripe.loop_fade_color(27.27, stripe.mage, true);
  }
  else if (ts < 98)
  {
    stripe.loop_fade_out(1000);
  }
  else if (ts < 100)
  {
    stripe.loop_color(stripe.black, 0);
  }
  else if (ts < 117)
  {
    if (ts < 105)
    {
      u8 r = map(ts, 100, 105, 0, stripe.orang.r);
      u8 g = map(ts, 100, 105, 0, stripe.orang.g);
      stripe.loop_wabern(1500, CRGBW(r, g, 0, 0));
    }
    else
    {
      stripe.loop_wabern(1500, stripe.orang);
    }
  }
  else if (ts < 117.8)
  {
    stripe.loop_fade_out(1000);
  }
  else if (ts < 170.5) // LR
  {
    stripe.reset_BPM(5);
    static bool dir = false;
    static CRGBW c = stripe.blue;
    bool ret = stripe.loop_control(36.9, c, stripe.black, 2, 8, 0, dir);
    if (ret)
    {
      dir = !dir;
      if (dir)
      {
        c = stripe.green;
      }
      else
      {
        c = stripe.blue;
      }
    }
    //stripe.loop_sinelon(36.8, stripe.blue, 8);
    //stripe.loop_fade_color(36.8, c, true);
  }
  else if (ts < 210) // Blitze
  {
    uint8_t chance = 0;
    chance = map(ts, 173000, 21100, 0, 80);
    //stripe.loop_color(255, 0, 0, 0, chance);
    stripe.loop_control(1, stripe.black, stripe.black, 64, 0, chance, false);
  }
  else if (ts < 215.6 + o)
  {
    stripe.loop_fade_out(1000);
  }
  else if (ts < 248)
  {
    stripe.reset_BPM(6);
    stripe.loop_control(27.27, stripe.cyan, stripe.black, 2, 8, 0, false);
  }
  else if (ts < 252.8)
  {
    stripe.loop_color(stripe.black, 0);
  }
  else if (ts < 252.9)
  {
    stripe.loop_color(stripe.red, 0);
  }
  else if (ts < 259)
  {
    stripe.loop_fade_out(500);
  }
  else if (ts < 355)
  {
    float o = -1;
    if (ts < 261.8 + o)
      stripe.loop_control(659, stripe.cyan, stripe.black, 1, 0, 0, false);
    else if (ts < 266.6 + o)
      stripe.loop_control(659, stripe.blue, stripe.black, 1, 0, 0, false);
    else if (ts < 273.3 + o)
      stripe.loop_color(stripe.dred, 0);

    else if (ts < 274.9 + o)
      stripe.loop_control(659, stripe.cyan, stripe.black, 1, 0, 0, false);
    else if (ts < 280.8 + o)
      stripe.loop_control(659, stripe.blue, stripe.black, 1, 0, 0, false);
    else if (ts < 287.6 + o)
      stripe.loop_color(stripe.dred, 0);

    else if (ts < 289 + o)
      stripe.loop_control(659, stripe.cyan, stripe.black, 1, 0, 0, false);
    else if (ts < 295.7 + o)
      stripe.loop_control(659, stripe.blue, stripe.black, 1, 0, 0, false);
    else if (ts < 303.3 + o)
      stripe.loop_color(stripe.dred, 0);

    else if (ts < 304.7 + o)
      stripe.loop_control(659, stripe.cyan, stripe.black, 1, 0, 0, false);
    else if (ts < 305.7 + o)
      stripe.loop_control(659, stripe.blue, stripe.black, 1, 0, 0, false);
    else if (ts < 309.7 + o)
      stripe.loop_color(stripe.dred, 0);

    else if (ts < 311.2 + o)
      stripe.loop_control(659, stripe.cyan, stripe.black, 1, 0, 0, false);
    else if (ts < 320.2 + o)
      stripe.loop_control(659, stripe.blue, stripe.black, 1, 0, 0, false);
    else if (ts < 330 + o)
      stripe.loop_color(stripe.dred, 0);

    else if (ts < 330.7 + o)
      stripe.loop_control(659, stripe.cyan, stripe.black, 1, 0, 0, false);
    else if (ts < 331.7 + o)
      stripe.loop_color(stripe.dred, 0);

    else if (ts < 332.6 + o)
      stripe.loop_control(659, stripe.cyan, stripe.black, 1, 0, 0, false);
    else if (ts < 333.7 + o)
      stripe.loop_color(stripe.dred, 0);

    else if (ts < 335.7 + o)
      stripe.loop_control(659, stripe.cyan, stripe.black, 1, 0, 0, false);
    else if (ts < 336 + o)
      stripe.loop_control(659, stripe.mage, stripe.black, 1, 0, 0, false);
    else
      stripe.loop_color(stripe.dred, 0);
  }
  else if (ts < 364.8)
  {
    stripe.loop_color(stripe.black, 0);
  }
  else if (ts < 396.2 + o)
  {
    static bool setcolor = true;
    if (ts < 375.1 + o)
    {
      if (setcolor)
      {
        setcolor = !setcolor;
        stripe.loop_color(CRGBW(0, 0, 255, 0), 0);
      }
      stripe.loop_fade_out(1500);
    }

    else if (ts < 384.8 + o)
    {
      if (!setcolor)
      {
        setcolor = !setcolor;
        stripe.loop_color(CRGBW(64, 0, 255, 25), 0);
      }
      stripe.loop_fade_out(1500);
    }

    else if (ts < 394.1 + o)
    {
      if (setcolor)
      {
        setcolor = !setcolor;
        stripe.loop_color(CRGBW(128, 0, 255, 50), 0);
      }
      stripe.loop_fade_out(1500);
    }

    else
    {
      if (!setcolor)
      {
        setcolor = !setcolor;
        stripe.loop_color(CRGBW(255, 0, 255, 100), 0);
      }
      stripe.loop_fade_out(1500);
    }
  }
  else if (ts < 404.5 + o)
  {
    float bpm = map(ts, 396.2 + o, 405, 64, 300);
    stripe.reset_BPM(7);
    stripe.loop_sinelon(bpm, stripe.white, 8);
    //stripe.loop_control(bpm, stripe.www, stripe.black, 1, 0, 0, true);
    //stripe.loop_wabern(bpm, stripe.white);
  }
  else
  {
    stripe.loop_color(stripe.black, 0);
  }

  stripe.show();
}
#endif