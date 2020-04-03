#include "main.h"

bool g_is_initialized = false;
c_ui* g_ui = nullptr;
IDirect3DDevice9Ex* g_device = nullptr;

auto g_accent = new float[4]{
	0.f,
	120.f / 255.f,
	1.f,
	1.f
};

int cfg_count;
std::vector <std::string>cfgs;
bindss binds;

void setup_window();

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l) {
	if (!g_is_initialized)
		return DefWindowProcA(hwnd, msg, w, l);

	static auto can_drag = [hwnd]() -> bool {
		RECT rect;
		if (!GetWindowRect(hwnd, &rect))
			return false;

		POINT cur;
		GetCursorPos(&cur);

		auto y = cur.y - rect.top;
		auto x = cur.x - rect.left;
		return y <= 40 && x <= (rect.right - rect.left) - 30;
	};

	ImGui_ImplWin32_WndProcHandler(hwnd, msg, w, l);

	if (msg == WM_CLOSE || msg == WM_DESTROY)
		exit(0);
	if (msg == WM_NCHITTEST && can_drag())
		return HTCAPTION;

	return DefWindowProcA(hwnd, msg, w, l);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	LoadLibraryA("wininet.dll");
	LoadLibraryA("ole32.dll");
	LoadLibraryA("ws2_32.dll");
	LoadLibraryA("d3d9.dll");
	LoadLibraryA("oleaut32.dll");

	WNDCLASSEX wc{};
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = 0;
	wc.hCursor = LoadCursorA(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)5;
	wc.lpszClassName = "Chrome_WidgetWin_69";

	if (!RegisterClassExA(&wc)) {
		MessageBoxA(0, "Failed to register window class", "Startup failed", MB_OK | MB_ICONERROR);
		return false;
	}

	RECT screen_size;
	GetWindowRect(GetDesktopWindow(), &screen_size);

	auto x = screen_size.right / 2 - 350 / 2, y = screen_size.bottom / 2 - 250 / 2;

	auto hwnd = CreateWindowExA(WS_EX_LAYERED, "Chrome_WidgetWin_69",
		"CFG Tool", WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, x, y,
		715, 480, 0, 0, 0, 0);
	if (!hwnd) {
		MessageBoxA(0, "Failed to create window", "Startup failed", MB_OK | MB_ICONERROR);
		return false;
	}

	IDirect3D9Ex* directx = nullptr;

	Direct3DCreate9Ex(D3D_SDK_VERSION, &directx);
	if (!directx) {
		MessageBoxA(0, "Failed to create D3D9", "Startup failed", MB_OK | MB_ICONERROR);
		return false;
	}

	D3DPRESENT_PARAMETERS pp{};
	pp.Windowed = true;
	pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pp.hDeviceWindow = hwnd;

	directx->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&pp, 0, &g_device);

	if (!g_device) {
		MessageBoxA(0, "Failed to create device", "Startup failed", MB_OK | MB_ICONERROR);
		return false;
	}

	ImGui::CreateContext();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(g_device);

	g_ui = new c_ui;
	g_ui->setup();

	setup_window();

	SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	SetFocus(hwnd);
	SetForegroundWindow(hwnd);

	g_is_initialized = true;

	while (true) {
		MSG msg;
		if (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		if (!g_device || !directx) break;

		g_device->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.f, 0);
		g_device->BeginScene();

		ImGui_ImplWin32_NewFrame();
		ImGui_ImplDX9_NewFrame();
		ImGui::NewFrame();

		g_ui->style.e_accent = ImColor(g_accent[0], g_accent[1], g_accent[2], g_accent[3]);
		g_ui->draw();

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		g_device->EndScene();
		g_device->Present(0, 0, 0, 0);
	}
}

std::string ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;
	return converterX.to_bytes(wstr);
}

//get key from registry
LONG GetStringRegKey(HKEY hKey, const std::wstring& strValueName, std::wstring& strValue, const std::wstring& strDefaultValue)
{
	strValue = strDefaultValue;
	WCHAR szBuffer[512];
	DWORD dwBufferSize = sizeof(szBuffer);
	ULONG nError;
	nError = RegQueryValueExW(hKey, strValueName.c_str(), 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);
	if (ERROR_SUCCESS == nError)
	{
		strValue = szBuffer;
	}
	return nError;
}

//find ev0 cfg folder if !find return (current + ev0lve)
std::string get_cfg_path() {
	HKEY k;
	LONG r = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Wow6432Node\\Valve\\Steam", 0, KEY_READ, &k);
	bool found(r == ERROR_SUCCESS);
	std::wstring steamPath;
	GetStringRegKey(k, L"InstallPath", steamPath, L"");
	std::string dir = ws2s(steamPath) + "\\steamapps\\common\\Counter-Strike Global Offensive\\ev0lve";
	RegCloseKey(k);
	//add here vdf check
	if (found && fs::is_directory(dir)) {
		return dir;
	}
	return fs::current_path().string() + "\\ev0lve";
}

