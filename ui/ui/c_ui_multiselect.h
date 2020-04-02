//
// Created by ruppet on 23.01.2020.
//

#ifndef GIDEONPROJECT_MENU_C_UI_MULTISELECT_H
#define GIDEONPROJECT_MENU_C_UI_MULTISELECT_H

#include "c_ui_element.h"

class c_ui_multiselect : public c_ui_element {
public:
    void setup(std::string n) override;
    void setup(std::string n, bool* v);
    void draw(c_ui_window* wnd) override;

    e_element_type get_type() override { return ELEMENT_TYPE_CONTROL; }
    e_control_type get_control_type() override { return CONTROL_MULTISELECT; }

    void add(const std::string& t) { dd_elements.push_back(t); }
    void remove(const std::string& t) { dd_elements.erase(std::find(dd_elements.begin(), dd_elements.end(), t)); }
    void remove_all() { dd_elements.clear(); }
    int count() { return dd_elements.size(); }

    bool get_value(int i) { return val ? val[i] : false; }
    void set_value(int i, bool v) { if (val) val[i] = v; }

    void set_var(bool* v) { val = v; }

private:
    bool* val = nullptr;

    float scroll = 0.f;

    ImGuiID scroll_id = 0;
    ImColor outline;
    bool applied = false;
    bool clear = false;

    std::vector<std::string> dd_elements;
};


#endif //GIDEONPROJECT_MENU_C_UI_MULTISELECT_H
