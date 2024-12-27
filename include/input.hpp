#pragma once
#include "raylib.h"

class Player;

class Controller{
private:
    bool clicking = false;
    Player* const player;
    Camera2D* const camera;
public:
    Controller(Player* c_player, Camera2D* c_camera);

    void handle_input();
};

#include "entity.hpp"