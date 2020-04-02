//
// Created by ruppet on 23.01.2020.
//

#include "c_ui.h"
#include "c_ui_listbox.h"

void c_ui_listbox::setup(std::string n, int *v) {
    name = std::move(n);
    id = ImHashStr(name.c_str(), name.size(), rand());

    val = v;
    size = ImVec2(195.f, 80.f);
}

void c_ui_listbox::draw(c_ui_window *wnd) {
    auto ui = wnd->get_parent();
    ui->set_current_id(id);

    // UPDATE
    auto ge = !dd_elements.empty();
    if (val && ge)
        *val = std::clamp(*val, 0, (int)(dd_elements.size() - 1));

    auto rect = ImRect(pos + ImVec2(30.f, 0.f), pos + size);

    auto y_size = dd_elements.size() * 20.f;
    auto sb_needed = false;
    if (y_size > size.y) {
        y_size -= size.y;
        sb_needed = true;
    }

    auto scroll_size = std::clamp(size.y - y_size, 20.f, size.y);
    auto scroll_size_real = size.y - y_size;

    auto scroll_pos = scroll / y_size * (size.y - 20.f);
    if (scroll_size_real >= 20.f)
        scroll_pos = scroll;

    auto old_value = val ? *val : 0;
    if (is_active && (parent ? parent->in_bounds() : true)) {
        if (ui->is_hovered(rect)) {
            ui->active_id = id;

            if (sb_needed) {
                scroll += ImGui::GetIO().MouseWheel * 20.f;
                scroll = std::clamp(scroll, -y_size, 0.f);
            } else
                scroll = 0.f;

            if (ui->is_clicked()) {
                auto i = (int)floorf(scroll / 20.f);

                auto el_id = 0;
                for (const auto &dd : dd_elements) {
                    auto dr = ImRect(rect.Min + ImVec2(0.f, 20.f * i), rect.Min + ImVec2(165.f, 20.f * i + 19.f));
                    if (dr.Max.y > rect.Max.y || dr.Min.y < rect.Min.y) {
                        i++;
                        el_id++;
                        continue;
                    }

                    if (ui->is_hovered(dr))
                        *val = el_id;

                    i++;
                    el_id++;
                }
            }
        } else {
            if (ui->active_id == id)
                ui->active_id = 0;
        }
    }

    if (val && old_value != *val && callback)
        callback(this);

    // DRAW
    auto d = ui->get_painting();
    d->PushClipRect(rect.Min - ImVec2(1.f, 1.f), rect.Max, true);
    d->AddRectFilled(rect.Min, rect.Max, ui->style.bottom);

    if (val && !dd_elements.empty()) {
        auto i = (int)floorf(scroll / 20.f);

        auto el_id = 0;
        for (const auto& dd : dd_elements) {
            auto dr = ImRect(rect.Min + ImVec2(0.f, 20.f * i), rect.Min + ImVec2(165.f, 20.f * i + 19.f));
            if (dr.Max.y > rect.Max.y || dr.Min.y < rect.Min.y) {
                i++;
                el_id++;
                continue;
            }

            if (ui->is_hovered(dr))
                d->AddRectFilled(dr.Min, dr.Max, ui->style.background_hover);

            if (*val == el_id) {
                c_ui::set_font(ui->fonts.main_bold);
                d->AddText(dr.Min + ImVec2(5.f, 4.f), ui->style.accent, dd.c_str());
                c_ui::set_font(nullptr);
            } else
                d->AddText(dr.Min + ImVec2(5.f, 4.f), is_active ? ui->style.text_normal : ui->style.text_disabled, dd.c_str());

            i++;
            el_id++;
        }
    }

    d->AddRect(rect.Min, rect.Max, ui->style.outline);
    d->AddRect(rect.Min + ImVec2(1.f, 1.f), rect.Max - ImVec2(1.f, 1.f), ui->style.shadow);

    d->PopClipRect();

    if (sb_needed) {
        d->AddRectFilled(rect.Max - ImVec2(7.f, size.y), rect.Max, ui->style.outline);

        d->AddRectFilled(rect.Max - ImVec2(5.f, size.y - (2.f - scroll_pos)),
                         rect.Max - ImVec2(2.f, size.y - (-2.f + scroll_size - scroll_pos)), ui->style.outline_hover);
    }

    ui->set_current_id(0);

    draw_desc(ui);
}

void c_ui_listbox::setup(std::string n) {
    setup(n, nullptr);
}
