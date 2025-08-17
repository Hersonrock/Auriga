#include "WinApp.h"

WinApp::WinApp(HINSTANCE hInstance, int nCmdShow)
	: windowHandle_(nullptr),
	className_(className),
	instanceHandle_(hInstance),
	initialWindowState_(nCmdShow)
{
	initWindow();
}

WinApp::~WinApp() {

	if (windowHandle_) {
		DestroyWindow(windowHandle_);
		windowHandle_ = nullptr;
	}
	UnregisterClass(className_.c_str(), instanceHandle_);
}

int WinApp::run() {
	return 	messageLoop();
}

int WinApp::messageLoop() {
	MSG msg{};
	int result;
	while ((result = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (result == 0)
	{
		return static_cast<int>(msg.wParam);
	}
	else
	{
		DWORD ec = GetLastError();
		throw std::system_error(static_cast<int>(ec), std::system_category());
	}

}

int WinApp::initWindow() {
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = this->instanceHandle_;
	wcex.hIcon = 0;
	wcex.hIconSm = 0;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = this->className_.c_str();
	wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

	// Registering the window
	if (!RegisterClassEx(&wcex)) {
		DWORD ec = GetLastError();
		throw std::system_error(static_cast<int>(ec), std::system_category());
	}

	// Creating the window
	windowHandle_ = CreateWindow(
		this->className_.c_str(),
		this->className_.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		640,
		480,
		nullptr,
		nullptr,
		this->instanceHandle_,
		nullptr
	);


	if (!windowHandle_) {
		DWORD ec = GetLastError();
		throw std::system_error(static_cast<int>(ec), std::system_category());

	}

	ShowWindow(windowHandle_, this->initialWindowState_);
	UpdateWindow(windowHandle_);

	return 0;
}

LRESULT CALLBACK WinApp::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	default:
		return  DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
