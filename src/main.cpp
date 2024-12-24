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
#include <optional>
#include <cstdint>

int main ()
{
	SetConfigFlags(FLAG_VSYNC_HINT);

	InitWindow(1280, 800, "ASDFASDFASDF");
	
	SetExitKey(KEY_NULL);

	SearchAndSetResourceDir("resources");

	Texture t_player = LoadTexture("player.png");
	Texture t_attack = LoadTexture("attack.png");
	Texture t_afterimage = LoadTexture("skill.png");
	Camera2D camera = { 0 };
	camera.target = { 0.0f, 0.0f };
	camera.offset = { 640.0f, 400.0f };
	// camera.offset = { 0.0f, 0.0f };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	Vector2 position = { 0.0f, 0.0f };
	std::vector<std::pair<Vector2,uint16_t>> attacks = std::vector<std::pair<Vector2,uint16_t>>();
	std::optional<std::pair<Vector2,uint16_t>> afterimage = std::optional<std::pair<Vector2,uint16_t>>();
	float player_speed = 5.0f;
	unsigned int frame = 0;

	bool clicking = false, using_skill = false;
	unsigned int skill_cooltime = 0; // cooltime in frames
	const float skill_range = 240.0f;

	SetTargetFPS(60);
	// game loop
	while (!WindowShouldClose())
	{
		frame += 1;
		if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
			if(!clicking){
				attacks.push_back(std::make_pair(GetScreenToWorld2D(GetMousePosition(), camera), 0));
				clicking = true;
			}
		}
		else{
			if(clicking){
				clicking = false;
			}
		}

		using_skill = false;
		if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
			if(skill_cooltime == 0){
				using_skill = true;
				afterimage = std::optional<std::pair<Vector2,uint16_t>>{std::make_pair(position, 0)};
				Vector2 target_position = GetScreenToWorld2D(GetMousePosition(), camera);
				if(Vector2Distance(position, target_position) <= skill_range){
					position = target_position;
				}
				else{
					position = Vector2Add(position, Vector2Scale(Vector2Normalize(Vector2Subtract(target_position, position)), skill_range));
				}

				skill_cooltime = 120;
			}
		}
		if(skill_cooltime > 0){
			skill_cooltime--;
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
		if(skill_cooltime == 0){
			DrawRing(position, skill_range, skill_range+3.0f, 0.0f, 360.0f, 120, {102, 191, 255, 255});
		}
		else{
			DrawRing(position, skill_range, skill_range+3.0f, 0.0f, 360.0f, 120, {192, 100, 100, 255});
			DrawRing(position, skill_range-5.0f, skill_range, -90.0f, -90.0f+3.0f*(120-skill_cooltime), 120, {150, 150, 150, 255});
			// DrawCircleLinesV(position, skill_range, {102, 191, 255, 255});
		}
		if(afterimage.has_value()){
			uint16_t anim_idx = (*afterimage).second / 2;
			if(anim_idx <= 3){
				*afterimage = std::make_pair((*afterimage).first, (*afterimage).second+1);
				DrawTextureRec(t_afterimage, {64.0f*anim_idx, 0.0f, 64.0f, 64.0f}, Vector2Add((*afterimage).first, {-32.0f, -32.0f}),WHITE);
			}
			else{
				afterimage = std::nullopt;
			}
		}
		DrawTextureRec(t_player, {64.0f*((frame/10)%2), 0.0f, 64.0f, 128.0f}, {position.x-32.0f, position.y-64.0f}, WHITE);

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
