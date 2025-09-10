#include "D3DApp.h"

//Class holding information about adapters.
struct D3DAdapterInfo
{
	D3DADAPTER_IDENTIFIER9 adapterIdentifier;
	D3DDISPLAYMODE desktopCurrentMode;
	uint32_t	numModes;
	std::vector<D3DDISPLAYMODE> modes;
};

D3DApp::D3DApp(HINSTANCE hInstance, int nCmdShow)
	: winApp_(hInstance, nCmdShow, BACKBUFFERWIDTH, BACKBUFFERHEIGHT),
	  d3d_(nullptr),
	  device_(nullptr),
	  adapterCount_(0),
	  backbufferDescription_{},
	  backbuffer_(nullptr),
	  offscreenSurface_(nullptr),
	  adapters_{}
{
	
}

D3DApp::~D3DApp(){
	cleanUp();
}

int D3DApp::run() {

	winApp_.initApp();
	initDirect3D(winApp_.windowHandle_, true);
	loadSurface();
	render();
	winApp_.messageLoop();

	return 	0;
}

bool D3DApp::initDirect3D(HWND wndHandle, bool isWindowed) {


	d3d_ = Direct3DCreate9(D3D_SDK_VERSION);
	check_ptr(d3d_);

		
	D3DPRESENT_PARAMETERS d3dpp{};
	d3dpp.Windowed = isWindowed;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = BACKBUFFERWIDTH;;
	d3dpp.BackBufferHeight = BACKBUFFERHEIGHT;
	d3dpp.hDeviceWindow = wndHandle;

	buildDeviceList();
	check_hr(d3d_->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, wndHandle, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &device_));

	//[Learning] - I dont need this object anymore.after creating the device list data andthe d3ddevice
	d3d_->Release();
	d3d_ = nullptr;

	return true;
}

void D3DApp::render(void) {

	device_->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(107, 140, 255), 1.0f, 0);

	if (!backbuffer_) {
		check_hr(device_->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer_));
	}

	device_->StretchRect(offscreenSurface_,NULL, backbuffer_, NULL, D3DTEXF_NONE);
	device_->Present(NULL, NULL, NULL, NULL);
}

void D3DApp::loadSurface(void) {


	check_hr(device_->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer_));
	check_hr(backbuffer_->GetDesc(&backbufferDescription_));
	if (backbuffer_) {
		backbuffer_->Release();
		backbuffer_ = nullptr;
	}

	check_hr(device_->CreateOffscreenPlainSurface(backbufferDescription_.Width, backbufferDescription_.Height,
		D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &texSurface_, NULL));


	RECT srcRect = { 0, 0, 191, 315 };// full size of sprite sheet
	RECT dstRect = { 0, 0, 191, 315 };
	std::wstring image1 = L".\\assets\\mario_tiles.bmp";
	check_hr(D3DXLoadSurfaceFromFileW(texSurface_, NULL, &dstRect, image1.c_str(), &srcRect, D3DX_DEFAULT, 0, NULL));


	check_hr(device_->CreateRenderTarget(backbufferDescription_.Width, backbufferDescription_.Height,
		D3DFMT_X8R8G8B8, D3DMULTISAMPLE_NONE, 0, FALSE, &offscreenSurface_, NULL));

	D3DCOLOR color = D3DCOLOR_XRGB(107, 140, 255);
	check_hr(device_->ColorFill(offscreenSurface_, nullptr, color));


	int scale = 4;
	int sSize = 7;
	int sqrSize = scale * sSize;
	int startOffsetX = 40;
	int startOffsetY = 2;
	RECT srcTileRect;
	RECT dstTileRect;
	int height = static_cast<int>(backbufferDescription_.Height);
	int width = static_cast<int>(backbufferDescription_.Width);

	//int width = 640;
	//int height = 480;

	for (size_t i = 0; i < 4; i++) {

		if (i == 0) {
			srcTileRect = { startOffsetX, startOffsetY, startOffsetX + sSize, startOffsetY + sSize };
			dstTileRect = { 0, height - sqrSize * 2 , sqrSize, height - sqrSize };
		}
		else if (i == 1) {
			srcTileRect = { startOffsetX + sSize + 2, startOffsetY, startOffsetX + sSize * 2 + 2, startOffsetY + sSize };
			dstTileRect = { sqrSize, height - sqrSize * 2 , sqrSize * 2, height - sqrSize };
		}
		else if (i == 2) {
			srcTileRect = { startOffsetX, startOffsetY + sSize + 2 , startOffsetX + sSize, startOffsetY + sSize * 2 + 2 };
			dstTileRect = { 0, height - sqrSize , sqrSize, height };
		}
		else if (i == 3) {
			srcTileRect = { startOffsetX + sSize + 2, startOffsetY + sSize + 2, startOffsetX + sSize * 2 + 2, startOffsetY + sSize * 2 + 2 };
			dstTileRect = { sqrSize, height - sqrSize , sqrSize * 2, height };
		}

		check_hr(device_->StretchRect(texSurface_, &srcTileRect, offscreenSurface_,&dstTileRect, D3DTEXF_NONE));
	}
}

