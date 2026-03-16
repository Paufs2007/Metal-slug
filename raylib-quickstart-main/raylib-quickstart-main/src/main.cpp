/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/
*/

#include "raylib.h"
#include "resource_dir.h"// utility header for SearchAndSetResourceDir
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
	InitWindow(1920, 1080, "Metal Slug");
	ToggleFullscreen();
	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");
	// Load a texture from the resources directory
	Texture p1 = LoadTexture("p1.2.png");
	// game loop
	player p = {0,0,0,0};

	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
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

		DrawRectangle(0, 775, 1920, 1000, BLACK);

		// draw some text using the default font
		DrawText("sol", 200, 200, 200, RED);

		// draw our texture to the screen
		DrawTexture(p1, p.x, p.y, WHITE);


		EndDrawing();// end the frame and get ready for the next one  (display frame, poll input, etc...)
	}
	UnloadTexture(p1);// unload our texture so it can be cleaned up
	CloseWindow();// destroy the window and cleanup the OpenGL context
	return 0;
}
