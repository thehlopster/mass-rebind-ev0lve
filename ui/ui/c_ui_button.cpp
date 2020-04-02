//
// Created by ruppet on 23.01.2020.
//

#include "c_ui.h"
#include "../imgui_internal.h"

void c_ui_button::setup(std::string n) {
    name = std::move(n);

    id = ImHashStr(name.c_str(), name.size(), rand());

    size = ImVec2(195.f, 25.f);
}

void c_ui_button::draw(c_ui_window *wnd) {
    auto ui = wnd->get_parent();
    ui->set_current_id(id);

    if (!applied) {
        outline = ui->style.outline;
        applied = true;
    }

    // UPDATE
    auto rect = ImRect(pos + ImVec2(30.f, 5.f), pos + size);

    if (is_active && (parent ? parent->in_bounds() : true)) {
        if (ui->is_held()) {
            if (ui->is_hovered(rect))
                ui->active_id = id;
        } else {
            if (ui->active_id == id) {
                if (callback)
                    callback(this);

                ui->active_id = 0;
            }
        }
    }

    // DRAW
    auto d = ui->get_painting();

    if (ui->active_id == id) {
        d->AddRectFilledMultiColor(rect.Min, rect.Max,
                                   ui->style.e_gradient_b, ui->style.e_gradient_b, ui->style.e_gradient_a,
                                   ui->style.e_gradient_a);
        d->AddRect(rect.Min, rect.Max, ui->style.e_accent);
    } else {
        d->AddRectFilledMultiColor(rect.Min, rect.Max,
                                   ui->style.e_gradient_a, ui->style.e_gradient_a, ui->style.e_gradient_b,
                                   ui->style.e_gradient_b);
        d->AddRect(rect.Min, rect.Max, ui->style.e_background_shade_2);
    }

    d->PushClipRect(rect.Min, rect.Max, true);
    d->AddTextShadow(rect.Min + ImVec2(rect.GetWidth() / 2, (size.y - 5.f) / 2) - c_ui::text_size(name) / 2,
            is_active ? ui->style.e_text : ui->style.e_text_disabled, name.c_str());
    d->PopClipRect();

    ui->set_current_id(0);

    draw_desc(ui);
}
