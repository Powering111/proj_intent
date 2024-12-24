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
#include "raymath.h"

#include "resource_dir.h"
#include <deque>
#include <vector>
#include <cstdint>

int main ()
{
	SetConfigFlags(FLAG_VSYNC_HINT);

	InitWindow(1280, 800, "ASDFASDFASDF");
	
	SetExitKey(KEY_NULL);

	SearchAndSetResourceDir("resources");

	Texture t_player = LoadTexture("player.png");
	Texture t_attack = LoadTexture("attack.png");
	Camera2D camera = { 0 };
	camera.target = { 0.0f, 0.0f };
	camera.offset = { 640.0f, 400.0f };
	// camera.offset = { 0.0f, 0.0f };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	Vector2 position = { 0.0f, 0.0f };
	std::deque<Vector2> trails = std::deque<Vector2>();
	std::vector<std::pair<Vector2,uint16_t>> attacks = std::vector<std::pair<Vector2,uint16_t>>();

	float player_speed = 5.0f;
	unsigned int frame = 0;
	bool clicking = false;

	SetTargetFPS(60);
	// game loop
	while (!WindowShouldClose())
	{
		frame += 1;
		if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
			if(!clicking){
				trails.clear();
			}
			trails.push_back(GetScreenToWorld2D(GetMousePosition(), camera));
			while(trails.size() > 20){
				trails.pop_front();
			}
			clicking = true;
		}
		else{
			if(clicking){
				attacks.push_back(std::make_pair(GetScreenToWorld2D(GetMousePosition(), camera), 0));
			}
			if(!trails.empty()) trails.pop_front();
			clicking = false;
		}

		// player move
		Vector2 next_vector = {0.0f, 0.0f};
		if(IsKeyDown(KEY_A)) next_vector.x -= 1.0f;
		if(IsKeyDown(KEY_D)) next_vector.x += 1.0f;
		if(IsKeyDown(KEY_W)) next_vector.y -= 1.0f;
		if(IsKeyDown(KEY_S)) next_vector.y += 1.0f;
		position = Vector2Add(position, Vector2Scale(next_vector, player_speed));

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
		DrawText(TextFormat("Frame %u", frame), 0, 0, 20, WHITE);
		DrawText(TextFormat("%d FPS", GetFPS()), 0, 20, 20, WHITE);
		
		BeginMode2D(camera);

		DrawCircle(0, 0, 3.0f, RED);
		DrawCircle(1280, 800, 3.0f, PURPLE);
		DrawText("HIHI world!!!!", -100,20,30,WHITE);
		
		// Render player
		DrawTextureRec(t_player, {64.0f*((frame/10)%2), 0.0f, 64.0f, 128.0f}, {position.x, position.y}, WHITE);

		// Render trail
		int i = 0;
		for(std::deque<Vector2>::iterator trail_pos = trails.begin(); trail_pos != trails.end();){
			Vector2 curr_pos = *trail_pos;
			float curr_radius = (float)(i+1) / 3.0f;
			DrawCircle(curr_pos.x, curr_pos.y, curr_radius, {102, 191, 255, 255});

			trail_pos++;
			if(trail_pos != trails.end()){
				Vector2 next_pos = *trail_pos;
				DrawLineEx(curr_pos, next_pos, curr_radius*2.0f, {102, 191, 255, 255});
			}
			i++;
		}

		// Render attacks
		for(std::vector<std::pair<Vector2, uint16_t>>::iterator attack = attacks.begin(); attack != attacks.end(); ){
			uint16_t anim_idx = (*attack).second / 2;
			if(anim_idx > 4){
				// destroy attack
				attack = attacks.erase(attack);
			}
			else{
				// DrawTextureRec(t_attack, {64.0f * anim_idx, 0.0f, 64.0f, 64.0f}, Vector2Add((*attack).first, {-32.0f, -32.0f}), WHITE);
				DrawTexturePro(t_attack, {64.0f * anim_idx, 0.0f, 64.0f, 64.0f}, {(*attack).first.x-64.0f, (*attack).first.y-64.0f, 128.0f, 128.0f}, {0.0f, 0.0f}, 0.0f, WHITE);
				*attack = std::make_pair((*attack).first, (*attack).second + 1);
				attack++;
			}
		}

		EndMode2D();

		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(t_player);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
