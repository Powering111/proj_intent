#pragma once

#include <vector>
#include <cstdint>
#include "texture.hpp"
#include "collider.hpp"
#include "raylib.h"
#include "raymath.h"
class World;

typedef uint64_t EntityID;

// Entity is what can be rendered.
class Entity{
protected:
    World* world; // world it is located in.
    TextureID texture_id;
    unsigned int anim_idx;
    float scale;
    static EntityID next_id;
public:
    EntityID id;
    Vector2 position;
    Entity(World* c_world, Vector2 c_position, TextureID c_texture_id, uint32_t c_anim_idx = 0, float c_scale = 1.0f): world(c_world), position(c_position), texture_id(c_texture_id), anim_idx(c_anim_idx), scale(c_scale) {
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
    MortalEntity(World* c_world, Vector2 c_position, TextureID c_texture_id, uint32_t c_anim_idx = 0, float c_scale = 1.0f, unsigned int c_max_health=100): 
        Entity(c_world, c_position, c_texture_id, c_anim_idx, c_scale), 
        max_health(c_max_health), 
        health(c_max_health) 
    {};

    virtual bool collide(Collider const& other) const = 0;

    void damage(unsigned int damage) {
        if(damage >= health){
            health = 0;
            if(!died){
                died = true;
                // death
                die();
            }
        }
        else{
            health -= damage;
        }
    };

    virtual void die() = 0;
};

class Player : public MortalEntity{
private:
    unsigned int anim_counter;
    float skill_teleport_range=240.0f;
    float skill_sweep_range=100.0f;
    float movement_speed=6.0f;

    enum {
        IDLE,
        ATTACKING,
    } status;
    unsigned int status_counter=0;
    Vector2 attack_direction;
    unsigned int attack_damage = 20;
public:
    unsigned int skill_teleport_cooltime=0;
    unsigned int skill_sweep_cooltime=0;
    
    Player(World* c_world, Vector2 c_position) : MortalEntity(c_world, c_position, TextureID::Player, 0, 1.0f, 100), anim_counter(0), status(IDLE) {};

    void update();
    void draw() const override;
    bool collide(Collider const& other) const override;

    void move(Vector2 move_direction);
    void attack(Vector2 attack_pos);
    void skill_teleport(Vector2 skill_pos);
    void skill_sweep();

    void die();
};

class Enemy: public MortalEntity{
public:
    Enemy(World* c_world, Vector2 c_position): MortalEntity(c_world, c_position, TextureID::Enemy, 0, 1.0f, 100) {}

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
    Particle(World* c_world, Vector2 c_position, TextureID c_texture_id, unsigned int c_speed, unsigned int c_lifetime, float c_scale = 1.0f): Entity(c_world, c_position, c_texture_id, 0, c_scale), speed(c_speed), lifetime(c_lifetime) , anim_cooltime(c_speed) {};

    void update() override;
};

template <typename E>
class EntityManager {
    std::vector<EntityID> to_remove;
public:
    std::vector<E> entities;
    void insert(E&& entity){
        entities.push_back(entity);
    }

    void remove(EntityID id){
        to_remove.push_back(id);
    }

    void update(){
        for(std::vector<EntityID>::iterator victim = to_remove.begin(); victim != to_remove.end(); victim++){
            for(typename std::vector<E>::iterator it = entities.begin(); it!=entities.end(); it++){
                if(it->id == *victim){
                    entities.erase(it);
                    break;
                }
            }
        }
        to_remove.clear();

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


#include "world.hpp"