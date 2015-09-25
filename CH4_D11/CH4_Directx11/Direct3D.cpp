#include "Direct3D.h"

ID3D11Device* mD3dDevice			= nullptr;
ID3D11DeviceContext* mD3dContext = nullptr;
IDXGISwapChain* mSwapChain		= nullptr;
D3D_FEATURE_LEVEL featureLevel;

ID3D11RenderTargetView* mRenderTargetView =nullptr;


bool Direct3D_Init(HWND window, UINT height, UINT width, bool fullscrean, bool msaa) {

	//Create the Direct3D device
	HRESULT result=D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&mD3dDevice,
		&featureLevel,
		&mD3dContext);

	if (FAILED(result)) {
		MessageBox(0, L"Faile to initialize Direct3D Device", 0, 0);
		return false;
	}
	if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
		MessageBox(0, L"Direct3D Feature Level 11 unsupported", 0, 0);
		return false;
	}

	//See if 4XMSAA is supported
	UINT m4saa;
	mD3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4saa);
	assert(m4saa > 0);

	DXGI_SWAP_CHAIN_DESC sd;
	//Buffer Description
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	//Are we using  4x msaa?
	if (msaa) {
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m4saa - 1;
	}
	else {
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}
	//Set other paramaters
	sd.BufferCount = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = window;
	sd.Windowed = fullscrean;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags =0;

	//Create the Factory then the Swap chain
	IDXGIDevice* dxgDevice;
	HRESULT result = mD3dDevice->QueryInterface(__uuidof(dxgDevice), (void**)&dxgDevice);

	IDXGIAdapter* dxgAdapter;
	result = dxgDevice->GetParent(__uuidof(dxgAdapter), (void**)&dxgAdapter);

	//Finally got the Factory
	IDXGIFactory* factory;
	result = dxgAdapter->GetParent(__uuidof(factory), (void**)&factory);

	result = factory->CreateSwapChain(mD3dDevice, &sd, &mSwapChain);

	//Release the interfaces we create to find the swap chain
	dxgAdapter->Release();
	dxgDevice->Release();
	factory->Release();

	//Create a RenderTargetView of the back buffer
	ID3D11Texture2D* backBuffer = nullptr;
	mSwapChain->GetBuffer(0, __uuidof(backBuffer), reinterpret_cast<void**>(&backBuffer));
	mD3dDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView);
	backBuffer->Release();

	//Create the Depth/Stencil buffer and View
	D3D11_TEXTURE2D_DESC text;
	text.Width = width;
	text.Height = height;
	text.MipLevels = 1;
	text.ArraySize = 1;
	text.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if (msaa) {
		text.SampleDesc.Count = 4;
		text.SampleDesc.Quality = m4saa - 1;
	}
	else {
		text.SampleDesc.Count = 1;
		text.SampleDesc.Quality = 0;
	}




}

