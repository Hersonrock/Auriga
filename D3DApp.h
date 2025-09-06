#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <fstream>
#include <string>
#include <print>
#include <iostream>
#include <vector>
#include <cstdint>
#include "DxErr.h"
#include "WinApp.h"
#include "ErrorHandler.h"

#define BACKBUFFERWIDTH  640
#define BACKBUFFERHEIGHT  480

struct D3DAdapterInfo;

class D3DApp {

public:
	D3DApp(HINSTANCE hInstance, int nCmdShow);
	~D3DApp();

	WinApp winApp_;

	IDirect3D9* d3d_;
	IDirect3DDevice9* device_;
	IDirect3DSurface9* backbuffer_;
	D3DSURFACE_DESC backbufferDescription_;

	uint32_t adapterCount_;
	std::vector<D3DAdapterInfo>	adapters_;

	bool initDirect3D(HWND wndHandle, bool isWindowed);
	void render(void);
	void cleanUp(void);

	HRESULT buildDeviceList(void);
	void loadSurface(void);

	int run();

private:
	
	IDirect3DSurface9* offscreenSurface_;
};
