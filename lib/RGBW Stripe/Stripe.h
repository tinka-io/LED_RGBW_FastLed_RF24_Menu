#ifndef STRIPE_H
#define STRIPE_H

#include <FastLED.h>

#define NUM_LEDS 20
#define ROUND 20
#define nomirror

#define DATA_PIN 15

struct CRGBW
{
  union
  {
    struct
    {
      uint8_t g;
      uint8_t r;
      uint8_t b;
      uint8_t w;
    };
    uint8_t v[4];
  };
  CRGBW() {}

  CRGBW(uint8_t rd, uint8_t grn, uint8_t blu, uint8_t wht)
  {
    r = rd;
    g = grn;
    b = blu;
    w = wht;
  }

  /*
    inline void operator = (const CRGBW c) __attribute__((always_inline)) {
    this->r = c.r;
    this->g = c.g;
    this->b = c.b;
    this->w = c.w;
  }*/
};

inline uint16_t getRGBWsize(uint16_t nleds)
{
  uint16_t nbytes = nleds * 4;
  if (nbytes % 3 > 0)
    return nbytes / 3 + 1;
  else
    return nbytes / 3;
}

#define maxColor 11
class Stripe
{
private:
  CRGBW leds[NUM_LEDS + 1];
  CRGB *ledsRGB = (CRGB *)&leds[0];

  uint8_t brightness = 255;
  int bpm = 60;
  int OnBeat = 4; // On Off Off Off

public:
  Stripe() {}
  void setup()
  {
    // Fast LED
    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(ledsRGB, getRGBWsize(NUM_LEDS + 1));
  }
  void show()
  {
    //leds_gamma();
    //leds_mirror();
    FastLED.show();
  }

  CRGBW black = CRGBW(0, 0, 0, 0);
  CRGBW red = CRGBW(225, 0, 0, 0);
  CRGBW wred = CRGBW(255, 0, 0, 25);
  CRGBW dred = CRGBW(32, 0, 0, 0);
  CRGBW green = CRGBW(0, 255, 0, 25);
  CRGBW dgreen = CRGBW(0, 255, 0, 0);
  CRGBW blue = CRGBW(0, 0, 255, 25);
  CRGBW dblue = CRGBW(0, 0, 255, 0);
  CRGBW cyan = CRGBW(0, 255, 100, 25);
  CRGBW orang = CRGBW(100, 20, 0, 0);
  CRGBW yelo = CRGBW(255, 100, 0, 0);
  CRGBW mage = CRGBW(50, 0, 255, 50);
  CRGBW pink = CRGBW(255, 0, 150, 25);
  CRGBW white = CRGBW(0, 0, 0, 255);
  CRGBW www = CRGBW(255, 255, 255, 255);

  CRGBW get_color(int nr);
  int reset_BPM(int i);

private:
  bool resetBPM = false;
  void leds_gamma();
  void leds_mirror();

  CRGBW interpolate(CRGBW startC, CRGBW endC, int stepNumber, int lastStepNumber);
  void set_part_color(int b, int e, CRGBW c1, CRGBW c2, int pos, int Q);

  void colorFillsmooth(CRGBW c, uint32_t bpm_ms, uint32_t time, int numled, int fade, bool ccw); // fipc -> fade in pixel count
  void colorFill(CRGBW c);

  //void fade(float fadetime, float startTime, CRGBW color, bool dirout);
  uint8_t snap_out_fade_in(uint8_t c, float percent);
  void addGlitter(fract8 chanceOfGlitter);
  void rainbow();

public:
  // for timecocde
  void loop_color(CRGBW color, uint8_t glitter);
  void loop_sinelon(u16 bpm, CRGBW color, u8 fade);
  void loop_fade_color(float bpm, CRGBW fcol, bool dirout);
  void loop_fade_out(float time);
  void loop_wabern(float bpm, CRGBW color);

  // for menu
  bool loop_control(u16 bpm, int fCol, int bCol, int BBlack, int fade, int gliter, bool dir);
  bool loop_control(u16 bpm, CRGBW fCol, CRGBW bCol, int BBlack, int fade, int gliter, bool dir);

  // experimental
  void loop_serial();
  void loop_random();
};

#endif