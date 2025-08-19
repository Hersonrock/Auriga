#pragma once
#include <string>
#include <Windows.h>
#include "DxErr.h"

#if defined(DEBUG) | defined(_DEBUG)
#define HR(x) __hr((x), __FILEW__, __LINE__)
#else
#define HR(x) (x)
#endif

inline void __hr(HRESULT hr, LPCWSTR filename, int line)
{
    if (FAILED(hr)) {

        const wchar_t* name = DXGetErrorStringW(hr);
        wchar_t totalCuffer[512];
        swprintf_s(totalCuffer, L"%s\nfile: %s\n line %d\n\n", name, filename, line);

        if (MessageBoxW(nullptr, totalCuffer, L"HRESULT Error", MB_ICONEXCLAMATION | MB_OK)) {
            ExitProcess(0);
        }
    }
}