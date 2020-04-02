//
// Created by ruppet on 21.01.2020.
//

#include <d3dx9.h>
#include "c_ui.h"
#include "c_ui_window.h"
#include "../icons.h"

void c_ui_window::set_parent(c_ui *ui) {
    parent = ui;
}

void c_ui_window::setup(std::string n, IDirect3DDevice9* device) {
    name = std::move(n);

    id = ImHashStr(name.c_str(), name.size(), rand());
    cb_id = ImHashStr((name + "__CB").c_str(), (name + "__CB").size(), rand());

    D3DXCreateTextureFromFileInMemoryEx(device, texture, 50299, 715, 480, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
                                        D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &tex);
}

void c_ui_window::draw() {
    if (!is_active)
        return;

    // UPDATE
    pos = ImFloor(pos);
    size = ImFloor(size);

    cb_rect = ImRect(pos + ImVec2(size.x - 25.f, 20.f), pos + ImVec2(size.x - 20.f, 25.f));

    parent->set_current_id(cb_id);
    if (parent->is_hovered(cb_rect)) {
        if (parent->active_id == cb_id && !parent->is_held()) {
            parent->active_id = 0;
            exit(0);
        } else {
            if (parent->is_held())
                parent->active_id = cb_id;
        }
    } else {
        if (parent->active_id == cb_id)
            parent->active_id = 0;
    }
    parent->set_current_id(0);

    // DRAW
    auto d = parent->get_painting();

    d->PushClipRect(pos, pos + size, true);
    d->AddImage(tex, pos, pos + size);

    d->AddRectFilled(pos, pos + ImVec2(size.x, 40.f), parent->style.e_background);
    d->AddLine(pos + ImVec2(0, 40.f), pos + ImVec2(size.x, 40.f), parent->style.e_background_shade_2);
    d->AddLine(pos + ImVec2(0, 39.f), pos + ImVec2(size.x, 39.f), parent->style.e_background_shade_1);

    parent->set_font(parent->fonts.icons);
    auto logo_size = ImGui::CalcTextSize(u8"\uE053");
    auto scratch_size = ImGui::CalcTextSize(u8"\uE053");

    d->AddText(pos + ImVec2(size.x / 2 - logo_size.x / 2, 20 - logo_size.y / 2), parent->style.e_text, u8"\uE052");
    d->AddText(pos + ImVec2(size.x / 2 - scratch_size.x / 2, 20 - scratch_size.y / 2), parent->style.e_accent, u8"\uE053");
    parent->set_font(0);

    auto cb_col = parent->style.e_text;
    if (parent->is_hovered(cb_rect))
        cb_col = parent->style.e_text_disabled;

    d->AddText(cb_rect.Min - ImVec2(1.f, 6.f), cb_col, "x");

    for (auto e : elements) {
        if (e->get_hidden())
            continue;

        e->draw(this);
    }

    d->AddRect(pos + ImVec2(1, 1), pos + size, parent->style.e_accent);
    d->PopClipRect();

    // POST UPDATE
    parent->set_current_id(0);
}

c_ui_element* find_recursive(const std::string& name, const std::vector<c_ui_element*>& elements) {
    for (auto e : elements) {
        if (e->get_type() == ELEMENT_TYPE_CONTAINER)
            return find_recursive(name, e->elements);
        else {
            if (e->get_name() == name)
                return e;
        }
    }

    return nullptr;
}

c_ui_element *c_ui_window::find(const std::string& n) {
    return find_recursive(n, elements);
}

c_ui_element *c_ui_window::find(const std::string &tab, const std::string &group, const std::string &name) {
    for (auto t : elements) {
        if (t->is_tab() && t->get_name() == tab) {
            for (auto g : t->elements) {
                if (g->get_type() == ELEMENT_TYPE_CONTAINER && g->get_name() == group) {
                    for (auto e : g->elements) {
                        if (e->get_name() == name)
                            return e;
                    }
                }
            }
        }
    }

    return nullptr;
}
