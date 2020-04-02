//
// Created by ruppet on 23.01.2020.
//

#include "c_ui.h"
#include "c_ui_colorpicker.h"

void c_ui_colorpicker::setup(std::string n, float *v) {
    name = std::move(n);
    id = ImHashStr(name.c_str(), name.size(), rand());
    beh_id = ImHashStr((name + "__CP").c_str(), (name + "__CP").size(), rand());

    val = v;

    size = ImVec2(60.f, 20.f);
}

void c_ui_colorpicker::draw(c_ui_window *wnd) {
    auto ui = wnd->get_parent();

    draw_name = false;
    if (size.y == 0.f)
        return;

    static auto dd_elements = std::vector<std::string> {
        "Copy", "Paste"
    };

    val[0] = std::clamp(val[0], 0.f, 1.f);
    val[1] = std::clamp(val[1], 0.f, 1.f);
    val[2] = std::clamp(val[2], 0.f, 1.f);
    val[3] = std::clamp(val[3], 0.f, 1.f);

    ui->set_current_id(id);

    // UPDATE
    if (!applied) {
        outline = ui->style.outline;
        applied = true;
    }

    if (clear) {
        ui->active_id = 0;
        clear = false;
    }

    if (draw_name)
        size.y = 40.f;
    else
        size.y = 20.f;

    pos.x += 180.f;

    auto label_rect = ImRect(pos + ImVec2(30.f, 4.f), pos + size - ImVec2(0.f, 20.f));
    auto control_rect = ImRect(pos + ImVec2(30.f, draw_name ? 25.f : 5.f), pos + size - ImVec2(0.f, 5.f));
    auto select_rect = ImRect(
            control_rect.Max - ImVec2(165.f, 0.f) + ImVec2(0.f, 2.f) + ImVec2(0.f, 5.f),
            control_rect.Max + ImVec2(0.f, 167.f) + ImVec2(0.f, 5.f) -
                    (draw_alpha_bar ? ImVec2(0.f, 0.f) : ImVec2(0.f, 25.f)));
    auto picker_rect = ImRect(select_rect.Min + ImVec2(5.f, 5.f), select_rect.Min + ImVec2(135.f, 135.f));
    auto hue_rect = ImRect(picker_rect.Max - ImVec2(0.f, picker_rect.GetHeight()) + ImVec2(6.f, 0.f), picker_rect.Max + ImVec2(25.f, 0.f));
    auto alpha_rect = ImRect(picker_rect.Max - ImVec2(picker_rect.GetWidth(), 0.f) + ImVec2(0.f, 10.f), picker_rect.Max + ImVec2(25.f, 21.f));

    if (ui->active_id == beh_id)
        select_rect = ImRect(
                control_rect.Max - ImVec2(165.f, 0.f) + ImVec2(0.f, 2.f),
                control_rect.Max + ImVec2(0.f, 20.f * dd_elements.size() + 2.f));

    float H = val[0], S = val[1], V = val[2];
    float R = val[0], G = val[1], B = val[2];

    const ImU32 col_hues[6 + 1] = {
            IM_COL32(255,0,0,255),
            IM_COL32(255,255,0,255),
            IM_COL32(0,255,0,255),
            IM_COL32(0,255,255,255),
            IM_COL32(0,0,255,255),
            IM_COL32(255,0,255,255),
            IM_COL32(255,0,0,255) };

    ImGui::ColorConvertRGBtoHSV(R, G, B, H, S, V);
    if (S == 0 && memcmp(col, val, sizeof(float) * 3) == 0)
        H = hue;

    if (is_active && (parent ? parent->in_bounds() : true)) {
        if (ui->is_hovered(control_rect)) {
            if (ui->is_clicked())
                ui->active_id = ui->active_id == id ? 0 : id;
        }
    }

    if (ui->active_id == id && ui->is_held()) {
        if (!ui->is_hovered(select_rect) && !ui->is_hovered(control_rect) && !changing_col && !changing_alpha && !changing_hue)
            clear = true;
        else {
            if ((ui->is_hovered(picker_rect) || changing_col) && !changing_hue && !changing_alpha) {
                S = ImSaturate((ImGui::GetIO().MousePos.x - picker_rect.Min.x) / (picker_rect.GetWidth() - 1));
                V = 1.0f -
                    ImSaturate((ImGui::GetIO().MousePos.y - picker_rect.Min.y) / (picker_rect.GetHeight() - 1));

                changing_col = true;
            }

            if ((ui->is_hovered(hue_rect) || changing_hue) && !changing_col && !changing_alpha) {
                H = ImSaturate((ImGui::GetIO().MousePos.y - hue_rect.Min.y) / (hue_rect.GetHeight() - 1));
                changing_hue = true;
            }

            if ((ui->is_hovered(alpha_rect) || changing_alpha) && !changing_col && !changing_hue && draw_alpha_bar) {
                val[3] = ImSaturate((ImGui::GetIO().MousePos.x - alpha_rect.Min.x) / (alpha_rect.GetWidth() - 1));
                changing_alpha = true;
            }

            ImGui::ColorConvertHSVtoRGB(H >= 1.0f ? H - 10 * 1e-6f : H, S > 0.0f ? S : 10*1e-6f, V > 0.0f ? V : 1e-6f, val[0], val[1], val[2]);
            hue = H;
            memcpy(col, val, sizeof(float) * 3);
        }
    }

    ui->set_current_id(id);

    if (ui->active_id == id && !ui->is_held()) {
        if ((changing_col || changing_hue || changing_alpha) && callback)
            callback(this);

        if (changing_col) changing_col = false;
        if (changing_hue) changing_hue = false;
        if (changing_alpha) changing_alpha = false;
    }

    // DRAW
    auto d = ui->get_painting();

    d->AddRectFilled(control_rect.Min, control_rect.Max, val ? ImColor(val[0], val[1], val[2]) : ui->style.e_dark);
    d->AddRectFilledMultiColor(control_rect.Min, control_rect.Max,
                               ImColor(0, 0, 0, 0), ImColor(0, 0, 0, 0),
                               ImColor(0, 0, 0, 80),ImColor(0, 0, 0, 80));

    d->AddRect(control_rect.Min, control_rect.Max, ui->style.e_background_shade_2);

    ui->set_current_id(id);

    if (ui->active_id == id && val && !clear) {
        auto f = &ImGui::GetCurrentContext()->ForegroundDrawList;

        f->PushClipRect(select_rect.Min - ImVec2(1.f, 1.f), select_rect.Max, true);
        f->AddRectFilled(select_rect.Min, select_rect.Max, ui->style.e_dark);

        const ImU32 col_black = IM_COL32(0,0,0,255);
        const ImU32 col_white = IM_COL32(255,255,255,255);

        ImVec4 hue_color_f(1, 1, 1, 1); ImGui::ColorConvertHSVtoRGB(H, 1, 1, hue_color_f.x, hue_color_f.y, hue_color_f.z);
        ImU32 hue_color32 = ImGui::ColorConvertFloat4ToU32(hue_color_f);

        f->AddRectFilledMultiColor(picker_rect.Min, picker_rect.Max, col_white, hue_color32, hue_color32, col_white);
        f->AddRectFilledMultiColor(picker_rect.Min, picker_rect.Max, 0, 0, col_black, col_black);

        auto knob_rect = ImRect(picker_rect.Min + ImVec2(ImSaturate(S) * picker_rect.GetWidth(), ImSaturate(1 - V) * picker_rect.GetWidth()) - ImVec2(1.f, 1.f),
                                picker_rect.Min + ImVec2(ImSaturate(S) * picker_rect.GetWidth(), ImSaturate(1 - V) * picker_rect.GetWidth()) + ImVec2(2.f, 2.f));

        knob_rect.Min = ImClamp(knob_rect.Min, picker_rect.Min, picker_rect.Max - ImVec2(2.f, 2.f));
        knob_rect.Max = ImClamp(knob_rect.Max, picker_rect.Min + ImVec2(2.f, 2.f), picker_rect.Max);

        f->AddRectFilled(knob_rect.Min, knob_rect.Max,
                   ImColor(0, 0, 0));
        f->AddRectFilled(knob_rect.Min + ImVec2(1.f, 1.f), knob_rect.Max - ImVec2(1.f, 1.f),
                         ImColor(255, 255, 255));

        f->AddRect(picker_rect.Min, picker_rect.Max, ui->style.e_background_shade_2);
        f->AddRect(picker_rect.Min + ImVec2(1.f, 1.f), picker_rect.Max - ImVec2(1.f, 1.f), ui->style.e_background_shade_1);

        for (int i = 0; i < 6; ++i)
            f->AddRectFilledMultiColor(
                    ImVec2(hue_rect.Min.x, hue_rect.Min.y + i * (hue_rect.GetHeight() / 6)),
                    ImVec2(hue_rect.Min.x + hue_rect.GetWidth(), hue_rect.Min.y + (float)(i + 1) * (hue_rect.GetHeight() / 6)),
                    col_hues[i], col_hues[i], col_hues[i + 1], col_hues[i + 1]);

        f->AddRectFilled(hue_rect.Min, hue_rect.Max, ImColor(0, 0, 0, 30));
        f->AddRect(hue_rect.Min, hue_rect.Max, ui->style.e_background_shade_2);
        f->AddRect(hue_rect.Min + ImVec2(1.f, 1.f), hue_rect.Max - ImVec2(1.f, 1.f), ui->style.e_background_shade_1);

        auto hue_knob_rect = ImRect(hue_rect.Min + ImVec2(1.f, H * (hue_rect.GetHeight() - 4) + 1),
                                    hue_rect.Min + ImVec2(hue_rect.GetWidth(), H * (hue_rect.GetHeight() - 4)) + ImVec2(-2.f, 2.f));

        f->AddRectFilled(hue_knob_rect.Min, hue_knob_rect.Max, ImColor(255, 255, 255));

        if (draw_alpha_bar) {
            f->AddRectFilledMultiColor(alpha_rect.Min, alpha_rect.Max,
                                       ImColor(val[0], val[1], val[2], 0.f), ImColor(val[0], val[1], val[2], 1.f),
                                       ImColor(val[0], val[1], val[2], 1.f), ImColor(val[0], val[1], val[2], 0.f));
            f->AddRect(alpha_rect.Min, alpha_rect.Max, ui->style.e_background_shade_2);
            f->AddRect(alpha_rect.Min + ImVec2(1.f, 1.f), alpha_rect.Max - ImVec2(1.f, 1.f), ui->style.e_background_shade_1);

            auto alpha_knob_rect = ImRect(alpha_rect.Min + ImVec2(val[3] * (alpha_rect.GetWidth() - 4) + 1, 1.f),
                                          alpha_rect.Min + ImVec2(val[3] * (alpha_rect.GetWidth() - 4) + 2,
                                                                  alpha_rect.GetHeight() - 2.f));

            f->AddRectFilled(alpha_knob_rect.Min, alpha_knob_rect.Max, ImColor(255, 255, 255));
        }

        f->AddRect(select_rect.Min, select_rect.Max, ui->style.e_background_shade_2);
        f->AddRect(select_rect.Min + ImVec2(1.f, 1.f), select_rect.Max - ImVec2(1.f, 1.f), ui->style.e_background_shade_1);
        f->PopClipRect();
    }

    ui->set_current_id(0);

    draw_desc(ui);
}
