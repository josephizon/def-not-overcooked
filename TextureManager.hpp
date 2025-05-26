#ifndef TEXTURE
#define TEXTURE

#include <raylib.h>
#include <raymath.h>
#include <string>
#include <unordered_map>

class TextureManager {
public:
    static void LoadTextures();
    static Texture2D GetTexture(const std::string& name);
    static void UnloadTextures();

private:
    static std::unordered_map<std::string, Texture2D> textures;
};


#endif