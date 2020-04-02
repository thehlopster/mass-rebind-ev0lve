//
// Created by ruppet on 23.01.2020.
//

#ifndef GIDEONPROJECT_MENU_C_UI_BUTTON_H
#define GIDEONPROJECT_MENU_C_UI_BUTTON_H

#include "c_ui_element.h"
class c_ui_button : public c_ui_element {
public:
    void setup(std::string n) override;
    void draw(c_ui_window* wnd) override;

    e_element_type get_type() override { return ELEMENT_TYPE_CONTROL; }
    e_control_type get_control_type() override { return CONTROL_BUTTON; }

private:
    ImColor outline;

    bool applied = false;
};


#endif //GIDEONPROJECT_MENU_C_UI_BUTTON_H
