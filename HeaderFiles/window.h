#pragma once

//#define NOMINMAX
#include <Windows.h>

#include <cstdio>
#include <io.h>
#include <fcntl.h>

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

	void CreateDebugConsole()
	{
		AllocConsole();

		FILE* fp;
		freopen_s(&fp, "CONOUT$", "w", stdout);   // printf
		freopen_s(&fp, "CONOUT$", "w", stderr);   // fprintf(stderr,…)
		freopen_s(&fp, "CONIN$", "r", stdin);    // scanf

		HANDLE hConOut = GetStdHandle(STD_OUTPUT_HANDLE);
		int fdOut = _open_osfhandle((intptr_t)hConOut, _O_TEXT);
		FILE* fOut = _fdopen(fdOut, "w");
		*stdout = *fOut;
		setvbuf(stdout, nullptr, _IONBF, 0);

		printf("Debug console created.\n");
	}

};