void parse_config() {
	std::string path = get_cfg_path();
	try
	{
		for (const auto& config : fs::directory_iterator(path)) {
			if (!fs::is_directory(config)) {
				cfg_count++;
				cfgs.push_back(config.path().string());
			}
		}
	}
	catch (const std::exception&)
	{
		MessageBoxA(0, "Put this app into csgo folder", "Incorect path", 0);
		exit(-1);
	}
	
}
void write_cfg() {
	using json = nlohmann::json;
	for (const auto& config : cfgs) {
		std::ifstream r(config);
		json j;
		r >> j;
		r.close();

		bool fake_duck_enable = !j["antiaim"]["fake_duck"]["bind"].is_null();
		bool desync_flip_enable = !j["antiaim"]["desync_flip"]["bind"].is_null();
		bool thirdperson_enable = !j["local_visuals"]["thirdperson"]["bind"].is_null();
		bool fast_fire_enable = !j["rage"]["enable_fast_fire"]["bind"].is_null();
		bool slow_walk_enable = !j["antiaim"]["slow_walk"]["bind"].is_null();

		if (fake_duck_enable && binds.fake_duck_key) {
			j["antiaim"]["fake_duck"]["bind"][0]["key"] = binds.fake_duck_key;
			j["antiaim"]["fake_duck"]["bind"][0]["type"] = binds.fake_duck_toggle ? 3 : 2;
		}
		if (desync_flip_enable && binds.desync_flip_key) {
			j["antiaim"]["desync_flip"]["bind"][0]["key"] = binds.desync_flip_key;
			j["antiaim"]["desync_flip"]["bind"][0]["type"] = binds.desync_flip_toggle ? 3 : 2;
		}
		if (thirdperson_enable && binds.Thirdperson_key) {
			j["local_visuals"]["thirdperson"]["bind"][0]["key"] = binds.Thirdperson_key;
			j["local_visuals"]["thirdperson"]["bind"][0]["type"] = binds.Thirdperson_toggle ? 3 : 2;
		}
		if (fast_fire_enable && binds.Fast_fire_key) {
			j["rage"]["enable_fast_fire"]["bind"][0]["key"] = binds.Fast_fire_key;
			j["rage"]["enable_fast_fire"]["bind"][0]["type"] = binds.Fast_fire_toggle ? 3 : 2;
		}
		if (slow_walk_enable && binds.slow_walk_key) {
			j["antiaim"]["slow_walk"]["bind"][0]["key"] = binds.slow_walk_key;
			j["antiaim"]["slow_walk"]["bind"][0]["type"] = binds.slow_walk_toggle ? 3 : 2;
		}
		std::ofstream w(config);
		w << std::setw(4) << j;
		w.close();
	}
	MessageBoxA(0, "Done", "", 0);
	
}

