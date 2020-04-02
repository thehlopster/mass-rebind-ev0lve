//
// Created by ruppet on 22.01.2020.
//

#include "c_ui.h"
#include "../imgui_internal.h"
#include "c_ui_checkbox.h"


void c_ui_checkbox::setup(std::string n, bool *v , ImVec2 poss) {
    name = std::move(n);
    value = v;

    id = ImHashStr(name.c_str(), name.size(), rand());
    size = ImVec2(20.f, 20.f);
}

void c_ui_checkbox::draw(c_ui_window *wnd) {
    auto ui = wnd->get_parent();

    if (!applied) {
        outline = ui->style.outline;
        applied = true;
    }

    // UPDATE
    size = ImVec2(30.f + c_ui::text_size(name).x, 20.f);
    if (c_type == hold) {
        pos.x += 100.f;
        pos.y -= 15.f;
    }
    else if (c_type == toggle){
        pos.x += 26.f;
        pos.y += 7.f;
    }
    auto rect = ImRect(pos, pos + size);

    ui->set_current_id(id);
    if (is_active && (parent ? parent->in_bounds() : true)) {
        if (ui->is_hovered(rect)) {
            if (ui->is_held())
                ui->active_id = id;
        } else {
            if (ui->active_id == id && !ui->is_held())
                ui->active_id = 0;
        }

        if (ui->is_hovered(rect) && ui->is_clicked()) {
            if (value) {
                *value = !*value;

                if (callback)
                    callback(this);
            }
        }
    }

    // DRAW
   auto d = ui->get_painting();
   
    d->AddRectFilledMultiColor(pos + ImVec2(5.f, 5.f), pos + ImVec2(13.f, size.y - 7.f),
            ui->style.e_gradient_a, ui->style.e_gradient_a, ui->style.e_gradient_b, ui->style.e_gradient_b);
    d->AddRect(pos + ImVec2(4.f, 4.f), pos + ImVec2(15.f, size.y - 5.f), ui->style.e_background_shade_2);
    d->AddRect(pos + ImVec2(5.f, 5.f), pos + ImVec2(14.f, size.y - 6.f), ui->style.e_outline);

    if (value && *value) {
        ui->set_font(ui->fonts.symbols);
        if (c_type == toggle) {

            d->AddTextSoftShadow(pos + ImVec2(5.f, 1.f), ImColor(0.f, 150.f/255.f,1.f,1.f), u8"\u2713");
        }
        else if(c_type == hold) {
            d->AddTextSoftShadow(pos + ImVec2(5.f, 1.f), ImColor(0.f, 1.f, 50.f / 255.f, 1.f), u8"\u2713");

        }
        else {
            d->AddTextSoftShadow(pos + ImVec2(5.f, 1.f), ui->style.e_accent, u8"\u2713");
        }
        ui->set_font(0);
    }

    d->AddText(pos + ImVec2(28.f, 2.f), is_active ? ui->style.e_text : ui->style.e_text_disabled, name.c_str());

    ui->set_current_id(0);

    draw_desc(ui);
}

void c_ui_checkbox::setup(std::string n) {
    name = std::move(n);
    value = new bool(false);

    id = ImHashStr(name.c_str(), 0, rand());

    size = ImVec2(20.f, 20.f);
}
void c_ui_checkbox::setup(std::string n, bool* v) {
    name = std::move(n);
    value = v;

    id = ImHashStr(name.c_str(), name.size(), rand());
    size = ImVec2(20.f, 20.f);
}
