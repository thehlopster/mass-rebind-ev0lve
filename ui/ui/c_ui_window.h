//
// Created by ruppet on 21.01.2020.
//

#ifndef GIDEONPROJECT_MENU_C_UI_WINDOW_H
#define GIDEONPROJECT_MENU_C_UI_WINDOW_H

#include <string>
#include "../imgui.h"

class c_ui;
class c_ui_element;

class IDirect3DTexture9;
class IDirect3DDevice9;

class c_ui_window {
public:
    void setup(std::string name, IDirect3DDevice9* device);
    void draw();

    void set_active(bool b) { is_active = b; }
    bool get_active() { return is_active; }

    void set_name(std::string n) { name = std::move(n); }
    std::string get_name() { return name; }

    void set_pos(ImVec2 p) { pos = p; }
    ImVec2 get_pos() { return pos; }

    void set_size(ImVec2 s) { size = s; }
    ImVec2 get_size() { return size; }

    void set_parent(c_ui* ui);
    c_ui* get_parent() { return parent; }

    void add(c_ui_element* e) { elements.push_back(e); }
    c_ui_element* find(const std::string& name);
    c_ui_element* find(const std::string& tab, const std::string& group, const std::string& name);

    std::vector<c_ui_element*> elements;

private:
    c_ui* parent = nullptr;

    bool is_active = false;
    bool is_movable = false;
    std::string name;

    ImVec2 pos;
    ImVec2 size;

    ImRect cb_rect;

    ImGuiID id;
    ImGuiID cb_id;

    IDirect3DTexture9* tex = nullptr;
};


#endif //GIDEONPROJECT_MENU_C_UI_WINDOW_H
