#pragma once

#include <Windows.h>
#include <string>
#include <stdexcept>
#include <system_error>
#include <format>
#include "resource.h"

const char className[] = "Auriga";

class WinApp {

public:

	WinApp(HINSTANCE hInstance, int nCmdShow);
	~WinApp();
	HWND windowHandle_;

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
	int run();

private:
	const std::string className_;
	HINSTANCE instanceHandle_;
	INT initialWindowState_;

	int initWindow();
	int messageLoop();
};
