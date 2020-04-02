//
// Created by ruppet on 22.01.2020.
//

#ifndef GIDEONPROJECT_MENU_C_UI_GROUP_H
#define GIDEONPROJECT_MENU_C_UI_GROUP_H

#include "c_ui_element.h"

class c_ui_group : public c_ui_element {
public:
    void setup(std::string name) override;
    void draw(c_ui_window* wnd) override;

    e_element_type get_type() override { return ELEMENT_TYPE_CONTAINER; }

private:
    float scroll = 0.f;

    ImGuiID scroll_id = 0;
};


#endif //GIDEONPROJECT_MENU_C_UI_GROUP_H
