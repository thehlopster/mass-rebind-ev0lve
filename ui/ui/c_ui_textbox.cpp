//
// Created by ruppet on 23.01.2020.
//

#include "c_ui.h"
#include "c_ui_textbox.h"

void c_ui_textbox::setup(std::string n, std::string *v, int m) {
    name = std::move(n);
    id = ImHashStr(name.c_str(), name.size(), rand());

    val = v;
    max = m;

    size = ImVec2(195.f, 40.f);
}

void c_ui_textbox::draw(c_ui_window *wnd) {
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

    // UPDATE
    auto label_rect = ImRect(pos + ImVec2(0.f, 4.f), pos + size - ImVec2(0.f, 20.f));
    auto control_rect = ImRect(pos + ImVec2(0.f, draw_name ? 20.f : 0.f), pos + size);

    auto old_value = val ? *val : "";
    if (is_active && (parent ? parent->in_bounds() : true)) {
        if (ui->is_hovered(control_rect)) {
            if (ui->is_clicked())
                ui->active_id = id;
        }
    }

    if (ui->active_id == id && val) {
        auto io = &ImGui::GetIO();
        io->WantTextInput = true;

        if (io->InputQueueCharacters.Size > 0) {
            for (auto c : io->InputQueueCharacters) {
                if (c == VK_ESCAPE || c == VK_RETURN)
                    ui->active_id = 0;
                else if (c == VK_BACK)
                    (*val) = (*val).substr(0, (*val).size() - 1);
                else if (c != VK_TAB) {
                    if (val->size() < max)
                        (*val) += (unsigned char) c;
                }
            }
        }

        if (!ui->is_hovered(control_rect) && ui->is_clicked() && ui->active_id == id)
            ui->active_id = 0;
    }

    if (val && old_value != *val && callback)
        callback(this);

    // DRAW
    auto d = ui->get_painting();

    if (draw_name)
        d->AddText(label_rect.Min, is_active ? ui->style.text_normal : ui->style.text_disabled, name.c_str());

    if (ui->is_hovered(control_rect))
        outline = c_ui::animate_color(outline, ui->style.outline, ui->style.outline_hover, 0.1f);
    else
        outline = c_ui::animate_color(outline, ui->style.outline, ui->style.outline_hover, 0.1f, true);

    d->AddRectFilled(control_rect.Min, control_rect.Max, ui->style.bottom);
    d->AddRect(control_rect.Min, control_rect.Max, ui->active_id == id ? ui->style.accent : outline);
    d->AddRect(control_rect.Min + ImVec2(1.f, 1.f), control_rect.Max - ImVec2(1.f, 1.f), ui->style.shadow);

    d->PushClipRect(control_rect.Min, control_rect.Max, true);
    if (val) {
        if (is_password) {
            auto glyph = ui->fonts.main_bold->FindGlyph('*');
            auto g = ImGui::GetCurrentContext();

            auto password_font = &g->InputTextPasswordFont;
            password_font->FontSize = g->Font->FontSize;
            password_font->Scale = g->Font->Scale;
            password_font->DisplayOffset = g->Font->DisplayOffset;
            password_font->Ascent = g->Font->Ascent;
            password_font->Descent = g->Font->Descent;
            password_font->ContainerAtlas = g->Font->ContainerAtlas;
            password_font->FallbackGlyph = glyph;
            password_font->FallbackAdvanceX = glyph->AdvanceX;

            c_ui::set_font(password_font);
        }

        d->AddText(control_rect.Min + ImVec2(5.f, 4.f), ui->style.text_normal, val->c_str());

        if (is_password)
            c_ui::set_font(nullptr);
    }
    d->PopClipRect();

    ui->set_current_id(0);

    draw_desc(ui);
}
