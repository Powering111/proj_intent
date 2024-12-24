/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

For a C++ project simply rename the file to .cpp and re-run the build script 

-- Copyright (c) 2020-2024 Jeffery Myers
--
--This software is provided "as-is", without any express or implied warranty. In no event 
--will the authors be held liable for any damages arising from the use of this software.

--Permission is granted to anyone to use this software for any purpose, including commercial 
--applications, and to alter it and redistribute it freely, subject to the following restrictions:

--  1. The origin of this software must not be misrepresented; you must not claim that you 
--  wrote the original software. If you use this software in a product, an acknowledgment 
--  in the product documentation would be appreciated but is not required.
--
--  2. Altered source versions must be plainly marked as such, and must not be misrepresented
--  as being the original software.
--
--  3. This notice may not be removed or altered from any source distribution.

*/

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

int main ()
{
	SetConfigFlags(FLAG_VSYNC_HINT);

	InitWindow(1280, 800, "ASDFASDFASDF");
	
	SetExitKey(KEY_NULL);

	SearchAndSetResourceDir("resources");

	Texture wabbit = LoadTexture("wabbit_alpha.png");
	Camera2D camera = { 0 };
	camera.target = { 0.0f, 0.0f };
	camera.offset = { 640.0f, 400.0f };
	// camera.offset = { 0.0f, 0.0f };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	Vector2 position = { 0.0f, 0.0f };
	// game loop
	while (!WindowShouldClose())
	{
		if(IsKeyDown(KEY_A)) position.x -= 20.0f;
		if(IsKeyDown(KEY_D)) position.x += 20.0f;
		if(IsKeyDown(KEY_W)) position.y -= 20.0f;
		if(IsKeyDown(KEY_S)) position.y += 20.0f;
		
		if(IsKeyDown(KEY_LEFT)) camera.target.x -= 20.0f;
		if(IsKeyDown(KEY_RIGHT)) camera.target.x += 20.0f;
		if(IsKeyDown(KEY_UP)) camera.target.y -= 20.0f;
		if(IsKeyDown(KEY_DOWN)) camera.target.y += 20.0f;

		// drawing
		BeginDrawing();
		ClearBackground(BLACK);
		DrawCircle(640, 400, 3.0f, BLUE);
		DrawCircle(0, 0, 10.0f, GREEN);
		DrawCircle(1280, 800, 10.0f, GREEN);
		
		BeginMode2D(camera);

		DrawCircle(0, 0, 3.0f, RED);
		DrawCircle(1280, 800, 3.0f, PURPLE);
		DrawText("HIHI world!!!!", -100,20,30,WHITE);
		DrawTexture(wabbit, position.x, position.y, WHITE);
		EndMode2D();
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
