#include "Stripe.h"
#include <Arduino.h>

CRGBW Stripe::get_color(int nr)
{
  CRGBW c;
  switch (nr)
  {
  case 0:
    c = black;
    break;
  case 1:
    c = red;
    break;
  case 2:
    c = green;
    break;
  case 3:
    c = blue;
    break;
  case 4:
    c = cyan;
    break;
  case 5:
    c = yelo;
    break;
  case 6:
    c = mage;
    break;
  case 7:
    c = wred;
    break;
  case 8:
    c = white;
    break;
  case 9:
    c = www;
    break;
  default:
    c = black;
  }
  return c;
}

int Stripe::reset_BPM(int i)
{
  static u8 lasti = 0;
  if (lasti != i)
  {
    resetBPM = true;
    lasti = i;
  }
  return lasti;
}

u8 led_gamma[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
    2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5,
    5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10,
    10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
    17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
    25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
    37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
    51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
    69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
    90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
    115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
    144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
    177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
    215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255};

void Stripe::leds_gamma()
{
  for (int i = 0; i < ROUND; i++)
  {
    leds[i].r = led_gamma[leds[i].r];
    leds[i].g = led_gamma[leds[i].g];
    leds[i].b = led_gamma[leds[i].b];
    leds[i].w = led_gamma[leds[i].w];
  }
}

void Stripe::leds_mirror()
{
  leds[NUM_LEDS - 1] = leds[0];
  for (int i = 0; i < ROUND - 1; i++)
  {
    leds[i + 16] = leds[i + 1];
  }
}


CRGBW Stripe::interpolate(CRGBW startC, CRGBW endC, int stepNumber, int lastStepNumber)
{
  //u8 sC[3] = { (u8)(startC >> 16), (u8)(startC >> 8), (u8)(startC) };
  //u8 eC[3] = { (u8)(  endC >> 16), (u8)(  endC >> 8), (u8)(  endC) };

  //Serial.print("u32:"+String(startC)+" R:"+String(sC[0])+" G:"+String(sC[1])+" B:"+String(sC[2])+"\n");

  CRGBW color = CRGBW(0, 0, 0, 0);
  for (int i = 0; i < 4; i++)
  {
    color.v[i] = (endC.v[i] - startC.v[i]) * stepNumber / lastStepNumber + startC.v[i];
  }

  //CRGB color = CRGB(0,0,0);
  //color  = ((u32)c[0] << 16) + ((u32)c[1] <<  8) + (u32)c[2];

  //Serial.print("u32:"+String(color)+" R:"+String(c[0])+" G:"+String(sC[1])+" B:"+String(sC[2])+"\n");

  return color;
}

void Stripe::set_part_color(int b, int e, CRGBW c1, CRGBW c2, int pos, int Q)
{
  int steps = e - b;                    // end - begin
  float os = (float)steps / (float)256; //
  int fadeB = round(os * ((float)pos - (float)Q / (float)2));
  int fadeE = round(os * ((float)pos + (float)Q / (float)2));
  if (fadeE > steps)
    fadeE = steps;
  int fadeS = fadeE - fadeB;

  //Serial.print("steps:"+String(steps)+" os:"+String(os, 8)+" pos:"+String(pos)+" Q:"+String(Q)+" fadeB:"+String(fadeB)+" fadeE:"+String(fadeE)+" fadeS:"+String(fadeS)+"\n");

  for (int i = 0; i < steps; i++)
  {
    CRGBW color = CRGBW(0, 0, 0, 0);
    if (i <= fadeB)
      color = c1;
    if (fadeB < i && i < fadeE)
      color = interpolate(c1, c2, i - fadeB, fadeS);
    if (fadeE <= i)
      color = c2;

    //if(i == steps-1) c = 0xFFFF;
    leds[b + i] = color;
  }
}

void Stripe::colorFillsmooth(CRGBW c, uint32_t bpm_ms, uint32_t curr_time, int numled, int fade, bool ccw) // fipc -> fade in pixel count
{
  int AllSteps = numled * 256 + 256 * fade;
  ;
  int CurrStep = (millis() - curr_time) / double(bpm_ms) * AllSteps;

  int CurrLED = CurrStep / 256;

  CRGBW pix_col;
  for (int i = 0; i < numled; i++)
  {
    int lastfadeled = CurrLED - fade;

    // the fade windows go under zero
    if (lastfadeled < 0)
    {
    }
    // LED behind the fade window
    if (i < lastfadeled)
    {
      pix_col = black;
    }
    else
    {
      // LED in fron of Current Step
      if (CurrLED < i)
      {
        pix_col = black;
      }
      else // LED get fade val
      {
        float f = float(CurrStep - i * 256) / float(fade * 256);
        float r = f * float(c.r);
        float g = f * float(c.g);
        float b = f * float(c.b);
        float w = f * float(c.w);
        pix_col = CRGBW(r, g, b, w);
      }
    }
    // set Pixel
    if (ccw)
    {
      leds[numled - 1 - i] = pix_col;
    }
    else
    {
      leds[i] = pix_col;
    }
  }
}

