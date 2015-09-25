#include "MyDirectX.h"

#include <iostream>
using namespace std;

//Direct3D variables
LPDIRECT3D9 d3d				  = NULL;
LPDIRECT3DDEVICE9 d3ddev	  = NULL;
LPDIRECT3DSURFACE9 backBuffer = NULL;

//DirectInput Variables
LPDIRECTINPUT8 dinput			= NULL;
LPDIRECTINPUTDEVICE8 dimouse	= NULL;
LPDIRECTINPUTDEVICE8 dikeyboard = NULL;
DIMOUSESTATE mouseState;
char keys[256];

/*This function creates the Direct3D object, creates the Direct3D device that we will use
   , and intializes our backbuffer with our d3d device*/
bool Direct3D_Init(HWND window, int width, int height, bool fullscrean) {
	
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d)return false;

	//Settting the direct3d presentation params
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed         = (!fullscrean);
	d3dpp.SwapEffect       = D3DSWAPEFFECT_COPY;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount  = 1;
	d3dpp.BackBufferWidth  = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.hDeviceWindow    = window;

	//create Direc3D device
	d3d->CreateDevice(
		D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL, 
		window, 
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
		&d3dpp, 
		&d3ddev);
	
	if (!d3ddev)return false;

	//Get a pointer to the back buffer
	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
	
	return true;
}
void Direct3D_Shutdown() {
	if (d3d) d3d->Release();
	if (d3ddev)d3ddev->Release();
}

/*This function will draw a SURFACE object. It takes the source(the object we want to draw)
  grabs the dimensions, creates a RECT for the source, RECT for the destination(this is where our
  source RECT will be drawn) then uses StretchRect to put our source into destination*/
void DrawSurface(LPDIRECT3DSURFACE9 dest, float x, float y, LPDIRECT3DSURFACE9 source) {
	//get width and height of the source rectangle
	D3DSURFACE_DESC desc;
	source->GetDesc(&desc);

	//create rects for both
	RECT sourceRect = { 0,0,(long)desc.Width,(long)desc.Height };
	RECT destRect = { (long)x,(long)y,(long)x + desc.Width,(long)y + desc.Height };

	//draw the surface onto the destination
	d3ddev->StretchRect(source, &sourceRect, dest, &destRect, D3DTEXF_NONE);
}

/*This Function creates a surface object from a bitmap file. Find the dimensions of the bitmap,
  create the plain surface, then load the bitmap into the new surface object*/
LPDIRECT3DSURFACE9 LoadSurface(string filename) {
	LPDIRECT3DSURFACE9 image = NULL;

	//Get width and Height of bitmap 
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(), &info);
	if (result != D3D_OK) return NULL;

	//creates a new surface object
	result = d3ddev->CreateOffscreenPlainSurface(
		info.Width,
		info.Height,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&image,
		NULL);
	if (result != D3D_OK) return NULL;

	//load the surface from the file into the newley created surface object
	result = D3DXLoadSurfaceFromFile(
		image,
		NULL,
		NULL,
		filename.c_str(),
		NULL,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(0, 0, 0),
		NULL);
	if (result != D3D_OK) return NULL;

	return image;
}

/* Initialize the DirectInput Object, Make Devices with the object
   (createdevice,setDataFormat,cooplevel,then aquire)*/
bool DirectInput_Init(HWND window) {
	//Initialize the DirectInput object
	HRESULT result = DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&dinput,
		NULL);
	
	//Init the keyboard
	dinput->CreateDevice(GUID_SysKeyboard, &dikeyboard, NULL);
	dikeyboard->SetDataFormat(&c_dfDIKeyboard);
	dikeyboard->SetCooperativeLevel(window, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	dikeyboard->Acquire();

	//Init the mouse
	dinput->CreateDevice(GUID_SysMouse, &dimouse, NULL);
	dimouse->SetDataFormat(&c_dfDIMouse);
	dimouse->SetCooperativeLevel(window, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	dimouse->Acquire();
	
	//Show cursor on the screen or not
	d3ddev->ShowCursor(false);
	return true;
}

void DirectInput_Update() {
	//update the mouse
	dimouse->GetDeviceState(sizeof(mouseState), (LPVOID)&mouseState);

	//update the keyboard
	dikeyboard->GetDeviceState(sizeof(keys), (LPVOID)&keys);
}


/*Return new mouse X coordinate*/
int Mouse_X() {
	return mouseState.lX;
}
/*Return new mouse Y coordinate*/
int Mouse_Y() {
	return mouseState.lY;
}
/*Return the buttons state*/
int Mouse_Button(int button) {
	return mouseState.rgbButtons[button] & 0x80;
}
/*Return what key was pressed*/
int Key_Down(int Key) {
	return(keys[Key] & 0x80);
}

/*Unaquire the devices then Release them and set the pointers to NULL*/
void DirectInput_Shutdown() {
	if (dimouse) {
		dimouse->Unacquire();
		dimouse->Release();
		dimouse = NULL;
	}
	if (dikeyboard) {
		dikeyboard->Unacquire();
		dikeyboard->Release();
		dikeyboard = NULL;
	}
}