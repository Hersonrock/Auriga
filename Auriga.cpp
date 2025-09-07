#include <iostream>
#include <print>
#include "ConsoleHelper.h"
#include "D3DApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	CreateDebugConsole();
	std::println("Console initialized");
#endif //_DEBUG

	int statusReturn;
	D3DApp app{ hInstance, nCmdShow };

	try
	{
		statusReturn = app.run();
	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Error!", MB_ICONEXCLAMATION | MB_OK);
		statusReturn = 1;
	}



#ifdef _DEBUG
	DestroyDebugConsole();
#endif //_DEBUG

	return statusReturn;
}

