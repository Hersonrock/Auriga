#include "D3DApp.h"

D3DApp::D3DApp(HINSTANCE hInstance, int nCmdShow)
	: winApp_(hInstance, nCmdShow, BACKBUFFERWIDTH, BACKBUFFERHEIGHT),
	  d3d_(nullptr),
	  device_(nullptr),
	  adapterCount_(0)
{

}

D3DApp::~D3DApp()
{
	cleanUp();
}

int D3DApp::run() {
	
	
	initDirect3D(winApp_.windowHandle_, true);
	BuildDeviceList();
	winApp_.messageLoop();

	return 	0;
}

bool D3DApp::initDirect3D(HWND wndHandle, bool windowed) {

	if (NULL == (d3d_ = Direct3DCreate9(D3D_SDK_VERSION)))
		return false;

	D3DPRESENT_PARAMETERS d3dpp{};
	d3dpp.Windowed = windowed;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = BACKBUFFERWIDTH;
	d3dpp.BackBufferHeight = BACKBUFFERHEIGHT;
	d3dpp.hDeviceWindow = wndHandle;

	if (FAILED(d3d_->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_REF,
		wndHandle,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&device_)))
	{
		return false;
	}
	/////Surface handling
	//device_->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer_);
	//backbuffer_->GetDesc(&backbufferDescription_);

	//if (FAILED(device_->CreateOffscreenPlainSurface(backbufferDescription_.Width, backbufferDescription_.Height, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &offscreenSurface_, NULL))) {

	//	return false;
	//}
	//RECT destRect = { 0, 0, 640, 389 };
	//std::wstring image1 = L"Segfault.bmp";
	//if (FAILED(D3DXLoadSurfaceFromFileW(offscreenSurface_, NULL, &destRect, image1.c_str(), NULL, D3DX_DEFAULT, 0, NULL))) {
	//	return false;
	//}
	///

	return TRUE;
}

void D3DApp::render(void) {

	if (NULL == device_)
		return;
	device_->Clear(0, NULL, D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	RECT srcRect = { 0, 0, 640, 389 };
	device_->StretchRect(offscreenSurface_, &srcRect, backbuffer_, NULL, D3DTEXF_NONE);

	device_->Present(NULL, NULL, NULL, NULL);
}

HRESULT D3DApp::BuildDeviceList()
{
	adapterCount_ = d3d_->GetAdapterCount();
	if (adapterCount_ == 0)
		return E_FAIL;
	adapters_.resize(adapterCount_);

#ifdef _DEBUG
	std::println("m_dwNumAdapters: {}", adapterCount_);
#endif //_DEBUG



	for (UINT iAdapter = 0; iAdapter < adapterCount_; iAdapter++)
	{
		D3DAdapterInfo* pAdapter = &adapters_[iAdapter]; // The address of each one of the elements in m_Adapters array

		// Populate the D3DADAPTER_IDENTIFIER9 structure 
		//Device name(display),Description(Graphics card), Driver,
		if (FAILED(d3d_->GetAdapterIdentifier(iAdapter, 0, &pAdapter->adapterIdentifier)))
			continue;

		if (FAILED(d3d_->GetAdapterDisplayMode(iAdapter, &pAdapter->desktopMode)))
			continue;

		//From current mode
		D3DFORMAT currentFormat = pAdapter->desktopMode.Format;
		pAdapter->numModes = d3d_->GetAdapterModeCount(iAdapter, currentFormat);
		std::vector<D3DDISPLAYMODE> modes;
		modes.resize(pAdapter->numModes);

		for (size_t iMode = 0; iMode < pAdapter->numModes; ++iMode) {
			d3d_->EnumAdapterModes(iAdapter, currentFormat, iMode, &modes[iMode]);
		}

		for (const auto& mode : modes) {
			if (mode.Width > 800 && mode.Height > 600 &&
				(mode.RefreshRate == 59 || mode.RefreshRate == 75)) {
				pAdapter->validModes.push_back(mode);
			}
		}
		pAdapter->numModes = static_cast<uint32_t>(pAdapter->validModes.size());
	}


#ifdef _DEBUG
	for (size_t iAdapter = 0; iAdapter < adapterCount_; iAdapter++) {
		std::println("For Adapter {}", iAdapter);
		std::println("\tDevice ID: {}", adapters_[iAdapter].adapterIdentifier.DeviceId);
		std::println("\tDevice name: {}", adapters_[iAdapter].adapterIdentifier.DeviceName);
		std::println("\tDevice Description: {}", adapters_[iAdapter].adapterIdentifier.Description);
		std::println("\tDisplay Mode");
		std::println("\t\tWidth {}", adapters_[iAdapter].desktopMode.Width);
		std::println("\t\tHeight {}", adapters_[iAdapter].desktopMode.Height);
		std::println("\t\tRefreshRate {}", adapters_[iAdapter].desktopMode.RefreshRate);
		std::println("\t\tFormat {}", static_cast<int>(adapters_[iAdapter].desktopMode.Format));
		std::println("\tAdapterModeCount: {}", adapters_[iAdapter].numModes);
		for (size_t iMode = 0; iMode < adapters_[iAdapter].numModes; iMode++) {
			if (adapters_[iAdapter].validModes[iMode].Width <= 800 || adapters_[iAdapter].validModes[iMode].Height <= 600) {
				continue;
			}
			if (adapters_[iAdapter].validModes[iMode].RefreshRate != 59 && adapters_[iAdapter].validModes[iMode].RefreshRate != 75) {
				continue;
			}

			std::println("\tMode {}", iMode);
			std::println("\t\tWidth {}", adapters_[iAdapter].validModes[iMode].Width);
			std::println("\t\tHeight {}", adapters_[iAdapter].validModes[iMode].Height);
			std::println("\t\tRefreshRate {}", adapters_[iAdapter].validModes[iMode].RefreshRate);
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