//
// Created by ruppet on 23.01.2020.
//

#include "c_ui.h"
#include "c_ui_hotkey.h"

static std::unordered_map<int, std::string> keys = {
        { 0x0, "none" }, { VK_LBUTTON, "mouse 1" }, { VK_RBUTTON, "mouse 2" }, { VK_MBUTTON, "mouse 3" }, { VK_XBUTTON1, "mouse 4" }, { VK_XBUTTON2, "mouse 5" },
        { VK_BACK, "backspace" }, { VK_TAB, "tab" }, { VK_CLEAR, "clear" }, { VK_RETURN, "enter" }, {  VK_SHIFT, "shift" }, { VK_CONTROL, "ctrl" }, { VK_MENU, "alt" },
        { VK_PAUSE, "pause" }, { VK_CAPITAL, "caps lock" }, { VK_SPACE, "space" }, { VK_PRIOR, "page up" }, { VK_NEXT, "page down"}, { VK_END, "end" },
        { VK_HOME, "home" }, {VK_LEFT, "left arrow" }, { VK_UP, "up arrow" }, { VK_RIGHT, "right arrow" }, { VK_DOWN, "down arrow" }, { VK_SELECT, "select" },
        { VK_PRINT, "print" }, { VK_EXECUTE, "execute" }, { VK_INSERT, "insert" }, { VK_DELETE, "delete" }, { VK_HELP, "help" },
        { 0x30, "0" }, { 0x31, "1" }, { 0x32, "2" }, { 0x33, "3" }, { 0x34, "4" }, { 0x35, "5" }, { 0x36, "6" }, { 0x37, "7" }, { 0x38, "8" }, { 0x39, "9" },
        { 0x41, "a" }, { 0x42, "b" }, { 0x43, "c" }, { 0x44, "d" }, { 0x45, "e" }, { 0x46, "f" }, { 0x47, "g" }, { 0x48, "h" }, { 0x49, "i" }, { 0x4A, "j" },
        { 0x4B, "k" }, { 0x4C, "l" }, { 0x4D, "m" }, { 0x4E, "n" }, { 0x4F, "o" }, { 0x50, "p" }, { 0x51, "q" }, { 0x52, "r" }, { 0x53, "s" }, { 0x54, "t" },
        { 0x55, "u" }, { 0x56, "v" }, { 0x57, "w" }, { 0x58, "x" }, { 0x59, "y" }, { 0x5A, "z" }, { VK_SLEEP, "sleep" }, { VK_NUMPAD0, "num 0" }, { VK_NUMPAD1, "num 1" },
        { VK_NUMPAD2, "num 2" }, { VK_NUMPAD3, "num 3" }, { VK_NUMPAD4, "num 4" }, { VK_NUMPAD5, "num 5" }, { VK_NUMPAD6, "num 6" }, { VK_NUMPAD7, "num 7" },
        { VK_NUMPAD8, "num 8" }, { VK_NUMPAD9, "num 9" }, { VK_MULTIPLY, "multiply" }, { VK_ADD, "add" }, { VK_SEPARATOR, "separator" },  {  VK_SUBTRACT, "subtract" },
        { VK_DECIMAL, "decimal" }, { VK_DIVIDE, "divide" }, { VK_F1, "f1" }, { VK_F2, "f2" }, { VK_F3, "f3" }, { VK_F4, "f4" }, { VK_F5, "f5" }, { VK_F6, "f6" },
        { VK_F7, "f7" }, { VK_F8, "f8" }, { VK_F9, "f9" }, { VK_F10, "f10" }, { VK_F11, "f11" }, { VK_F12, "f12" }, { VK_F13, "f13" }, { VK_F14, "f14" }, { VK_F15, "f15" },
        { VK_F16, "f16" }, { VK_F17, "f17" }, { VK_F18, "f18" }, { VK_F19, "f19" }, { VK_F20, "f20" }, { VK_F21, "f21" }, { VK_F22, "f22" }, { VK_F23, "f23" },
        { VK_F24, "f24" }, { VK_NUMLOCK, "num lock" }, { VK_SCROLL, "scroll lock" }, { VK_LSHIFT, "left shift" }, { VK_RSHIFT, "right shift" },
        { VK_LCONTROL, "left ctrl" }, { VK_RCONTROL, "right ctrl" }, { VK_LMENU, "left alt" }, { VK_RMENU, "right alt" }, { VK_BROWSER_BACK, "browser bw" },
        { VK_BROWSER_FORWARD, "browser fw" }, { VK_BROWSER_REFRESH, "browser rr" }, { VK_BROWSER_STOP, "browser stop" }, { VK_BROWSER_SEARCH, "browser sch" },
        { VK_BROWSER_FAVORITES, "browser fv" }, { VK_BROWSER_HOME, "browser home" }, { VK_VOLUME_MUTE, "mute" }, { VK_VOLUME_DOWN, "volume down" },
        { VK_VOLUME_UP, "volume up" }, { VK_MEDIA_NEXT_TRACK, "next track" }, { VK_MEDIA_PREV_TRACK, "prev track" }, { VK_MEDIA_STOP, "stop media" },
        { VK_MEDIA_PLAY_PAUSE, "play/pause" }, { VK_LAUNCH_MAIL, "mail" }, { VK_LAUNCH_MEDIA_SELECT, "media" }, { VK_LAUNCH_APP1, "app" },
        { VK_LAUNCH_APP2, "app" }, { VK_OEM_1, "semicolon" }, { VK_OEM_PLUS, "plus" }, { VK_OEM_COMMA, "comma" }, { VK_OEM_MINUS, "minus" }, { VK_OEM_PERIOD, "dot" },
        { VK_OEM_2, "slash" }, { VK_OEM_3, "tilde" }, { VK_OEM_4, "open bracket" }, { VK_OEM_5, "backslash"  }, { VK_OEM_6, "close bracket" },
        { VK_PROCESSKEY, "processkey" }, { VK_OEM_7, "quote" }
};

