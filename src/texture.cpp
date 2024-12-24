#include "raylib.h"
#include "raymath.h"
#include "texture.hpp"

TextureInfo::TextureInfo(TextureID texture_id, Texture texture_texture, Vector2 texture_size){
    this->id = texture_id;
    this->texture = texture_texture;
    this->size = texture_size;
}

void TextureInfo::render(Vector2 center, float scale, int index){
    DrawTextureRec(texture, {size.x * (index % this->frame_count()), 0.0f, size.x, size.y}, Vector2Add(center, {-size.x/2.0f, -size.y/2.0f}), WHITE);
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