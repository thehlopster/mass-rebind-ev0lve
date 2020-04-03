//
// Created by ruppet on 3/1/2020.
//

#ifndef EV0LVE_APEX_CM_MAIN_H
#define EV0LVE_APEX_CM_MAIN_H

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <d3d9.h>
#include <d3dx9.h>
#include <Shlobj.h>
#include <WinInet.h>
#include <locale>
#include <codecvt>
#include "ui/imgui.h"
#include "ui\imgui_internal.h"
#include "ui\imgui_impl_win32.h"
#include "ui\imgui_impl_dx9.h"
#include "ui\ui\c_ui.h"

#include "json.hpp"

struct bindss
{
	bool Thirdperson_toggle;
	bool Thirdperson_hold;
	int Thirdperson_key;

	bool Fast_fire_toggle;
	bool Fast_fire_hold;
	int Fast_fire_key;

	bool desync_flip_toggle;
	bool desync_flip_hold;
	int desync_flip_key;

	bool fake_duck_toggle;
	bool fake_duck_hold;
	int fake_duck_key;

	bool slow_walk_toggle;
	bool slow_walk_hold;
	int slow_walk_key;
};
extern bool g_is_initialized;
extern c_ui* g_ui;
extern IDirect3DDevice9Ex* g_device;

namespace fs = std::filesystem;

#pragma comment(lib, "WinInet.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#endif //EV0LVE_APEX_CM_MAIN_H
