#include "raylib.h"
#include "raymath.h"
#include "resource_dir.h"

#include "texture.hpp"
#include "entity.hpp"
#include "input.hpp"


int main(){
	SetConfigFlags(FLAG_VSYNC_HINT);

	InitWindow(1280, 800, "ASDFASDFASDF");
	
	SetExitKey(KEY_NULL);

	SearchAndSetResourceDir("resources");

	World world;

	world.enemy_m.insert(Enemy(&world, {100.0f, 0.0f}));
	world.enemy_m.insert(Enemy(&world, {-100.0f, 0.0f}));


	SetTargetFPS(60);
	// game loop
	while (!WindowShouldClose())
	{
		world.update();
		world.draw();
	}
	
	CloseWindow();
	return 0;
}
