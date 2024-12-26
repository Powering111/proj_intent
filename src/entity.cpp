#include "entity.hpp"

extern TextureManager textureManager;
EntityManager<Particle> particleManager;



void Entity::draw() const{
    textureManager.get_texture(texture_id).render(position, scale, anim_idx);
}


void Player::update(){
    anim_counter++;
    if(anim_counter > 30){
        anim_idx = (anim_idx + 1) % 2;
        anim_counter = 0;
    }
    if(skill_cooltime > 0){
        skill_cooltime--;
    }
}

void Player::draw() const{
    Entity::draw();
    if(skill_cooltime == 0){
        DrawRing(position, skill_range, skill_range+3.0f, 0.0f, 360.0f, 120, {102, 191, 255, 255});
    }
    else{
        DrawRing(position, skill_range, skill_range+3.0f, 0.0f, 360.0f, 120, {192, 100, 100, 255});
        DrawRing(position, skill_range-5.0f, skill_range, -90.0f, -90.0f+3.0f*(120-skill_cooltime), 120, {150, 150, 150, 255});
    }
}

void Player::move(Vector2 move_direction){
    if(move_direction.x!=0.0f || move_direction.y!=0.0f){
        position = Vector2Add(position, Vector2Scale(Vector2Normalize(move_direction), movement_speed));
    }
}

void Player::attack(Vector2 attack_pos){
    particleManager.insert(Particle(attack_pos, TextureID::Attack, 6, 30, 2.0f));
}

void Player::use_skill(Vector2 skill_pos){
    if(skill_cooltime == 0){
        particleManager.insert(Particle(position, TextureID::Skill, 3, 12));

        if(Vector2Distance(position, skill_pos) <= skill_range){
            position = skill_pos;
        }
        else{
            position = Vector2Add(position, Vector2Scale(Vector2Normalize(Vector2Subtract(skill_pos, position)), skill_range));
        }

        skill_cooltime = 120;
    }
}


void Particle::update(){
    anim_cooltime--;
    if(anim_cooltime == 0){
        anim_idx++;
        anim_cooltime = speed;
    }

    lifetime--;
    if(lifetime == 0){
        particleManager.remove(id);
    }
}