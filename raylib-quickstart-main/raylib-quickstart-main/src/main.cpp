/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/
*/

#include "raylib.h"
#include "resource_dir.h"// utility header for SearchAndSetResourceDir
#define MAX_FRAME_SPEED     15
#define MIN_FRAME_SPEED      1
class player
{
public:
	int x;
	int y;
	int vx;
	int vy;
	bool canJump;
	void jump() {
		vy = 30;
	}
};


int main()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	// Create the window and OpenGL context
	const int screenWidth = 1920;
	const int screenHeight = 1080;
	InitWindow(1920, 1080, "Metal Slug");
	const char msg[256] = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHI\nJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmn\nopqrstuvwxyz{|}~¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓ\nÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷\nøùúûüýþÿ";
	ToggleFullscreen();
	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");
	// Load a texture from the resources directory
	Texture p1 = LoadTexture("treure fons.png");
	// game loop
	player p = {0,0,0,0, true};
	Vector2 position = { p.x, p.y };
	Rectangle frameRec = { p.x, p.y, (float)p1.width / 9, (float)p1.height };
	int currentFrame = 0;
	int framesCounter = 0;
	int framesSpeed = 3;            // Number of spritesheet frames shown by second
	Font fontBm = LoadFont("font.fnt");
	SetTextureFilter(fontBm.texture, TEXTURE_FILTER_POINT);
	// NOTE: We define a font base size of 32 pixels tall and up-to 250 characters
	SetTextLineSpacing(16);         // Set line spacing for multiline text (when line breaks are included '\n')
	bool useTtf = false;	

	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		framesCounter++;

		if (framesCounter >= (60 / framesSpeed))
		{
			framesCounter = 0;
			currentFrame++;

			if (currentFrame > 9) currentFrame = 0; // el primer numero ha de ser el mateix que les divisions de la sprite sheet

			frameRec.x = (float)currentFrame * (float)p1.width / 9; //numero divisons sprite sheet.
		}
		
		p.x += p.vx;
		p.y -= p.vy;
		
		if (p.y < 500)														//Gravity
		{
			p.canJump = false;
			if (p.vy > -8)
			{
				p.vy--;
			}
		} else {
			p.canJump = true;
			p.vy = 0;
		}
		if (IsKeyDown(KEY_D) && p.vx < 5 && !IsKeyDown(KEY_A))				//Movement X
		{
			p.vx++;
		}
		else if (IsKeyDown(KEY_A) && p.vx > -5 && !IsKeyDown(KEY_D)){
			p.vx--;
		}
		else if (!IsKeyDown(KEY_D) && !IsKeyDown(KEY_A)){
			p.vx = 0;
		}
		if (IsKeyDown(KEY_W)&&p.canJump)								    //Jump
		{
			p.jump();
		}

		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(WHITE);

		position.x = p.x;
		position.y = p.y;

		// draw our texture to the screen

		DrawTextureRec(p1, frameRec, position, WHITE);  // Draw part of the texture
			
		DrawTextEx(fontBm, "HELLO 123!?", { 100,100 }, 32, 0, BLACK);

		EndDrawing();// end the frame and get ready for the next one  (display frame, poll input, etc...)
	}
	UnloadTexture(p1);// unload our texture so it can be cleaned up
	CloseWindow();// destroy the window and cleanup the OpenGL context
	UnloadFont(fontBm);     // AngelCode Font unloading
	return 0;
}