#include "MyDirectX.h"
#include <iostream>
using namespace std;

const string TITLE = "Bomb Catcher Game";
const int SCREENH = 1024;
const int SCREENW = 768;

LPDIRECT3DSURFACE9 bombSurface = NULL;
LPDIRECT3DSURFACE9 bucketSurface = NULL;

struct BOMB {
	float x, y;
	void reset() {
		x = (float)(rand() % (SCREENW - 128)); 
		y = 0;
	}
};
BOMB bomb;
struct BUCKET {
	float x, y;
};
BUCKET bucket;

int score = 0;
int lives = 3;

bool Game_Init(HWND window) {
	/*Init Direct3D and Direct3DInput*/
	Direct3D_Init(window, SCREENW, SCREENH, false);
	DirectInput_Init(window);

	/*Load the surfaces into their objects*/
	bombSurface = LoadSurface("bomb.bmp");
	if (!bombSurface) {
		MessageBox(window, "Error Loading Bomb Image", "Error", 0);
		return false;
	}
	bucketSurface = LoadSurface("bucket.bmp");
	if (!bucketSurface) {
		MessageBox(window, "Error Loading Bucket Image", "Error", 0);
		return false;
	}

	//get the back buffer surface
	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	//postion the bomb
	srand((unsigned int)time(NULL));
	bomb.reset();

	//positon the bucket
	bucket.x = 500;
	bucket.y = 630;

	return true;
}

void Game_Run(HWND window) {
	float Bspeed = 2.0f;
	float Buspeed = 6.0f;
	Bspeed += score/2;
	Buspeed += score/2;

	//Make sure the Direct3D device is valid
	if (!d3ddev) return;

	//update the input device
	DirectInput_Update();

	//move the bomb down the screen
	bomb.y += Bspeed;

	//see if the bomb hit the floor
	if (bomb.y > SCREENH) {
		lives--;
		char s[255];
		sprintf_s(s, "%s [SCORE %d] [LIVES %d]", TITLE.c_str(), score, lives);
		SetWindowText(window, s);
		if (lives < 1) {
			MessageBox(window, "BOOOOOOOOOOOOOOOOMMMMMMMM!!", "Get Better Scrub", 0);
			GameOver = true;
		}
		bomb.reset();
	}

	//move the bucket with the mouse
	int mx = Mouse_X();
	if (mx < 0) bucket.x -= Buspeed;
	else if (mx>0) bucket.x += Buspeed;

	//move the bucket with the Keyboard
	if (Key_Down(DIK_A)) bucket.x -= Buspeed;
	else if (Key_Down(DIK_D)) bucket.x += Buspeed;

	//Keep the bucket inside the screen 
	if (bucket.x > SCREENW - 180)bucket.x = SCREENW - 180;
	else if (bucket.x < 0) bucket.x = 0;

	//see if bucket caught the bomb
	int cx = bomb.x + 64;
	int cy = bomb.y + 64;
	if (cx > bucket.x && cx<bucket.x + 128 && cy>bucket.y && cy < bucket.y + 128) {
		//Update and display the score
		score++;
		char s[255];
		sprintf_s(s, "%s [SCORE %d] [LIVES %d]", TITLE.c_str(), score, lives);
		SetWindowText(window, s);

		//restart Bomb
		bomb.reset();
	}

	//clear the back buffer
	d3ddev->ColorFill(backBuffer, NULL, D3DCOLOR_XRGB(0, 0, 0));

	//Start rendering the screen
	if (d3ddev->BeginScene()) {
		//draw the bomb
		DrawSurface(backBuffer, bomb.x, bomb.y, bombSurface);

		//draw the bucket
		DrawSurface(backBuffer, bucket.x, bucket.y, bucketSurface);

		//stop Rendering
		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}
	//Escape_Key means to quit
	if (Key_Down(DIK_ESCAPE)) { GameOver = true; }
}

void Game_End() {
	if (bombSurface) bombSurface->Release();
	if (bucketSurface) bucketSurface->Release();
	//Shutdown the other objects
	DirectInput_Shutdown();
	Direct3D_Shutdown();
}