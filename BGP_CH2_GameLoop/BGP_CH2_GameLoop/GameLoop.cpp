#include <Windows.h>
#include <iostream>
#include <time.h>
using namespace std;

const string TITLE = "GAME LOOP";
HWND window;
HDC device;
bool GameOver = false;

void DrawBitmap(char*filename, int x, int y)
{
	HBITMAP image = (HBITMAP)LoadImage(0, "c.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	BITMAP bm;
	GetObject(image, sizeof(BITMAP), &bm);

	HDC hdcImage = CreateCompatibleDC(device);
	SelectObject(hdcImage, image);

	BitBlt(
		device,
		x, y,
		bm.bmWidth, bm.bmHeight,
		hdcImage,
		0, 0,
		SRCCOPY);

	DeleteDC(hdcImage);
	DeleteObject((HBITMAP)image);
}

//Start up and loading the code
bool Game_Init() 
{
	//rand number generator 
	srand(time(NULL));

	return 1;
}

//Update function called inside the game loop

void Game_Run() 
{
	if (GameOver == true) return;
	
	RECT rect;
	GetClientRect(window, &rect);

	int x = rand() % (rect.right - rect.left);
	int y = rand() % (rect.bottom - rect.top);
	DrawBitmap("c.bmp", x, y);
}

void Game_End() {
	ReleaseDC(window, device);
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		GameOver = true;
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
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
	return RegisterClassEx(&wc);
}

bool InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	window = CreateWindow(
		TITLE.c_str(),
		TITLE.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		1000, 900,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (window == 0)return 0;

	ShowWindow(window, nCmdShow);
	UpdateWindow(window);
	device = GetDC(window);

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	MyRegisterClass(hInstance);
	if (!InitInstance(hInstance, nCmdShow))return 0;

	if (!Game_Init) return 0;

	//main message loop
	while (!GameOver) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Game_Run();
	}
	Game_End();
	return msg.wParam;
}