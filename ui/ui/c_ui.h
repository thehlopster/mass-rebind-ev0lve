//
// Created by ruppet on 21.01.2020.
//

#ifndef GIDEONPROJECT_MENU_C_UI_H
#define GIDEONPROJECT_MENU_C_UI_H

#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
#include <windows.h>

#define IMGUI_DEFINE_MATH_OPERATORS

#include "../imgui.h"
#include "../imgui_internal.h"
#include "../imgui_impl_win32.h"
#include "../imgui_impl_dx9.h"

#include "c_ui_window.h"
#include "c_ui_element.h"
#include "c_ui_tab.h"
#include "c_ui_group.h"
#include "c_ui_checkbox.h"
#include "c_ui_slider_int.h"
#include "c_ui_slider_float.h"
#include "c_ui_label.h"
#include "c_ui_button.h"
#include "c_ui_dropdown.h"
#include "c_ui_multiselect.h"
#include "c_ui_listbox.h"
#include "c_ui_hotkey.h"
#include "c_ui_textbox.h"
#include "c_ui_colorpicker.h"
#include "c_ui_spinner.h"
#include "c_ui_products.h"

class c_ui;

class c_ui_message {
public:
    void setup(std::string ti, std::string te, c_ui* u);
    void draw(ImDrawList* d);

    bool drawable() { return is_drawable; }
    void set_pos(ImVec2 p) { pos = p; }

private:
    c_ui* ui = nullptr;

    std::string title;
    std::string text;

    ImVec2 pos;

    bool is_drawable = false;
    bool set_x = false;

    unsigned int tick = 0;

    float x = 0.f;
};

class c_ui {
public:
    void setup();
    void draw();

    void add(c_ui_window* wnd);
    void set_current_id(ImGuiID id) { current_id = id; }

    bool is_hovered(ImRect r);
    bool is_held();
    bool is_released();
    bool is_clicked();
    bool is_unclicked();
    bool is_right_clicked();

    void add_message(std::string title, std::string text);

    static ImColor animate_color(ImColor cur, ImColor a, ImColor b, float t, bool dec = false);

    static void set_font(ImFont* f) { if (!f) ImGui::PopFont(); else ImGui::PushFont(f); }
    static ImVec2 text_size(const std::string& txt) { return ImGui::CalcTextSize(txt.c_str(), nullptr, true); }

    ImGuiID active_id;

    struct {
        ImColor background;
        ImColor background_hover;
        ImColor bottom;
        ImColor bottom_hover;
        ImColor outline;
        ImColor outline_hover;
        ImColor accent;
        ImColor accent_hover;
        ImColor shadow;
        ImColor shadow_hover;
        ImColor text_normal;
        ImColor text_disabled;
        ImColor text_hovered;

        // ev0 colors
        ImColor e_background;
        ImColor e_dark;
        ImColor e_background_shade_1;
        ImColor e_background_shade_2;
        ImColor e_accent;
        ImColor e_text;
        ImColor e_text_disabled;
        ImColor e_outline;
        ImColor e_gradient_a;
        ImColor e_gradient_b;
    } style;

    struct {
        ImFont* main        = nullptr;
        ImFont* main_bold   = nullptr;
        ImFont* icons       = nullptr;
        ImFont* symbols     = nullptr;
    } fonts;

    ImDrawList* get_painting();
private:
    ImDrawList* painting = nullptr;
    ImGuiID current_id = 0;

    bool is_setup = false;
    bool is_message_shown = true;

    std::vector<c_ui_window*> windows;
    std::vector<c_ui_message*> messages;
};


#endif //GIDEONPROJECT_MENU_C_UI_H
