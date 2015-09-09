//CHAPTER 3
//Direct3D_windowed
#include <d3d9.h>
#include <Windows.h>
#include <time.h>
#include <iostream>

using namespace std;

#pragma comment(lib,"d3d9.lib")


//program settings
const string TITLE = "Direct3D_Windowed";
const int SCREENW = 1024;
const int SCREENH = 768;

LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;

bool GameOver = false;

//macro to detect key press
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?1:0)

bool Game_Init(HWND window)
{
	MessageBox(window, "Game_Init", "BreakPOINT", 0);

	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d == NULL) {
		MessageBox(window, "Error Initializing Direc3d", "Error", MB_OK);
		return 0;
	}
	//Settting the direct3d presentation params
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed         = FALSE;
	d3dpp.SwapEffect	   = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount  = 1;
	d3dpp.BackBufferWidth  = SCREENW;
	d3dpp.BackBufferHeight = SCREENH;
	d3dpp.hDeviceWindow	   = window;

	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);
	if (d3ddev == NULL) {
		MessageBox(window, "Error creating Direct3d device", "Error", MB_OK);
		return 0;
	}
	return true;
}
void Game_Run(HWND hwnd) {
	//make sure the d3d is valid
	if (!d3ddev) return;

	//clear the back buffer to bright green
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 0, 0), 1.0f, 0);

	//start rendering
	if (d3ddev->BeginScene()) {
		//do something badass?

		//stop rendering
		d3ddev->EndScene();
		//copy the back buffer onto the screen
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}
	//Watch out fo rthe ESC key
	if (KEY_DOWN(VK_ESCAPE))
		PostMessage(hwnd, WM_DESTROY, 0, 0);
}
//SHUTDOWN Function
void Game_End(HWND hwnd) {
	MessageBox(hwnd, "Shutting Down Program", "Game_End", MB_OK);

	//free the memory
	if (d3ddev) d3ddev->Release();
	if (d3d)d3d->Release();
}

//windows event handling function

LRESULT WINAPI WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_DESTROY:
		GameOver = true;
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//Main window function 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	//set the new Windows properties
	//this was previously found in the Myregisterclass
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = TITLE.c_str();
	wc.hIconSm = NULL;
	RegisterClassEx(&wc);

	//create a new window
	//this was in out initinstance function
	HWND window = CreateWindow(
		TITLE.c_str(),
		TITLE.c_str(),
		WS_EX_TOPMOST|WS_VISIBLE|WS_POPUP,
		CW_USEDEFAULT, CW_USEDEFAULT,
		SCREENW, SCREENH,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (window == 0)return 0;
	
	//display the window
	ShowWindow(window, nCmdShow);
	UpdateWindow(window);
	
	if (!Game_Init(window))return 0;

	//main message loop
	MSG msg;
	while (!GameOver) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Game_Run(window);
	}
	Game_End(window);
	return msg.wParam;
}