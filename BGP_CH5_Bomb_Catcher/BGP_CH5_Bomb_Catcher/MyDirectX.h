#pragma once
#define WIN_EXTRA_lEAN
#define DIRECTINPUT_VERSION 0X0800
#include <d3d9.h>
#include <Windows.h>
#include <time.h>
#include <dinput.h>
#include <d3dx9>
#include <iostream>
#include <iomanip>
using namespace std;

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"winmm.lib")

//Program values
extern const string TITLE;
extern const int SCREENH;
extern const int SCREENW;
extern bool GameOver;

//Direct3D objects
extern LPDIRECT3D9 d3d;
extern LPDIRECT3DDEVICE9 d3ddev;
extern LPDIRECT3DSURFACE9 backBuffer;

//Direct3D functions
bool Direct3D_Init(HWND hWnd, int width, int height, bool fullscrean);
void Direct3D_Shutdown();
LPDIRECT3DSURFACE9 LoadSurface(string filename);
void DrawSurface(LPDIRECT3DSURFACE9 dest, float x, float y, LPDIRECT3DSURFACE9 source);

//DirectInput Objects
extern LPDIRECTINPUT8 dinput;
extern LPDIRECTINPUTDEVICE8 dimouse;
extern LPDIRECTINPUTDEVICE8 dikeyboard;
extern DIMOUSESTATE mouseState;

//DirectInput Functions
bool DirectInput_Init(HWND);
void DirectInput_Update();
void DirectInput_Shutdown();
int Key_Down(int);
int Mouse_Button(int);
int Mouse_X();
int Mouse_Y();

//game Functions
bool Game_Init(HWND window);
void Game_Run(HWND window);
void Game_End();