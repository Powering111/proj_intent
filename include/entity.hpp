#pragma once
#include <vector>
#include <cstdint>
#include "texture.hpp"
#include "raylib.h"
#include "raymath.h"

typedef uint64_t EntityID;

class Entity{
protected:
    TextureID texture_id;
    unsigned int anim_idx;
    float scale;
public:
    EntityID id;
    Vector2 position;
    Entity(Vector2 c_position, TextureID c_texture_id, uint32_t c_anim_idx = 0, float c_scale = 1.0f): position(c_position), texture_id(c_texture_id), anim_idx(c_anim_idx), scale(c_scale) {};
    virtual void draw() const;
    virtual void update() = 0;
};

class Player : public Entity{
private:
    unsigned int anim_counter;
    float skill_range=240.0f;
    float movement_speed=30.0f;

public:
    unsigned int skill_cooltime=0;
    
    Player(Vector2 c_position) : Entity(c_position, TextureID::Player, 0), anim_counter(0){};

    void update();
    void draw() const override;

    void move(Vector2 move_direction);
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
    unsigned int lifetime;
public:
    Particle(Vector2 c_position, TextureID c_texture_id, unsigned int c_speed, unsigned int c_lifetime, float c_scale = 1.0f): Entity(c_position, c_texture_id, 0, c_scale), speed(c_speed), lifetime(c_lifetime) , anim_cooltime(c_speed) {};

    void update() override;
};

template <typename E>
class EntityManager {
private:
    std::vector<E> entities;
public:
    void insert(E entity){
        entities.push_back(entity);
    }

    bool remove(EntityID id){
        for(typename std::vector<E>::iterator it = entities.begin(); it!=entities.end(); it++){
            if(it->id == id){
                entities.erase(it);
                return true;
            }
        }
        return false;
    }

    void update(){
        for(E& entity : entities){
            entity.update();
        }
    }

    void draw(){
        for(E& entity : entities){
            entity.draw();
        }
    }
};