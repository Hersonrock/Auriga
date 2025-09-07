#pragma once
#include <system_error>
#include <string>
#include <format>
#include <Windows.h>
#include "DxErr.h"
#include <comdef.h>

#if defined(DEBUG) || defined(_DEBUG)
#define check_hr(x) __check_hr((x), __FILE__, __LINE__)
#else
#define hr(x) (x)
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
inline void check_win(BOOL ok, std::string_view where) {
	if (!ok) { // throws if ok is false.
		const DWORD ec = GetLastError();
		throw std::system_error(
			std::error_code(static_cast<int>(ec), std::system_category()),
			std::string(where)
		);
	}
}

