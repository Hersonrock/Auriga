#pragma once
#include <system_error>
#include <string>
#include <format>
#include <Windows.h>
#include "DxErr.h"
#include <comdef.h>

#if defined(DEBUG) || defined(_DEBUG)
#define check_hr(x) __check_hr((x), __FILE__, __LINE__)
#define check_win(x) __check_win((x), __FILE__, __LINE__)
#else
#define check_hr(x) (x)
#define check_win(x) (x)
#endif //DEBUG || _DEBUG

//Inline is used so the throw comes from the affected line. 

// For COM and DirectX I need to check HRESULT
inline void __check_hr(HRESULT hr, const char* file, int line) {
    if (FAILED(hr)) {
        _com_error err(hr);
        std::string err_name = DXGetErrorStringA(hr);
        std::string msg = std::format(
            "HRESULT {:#010x} ({})[{}] at {}:{}",
            static_cast<unsigned long>(hr),
            err.ErrorMessage(), err_name, file, line
        );

        throw std::runtime_error(msg);
    }
}

// For Win32 I need to check BOOL and use GetLastError()
inline void __check_win(BOOL ok, const char* file, int line) {
    if (!ok) {
        const DWORD ec = GetLastError();
        const std::string sys_msg = std::system_category().message(ec); 

        std::string msg = std::format(
            "Win32 error {:#010x}: {} {}:{}",
            static_cast<unsigned long>(ec),
            sys_msg,
            file,
            line
        );

        throw std::runtime_error(msg);
    }
}

