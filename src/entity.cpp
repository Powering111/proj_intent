#include "entity.hpp"

extern TextureManager textureManager;
extern EntityManager entityManager;

void Entity::draw(){
    textureManager.get_texture(textureID).render(this->position, 1.0f, this->anim_idx);
}


Player::Player(Vector2 pos){
    this->textureID = TextureID::Player;
    this->position = pos;
    this->anim_counter = 0;
}

void Player::move(Vector2 move_direction){
    if(move_direction.x!=0.0f || move_direction.y!=0.0f){
        position = Vector2Add(position, Vector2Scale(Vector2Normalize(move_direction), movement_speed));
    }
}

void Player::update(){
    this->anim_counter++;
    if(this->anim_counter > 30){
        this->anim_idx = (this->anim_idx + 1) % 2;
        this->anim_counter = 0;
    }
    if(skill_cooltime > 0){
        skill_cooltime--;
    }
}

void Player::draw(){
    Entity::draw();
    if(skill_cooltime == 0){
        DrawRing(position, skill_range, skill_range+3.0f, 0.0f, 360.0f, 120, {102, 191, 255, 255});
    }
    else{
        DrawRing(position, skill_range, skill_range+3.0f, 0.0f, 360.0f, 120, {192, 100, 100, 255});
        DrawRing(position, skill_range-5.0f, skill_range, -90.0f, -90.0f+3.0f*(120-skill_cooltime), 120, {150, 150, 150, 255});
    }
}

void Player::attack(Vector2 attack_pos){
    entityManager.add_entity(new Particle(attack_pos, TextureID::Attack, 6, 30));
}

void Player::use_skill(Vector2 skill_pos){
    if(skill_cooltime == 0){
        entityManager.add_entity(new Particle(position, TextureID::Skill, 3, 12));

        if(Vector2Distance(position, skill_pos) <= skill_range){
            position = skill_pos;
        }
        else{
            position = Vector2Add(position, Vector2Scale(Vector2Normalize(Vector2Subtract(skill_pos, position)), skill_range));
        }

        skill_cooltime = 120;
    }
}

Particle::Particle(Vector2 pos, TextureID texture_id, unsigned int particle_speed, unsigned int lifetime){
    position = pos;
    textureID = texture_id;
    speed = particle_speed;
    life = lifetime;
    anim_idx = 0;
    anim_cooltime = speed;
}
void Particle::update(){
    anim_cooltime--;
    if(anim_cooltime == 0){
        anim_idx++;
        anim_cooltime = speed;
    }

    life--;
    if(life == 0){
        entityManager.remove_entity(this);
        delete this;
    }
}

void EntityManager::add_entity(Entity* entity){
    entities.insert(entity);
}
void EntityManager::remove_entity(Entity* entity){
    entities.erase(entity);
}

void EntityManager::update(){
    for(Entity* entity : entities){
        entity->update();
    }
}
void EntityManager::draw(){
    for(Entity* entity : entities){
        entity->draw();
    }
}