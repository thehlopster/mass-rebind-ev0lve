//
// Created by ruppet on 22.01.2020.
//

#include "c_ui.h"

void c_ui_slider_int::setup(std::string n) {
    setup(n, new int(0), 0, 100);
}

void c_ui_slider_int::setup(std::string n, int *v, int mi, int ma, std::string f) {
    name = std::move(n);
    fmt = std::move(f);

    val = v;
    min = mi;
    max = ma;

    size = ImVec2(195.f, 40.f);

    id = ImHashStr(name.c_str(), name.size(), rand());

    minus = ImHashStr((name + "__MINUS").c_str(), (name + "__MINUS").size(), rand());
    plus = ImHashStr((name + "__PLUS").c_str(), (name + "__PLUS").size(), rand());
}

void c_ui_slider_int::draw(c_ui_window *wnd) {
    auto ui = wnd->get_parent();

    if (!applied) {
        outline = ui->style.outline;
        applied = true;
    }

    ui->set_current_id(id);

    if (draw_name)
        size.y = 40.f;
    else
        size.y = 20.f;

    if (val)
        *val = std::clamp(*val, min, max);

    // UPDATE
    auto text_area = ImRect(pos + ImVec2(30.f, 4.f), pos + size - ImVec2(0.f, 20.f));
    auto slider_area = ImRect(pos + ImVec2(30.f, draw_name ? 16.f : 0.f), pos + size);

    auto old_value = val ? *val : 0;

    if (is_active && (parent ? parent->in_bounds() : true)) {
        if (ui->is_held()) {
            if (ui->is_hovered(slider_area))
                ui->active_id = id;

            if (pm && val && ui->active_id != id) {
                ui->set_current_id(minus);
                if (ui->is_hovered(ImRect(slider_area.Min - ImVec2(10.f, 0.f), slider_area.Min - ImVec2(1.f, -20.f))))
                    ui->active_id = minus;
                ui->set_current_id(plus);
                if (ui->is_hovered(ImRect(slider_area.Max + ImVec2(1.f, -20.f), slider_area.Max + ImVec2(10.f, 0.f))))
                    ui->active_id = plus;
            }
        } else {
            if (ui->active_id == id)
                ui->active_id = 0;

            if (pm && val) {
                ui->set_current_id(minus);
                if (!ui->is_held()) {
                    ui->set_current_id(plus);
                    if (!ui->is_held()) {
                        if (ui->active_id == minus) {
                            (*val)--;
                            ui->active_id = 0;
                        }

                        if (ui->active_id == plus) {
                            (*val)++;
                            ui->active_id = 0;
                        }

                        *val = std::clamp(*val, min, max);
                    }
                }
            }
        }
    }

    if (ui->active_id == id && val && ui->is_held()) {
        auto m_on_track = std::clamp(ImGui::GetIO().MousePos.x - slider_area.Min.x, 0.f, 165.f);

        *val = (int)floorf(m_on_track / 165.f * ((float)max - (float)min)) + min;
        *val = std::clamp(*val, min, max);
    } else {
        if (ui->active_id == id)
            ui->active_id = 0;
    }

    if (val && old_value != *val && callback)
        callback(this);

    // DRAW
    auto d = ui->get_painting();

    if (draw_name)
        d->AddText(text_area.Min, is_active ? ui->style.e_text : ui->style.e_text_disabled, name.c_str());

    auto slider_w = (slider_area.Max - slider_area.Min).x;
    auto slider_h = (slider_area.Max - slider_area.Min).y;

    d->AddRectFilledMultiColor(slider_area.Min + ImVec2(0.f, 5.f), slider_area.Max - ImVec2(0.f, 5.f),
                               ui->style.e_gradient_b, ui->style.e_gradient_b, ui->style.e_gradient_a, ui->style.e_gradient_a);

    if (val) {
        d->AddRectFilled(slider_area.Min + ImVec2(0.f, 5.f),
                         slider_area.Min +
                         ImVec2(((float) (*val) - (float) min) / ((float) max - (float) min) * slider_w,
                                slider_h - 5.f), ui->style.e_accent);
        d->AddRectFilledMultiColor(slider_area.Min + ImVec2(0.f, 5.f), slider_area.Min +
                                   ImVec2(((float) (*val) - (float) min) / ((float) max - (float) min) * slider_w, slider_h - 5.f),
                                   ImColor(0, 0, 0, 0), ImColor(0, 0, 0, 0),
                                   ImColor(0, 0, 0, 80),ImColor(0, 0, 0, 80));
    }

    d->AddRect(slider_area.Min + ImVec2(0.f, 5.f), slider_area.Max - ImVec2(0.f, 5.f), ui->style.e_background_shade_2);
    d->AddRect(slider_area.Min + ImVec2(1.f, 6.f), slider_area.Max - ImVec2(1.f, 6.f), ui->style.e_outline);

    if (pm && val) {
        if (*val != min)
            d->AddText(slider_area.Min - ImVec2(9.f, -3.f), ui->style.text_disabled, "-");
        if (*val != max)
            d->AddText(slider_area.Max + ImVec2(2.f, -17.f), ui->style.text_disabled, "+");
    }

    if (val) {
        char buffer[256] = {};
        ImFormatString(buffer, 256, fmt.c_str(), *val);

        auto text_size = c_ui::text_size(buffer);
        d->AddTextShadow(slider_area.Min + ImVec2(slider_area.GetWidth() / 2 - text_size.x / 2, 5.f),
                   ui->style.e_text, buffer);
    }

    ui->set_current_id(0);

    draw_desc(ui);
}