HRESULT D3DApp::buildDeviceList(void)
{
	adapterCount_ = d3d_->GetAdapterCount();
	check_nonzero(adapterCount_);

	adapters_.resize(adapterCount_);

	for (size_t iAdapter = 0; iAdapter < adapterCount_; iAdapter++){
		D3DAdapterInfo* pAdapter = &adapters_[iAdapter]; 

		d3d_->GetAdapterIdentifier(iAdapter, 0, &pAdapter->adapterIdentifier);
		if (!&pAdapter->adapterIdentifier) {
			continue;
		}

		d3d_->GetAdapterDisplayMode(iAdapter, &pAdapter->desktopCurrentMode);
		if (!&pAdapter->desktopCurrentMode) {
			continue;
		}

		//Find other modes that share current format i.e X8R8G8B8
		uint32_t	rawNumModes{ 0 };
		D3DFORMAT currentFormat = pAdapter->desktopCurrentMode.Format;
		rawNumModes = d3d_->GetAdapterModeCount(iAdapter, currentFormat);

		//Populate Modes
		std::vector<D3DDISPLAYMODE> rawModes;
		rawModes.resize(rawNumModes);
		for (size_t iMode = 0; iMode < rawNumModes; ++iMode) {
			int result = d3d_->EnumAdapterModes(iAdapter, currentFormat, iMode, &rawModes[iMode]);
		}

		//Select Valid Modes.
		for (const auto& mode : rawModes) {
			if (mode.Width > 800 && mode.Height > 600 &&
				(mode.RefreshRate == 59 || mode.RefreshRate == 75)) {
				pAdapter->modes.push_back(mode);
			}
		}

		pAdapter->numModes = static_cast<uint32_t>(pAdapter->modes.size());
	}

#ifdef _DEBUG
	std::println("NumberOfAdapters: {}", adapterCount_);
	for (size_t iAdapter = 0; iAdapter < adapterCount_; iAdapter++) {
		std::println("For Adapter {}", iAdapter);
		std::println("\tDevice ID: {}", adapters_[iAdapter].adapterIdentifier.DeviceId);
		std::println("\tDevice name: {}", adapters_[iAdapter].adapterIdentifier.DeviceName);
		std::println("\tDevice Description: {}", adapters_[iAdapter].adapterIdentifier.Description);
		std::println("\tDisplay Mode");
		std::println("\t\tWidth {}", adapters_[iAdapter].desktopCurrentMode.Width);
		std::println("\t\tHeight {}", adapters_[iAdapter].desktopCurrentMode.Height);
		std::println("\t\tRefreshRate {}", adapters_[iAdapter].desktopCurrentMode.RefreshRate);
		std::println("\t\tFormat {}", static_cast<int>(adapters_[iAdapter].desktopCurrentMode.Format));
		std::println("\tAdapterModeCount: {}", adapters_[iAdapter].numModes);

		for (size_t iMode = 0; iMode < adapters_[iAdapter].numModes; iMode++) {
			std::println("\tMode {}", iMode);
			std::println("\t\tWidth {}", adapters_[iAdapter].modes[iMode].Width);
			std::println("\t\tHeight {}", adapters_[iAdapter].modes[iMode].Height);
			std::println("\t\tRefreshRate {}", adapters_[iAdapter].modes[iMode].RefreshRate);
		}
	}
#endif //_DEBUG

	return S_OK;
}

void D3DApp::cleanUp(void)
{
	if (device_) {
		device_->Release();
		device_ = nullptr;
	}

	// Redudant release, keeping it for safety.
	if (d3d_) {
		d3d_->Release();
		d3d_ = nullptr;
	}

	if (offscreenSurface_) {
		offscreenSurface_->Release();
		offscreenSurface_ = nullptr;
	}
	if (backbuffer_) {
		backbuffer_->Release();
		backbuffer_ = nullptr;
	}
}