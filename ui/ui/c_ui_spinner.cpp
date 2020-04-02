//
// Created by ruppet on 2/4/2020.
//

#include <d3dx9.h>
#include "c_ui.h"
#include "../spinners.h"

void c_ui_spinner::setup(std::string n, IDirect3DDevice9* device) {
    name = std::move(n);

    id = ImHashStr(name.c_str(), name.size(), rand());

    size = ImVec2(24.f, 24.f);

    D3DXCreateTextureFromFileInMemoryEx(device, spinner_0, 355, 24, 24, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
                                        D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &spinners[0]);
    D3DXCreateTextureFromFileInMemoryEx(device, spinner_1, 350, 24, 24, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
                                        D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &spinners[1]);
    D3DXCreateTextureFromFileInMemoryEx(device, spinner_2, 329, 24, 24, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
                                        D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &spinners[2]);
    D3DXCreateTextureFromFileInMemoryEx(device, spinner_3, 347, 24, 24, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
                                        D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &spinners[3]);
    D3DXCreateTextureFromFileInMemoryEx(device, spinner_4, 350, 24, 24, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
                                        D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &spinners[4]);
    D3DXCreateTextureFromFileInMemoryEx(device, spinner_5, 348, 24, 24, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
                                        D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &spinners[5]);
    D3DXCreateTextureFromFileInMemoryEx(device, spinner_6, 329, 24, 24, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
                                        D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &spinners[6]);
    D3DXCreateTextureFromFileInMemoryEx(device, spinner_7, 347, 24, 24, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
                                        D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &spinners[7]);
}

void c_ui_spinner::draw(c_ui_window *wnd) {
    auto ui = wnd->get_parent();

    // UPDATE
    if (time - lasttime > 0.1f) {
        lasttime = time;
        spinner++;

        if (spinner > 7)
            spinner = 0;
    }

    // DRAW
    auto d = ui->get_painting();
    d->AddImage(spinners[spinner], pos, pos + size);

    draw_desc(ui);

    time += ImGui::GetIO().DeltaTime;
}