//
// Created by ruppet on 23.01.2020.
//

#include "c_ui.h"

void c_ui_multiselect::setup(std::string n) {
    setup(n, nullptr);
}

void c_ui_multiselect::setup(std::string n, bool *v) {
    name = std::move(n);
    id = ImHashStr(name.c_str(), name.size(), rand());

    val = v;
    size = ImVec2(195.f, 40.f);
}

void c_ui_multiselect::draw(c_ui_window *wnd) {
    auto ui = wnd->get_parent();
    ui->set_current_id(id);

    if (!applied) {
        outline = ui->style.outline;
        applied = true;
    }

    // UPDATE
    if (clear) {
        ui->active_id = 0;
        clear = false;
    }

    if (draw_name)
        size.y = 40.f;
    else
        size.y = 20.f;

    auto label_rect = ImRect(pos + ImVec2(30.f, 4.f), pos + size - ImVec2(0.f, 20.f));
    auto control_rect = ImRect(pos + ImVec2(30.f, draw_name ? 20.f : 0.f), pos + size);
    auto select_rect = ImRect(
            control_rect.Max - ImVec2(165.f, 0.f) + ImVec2(0.f, 2.f),
            control_rect.Max + ImVec2(0.f, 20.f * dd_elements.size() + 2.f));

    auto old_value = val ? *val : 0;
    if (is_active && (parent ? parent->in_bounds() : true)) {
        if (ui->is_hovered(control_rect)) {
            if (ui->is_clicked())
                ui->active_id = ui->active_id == id ? 0 : id;
        }
    }

    if (ui->active_id == id && ui->is_clicked() && !ui->is_hovered(control_rect)) {
        auto hovered_any = false;

        auto i = 0;
        for (const auto& dd : dd_elements) {
            auto rect = ImRect(select_rect.Min + ImVec2(0.f, 20.f * i), select_rect.Min + ImVec2(165.f, 20.f * i + 19.f));

            if (ui->is_hovered(rect)) {
                val[i] = !val[i];
                hovered_any = true;
            }

            i++;
        }

        if (!hovered_any)
            clear = true;
    }

    if (val && old_value != *val && callback)
        callback(this);

    // DRAW
    auto d = ui->get_painting();

    if (draw_name) {
        d->PushClipRect(label_rect.Min, label_rect.Max, true);
        d->AddText(label_rect.Min, is_active ? ui->style.e_text : ui->style.e_text_disabled, name.c_str());
        d->PopClipRect();
    }

    d->AddRectFilledMultiColor(control_rect.Min, control_rect.Max,
                               ui->style.e_gradient_b, ui->style.e_gradient_b, ui->style.e_gradient_a, ui->style.e_gradient_a);

    d->AddRect(control_rect.Min, control_rect.Max, ui->style.e_background_shade_2);
    d->AddRect(control_rect.Min + ImVec2(1.f, 1.f), control_rect.Max - ImVec2(1.f, 1.f), ui->style.e_outline);

    d->PushClipRect(control_rect.Min, control_rect.Max, true);
    if (!val || dd_elements.empty())
        d->AddText(control_rect.Min + ImVec2(5.f, 3.f), ui->style.e_text_disabled, "-");
    else {
        auto selected = 0;
        for (auto i = 0; i < dd_elements.size(); i++) {
            if (val[i])
                selected++;
        }

        d->AddTextShadow(control_rect.Min + ImVec2(5.f, 3.f), ui->style.e_text, selected == 0 ? "None" : (std::to_string(selected) + " selected").c_str());
    }
    d->PopClipRect();

    if (dd_elements.size() > 1) {
        auto t_base = control_rect.Max - ImVec2(10.f, 9.f);

        if (ui->active_id != id)
            d->AddTriangleFilled(t_base - ImVec2(0.f, 3.f), t_base + ImVec2(2.f, 1.f), t_base + ImVec2(4.f, -3.f), ui->style.e_text);
        else
            d->AddTriangleFilled(t_base - ImVec2(0.f, -1.f), t_base + ImVec2(2.f, -3.f), t_base + ImVec2(4.f, 1.f), ui->style.e_text);
    }

    if (ui->active_id == id && val && !clear) {
        auto f = &ImGui::GetCurrentContext()->ForegroundDrawList;

        f->PushClipRect(select_rect.Min - ImVec2(1.f, 1.f), select_rect.Max, true);
        f->AddRectFilled(select_rect.Min, select_rect.Max, ui->style.e_dark);

        auto i = 0;
        for (const auto& dd : dd_elements) {
            auto rect = ImRect(select_rect.Min + ImVec2(0.f, 20.f * i), select_rect.Min + ImVec2(165.f, 20.f * i + 19.f));

            if (ui->is_hovered(rect))
                f->AddRectFilled(rect.Min, rect.Max, ui->style.e_accent);

            if (val[i])
                f->AddTextShadow(rect.Min + ImVec2(5.f, 4.f), ui->style.e_accent, dd.c_str());
            else
                f->AddTextShadow(rect.Min + ImVec2(5.f, 4.f), ui->style.e_text, dd.c_str());

            i++;
        }

        f->AddRect(select_rect.Min, select_rect.Max, ui->style.e_background_shade_2);
        f->AddRect(select_rect.Min + ImVec2(1.f, 1.f), select_rect.Max - ImVec2(1.f, 1.f), ui->style.e_background_shade_1);
        f->PopClipRect();
    }

    ui->set_current_id(0);

    draw_desc(ui);
}
