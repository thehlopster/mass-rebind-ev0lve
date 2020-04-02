//
// Created by ruppet on 21.01.2020.
//

#ifndef GIDEONPROJECT_MENU_C_UI_TAB_H
#define GIDEONPROJECT_MENU_C_UI_TAB_H

#include "c_ui_element.h"

class c_ui_tab : public c_ui_element {
public:
    void setup(std::string name) override;
    void draw(c_ui_window* wnd) override;

    void set_tab(int i, int m) { tab_id = i; tab_max = m; }

    void set_icon(std::string ico) { icon = std::move(ico); }
    std::string get_icon() { return icon; }

    e_element_type get_type() override { return ELEMENT_TYPE_CONTAINER; }
    bool is_tab() override { return true; }

private:
    int tab_id = 0;
    int tab_max = 0;

    std::string icon = "";

    bool set_color = false;
    bool was_active = false;

    float height = 0.f;
    ImColor text_color;
};


#endif //GIDEONPROJECT_MENU_C_UI_TAB_H
