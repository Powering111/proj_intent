#include "raylib.h"
#include "raymath.h"
#include "resource_dir.h"

#include "texture.hpp"
#include "entity.hpp"
#include "input.hpp"

TextureManager textureManager = TextureManager();
extern EntityManager<Particle> particleManager;
extern EntityManager<Enemy> enemyManager;
Player player({0.0f, 0.0f});

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

	
	Controller controller(&player, &camera);

	unsigned int frame = 0;

	enemyManager.insert(Enemy({100.0f, 0.0f}));
	enemyManager.insert(Enemy({-100.0f, 0.0f}));


	SetTargetFPS(60);
	// game loop
	while (!WindowShouldClose())
	{
		frame += 1;
		if(IsKeyPressed(KEY_EIGHT)){
			enemyManager.insert(Enemy({(rand()%5-2)*100.0f, (rand()%5-2)*100.0f}));
		}

		controller.handle_input();

		player.update();
		enemyManager.update();
		particleManager.update();

		// drawing
		BeginDrawing();

		ClearBackground(BLACK);

		// UI
		DrawCircle(640, 400, 3.0f, BLUE);
		DrawText(TextFormat("Frame %u", frame), 0, 0, 20, WHITE);
		DrawText(TextFormat("%d FPS", GetFPS()), 0, 20, 20, WHITE);
		
		// World
		BeginMode2D(camera);

		DrawCircle(0, 0, 3.0f, WHITE);
		DrawText("HIHI world!!!!", -100,20,30,WHITE);
		
		// Render player
		player.draw();
		enemyManager.draw();
		particleManager.draw();


		// 
		Vector2 mouse_pos = GetScreenToWorld2D(GetMousePosition(), camera);
		Vector2 direction_vector = Vector2Normalize(Vector2Subtract(mouse_pos, player.position));
		Vector2 line_start = player.position;
		Vector2 line_end = Vector2Add(player.position, Vector2Scale(direction_vector, 100.0f));
		LineCollider line_collider = LineCollider(line_start, line_end);
		bool k = false;
		for(Enemy& enemy : enemyManager.entities){
			if(enemy.collide(line_collider)){
				k = true;
				DrawCircleLinesV(enemy.position, 30.0f, RED);
			}
			else{
				DrawCircleLinesV(enemy.position, 30.0f, BLUE);
			}
		}
		if(k){
			DrawLineEx(line_start, line_end, 5.0f, RED);
		}
		else{
			DrawLineEx(line_start, line_end, 5.0f, BLUE);
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
