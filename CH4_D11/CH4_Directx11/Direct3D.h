//Directx3D functions and variables
#include <Windows.h>
#include<d3d11.h>
#include<dxgi.h>
#include<assert.h>



ID3D11Device* mD3dDevice;
ID3D11DeviceContext* mD3dContext;
IDXGISwapChain* mSwapChain;
D3D_FEATURE_LEVEL featureLevel;

ID3D11RenderTargetView* mRenderTargetView;



bool Direct3D_Init(HWND window, UINT height, UINT width, bool fullscrean, bool msaa);
void Direct3D_Shutdown();



#pragma once
