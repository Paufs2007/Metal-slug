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
int y = 500;
int x;
int yVelocity;
bool jumping = false;
bool jumpE = false;

void jump() {
	jumping = true;
	jumpE = true;
	for (int i = 0; i < 50; i++)
	{
		yVelocity = -20;
	}
	jumping = false;
}


int main()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	// Create the window and OpenGL context
	const int screenWidth = 800;
	const int screenHeight = 450;
	InitWindow(1920, 1080, "Metal Slug");
	ToggleFullscreen();
	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");
	// Load a texture from the resources directory
	Texture p1 = LoadTexture("a1.png");
	Vector2 position = { 350.0f, 280.0f };
	Rectangle frameRec = { 0.0f, 0.0f, (float)p1.width / 6, (float)p1.height };
	int currentFrame = 0;

	int framesCounter = 0;
	int framesSpeed = 8;            // Number of spritesheet frames shown by second

	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
	
	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		framesCounter++;

		if (framesCounter >= (60 / framesSpeed))
		{
			framesCounter = 0;
			currentFrame++;

			if (currentFrame > 5) currentFrame = 0;

			frameRec.x = (float)currentFrame * (float)p1.width / 6;
		}


		y += yVelocity;
		if (IsKeyDown(KEY_D))
		{
			x+=5;
		}
		if (IsKeyDown(KEY_A))
		{
			x-=5;
		}
		if (IsKeyDown(KEY_W)&&jumpE==false)
		{
			jump();

		}else if (y < 500&&jumping==false) {
			if (yVelocity != 5) {
				yVelocity++;
			}
		}else {
			jumpE = false;
			yVelocity = 0;
		}

		BeginDrawing();

		ClearBackground(RAYWHITE);

		DrawTexture(p1, 15, 40, WHITE);
		DrawRectangleLines(15, 40, p1.width, p1.height, LIME);
		DrawRectangleLines(15 + (int)frameRec.x, 40 + (int)frameRec.y, (int)frameRec.width, (int)frameRec.height, RED);

		DrawText("FRAME SPEED: ", 165, 210, 10, DARKGRAY);
		DrawText(TextFormat("%02i FPS", framesSpeed), 575, 210, 10, DARKGRAY);
		DrawText("PRESS RIGHT/LEFT KEYS to CHANGE SPEED!", 290, 240, 10, DARKGRAY);

		for (int i = 0; i < MAX_FRAME_SPEED; i++)
		{
			if (i < framesSpeed) DrawRectangle(250 + 21 * i, 205, 20, 20, RED);
			DrawRectangleLines(250 + 21 * i, 205, 20, 20, MAROON);
		}

		DrawTextureRec(p1, frameRec, position, WHITE);  // Draw part of the texture

		DrawText("(c) Scarfy sprite by Eiden Marsal", screenWidth - 200, screenHeight - 20, 10, GRAY);

		
		EndDrawing();// end the frame and get ready for the next one  (display frame, poll input, etc...)
	}
	UnloadTexture(p1);// unload our texture so it can be cleaned up
	CloseWindow();// destroy the window and cleanup the OpenGL context
	return 0;
}
