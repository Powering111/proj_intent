#pragma once
#include <map>
#include <vector>
#include "raylib.h"

enum class TextureID {
    Player,
    Attack,
    Skill,
    Enemy,
    Sweep,
};

class TextureInfo {
    friend class TextureManager;
private:
    TextureID id;
    Texture texture;
    Vector2 size; // size per frame
public:
    void render(Vector2 center, float scale, int index);
    TextureInfo(TextureID texture_id, Texture texture_texture, Vector2 texture_size);
    unsigned int frame_count();
};

class TextureManager {
private:
    std::map<TextureID, TextureInfo> textures;
public:
    void load_textures();
    void unload_textures();
    TextureInfo get_texture(TextureID id);
};