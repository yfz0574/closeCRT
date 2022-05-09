#include "pch.h"
#include <iostream>
#include <vector>

#define HOT_KEY_IMP_
#include "HotKey.h"

bool stop = false;

#define WM_ADD_HOTKEY (WM_USER + 0x0001)
#define WM_DEL_HOTKEY (WM_USER + 0x0002)
#define WM_END (WM_USER + 0x0003)

struct HotKeyInfo {
	UINT fsModifiers;
	UINT vk;
	int delay;
	std::function<void(int)> func;
	bool active;
};

DWORD WINAPI hotkeythreadfunc(LPVOID lpParameter) {
	std::vector<HotKeyInfo> hotkeys;
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		std::cout << "message: " << msg.message << "," << msg.hwnd
			<< "," << msg.wParam << "," << std::endl;
		if (msg.message == WM_HOTKEY)
		{
			UINT vk = msg.wParam;
			UINT mod = msg.lParam;
			std::cout << "WM_HOTKEY received" << std::endl;
			for (auto info : hotkeys) {
				if (info.fsModifiers == mod && info.vk == vk) {
					try {
						info.func(info.delay);
					}
					catch (std::exception e) {
						std::cerr << e.what() << std::endl;
					}
				}
			}
		}
		else if (msg.message == WM_ADD_HOTKEY) {
			HotKeyInfo* info_p = (HotKeyInfo*)msg.wParam;
			HotKeyInfo info = *info_p;
			delete info_p;

			RegisterHotKey(NULL, hotkeys.size() + 1, info.fsModifiers, info.vk);
			info.active = true;
			hotkeys.push_back(info);

		}
		else if (msg.message == WM_DEL_HOTKEY) {
			HotKeyInfo* info_p = (HotKeyInfo*)msg.wParam;
			HotKeyInfo info = *info_p;
			delete info_p;

			for (int i = 0; i < hotkeys.size(); i++) {
				HotKeyInfo hotkey = hotkeys[i];
				if (hotkey.fsModifiers == info.fsModifiers && hotkey.vk == info.vk) {
					UnregisterHotKey(NULL, i + 1);
					hotkey.active = false;
				}
			}
		}
		else if (msg.message == WM_END) {
			for (int i = 0; i < hotkeys.size(); i++) {
				HotKeyInfo hotkey = hotkeys[i];
				if (hotkey.active) {
					UnregisterHotKey(NULL, i);
					hotkey.active = false;
				}
			}
			break; // end message loop
		}
	}
	return 0;
}

HotKey::HotKey() {
	t = CreateThread(NULL, 0, hotkeythreadfunc, NULL, 0, &tid);
}

void HotKey::Register(UINT mod, UINT key, std::function<void(int)> func) {
	HotKeyInfo* info = new HotKeyInfo();
	info->fsModifiers = mod;
	info->vk = key;
	info->func = func;
	PostThreadMessage(tid, WM_ADD_HOTKEY, (WPARAM)info, 0);
}

void HotKey::Unregister(UINT mod, UINT key) {
	HotKeyInfo* info = new HotKeyInfo();
	info->fsModifiers = mod;
	info->vk = key;
	PostThreadMessage(tid, WM_DEL_HOTKEY, (WPARAM)info, 0);
}

HotKey::~HotKey() {
	PostThreadMessage(tid, WM_END, 0, 0);
	while (WaitForSingleObject(t, 100) == WAIT_TIMEOUT) {
		Sleep(50);
	}
}

// Ô­ÎÄÁ´½Ó£ºhttps ://blog.csdn.net/u010745620/article/details/97983387