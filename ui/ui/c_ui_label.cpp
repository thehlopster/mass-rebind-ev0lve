//
// Created by ruppet on 22.01.2020.
//

#include "c_ui.h"
#include "../imgui_internal.h"

void c_ui_label::setup(std::string n) {
    name = std::move(n);

    id = ImHashStr(name.c_str(), name.size(), rand());

    size = ImVec2(30.f, 20.f);
}

void c_ui_label::draw(c_ui_window *wnd) {
    auto ui = wnd->get_parent();

    // UPDATE
    size = ImVec2(c_ui::text_size(name).x + 30.f, 20.f);

    // DRAW
    auto d = ui->get_painting();

    if (is_bold)
        c_ui::set_font(ui->fonts.main_bold);

    d->AddText(pos + ImVec2(30.f, 4.f), is_active ? ui->style.e_text : ui->style.e_text_disabled, text.c_str());

    if (is_bold)
        c_ui::set_font(nullptr);

    draw_desc(ui);
}
