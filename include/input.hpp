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
			player->skill_teleport(GetScreenToWorld2D(GetMousePosition(), *camera));
		}

		if(IsKeyPressed(KEY_E)){
			player->skill_sweep();
		}

		float wheel = GetMouseWheelMove();
		if(wheel!=0.0f) {
			if(wheel>0.0f){
				camera->zoom = Clamp(camera->zoom * 1.1f, 1.0f, 10.0f);

			}
			else if(wheel<0.0f){
				camera->zoom = Clamp(camera->zoom * 0.9f, 1.0f, 10.0f);

			}
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