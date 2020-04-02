//
// Created by ruppet on 23.01.2020.
//

#ifndef GIDEONPROJECT_MENU_C_UI_LISTBOX_H
#define GIDEONPROJECT_MENU_C_UI_LISTBOX_H

#include "c_ui_element.h"

class c_ui_listbox : public c_ui_element {
public:
    void setup(std::string n) override;
    void setup(std::string n, int* v);
    void draw(c_ui_window* wnd) override;

    e_element_type get_type() override { return ELEMENT_TYPE_CONTROL; }
    e_control_type get_control_type() override { return CONTROL_LISTBOX; }

    void add(const std::string& t) { dd_elements.push_back(t); }
    void remove(const std::string& t) { dd_elements.erase(std::find(dd_elements.begin(), dd_elements.end(), t)); }
    void remove_all() { dd_elements.clear(); }
    int count() { return dd_elements.size(); }

    int get_value() { return val ? *val : 0; }
    void set_value(int v) { if (val) *val = v; }

    void set_var(int* v) { val = v; }

private:
    int* val = nullptr;

    float scroll = 0.f;

    std::vector<std::string> dd_elements;
};


#endif //GIDEONPROJECT_MENU_C_UI_LISTBOX_H
