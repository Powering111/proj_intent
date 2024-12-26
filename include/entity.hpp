#pragma once
#include <vector>
#include <cstdint>
#include "texture.hpp"
#include "collider.hpp"
#include "raylib.h"
#include "raymath.h"

typedef uint64_t EntityID;

// Entity is what can be rendered.
class Entity{
protected:
    TextureID texture_id;
    unsigned int anim_idx;
    float scale;
    static EntityID next_id;
public:
    EntityID id;
    Vector2 position;
    Entity(Vector2 c_position, TextureID c_texture_id, uint32_t c_anim_idx = 0, float c_scale = 1.0f): position(c_position), texture_id(c_texture_id), anim_idx(c_anim_idx), scale(c_scale) {
        id = next_id++;
    };

    virtual void draw() const;
    virtual void update() {};
};

class MortalEntity : public Entity{
public:
    unsigned int max_health;
    unsigned int health;
    bool died = false;
    MortalEntity(Vector2 c_position, TextureID c_texture_id, uint32_t c_anim_idx = 0, float c_scale = 1.0f, unsigned int c_max_health=100): 
        Entity(c_position, c_texture_id, c_anim_idx, c_scale), 
        max_health(c_max_health), 
        health(c_max_health) 
    {};

    virtual bool collide(Collider const& other) const = 0;

    void damage(unsigned int damage) {
        health -= damage;
        if(health <= 0){
            health = 0;
            if(!died){
                died = true;
                // death
                die();
            }
        }
    };

    virtual void die() = 0;
};

class Player : public MortalEntity{
private:
    unsigned int anim_counter;
    float skill_range=240.0f;
    float movement_speed=6.0f;

    enum {
        IDLE,
        ATTACKING,
    } status;
    unsigned int status_counter=0;
    Vector2 attack_direction;
    unsigned int attack_damage = 20;
public:
    unsigned int skill_cooltime=0;
    
    Player(Vector2 c_position) : MortalEntity(c_position, TextureID::Player, 0, 1.0f, 100), anim_counter(0), status(IDLE) {};

    void update();
    void draw() const override;
    bool collide(Collider const& other) const override;

    void move(Vector2 move_direction);
    void attack(Vector2 attack_pos);
    void use_skill(Vector2 skill_pos);

    void die();
};

class Enemy: public MortalEntity{
public:
    Enemy(Vector2 c_position): MortalEntity(c_position, TextureID::Enemy, 0, 1.0f, 100) {}

    bool collide(Collider const& other) const override;
    CircleCollider get_collider(){
        return CircleCollider(position, 30.0f);
    };
    void update() override;
    void draw() const override;

    void die();
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
public:
    std::vector<E> entities;
    void insert(E&& entity){
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