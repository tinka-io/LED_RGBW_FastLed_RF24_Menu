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

    static void setup();
    static void loop();

    // Share myMenu Values
    static const int max_mval = 8;
    static mval_type mval[max_mval];

private:
    static int pos;
    static int max_pos; //depends on Menu
    static bool user_input;

    // Staic Color
    static void set_mval_limits_static_color();
    static void draw_static_color();

    // BPM Programms
    static void set_mval_limits_bpm();
    static void draw_bpm();
    static void draw_menus();

    // Encoder
    static void handleEncoder();
    static void handleButton();

    static void get_Button_val();
    static void new_Encoder_val(int step);

    static void getEncoder();
};

#endif