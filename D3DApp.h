#pragma once
#include "WinApp.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <fstream>
#include <string>
#include <print>
#include <iostream>
#include <vector>
#include <cstdint>

//Class holding information about adapters.
struct D3DAdapterInfo
{
	D3DADAPTER_IDENTIFIER9 adapterIdentifier;
	D3DDISPLAYMODE desktopMode;      
	uint32_t	numModes;
	std::vector<D3DDISPLAYMODE> validModes;
};

class D3DApp {

public:
	D3DApp(HINSTANCE hInstance, int nCmdShow);
	~D3DApp() = default;

	WinApp winApp_;

	IDirect3D9* d3d_;
	IDirect3D9* device_;
	IDirect3D9* backbuffer_;
	D3DSURFACE_DESC backbufferDescription_;

	uint32_t adapterCount_;
	std::vector<D3DAdapterInfo>	adapters_;

	bool initDirect3D(HWND wndHandle, bool windowed);
	void render(void);
	void cleanUp(void);

	HRESULT BuildDeviceList();
private:
	
	IDirect3DSurface9* offscreenSurface_;
};
