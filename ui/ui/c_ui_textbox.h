//
// Created by ruppet on 23.01.2020.
//

#ifndef GIDEONPROJECT_MENU_C_UI_TEXTBOX_H
#define GIDEONPROJECT_MENU_C_UI_TEXTBOX_H

#include "c_ui_element.h"

class c_ui_textbox : public c_ui_element {
public:
    void setup(std::string n) override { setup(n, nullptr, 0); }
    void setup(std::string n, std::string* v, int m);
    void draw(c_ui_window* wnd) override;

    e_element_type get_type() override { return ELEMENT_TYPE_CONTROL; }
    e_control_type get_control_type() override { return CONTROL_TEXTBOX; }

    void set_password(bool v) { is_password = v; }
    bool get_password() { return is_password; }

    std::string get_value() { return val ? *val : ""; }
    void set_value(std::string v) { if (val) *val = std::move(v); }

    void set_var(std::string* v) { val = v; }

private:
    std::string* val = nullptr;
    int max = 0;

    ImColor outline;
    bool applied = false;
    bool is_password = false;
};


#endif //GIDEONPROJECT_MENU_C_UI_TEXTBOX_H
