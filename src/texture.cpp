#include "raylib.h"
#include "raymath.h"
#include "texture.hpp"

TextureInfo::TextureInfo(TextureID texture_id, Texture texture_texture, Vector2 texture_size){
    this->id = texture_id;
    this->texture = texture_texture;
    this->size = texture_size;
}

void TextureInfo::render(Vector2 center, float scale, int index){
    DrawTexturePro(
        texture, 
        {
            size.x * (index % this->frame_count()),
            0.0f,
            size.x,
            size.y
        },
        {
            center.x,
            center.y,
            scale * size.x,
            scale * size.y
        },
        {
            scale*size.x/2.0f, 
            scale*size.y/2.0f
        },
        0.0f,
        WHITE
    );

    // DrawRectangleLinesEx({
    //     center.x - scale * size.x / 2.0f,
    //     center.y - scale * size.y / 2.0f,
    //     scale * size.x,
    //     scale * size.y
    // }, 1.0f, WHITE);
}
unsigned int TextureInfo::frame_count(){
    return texture.width / int(size.x);
}

struct {
    TextureID id;
    const char* path;
    Vector2 size;
} TEXTURE_INFO[] = {
    {TextureID::Player, "player.png", {64.0f, 128.0f}},
    {TextureID::Attack, "attack.png", {64.0f, 64.0f}},
    {TextureID::Skill, "skill.png", {64.0f, 64.0f}},
    {TextureID::Enemy, "enemy.png", {64.0f, 64.0f}},
    {TextureID::Sweep, "sweep.png", {160.0f, 160.0f}},
};

void TextureManager::load_textures(){
    for(auto texture_info : TEXTURE_INFO){
        Texture texture = LoadTexture(texture_info.path);
        this->textures.emplace(std::make_pair(texture_info.id, TextureInfo(texture_info.id, texture, texture_info.size)));
    }
}
void TextureManager::unload_textures(){
    for(std::pair<TextureID, TextureInfo> texture : textures){
        UnloadTexture(texture.second.texture);
    }
    this->textures.clear();
}

TextureInfo TextureManager::get_texture(TextureID id){
    return this->textures.find(id)->second;
}