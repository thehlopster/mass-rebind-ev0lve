//
// Created by ruppet on 2/4/2020.
//

#ifndef GIDEONPROJECT_LOADER_C_UI_SPINNER_H
#define GIDEONPROJECT_LOADER_C_UI_SPINNER_H

#include "c_ui_element.h"

class IDirect3DTexture9;

class c_ui_spinner : public c_ui_element {
public:
    void setup(std::string name) override {}
    void setup(std::string name, IDirect3DDevice9* device);
    void draw(c_ui_window* wnd) override;

    e_element_type get_type() override { return ELEMENT_TYPE_CONTROL; }
    e_control_type get_control_type() override { return CONTROL_SPINNER; }

private:
    IDirect3DTexture9* spinners[8] = {0};

    int spinner = 0;

    float time = 0.f;
    float lasttime = 0.f;
};


#endif //GIDEONPROJECT_LOADER_C_UI_SPINNER_H
