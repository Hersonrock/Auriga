#pragma once

#include <Windows.h>
#include <string>
#include <stdexcept>
#include <system_error>
#include <format>
#include "resource.h"
#include "ErrorHandler.h"

const char className[] = "Auriga";

class WinApp {

public:

	WinApp(HINSTANCE hInstance, int nCmdShow, int width, int height);
	~WinApp();
	HWND windowHandle_;
	HINSTANCE instanceHandle_;

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
	
	int initWindow();
	int messageLoop();


private:
	const std::string className_;
	
	INT initialWindowState_;

	uint32_t  WindowWidth_;
	uint32_t  WindowHeight_;
};
