#include "world.hpp"

World::World(): player(this, {0.0f, 0.0f}),
    camera{.offset={640.0f, 400.0f}, .target={0.0f, 0.0f}, .rotation=0.0f, .zoom=1.0f},
    controller(&player, &camera) {
    texture_m.load_textures();
}

World::~World(){
    texture_m.unload_textures();
}

void World::update(){
    if(IsKeyPressed(KEY_EIGHT)){
        enemy_m.insert(Enemy(this, {(rand()%5-2)*100.0f, (rand()%5-2)*100.0f}));
    }
    controller.handle_input();
    if(!IsKeyDown(KEY_SPACE)){
        player.update();
        enemy_m.update();
        particle_m.update();
    }
}

void World::draw(){
    BeginDrawing();
    ClearBackground(BLACK);

    // UI
    DrawCircle(640, 400, 3.0f, BLUE);
    DrawText(TextFormat("%d FPS", GetFPS()), 0, 0, 20, WHITE);
    
    BeginMode2D(camera);
    
    DrawCircle(0, 0, 3.0f, WHITE);
    DrawText("HIHI world!!!!", -100,20,30,WHITE);
    player.draw();
    enemy_m.draw();
    particle_m.draw();

    // 
    Vector2 mouse_pos = GetScreenToWorld2D(GetMousePosition(), camera);
    Vector2 direction_vector = Vector2Normalize(Vector2Subtract(mouse_pos, player.position));
    Vector2 line_start = player.position;
    Vector2 line_end = Vector2Add(player.position, Vector2Scale(direction_vector, 100.0f));
    LineCollider line_collider = LineCollider(line_start, line_end);
    bool k = false;
    for(Enemy& enemy : enemy_m.entities){
        if(enemy.collide(line_collider)){
            k = true;
            DrawCircleLinesV(enemy.position, 30.0f, RED);
        }
        else{
            DrawCircleLinesV(enemy.position, 30.0f, BLUE);
        }
    }
    if(k){
        DrawLineEx(line_start, line_end, 5.0f, RED);
    }
    else{
        DrawLineEx(line_start, line_end, 5.0f, BLUE);
    }
    EndMode2D();
    EndDrawing();
}