//
// Created by ruppet on 22.01.2020.
//

#ifndef GIDEONPROJECT_MENU_C_UI_SLIDER_INT_H
#define GIDEONPROJECT_MENU_C_UI_SLIDER_INT_H

#include "c_ui_element.h"
class c_ui_slider_int : public c_ui_element {
public:
    void setup(std::string name) override;
    void setup(std::string name, int* v, int min, int max, std::string fmt = "%d");
    void draw(c_ui_window* wnd) override;

    void set_pm_active(bool b) { pm = b; }
    bool get_pm_active() { return pm; }

    void set_format(std::string f) { fmt = std::move(f); }
    std::string get_format() { return fmt; }

    e_element_type get_type() override { return ELEMENT_TYPE_CONTROL; }
    e_control_type get_control_type() override { return CONTROL_SLIDER_INT; }

    int get_value() { return val ? *val : 0; }
    void set_value(int v) { if (val) *val = v; }

    void set_var(int* v) { val = v; }

private:
    int* val = nullptr;
    int min = 0;
    int max = 100;

    std::string fmt = "%d";

    bool applied = false;
    bool pm = false;

    ImColor outline;

    ImGuiID minus;
    ImGuiID plus;
};


#endif //GIDEONPROJECT_MENU_C_UI_SLIDER_INT_H