void Stripe::colorFill(CRGBW c)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = c;
  }
}

uint8_t Stripe::snap_out_fade_in(uint8_t c, float percent)
{
  float f = float(c) * (float)NUM_LEDS * percent / 100.00;
  uint8_t v = (int)f % c;
  //Serial.print(percent);
  //Serial.print(" ");
  //Serial.print(f);
  //Serial.print("-");
  //Serial.println(v);

  return v;
}

void Stripe::addGlitter(fract8 chanceOfGlitter)
{
  if (random8() < chanceOfGlitter)
  {
    leds[random16(NUM_LEDS)].w = 255;
  }
}

void Stripe::rainbow()
{
  static uint8_t hue;

  for (int i = 0; i < NUM_LEDS; i++)
  {
    CRGB led = CHSV((i * 256 / NUM_LEDS) + hue, 255, 255);
    leds[i] = CRGBW(led.g, led.r, led.b, i * 256 / NUM_LEDS + hue);
  }

  hue++;
}


void Stripe::loop_color(CRGBW color, uint8_t glitter)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = color;
  }

  addGlitter(glitter);
}

void Stripe::loop_sinelon(u16 bpm, CRGBW color, u8 fade)
{
  if (bpm <= 0)
  {
    return;
  }

  uint32_t bpm_ms = 60000 / bpm;
  static uint32_t time;
  if (time + bpm_ms < millis() || resetBPM)
  {
    resetBPM = false;
    time = millis();
  }

  colorFillsmooth(color, bpm_ms, time, ROUND, fade, 0);
}

void Stripe::loop_fade_color(float bpm, CRGBW c, bool dirout)
{
  float bpm_ms = 60000 / bpm;
  static float startTime;
  if (startTime + bpm_ms < millis() || resetBPM)
  {
    resetBPM = false;
    startTime = millis();
  }

  float fade = 1.0 / bpm_ms * float(millis() - startTime);
  if (dirout)
  {
    fade = 1 - fade;
  }

  //Serial.println(String(fade) + " " + String(startTime));
  //delay(500);

  for (int i = 0; i < ROUND; i++)
  {
    CRGBW cfull = CRGBW(c.r * fade, c.g * fade, c.b * fade, c.w * fade);
    leds[i] = cfull;
  }
}

void Stripe::loop_fade_out(float fadetime)
{
  static u32 lastStep = 0;

  //Serial.println(String(millis()-lastStep)+" "+String(fadetime/0xFF));
  if (millis() - lastStep >= fadetime / 0xFF)
  {
    lastStep = millis();
    for (int i = 0; i < ROUND; i++)
    {
      if (leds[i].r - 1 >= 0)
        leds[i].r--;

      if (leds[i].g - 1 >= 0)
        leds[i].g--;

      if (leds[i].b - 1 >= 0)
        leds[i].b--;

      if (leds[i].w - 1 >= 0)
        leds[i].w--;
    }
  }
}

void Stripe::loop_wabern(float fadetime, CRGBW c)
{
  loop_fade_out(fadetime * 4);

  // get time and random pos
  static u8 pos = 0;
  static float startTime;
  static float fade;
  if (resetBPM || fade >= 1.0)
  {
    resetBPM = false;
    pos = map(random8(), 0, 255, 0, ROUND);
    startTime = millis();
  }

  // fade in on pos
  fade = 1.0 / float(fadetime / (millis() - startTime));
  CRGBW cfull = CRGBW(c.r * fade, c.g * fade, c.b * fade, c.w * fade);
  //CRGBW cdrvi = CRGBW(cfull.r * 0.75, cfull.g * 0.75, cfull.b * 0.75, cfull.w * 0.75);
  CRGBW cdrvi = cfull;

  leds[pos] = cfull;

  // one befor
  if (pos == 0)
  {
    leds[ROUND - 1] = cdrvi;
  }
  else
  {
    leds[pos - 1] = cdrvi;
  }

  // one after
  if (pos == ROUND - 1)
  {
    leds[0] = cdrvi;
  }
  else
  {
    leds[pos + 1] = cdrvi;
  }
}


