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

	try
	{
		D3DApp app{ hInstance, nCmdShow };
		statusReturn = app.run();
	}
	catch (const std::system_error& e)
	{
		std::string error_str = std::format("Caught system_error with code {} meaning {}",
			e.code().value(),
			e.what());

		MessageBox(nullptr, error_str.c_str(), "Error!", MB_ICONEXCLAMATION | MB_OK);
		statusReturn = 1;
	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Error!", MB_ICONEXCLAMATION | MB_OK);
		statusReturn = 1;
	}

	return statusReturn;

#ifdef _DEBUG
	DestroyDebugConsole();
#endif //_DEBUG

	return 0;
}

