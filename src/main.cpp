#include "raylib.h"
#include "raymath.h"

#include "resource_dir.h"
#include <deque>
#include <vector>
#include <optional>
#include <cstdint>
#include <map>
#include "texture.hpp"
#include "entity.hpp"

TextureManager textureManager = TextureManager();
EntityManager entityManager = EntityManager();

int main(){
	SetConfigFlags(FLAG_VSYNC_HINT);

	InitWindow(1280, 800, "ASDFASDFASDF");
	
	SetExitKey(KEY_NULL);

	SearchAndSetResourceDir("resources");
	textureManager.load_textures();

	Camera2D camera = { 0 };
	camera.target = { 0.0f, 0.0f };
	camera.offset = { 640.0f, 400.0f };
	// camera.offset = { 0.0f, 0.0f };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	Vector2 position = { 0.0f, 0.0f };
	std::vector<std::pair<Vector2,uint16_t>> attacks = std::vector<std::pair<Vector2,uint16_t>>();
	std::optional<std::pair<Vector2,uint16_t>> afterimage = std::optional<std::pair<Vector2,uint16_t>>();

	unsigned int frame = 0;

	bool clicking = false;
	Player player({0.0f, 0.0f});

	entityManager.add_entity(&player);
	SetTargetFPS(60);
	// game loop
	while (!WindowShouldClose())
	{
		frame += 1;
		if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
			if(!clicking){
				player.attack(GetScreenToWorld2D(GetMousePosition(), camera));
				clicking = true;
			}
		}
		else{
			if(clicking){
				clicking = false;
			}
		}

		if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
			player.use_skill(GetScreenToWorld2D(GetMousePosition(), camera));
		}

		// player move
		Vector2 move_direction = {0.0f, 0.0f};
		if(IsKeyDown(KEY_A)) move_direction.x -= 1.0f;
		if(IsKeyDown(KEY_D)) move_direction.x += 1.0f;
		if(IsKeyDown(KEY_W)) move_direction.y -= 1.0f;
		if(IsKeyDown(KEY_S)) move_direction.y += 1.0f;
		player.move(move_direction);

		if(IsKeyDown(KEY_LEFT)) camera.target.x -= 20.0f;
		if(IsKeyDown(KEY_RIGHT)) camera.target.x += 20.0f;
		if(IsKeyDown(KEY_UP)) camera.target.y -= 20.0f;
		if(IsKeyDown(KEY_DOWN)) camera.target.y += 20.0f;

		entityManager.update();

		// drawing
		BeginDrawing();

		ClearBackground(BLACK);

		// UI
		DrawCircle(640, 400, 3.0f, BLUE);
		DrawCircle(0, 0, 10.0f, GREEN);
		DrawCircle(1280, 800, 10.0f, GREEN);
		DrawText(TextFormat("Frame %u", frame), 0, 0, 20, WHITE);
		DrawText(TextFormat("%d FPS", GetFPS()), 0, 20, 20, WHITE);
		
		// World
		BeginMode2D(camera);

		DrawCircle(0, 0, 3.0f, RED);
		DrawCircle(1280, 800, 3.0f, PURPLE);
		DrawText("HIHI world!!!!", -100,20,30,WHITE);
		
		// Render player
		if(afterimage.has_value()){
			uint16_t anim_idx = (*afterimage).second / 2;
			if(anim_idx <= 3){
				*afterimage = std::make_pair((*afterimage).first, (*afterimage).second+1);
				textureManager.get_texture(TextureID::Skill).render((*afterimage).first, 1.0f, anim_idx);
			}
			else{
				afterimage = std::nullopt;
			}
		}
		// player.draw();
		entityManager.draw();

		// Render attacks
		for(std::vector<std::pair<Vector2, uint16_t>>::iterator attack = attacks.begin(); attack != attacks.end(); ){
			uint16_t anim_idx = (*attack).second / 2;
			if(anim_idx > 4){
				// destroy attack
				attack = attacks.erase(attack);
			}
			else{
				// DrawTextureRec(t_attack, {64.0f * anim_idx, 0.0f, 64.0f, 64.0f}, Vector2Add((*attack).first, {-32.0f, -32.0f}), WHITE);
				textureManager.get_texture(TextureID::Attack).render((*attack).first,2.0f,anim_idx);
				*attack = std::make_pair((*attack).first, (*attack).second + 1);
				attack++;
			}
		}


		EndMode2D();

		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	textureManager.unload_textures();

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
