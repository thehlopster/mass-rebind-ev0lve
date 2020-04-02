//
// Created by ruppet on 21.01.2020.
//

#include "c_ui.h"
#include "c_ui_tab.h"

void c_ui_tab::setup(std::string n) {
    name = std::move(n);

    id = ImHashStr(name.c_str(), name.size(), rand());
    is_active = false;
}

void c_ui_tab::draw(c_ui_window *wnd) {
    auto ui = wnd->get_parent();

    if (!set_color) {
        text_color = ui->style.e_text_disabled;
        set_color = true;
    }

    // UPDATE
    pos = ImFloor(pos);
    size = ImFloor(size);

    auto w_pos = wnd->get_pos();
    auto t_size = 70.f;

    pos = w_pos + ImVec2(0.f, 41.f + (float)tab_id * t_size);
    size = ImVec2(t_size, t_size);

    auto rect = ImRect(pos, pos + size);

    ui->set_current_id(id);
    if (ui->is_hovered(rect)) {
        if (ui->is_held())
            ui->active_id = id;
    }

    if (ui->active_id == id && !ui->is_held()) {
        ui->active_id = 0;

        for (auto e : wnd->elements) {
            if (e->is_tab())
                ((c_ui_tab*)e)->set_active(false);
        }

        is_active = true;
    }

    auto d = ui->get_painting();
    if (tab_id == 0) {
        d->AddRectFilled(w_pos + ImVec2(0.f, 41.f), w_pos + ImVec2(t_size, wnd->get_size().y), ui->style.e_dark);
        d->AddLine(w_pos + ImVec2(t_size, 41.f), w_pos + ImVec2(t_size, wnd->get_size().y), ui->style.e_background_shade_2);
    }

    if (is_active) {
        text_color = ui->style.e_accent;
        was_active = true;
    }
    else {
        if (was_active) {
            text_color = ui->style.e_text;
            was_active = false;
        }

        if (ui->is_hovered(rect))
            text_color = ui->animate_color(text_color, ui->style.e_text_disabled, ui->style.e_text, 0.2f);
        else
            text_color = ui->animate_color(text_color, ui->style.e_text_disabled, ui->style.e_text, 0.2f, true);
    }

    ui->set_font(ui->fonts.icons);
    auto icon_size = ImGui::CalcTextSize(icon.c_str());
    d->AddText(pos + ImVec2(size.x / 2 - icon_size.x / 2, 10.f + icon_size.y / 2), text_color, icon.c_str());
    ui->set_font(0);

    auto text_size = ImGui::CalcTextSize(name.c_str());
    d->AddTextSoftShadow(pos + ImVec2(size.x / 2 - text_size.x / 2, 45.f + text_size.y / 2), text_color, name.c_str());

    ui->set_current_id(0);

    if (is_active) {
        for (auto e : elements) {
            if (e->get_hidden())
                continue;

            e->draw(wnd);
        }
    }
}
