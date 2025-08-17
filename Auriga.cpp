#include <iostream>
#include <print>
#include "ConsoleHelper.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	CreateDebugConsole();
	std::println("Console initialized");
#endif //_DEBUG

	std::cin.get();

#ifdef _DEBUG
	DestroyDebugConsole();
#endif //_DEBUG

	return 0;
}

