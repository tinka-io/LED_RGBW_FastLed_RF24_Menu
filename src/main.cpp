#include <Arduino.h>
#include "Stripe.h"
#include "myMenu.h"
#include "RF24_reciver.hpp"

Stripe stripe = Stripe();

#define noschlitten
#ifdef schlitten
#define swpin 2
#endif

#define wmenu
#ifdef wmenu
myMenu menu = myMenu();
#endif

#define notimestamp
#ifdef timestamp
RF24reciver rf = RF24reciver();
#endif

void setup()
{
  Serial.begin(115200);
  Serial.println("Welcome to Tinka's LED RGBW FastLed RF24 Menu");

  stripe.setup();

#ifdef schlitten
  pinMode(swpin, INPUT_PULLUP);
#endif

#ifdef wmenu
  menu.setup();
#endif

#ifdef timestamp
  rf.setup();
#endif
}

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
    stripe.loop_color(stripe.dred.val, 32);
    break;
  case 1:
    stripe.loop_control(27, stripe.cyan.val, stripe.red.val, 3, 8, 0, false);
    break;
  case 2:
    stripe.loop_control(240, stripe.cyan.val, stripe.black.val, 1, 16, 0, false);
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
    stripe.loop_color(stripe.black.val, 0);
  }
  else if (ts < 30.5)
  {
    stripe.reset_BPM(1);
    int g = map(ts, 24.5, 30.5, 255, 0);
    int b = map(ts, 24.5, 32.5, 100, 0);
    stripe.loop_control(27, CRGBW(0, g, b, 0), stripe.black.val, 0, 0, 0, false);
  }
  else if (ts < 32)
  {
    stripe.loop_color(stripe.black.val, 0);
  }
  else if (ts < 42)
  {
    stripe.reset_BPM(2);
    stripe.loop_sinelon(27 * 8, stripe.cyan.val, 8);
  }
  else if (ts < 55)
  {
    stripe.reset_BPM(3);
    u8 bpm = map(ts, 44, 55, 27 * 8, 27);
    stripe.loop_sinelon(bpm, stripe.green.val, 8);
  }
  else if (ts < 56)
  {
    stripe.loop_fade_out(1000);
  }
  else if (ts < 94)
  {
    stripe.reset_BPM(4);
    stripe.loop_fade_color(27.27, stripe.magenta.val, true);
  }
  else if (ts < 98)
  {
    stripe.loop_fade_out(1000);
  }
  else if (ts < 100)
  {
    stripe.loop_color(stripe.black.val, 0);
  }
  else if (ts < 117)
  {
    if (ts < 105)
    {
      u8 r = map(ts, 100, 105, 0, stripe.orange.val.r);
      u8 g = map(ts, 100, 105, 0, stripe.orange.val.g);
      stripe.loop_wabern(1500, CRGBW(r, g, 0, 0));
    }
    else
    {
      stripe.loop_wabern(1500, stripe.orange.val);
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
    static CRGBW c = stripe.blue.val;
    bool ret = stripe.loop_control(36.9, c, stripe.black.val, 2, 8, 0, dir);
    if (ret)
    {
      dir = !dir;
      if (dir)
      {
        c = stripe.green.val;
      }
      else
      {
        c = stripe.blue.val;
      }
    }
    //stripe.loop_sinelon(36.8, stripe.blue.val, 8);
    //stripe.loop_fade_color(36.8, c, true);
  }
  else if (ts < 210) // Blitze
  {
    uint8_t chance = 0;
    chance = map(ts, 173000, 21100, 0, 80);
    //stripe.loop_color(255, 0, 0, 0, chance);
    stripe.loop_control(1, stripe.black.val, stripe.black.val, 64, 0, chance, false);
  }
  else if (ts < 215.6 + o)
  {
    stripe.loop_fade_out(1000);
  }
  else if (ts < 248)
  {
    stripe.reset_BPM(6);
    stripe.loop_control(27.27, stripe.cyan.val, stripe.black.val, 2, 8, 0, false);
  }
  else if (ts < 252.8)
  {
    stripe.loop_color(stripe.black.val, 0);
  }
  else if (ts < 252.9)
  {
    stripe.loop_color(stripe.red.val, 0);
  }
  else if (ts < 259)
  {
    stripe.loop_fade_out(500);
  }
  else if (ts < 355)
  {
    float o = -1;
    if (ts < 261.8 + o)
      stripe.loop_control(659, stripe.cyan.val, stripe.black.val, 1, 0, 0, false);
    else if (ts < 266.6 + o)
      stripe.loop_control(659, stripe.blue.val, stripe.black.val, 1, 0, 0, false);
    else if (ts < 273.3 + o)
      stripe.loop_color(stripe.dred.val, 0);

    else if (ts < 274.9 + o)
      stripe.loop_control(659, stripe.cyan.val, stripe.black.val, 1, 0, 0, false);
    else if (ts < 280.8 + o)
      stripe.loop_control(659, stripe.blue.val, stripe.black.val, 1, 0, 0, false);
    else if (ts < 287.6 + o)
      stripe.loop_color(stripe.dred.val, 0);

    else if (ts < 289 + o)
      stripe.loop_control(659, stripe.cyan.val, stripe.black.val, 1, 0, 0, false);
    else if (ts < 295.7 + o)
      stripe.loop_control(659, stripe.blue.val, stripe.black.val, 1, 0, 0, false);
    else if (ts < 303.3 + o)
      stripe.loop_color(stripe.dred.val, 0);

    else if (ts < 304.7 + o)
      stripe.loop_control(659, stripe.cyan.val, stripe.black.val, 1, 0, 0, false);
    else if (ts < 305.7 + o)
      stripe.loop_control(659, stripe.blue.val, stripe.black.val, 1, 0, 0, false);
    else if (ts < 309.7 + o)
      stripe.loop_color(stripe.dred.val, 0);

    else if (ts < 311.2 + o)
      stripe.loop_control(659, stripe.cyan.val, stripe.black.val, 1, 0, 0, false);
    else if (ts < 320.2 + o)
      stripe.loop_control(659, stripe.blue.val, stripe.black.val, 1, 0, 0, false);
    else if (ts < 330 + o)
      stripe.loop_color(stripe.dred.val, 0);

    else if (ts < 330.7 + o)
      stripe.loop_control(659, stripe.cyan.val, stripe.black.val, 1, 0, 0, false);
    else if (ts < 331.7 + o)
      stripe.loop_color(stripe.dred.val, 0);

    else if (ts < 332.6 + o)
      stripe.loop_control(659, stripe.cyan.val, stripe.black.val, 1, 0, 0, false);
    else if (ts < 333.7 + o)
      stripe.loop_color(stripe.dred.val, 0);

    else if (ts < 335.7 + o)
      stripe.loop_control(659, stripe.cyan.val, stripe.black.val, 1, 0, 0, false);
    else if (ts < 336 + o)
      stripe.loop_control(659, stripe.magenta.val, stripe.black.val, 1, 0, 0, false);
    else
      stripe.loop_color(stripe.dred.val, 0);
  }
  else if (ts < 364.8)
  {
    stripe.loop_color(stripe.black.val, 0);
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
    stripe.loop_sinelon(bpm, stripe.white.val, 8);
    //stripe.loop_control(bpm, stripe.www, stripe.black.val, 1, 0, 0, true);
    //stripe.loop_wabern(bpm, stripe.white);
  }
  else
  {
    stripe.loop_color(stripe.black.val, 0);
  }

  stripe.show();
}
#endif

#ifdef wmenu
void loop()
{
  menu.loop();

  switch (menu.mval[0].v)
  {
  case 0:
    //CRGBW c =CRGBW(menu.mval[1].v, menu.mval[2].v, menu.mval[3].v, menu.mval[4].v);
    stripe.loop_color(CRGBW(menu.mval[1].v, menu.mval[2].v, menu.mval[3].v, menu.mval[4].v), 0);
    break;
  case 1:
    stripe.reset_BPM(1);
    //stripe.loop_control(659, stripe.cyan.val, stripe.black.val, 1, 0, 0, false);
    stripe.loop_control(menu.mval[1].v, menu.mval[2].v, menu.mval[3].v, menu.mval[4].v,
                        menu.mval[5].v, menu.mval[6].v, menu.mval[7].v);
    break;
  }
  stripe.show();
}
#endif