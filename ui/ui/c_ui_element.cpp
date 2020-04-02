//
// Created by ruppet on 21.01.2020.
//

#include "c_ui.h"
#include "c_ui_element.h"
#include "../imgui_internal.h"

bool c_ui_element::in_bounds() {
    auto ms = ImGui::GetIO().MousePos;
    return ms.x >= pos.x && ms.x <= pos.x + size.x && ms.y >= pos.y && ms.y <= pos.y + size.y;
}

void c_ui_element::draw_desc(c_ui* ui) {
    if (description.empty()) return;

    ui->set_current_id(id);
    if (!ui->is_hovered(ImRect(pos, pos + size))) {
        ui->set_current_id(0);
        return;
    }

    auto d = &ImGui::GetCurrentContext()->ForegroundDrawList;

    auto _pos = ImGui::GetIO().MousePos + ImVec2(15.f, 15.f);
    auto _size = ImVec2(10.f + c_ui::text_size(description).x, 20.f);

    auto rect = ImRect(_pos, _pos + _size);
    d->AddRectFilled(rect.Min, rect.Max, ui->style.bottom);
    d->AddRect(rect.Min, rect.Max, ui->style.outline);
    d->AddRect(rect.Min + ImVec2(1.f, 1.f), rect.Max - ImVec2(1.f, 1.f), ui->style.shadow);

    d->AddText(rect.Min + ImVec2(5.f, 4.f), ui->style.text_normal, description.c_str());

    ui->set_current_id(0);
}