bool Stripe::loop_control(u16 bpm, int fCol, int bCol, int BBlack, int fade, int gliter, bool dir)
{
  CRGBW c1 = get_color(fCol);
  CRGBW c2 = get_color(bCol);
  loop_control(bpm, c1, c2, BBlack, fade, gliter, dir);
}

bool Stripe::loop_control(u16 bpm, CRGBW fCol, CRGBW bCol, int BBlack, int Fade, int glitter, bool dir)
{
  static int off = 0;

  //uint32_t bpm_ms = 60000 / bpm / (BBlack + 1);
  uint32_t bpm_ms = 60000 / bpm;
  static uint32_t time = 0;
  bool startover = false;
  if (time + bpm_ms < millis() || resetBPM)
  {
    if (resetBPM || off >= BBlack)
    {
      off = 0;
    }
    else
    {
      off++;
    }

    resetBPM = false;
    time = millis();
    startover = true;
  }

  CRGBW c = fCol;
  if (off != 0)
  {
    c = bCol;
  }
  else
  {
    glitter = 0;
  }

  if (Fade == 0)
  {
    colorFill(c);
  }
  else
  {
    colorFillsmooth(c, bpm_ms, time, ROUND, Fade, dir);
  }

  if (glitter > 0)
  {
    addGlitter(glitter);
  }

  return startover;
  //leds_gamma();
  //leds_mirror();
  //FastLED.show();
}


void Stripe::loop_serial()
{
  static int s = 1;
  static int t = 1000;
  static int z = 0;
  static bool d;
  static bool r = true;
  static int b = 1;
  static int p = 1;
  static uint32_t last_tap;

  if (Serial.available() > 0)
  {
    char in = Serial.read();
    int inv = Serial.read() - 48;
    Serial.println(String(in) + String(inv));

    static int tdiff = 1;
    switch (in)
    {
    case 't':
      if (0 < inv && inv < 16)
      {
        tdiff = inv;
        t = t / tdiff;
      }
      else
      {
        t = millis() - last_tap;
        t = t / tdiff;
        last_tap = millis();
      }
      break;
    case 's':
      z = map(inv, 0, 9, 0, 64);
      break;
    case 'd':
      d = !d;
      break;
    case 'c':
      s = inv;
      r = false;
      break;
    case 'r':
      r = !r;
      break;
    case 'b':
      b = inv;
      break;
    case 'p':
      p = inv;
      break;
    default:
      Serial.println("no cmd");
    }
  }

  CRGBW c;
  switch (s)
  {
  case 0:
    c = black;
    break;
  case 1:
    c = mage;
    break;
  case 2:
    c = cyan;
    break;
  case 3:
    c = red;
    break;
  case 4:
    c = green;
    break;
  case 5:
    c = blue;
    break;
  case 6:
    c = yelo;
    break;
  }

  static int off = 0;
  static int pc = 0;
  int zt = z;
  if (off < b)
  {
    c = black;
  }
  else
  {
    zt = 0;
  }

  /*
  if (colorFillsmooth(c, t, 5, d))
  {
    off++;
    pc++;
    if (off > b)
    {
      off = 0;
    }
    if (r)
    {
      s = map(random8(), 0, 255, 1, 6);
    }
  }
  */

  if (zt > 0)
  {
    addGlitter(zt);
  }
  if (p == 1)
  {
    if (pc == 4)
      pc = 0;
    //float pf = NUM_LEDS / 4;
    set_part_color(pc * 4, (1 * pc) * 4, red, black, 64, 32);
  }

  FastLED.show();
}

void Stripe::loop_random()
{
  static bool off;

  int s = random8();
  int t = random8();
  int w = random8();
  int z = random8();
  int d = random8();

  s = map(s, 0, 255, 0, 5);
  w = map(w, 0, 255, 0, NUM_LEDS);
  z = map(z, 0, 255, 0, 20);
  d = d % 2;

  CRGBW c;

  switch (s)
  {
  case 0:
    c = yelo;
    break;
  case 1:
    c = mage;
    break;
  case 2:
    c = cyan;
    break;
  case 3:
    c = red;
    break;
  case 4:
    c = green;
    break;
  case 5:
    c = blue;
    break;
  }

  if (off)
  {
    z = 0;
    t = map(t, 0, 255, 1000, 1000);
    c = black;
  }
  else
  {
    z = 0;
    t = map(t, 0, 255, 1000, 1000);
    if (z < 16)
      z = 0;
  }

  off = !off;
}