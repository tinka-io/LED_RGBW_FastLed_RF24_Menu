#ifndef MENU_H
#define MENU_H

#include <Arduino.h>

// Encoder define
#define CLK_PIN 0
#define DT_PIN 1
#define SW_PIN 7
#define ENCODER_PWR_PIN 6

const uint8_t maxSettings = 16;
const uint8_t maxPages = 8;

struct typePosition
{
    int row; // row
    int x;   // x Position
};

class Setting
{
public:
    Setting() {};
    Setting(int id, int _start_value, int _min, int _max, typePosition _position)
    {
        id = id;
        start_value = start_value;
        min = _min;
        max = _max;
        position = _position;
        value = start_value;
    };

    int get_value();
    void set_value(int value);
    void reset();

    int value;
    int min;
    int max;
    int start_value;
    typePosition position;
    String name;
};

class MenuPage
{
public:
    MenuPage() {};
    MenuPage(String _title, int _used_settings)
    {
        title = _title;
        used_settings = constrain(_used_settings, 0, maxSettings);
        used_settings = _used_settings;
    };

    String title;
    int used_settings;
    Setting settings[16];
};

class Menu
{
public:
    Menu() {};
    Menu(MenuPage *_pages, int _used_pages) {
        used_pages = _used_pages;
        pages = _pages;
    };

    void setup(String welcome_message);
    void loop();

    MenuPage *pages;
    int used_pages;
    int page_index;

private:
    uint8_t front_size = 1;
    uint8_t line_height = 8*front_size;
    void setup_display(String welcome_message);
    void setup_encoder();

    int get_Encoder_Rotation();
    int get_Encoder_Button();
    void set_value(MenuPage *page, int value);
    void draw(MenuPage *page);

    int curser_position;
};

#endif