void setup_window() {
	static auto window = new c_ui_window;
	window->setup("mass rebind for ev0lve.xyz", g_device);
	window->set_pos(ImVec2(0, 0));
	window->set_size(ImVec2(715, 480));
	window->set_active(true);

	{
		static auto main = new c_ui_tab;
		main->setup("main");
		main->set_tab(0, 3);
		main->set_icon(u8"\uE04C");
		main->set_active(true);
		{
			static auto general = new c_ui_group;
			general->setup("Configs");
			general->set_pos(ImVec2(20.f, 10.f));
			general->set_size(ImVec2(295.f, 410.f));

			{
				parse_config();
				auto cfg_c = new c_ui_label;
				cfg_c->setup("");
				cfg_c->set_text("CFG count :" + std::to_string(cfg_count));
				general->add(cfg_c);
			}

			static auto bind = new c_ui_group;
			bind->setup("Binds");
			bind->set_pos(ImVec2(325.f, 10.f));
			bind->set_size(ImVec2(295.f, 410.f));
			{
				static auto Thirdperson_t = new c_ui_checkbox;
				Thirdperson_t->setup("toggle", &binds.Thirdperson_toggle);
				Thirdperson_t->c_type = c_ui_checkbox::toggle;
				Thirdperson_t->set_callback([](c_ui_element*) {
					binds.Thirdperson_hold = false;
					});

				static auto Thirdperson_h = new c_ui_checkbox;
				Thirdperson_h->setup("hold", &binds.Thirdperson_hold);
				Thirdperson_h->c_type = c_ui_checkbox::hold;
				Thirdperson_h->set_callback([](c_ui_element*) {
					binds.Thirdperson_toggle = false;
					});

				static auto Thirdperson = new c_ui_hotkey;
				Thirdperson->setup("Thirdperson", new int(0));
				Thirdperson->set_callback([](c_ui_element*) {
					binds.Thirdperson_key = Thirdperson->get_value();
					});

				static auto Fast_fire_t = new c_ui_checkbox;
				Fast_fire_t->setup("toggle", &binds.Fast_fire_toggle);
				Fast_fire_t->c_type = c_ui_checkbox::toggle;
				Fast_fire_t->set_callback([](c_ui_element*) {
					binds.Fast_fire_hold = false;
					});

				static auto Fast_fire_h = new c_ui_checkbox;
				Fast_fire_h->setup("hold", &binds.Fast_fire_hold);
				Fast_fire_h->c_type = c_ui_checkbox::hold;
				Fast_fire_h->set_callback([](c_ui_element*) {
					binds.Fast_fire_toggle = false;
					});

				static auto Fast_fire = new c_ui_hotkey;
				Fast_fire->setup("Enable fast fire", new int(0));
				Fast_fire->set_callback([](c_ui_element*) {
					binds.Fast_fire_key = Fast_fire->get_value();
					});

				static auto desync_flip_t = new c_ui_checkbox;
				desync_flip_t->setup("toggle", &binds.desync_flip_toggle);
				desync_flip_t->c_type = c_ui_checkbox::toggle;
				desync_flip_t->set_callback([](c_ui_element*) {
					binds.desync_flip_hold = false;
					});

				static auto desync_flip_h = new c_ui_checkbox;
				desync_flip_h->setup("hold", &binds.desync_flip_hold);
				desync_flip_h->c_type = c_ui_checkbox::hold;
				desync_flip_h->set_callback([](c_ui_element*) {
					binds.desync_flip_toggle = false;
					});

				static auto desync_flip = new c_ui_hotkey;
				desync_flip->setup("Desync flip", new int(0));
				desync_flip->set_callback([](c_ui_element*) {
					binds.desync_flip_key = desync_flip->get_value();
					});

				static auto fake_duck_t = new c_ui_checkbox;
				fake_duck_t->setup("toggle", &binds.fake_duck_toggle);
				fake_duck_t->c_type = c_ui_checkbox::toggle;
				fake_duck_t->set_callback([](c_ui_element*) {
					binds.fake_duck_hold = false;
					});
				static auto fake_duck_h = new c_ui_checkbox;
				fake_duck_h->setup("hold", &binds.fake_duck_hold);
				fake_duck_h->c_type = c_ui_checkbox::hold;
				fake_duck_h->set_callback([](c_ui_element*) {
					binds.fake_duck_toggle = false;
					});
				static auto fake_duck = new c_ui_hotkey;
				fake_duck->setup("Fake duck", new int(0));
				fake_duck->set_callback([](c_ui_element*) {
					binds.fake_duck_key = fake_duck->get_value();
					});

				static auto slow_walk_t = new c_ui_checkbox;
				slow_walk_t->setup("toggle", &binds.slow_walk_toggle);
				slow_walk_t->c_type = c_ui_checkbox::toggle;
				slow_walk_t->set_callback([](c_ui_element*) {
					binds.slow_walk_hold = false;
					});
				static auto slow_walk_h = new c_ui_checkbox;
				slow_walk_h->setup("hold", &binds.slow_walk_hold);
				slow_walk_h->c_type = c_ui_checkbox::hold;
				slow_walk_h->set_callback([](c_ui_element*) {
					binds.slow_walk_toggle = false;
					});
				static auto slow_walk = new c_ui_hotkey;
				slow_walk->setup("Slow walk", new int(0));
				slow_walk->set_callback([](c_ui_element*) {
					binds.slow_walk_key = slow_walk->get_value();
					});

				static auto apply = new c_ui_button;
				apply->setup("Apply");
				static auto mm = new c_ui_message;
				apply->set_callback([](c_ui_element*) {
					write_cfg();
					});

				bind->add(apply);
				bind->add(Thirdperson);
				bind->add(Thirdperson_t);
				bind->add(Thirdperson_h);

				bind->add(Fast_fire);
				bind->add(Fast_fire_t);
				bind->add(Fast_fire_h);

				bind->add(desync_flip);
				bind->add(desync_flip_t);
				bind->add(desync_flip_h);

				bind->add(fake_duck);
				bind->add(fake_duck_t);
				bind->add(fake_duck_h);

				bind->add(slow_walk);
				bind->add(slow_walk_t);
				bind->add(slow_walk_h);
			}

			main->add(general);
			main->add(bind);
		}
		window->add(main);
	}

	g_ui->add(window);
}