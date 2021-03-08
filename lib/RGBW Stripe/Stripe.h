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

  CRGBW(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _w)
  {
    r = _r;
    g = _g;
    b = _b;
    w = _w;
  }

  inline void operator=(const CRGBW c) __attribute__((always_inline))
  {
    this->r = c.r;
    this->g = c.g;
    this->b = c.b;
    this->w = c.w;
  }

  inline uint16_t getRGBWsize(uint16_t nleds)
  {
    uint16_t nbytes = nleds * 4;
    if (nbytes % 3 > 0)
      return nbytes / 3 + 1;
    else
      return nbytes / 3;
  }
};

struct TypeColor
{
  CRGBW val;
  String name;

  TypeColor(){};
  TypeColor(CRGBW color, String Name)
  {
    val = color;
    name = Name;
  }
};

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
    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(ledsRGB, leds->getRGBWsize(NUM_LEDS + 1));
  }
  void show()
  {
    //leds_gamma();
    //leds_mirror();
    FastLED.show();
  }

  TypeColor black = {CRGBW(0, 0, 0, 0), "black"};
  TypeColor red = {CRGBW(225, 0, 0, 0), "red"};
  TypeColor green = {CRGBW(0, 255, 0, 0), "green"};
  TypeColor blue = {CRGBW(0, 0, 255, 0), "blue"};
  TypeColor white = {CRGBW(0, 0, 0, 255), "white"}; //5
  TypeColor cyan = {CRGBW(0, 255, 100, 25), "cyan"};
  TypeColor orange = {CRGBW(100, 20, 0, 0), "orange"};
  TypeColor yellow = {CRGBW(255, 100, 0, 0), "yellow"};
  TypeColor magenta = {CRGBW(50, 0, 255, 50), "magenta"};
  TypeColor pink = {CRGBW(255, 0, 150, 25), "pink"}; // 10
  TypeColor wred = {CRGBW(255, 0, 0, 25), "wred"};
  TypeColor wgreen = {CRGBW(0, 255, 0, 25), "wgreen"};
  TypeColor wblue = {CRGBW(0, 0, 64, 0), "wblue"};
  TypeColor dred = {CRGBW(64, 0, 0, 0), "dred"};
  TypeColor dgreen = {CRGBW(0, 64, 0, 0), "dgreen"}; // 15
  TypeColor dblue = {CRGBW(0, 0, 64, 0), "dblue"};
  TypeColor full = {CRGBW(255, 255, 255, 255), "full"}; //17

  static const int maxColors = 17;
  TypeColor color[maxColors]{
      black, red, green, blue, white, cyan, orange, yellow, magenta, pink,
      wred, wgreen, wblue, dred, dblue, dgreen, full};

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