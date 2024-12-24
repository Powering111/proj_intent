#pragma once
#include <memory>
#include <set>
#include "texture.hpp"
#include "raylib.h"
#include "raymath.h"

// class Collider{
// public:
//     enum ColliderType{
//         Rectangle,
//         Circle,
//     } type;

// }

class Entity{
protected:
    TextureID textureID;
    unsigned int anim_idx;
public:
    Vector2 position;
    virtual void draw();
    virtual void update() = 0;
};

class Player : public Entity{
private:
    unsigned int anim_counter;
    float skill_range=240.0f;
    float movement_speed=30.0f;

public:
    unsigned int skill_cooltime=0;
    Player(Vector2 pos);
    void update();
    void move(Vector2 move_direction);
    void draw() override;
    void attack(Vector2 attack_pos);
    void use_skill(Vector2 skill_pos);
};


// Particle class must be allocated with `new`
// because it uses `delete this` when it is destroyed.
// This behavior should be fixed later.
class Particle: public Entity{
private:
    unsigned int speed;
    unsigned int anim_cooltime;
    unsigned int life;
public:
    Particle(Vector2 pos, TextureID texture_id, unsigned int particle_speed, unsigned int lifetime);
    void update() override;
};

class EntityManager {
private:
    std::set<Entity*> entities;
public:
    // register heap-allocated entity
    void add_entity(Entity* entity);
    void remove_entity(Entity* entity);

    void update();
    void draw();
};