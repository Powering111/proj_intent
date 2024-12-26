#include "raylib.h"
#include "entity.hpp"

class Controller{
private:
    bool clicking = false;
    Player* const player;
    Camera2D* const camera;
public:
    Controller(Player* c_player, Camera2D* c_camera): player(c_player), camera(c_camera) {};

    void handle_input(){
		if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
			if(!clicking){
				player->attack(GetScreenToWorld2D(GetMousePosition(), *camera));
				clicking = true;
			}
		}
		else{
			if(clicking){
				clicking = false;
			}
		}
        
		if(IsKeyPressed(KEY_LEFT_SHIFT)){
			player->use_skill(GetScreenToWorld2D(GetMousePosition(), *camera));
		}

        // player move
		Vector2 move_direction = {0.0f, 0.0f};
		if(IsKeyDown(KEY_A)) move_direction.x -= 1.0f;
		if(IsKeyDown(KEY_D)) move_direction.x += 1.0f;
		if(IsKeyDown(KEY_W)) move_direction.y -= 1.0f;
		if(IsKeyDown(KEY_S)) move_direction.y += 1.0f;
		if(move_direction.x!=0.0f || move_direction.y!=0.0f){
			player->move(move_direction);
		}

		if(IsKeyDown(KEY_LEFT)) camera->target.x -= 20.0f;
		if(IsKeyDown(KEY_RIGHT)) camera->target.x += 20.0f;
		if(IsKeyDown(KEY_UP)) camera->target.y -= 20.0f;
		if(IsKeyDown(KEY_DOWN)) camera->target.y += 20.0f;
    };
};