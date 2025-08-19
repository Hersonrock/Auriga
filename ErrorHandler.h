#pragma once
#include <system_error>
#include <string_view>
#include <Windows.h>

//Inline is used to the throw comes from the affected line. 

// For COM and DirectX I need to check HRESULT
inline void check_hr(HRESULT hr, std::string_view where) {
	if (FAILED(hr)) {
		throw std::system_error(
			std::error_code(static_cast<int>(hr), std::system_category()),
			std::string(where)
		);
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