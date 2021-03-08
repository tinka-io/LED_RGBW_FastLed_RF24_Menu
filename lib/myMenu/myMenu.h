#ifndef MYMENU_H
#define MYMENU_H

typedef struct
{
    int v;
    int min;
    int max;
} mval_type;

class myMenu
{
public:
    myMenu(){};

    void setup();
    void loop();

    // Share myMenu Values
    static const int max_mval = 8;
    mval_type mval[max_mval];

private:
    int pos;
    int max_pos; //depends on Menu
    bool user_input;

    // handle Encoder Input
    int set_MenuVal_min_max(int Clicks);
    int get_Encoder_Clicks();
    int get_Button_val();

    // Staic Color
    void set_mval_limits_static_color();
    void set_mal_default_values_static_color();
    void draw_static_color();

    // BPM Programms
    void set_mval_limits_bpm();
    void set_mal_default_values_bpm();
    void draw_bpm();
    void draw_menus();

    // Encoder
    void handleEncoder();
    void handleButton();
};

#endif