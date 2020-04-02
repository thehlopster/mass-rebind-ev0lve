//
// Created by ruppet on 23.01.2020.
//

#ifndef GIDEONPROJECT_MENU_C_UI_COLORPICKER_H
#define GIDEONPROJECT_MENU_C_UI_COLORPICKER_H

#include "c_ui_element.h"

class c_ui_colorpicker : public c_ui_element {
public:
    void setup(std::string n) override { setup(n, nullptr); };
    void setup(std::string n, float* v);
    void draw(c_ui_window* wnd) override;

    e_element_type get_type() override { return ELEMENT_TYPE_CONTROL; }
    e_control_type get_control_type() override { return CONTROL_COLORPICKER; }

    void set_var(float* v) { val = v; }

    void set_value(int i, float v) { if (val) val[i] = v; }
    float get_value(int i) { return val ? val[i] : 0.f; }

    void set_alphabar(bool b) { draw_alpha_bar = b; }
    bool get_alphabar() { return draw_alpha_bar; }
private:
    float* val;

    ImGuiID beh_id = 0;

    ImColor outline;
    bool applied = false;
    bool clear = false;
    bool draw_alpha_bar = true;

    bool changing_col = false;
    bool changing_hue = false;
    bool changing_alpha = false;

    float hue = 0.f;
    float col[3] = {};
};


#endif //GIDEONPROJECT_MENU_C_UI_COLORPICKER_H
