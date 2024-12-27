#pragma once
#include "raylib.h"

#include "entity.hpp"
#include "input.hpp"

class World{
public:
    Camera2D camera;
    Player player;
    EntityManager<Particle> particle_m;
    EntityManager<Enemy> enemy_m;
    TextureManager texture_m; // maybe switch to global texture manager?

    Controller controller;

    World();
    ~World();

    World(const World& other) = delete;
    World(const World&& other) = delete;

    void update();

    void draw();
};