#pragma once

//#define NOMINMAX
#include <Windows.h>
#include <string>

#define WINDOW_GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define WINDOW_GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))


class Window {
public:
	std::string name;
	unsigned int width;
	unsigned int height;
	unsigned int window_x;
	unsigned int window_y;
	bool keys[256];
	int mousex;
	int mousey;
	bool mouseButtons[3];
	HWND hwnd;
	HINSTANCE hinstance;
	void initWnd(unsigned int window_width, unsigned int window_height, std::string window_name,
		int window_x = 0, int window_y = 0);
	void updateMouse(int x, int y) {
		mousex = x;
		mousey = y;
	};
	void processMessages();
};