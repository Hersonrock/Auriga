#include "WinApp.h"

WinApp::WinApp(HINSTANCE hInstance, int nCmdShow, int width, int height)
	: windowHandle_(nullptr),
	className_(className),
	instanceHandle_(hInstance),
	initialWindowState_(nCmdShow),
	WindowWidth_(width),
	WindowHeight_(height)
{
	
}

WinApp::~WinApp() {

	if (windowHandle_) {
		DestroyWindow(windowHandle_);
		windowHandle_ = nullptr;
	}
	UnregisterClass(className_.c_str(), instanceHandle_);
}
void WinApp::initApp() {
	initWindow();
}

int WinApp::messageLoop() {
	MSG msg{};
	int result;
	while ((result = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	check_win(!result);

	return static_cast<int>(msg.wParam);
}

int WinApp::initWindow() {
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = this->instanceHandle_;
	wcex.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	wcex.hIconSm = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON2));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = this->className_.c_str();
	wcex.hIconSm = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON2));

	// Registering the window
	check_win(RegisterClassEx(&wcex));

	// Creating the window
	windowHandle_ = CreateWindow(
		this->className_.c_str(),
		this->className_.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WindowWidth_,
		WindowHeight_,
		nullptr,
		nullptr,
		this->instanceHandle_,
		nullptr
	);
	check_win(windowHandle_ != nullptr);
	
	ShowWindow(windowHandle_, this->initialWindowState_);
	UpdateWindow(windowHandle_);

	return 0;
}

LRESULT CALLBACK WinApp::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

		case WM_CREATE:
			//Creating the menus
			HMENU menuHandler, subMenuHandler;

			menuHandler = CreateMenu();

			subMenuHandler = CreatePopupMenu();
			AppendMenu(subMenuHandler, MF_STRING, ID_FILE_EXIT, "Exit");
			AppendMenu(menuHandler, MF_STRING | MF_POPUP, (UINT)subMenuHandler, "File");

			subMenuHandler = CreatePopupMenu();
			AppendMenu(subMenuHandler, MF_STRING, ID_HELP_ABOUT, "About");
			AppendMenu(menuHandler, MF_STRING | MF_POPUP, (UINT)subMenuHandler, "Help");

			SetMenu(hwnd, menuHandler);
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case ID_FILE_EXIT:
					PostMessage(hwnd, WM_CLOSE, 0, 0);
					break;
				case ID_HELP_ABOUT:
					DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hwnd, AboutDlgProc);
					break;
			}
			break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return  DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

BOOL CALLBACK WinApp::AboutDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_INITDIALOG:

		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			EndDialog(hwnd, IDCANCEL);
			break;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}