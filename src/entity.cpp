#include "entity.hpp"

EntityID Entity::next_id = 0;

void Entity::draw() const{
    world->texture_m.get_texture(texture_id).render(position, scale, anim_idx);
}


void Player::update(){
    if(status == ATTACKING){
        if(status_counter == 0){
            // actual attack
            Vector2 attack_pos = Vector2Add(position, Vector2Scale(attack_direction, 100.0f));
            Vector2 line_start = position;
		    Vector2 line_end = Vector2Add(position, Vector2Scale(attack_direction, 100.0f));
            LineCollider attack_collider(line_start, line_end);
            for(Enemy& enemy: world->enemy_m.entities){
                if(enemy.collide(attack_collider)){
                    enemy.damage(attack_damage);
                }
            }

            status = IDLE;
        }
        else {
            status_counter--;
        }
    }

    anim_counter++;
    if(anim_counter > 30){
        anim_idx = (anim_idx + 1) % 2;
        anim_counter = 0;
    }
    if(skill_teleport_cooltime > 0){
        skill_teleport_cooltime--;
    }
    if(skill_sweep_cooltime > 0){
        skill_sweep_cooltime--;
    }
}

void Player::draw() const{
    Entity::draw();
    if(skill_teleport_cooltime == 0){
        DrawRing(position, skill_teleport_range, skill_teleport_range+2.0f, 0.0f, 360.0f, 120, {102, 191, 255, 255});
    }
    else{
        DrawRing(position, skill_teleport_range, skill_teleport_range+2.0f, 0.0f, 360.0f, 120, {192, 100, 100, 255});
        DrawRing(position, skill_teleport_range-2.0f, skill_teleport_range, -90.0f, -90.0f+3.0f*(120-skill_teleport_cooltime), 120, {150, 150, 150, 255});
    }

    if(skill_sweep_cooltime == 0){
        DrawRing(position, skill_sweep_range, skill_sweep_range+2.0f, 0.0f, 360.0f, 120, {78, 252, 113, 255});
    }
    else{
        DrawRing(position, skill_sweep_range, skill_sweep_range+2.0f, 0.0f, 360.0f, 120, {192, 100, 100, 255});
        DrawRing(position, skill_sweep_range-2.0f, skill_sweep_range, -90.0f, -90.0f+1.5f*(240-skill_sweep_cooltime), 120, {150, 150, 150, 255});
    }

}

void Player::move(Vector2 move_direction){
    if(status != ATTACKING){
        if(move_direction.x!=0.0f || move_direction.y!=0.0f){
            position = Vector2Add(position, Vector2Scale(Vector2Normalize(move_direction), movement_speed));
        }
    }
}

void Player::attack(Vector2 attack_pos){
    if(status == IDLE){
        attack_direction = Vector2Normalize(Vector2Subtract(attack_pos, position));
        Vector2 attack_particle_pos = Vector2Add(position, Vector2Scale(attack_direction, 100.0f));
        world->particle_m.insert(std::move(Particle(world, attack_particle_pos, TextureID::Attack, 6, 30, 2.0f)));

        status = ATTACKING;
        status_counter = 30;
    }
}

void Player::skill_teleport(Vector2 skill_pos){
    if(skill_teleport_cooltime == 0 && status==IDLE){
        world->particle_m.insert(std::move(Particle(world, position, TextureID::Skill, 3, 12)));

        if(Vector2Distance(position, skill_pos) <= skill_teleport_range){
            position = skill_pos;
        }
        else{
            position = Vector2Add(position, Vector2Scale(Vector2Normalize(Vector2Subtract(skill_pos, position)), skill_teleport_range));
        }

        skill_teleport_cooltime = 120;
    }
}

void Player::skill_sweep(){
    if(skill_sweep_cooltime == 0){
        world->particle_m.insert(std::move(Particle(world, position, TextureID::Sweep, 3, 12)));
        for(Enemy& enemy : world->enemy_m.entities){
            if(enemy.collide(CircleCollider(position, skill_sweep_range))){
                enemy.damage(40);
            }
        }
        skill_sweep_cooltime = 240;
    }
}

bool Player::collide(Collider const& other) const{
    CircleCollider col(position, 30.0f);
    Collider* col_ref = &col;
    return other.collide(*col_ref);
}

void Player::die(){
    TraceLog(LOG_INFO, "Player died.");
}

void Enemy::update(){
    if(Vector2Distance(world->player.position, position) > 500.0f){
        // move towards player
        position = Vector2Add(position, Vector2Scale(Vector2Normalize(Vector2Subtract(world->player.position, position)), 1.0f));
    }
}

void Enemy::draw() const{
    Entity::draw();
    DrawRectangle(position.x-50, position.y-76, 100, 16, GRAY);
    DrawRectangle(position.x-50, position.y-76, 100.0f*((float)health / (float)max_health), 16, RED);
    DrawRectangleLinesEx({position.x-52.0f, position.y-78.0f, 104.0f, 20.0f}, 2.0f, WHITE);
}

bool Enemy::collide(Collider const& other) const{
    CircleCollider col(position, 30.0f);
    Collider* col_ref = &col;
    return other.collide(*col_ref);
}

void Enemy::die(){
    world->enemy_m.remove(id);
}


void Particle::update(){
    anim_cooltime--;
    if(anim_cooltime == 0){
        anim_idx++;
        anim_cooltime = speed;
    }

    lifetime--;
    if(lifetime == 0){
        world->particle_m.remove(id);
    }
}