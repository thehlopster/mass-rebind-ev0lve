//
// Created by ruppet on 21.01.2020.
//

#ifndef GIDEONPROJECT_MENU_C_UI_ELEMENT_H
#define GIDEONPROJECT_MENU_C_UI_ELEMENT_H

#include <string>
#include <vector>
#include <functional>

#include "../imgui.h"

enum e_element_type {
    ELEMENT_TYPE_CONTROL,
    ELEMENT_TYPE_CONTAINER
};

enum e_control_type {
    CONTROL_BUTTON,
    CONTROL_CHECKBOX,
    CONTROL_COLORPICKER,
    CONTROL_DROPDOWN,
    CONTROL_HOTKEY,
    CONTROL_LABEL,
    CONTROL_LISTBOX,
    CONTROL_MULTISELECT,
    CONTROL_SLIDER_FLOAT,
    CONTROL_SLIDER_INT,
    CONTROL_TEXTBOX,
    CONTROL_SPINNER,
    CONTROL_NONE
};

class c_ui_window;

class c_ui_element {
public:
    virtual void setup(std::string name) = 0;
    virtual void draw(c_ui_window* wnd) = 0;

    virtual e_element_type get_type() = 0;
    virtual e_control_type get_control_type() { return CONTROL_NONE; }

    virtual bool is_tab() { return false; }

    void set_name(std::string n) { name = std::move(n); }
    std::string get_name() { return name; }

    void set_desc(std::string d) { description = std::move(d); }
    std::string get_desc() { return description; }

    void set_hidden(bool h) { is_hidden = h; }
    bool get_hidden() { return is_hidden; }

    void set_active(bool a) { is_active = a; }
    bool get_active() { return is_active; }

    void set_draw_name(bool a) { draw_name = a; }
    bool get_draw_name() { return draw_name; }

    void set_pos(ImVec2 p) { pos = p; }
    ImVec2 get_pos() { return pos; }

    void set_size(ImVec2 s) { size = s; }
    ImVec2 get_size() { return size; }

    void add(c_ui_element* e) { elements.push_back(e); }

    void set_callback(std::function<void(c_ui_element*)> f) { callback = std::move(f); }
    void call_callback() { if (callback) callback(this); }

    void set_parent(c_ui_element* p) { parent = p; }
    bool in_bounds();

    void draw_desc(c_ui* ui);

    std::vector<c_ui_element*> elements;
protected:
    c_ui_element* parent = nullptr;

    ImGuiID id = 0;
    std::string name;
    std::string description;

    bool is_active = true;
    bool is_hidden = false;
    bool draw_name = true;

    ImVec2 pos;
    ImVec2 size;

    std::function<void(c_ui_element*)> callback = nullptr;
};


#endif //GIDEONPROJECT_MENU_C_UI_ELEMENT_H
