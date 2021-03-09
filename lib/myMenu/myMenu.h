#ifndef MYMENU_H
#define MYMENU_H

struct typePosition {
  int y; // row
  int x; // column
};

class Setting {
  public:
    Setting(int _id, int _default, int _min, int _max, typePosition _position)  {
      id = _id;
      default = _default;
      min = _min;
      max = _max;
      position = _position;
      value = default;
    };

    int get_value();
    void set_value(int value);
    void reset();

private:
    int id;
    int value;
    int min;
    int max;
    int default;
    typePosition position;
    String name;
};

class Menu {
    public:
      Menu(String _title, int _max_settings) {
        title = _title;
        max_settings = _max_settings;
      };
      void setup();

      void setup_display(typeResolution _resolution);
      void draw();

    private:
      String title;
      static const max_settings;
      Setting settings[max_settings];
};

#endif
