//
// Created by ruppet on 2/5/2020.
//

#ifndef GIDEONPROJECT_LOADER_C_UI_PRODUCTS_H
#define GIDEONPROJECT_LOADER_C_UI_PRODUCTS_H

#include "c_ui_element.h"


class c_ui_products : public c_ui_element {
public:
    void setup(std::string n) override;
    void draw(c_ui_window* wnd) override;

    e_element_type get_type() override { return ELEMENT_TYPE_CONTROL; }
    e_control_type get_control_type() override { return CONTROL_LISTBOX; }

private:

};


#endif //GIDEONPROJECT_LOADER_C_UI_PRODUCTS_H