void c_ui_hotkey::setup(std::string n, int *v, int* b) {
    name = std::move(n);
    id = ImHashStr(name.c_str(), name.size(), rand());
    beh_id = ImHashStr((name + "__STYLE").c_str(), (name + "__STYLE").size(), rand());

    val = v;
    beh = b;

    size = ImVec2(195.f, 40.f);
}

void c_ui_hotkey::draw(c_ui_window *wnd) {
    auto ui = wnd->get_parent();
    ui->set_current_id(id);

    if (!applied) {
        outline = ui->style.outline;
        applied = true;
    }

    if (draw_name)
        size.y = 40.f;
    else
        size.y = 20.f;

    if (clear) {
        ui->active_id = 0;
        clear = false;
    }

    static auto dd_elements = std::vector<std::string> {
        "Always on", "Hold", "Toggle", "Release"
    };

    // UPDATE
    auto label_rect = ImRect(pos + ImVec2(30.f, 4.f), pos + size - ImVec2(0.f, 20.f));
    auto control_rect = ImRect(pos + ImVec2(30.f, draw_name ? 20.f : 0.f), pos + size);
    auto select_rect = ImRect(
            control_rect.Max - ImVec2(165.f, 0.f) + ImVec2(0.f, 2.f),
            control_rect.Max + ImVec2(0.f, 20.f * dd_elements.size() + 2.f));

    auto old_value = val ? *val : 0;
    if (is_active && (parent ? parent->in_bounds() : true)) {
        if (ui->is_hovered(control_rect)) {
            if (ui->is_clicked())
                ui->active_id = id;
            if (ui->active_id != id && ui->is_right_clicked() && beh)
                ui->active_id = ui->active_id == beh_id ? 0 : beh_id;
        }
    }

    if (ui->active_id == id) {
        if (HIWORD(GetKeyState(VK_ESCAPE))) {
            *val = 0;
            ui->active_id = 0;
        } else {
            if (!ui->is_hovered(control_rect)) {
                if (HIWORD(GetKeyState(VK_LBUTTON))) {
                    *val = VK_LBUTTON;
                    ui->active_id = 0;
                }
            }

            if (HIWORD(GetKeyState(VK_RBUTTON))) {
                *val = VK_RBUTTON;
                ui->active_id = 0;
            }

            if (HIWORD(GetKeyState(VK_MBUTTON))) {
                *val = VK_MBUTTON;
                ui->active_id = 0;
            }

            if (HIWORD(GetKeyState(VK_XBUTTON1))) {
                *val = VK_XBUTTON1;
                ui->active_id = 0;
            }

            if (HIWORD(GetKeyState(VK_XBUTTON2))) {
                *val = VK_XBUTTON2;
                ui->active_id = 0;
            }

            for (int i = 0x8; i < 0xFF; i++) {
                if (HIWORD(GetKeyState(i))) {
                    *val = i;
                    ui->active_id = 0;
                    break;
                }
            }
        }
    }

    ui->set_current_id(beh_id);
    if (ui->active_id == beh_id && ui->is_unclicked() && !ui->is_hovered(control_rect)) {
        clear = true;

        auto i = 0;
        for (const auto& dd : dd_elements) {
            auto rect = ImRect(select_rect.Min + ImVec2(0.f, 20.f * i), select_rect.Min + ImVec2(165.f, 20.f * i + 19.f));

            if (ui->is_hovered(rect))
                *beh = i;

            i++;
        }
    }
    ui->set_current_id(id);

    if (val && old_value != *val && callback)
        callback(this);

    // DRAW
    auto d = ui->get_painting();

    if (draw_name)
        d->AddText(label_rect.Min, is_active ? ui->style.e_text : ui->style.e_text_disabled, name.c_str());

    d->AddRectFilledMultiColor(control_rect.Min, control_rect.Max,
                               ui->style.e_gradient_b, ui->style.e_gradient_b, ui->style.e_gradient_a, ui->style.e_gradient_a);
    d->AddRect(control_rect.Min, control_rect.Max, ui->style.e_background_shade_2);
    d->AddRect(control_rect.Min + ImVec2(1.f, 1.f), control_rect.Max - ImVec2(1.f, 1.f), ui->style.e_outline);

    if (val) {
        if (!beh)
            d->AddTextShadow(control_rect.Min + ImVec2(5.f, 3.f), ui->style.e_text,
                    ui->active_id == id ? "Press any key..." : keys.at(*val).c_str());
        else {
            auto txt = ui->active_id == id ? "Press any key..." : keys.at(*val);
            if (*beh == 0) txt += " (always on)";
            if (*beh == 1) txt += " (hold)";
            if (*beh == 2) txt += " (toggle)";
            if (*beh == 3) txt += " (release)";

            d->AddTextShadow(control_rect.Min + ImVec2(5.f, 3.f), ui->style.e_text, txt.c_str());
        }
    }
    else
        d->AddText(control_rect.Min + ImVec2(5.f, 4.f), ui->style.e_text_disabled, "none");

    ui->set_current_id(0);

    draw_desc(ui);
}
