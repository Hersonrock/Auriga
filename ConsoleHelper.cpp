#include "ConsoleHelper.h"

void CreateDebugConsole() {
    AllocConsole();

    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    freopen_s(&stream, "CONOUT$", "w", stderr);
    freopen_s(&stream, "CONIN$", "r", stdin);

    std::ios::sync_with_stdio();
    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();

    SetConsoleTitleA("Debug Console");
}

void DestroyDebugConsole() {
    FreeConsole();
}
