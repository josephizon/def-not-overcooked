#include <raylib.h>
#include <raymath.h>
#include <string>
#include <unordered_map>

#include "TextureManager.hpp"

std::unordered_map<std::string, Texture2D> TextureManager::textures;

void TextureManager::LoadTextures() {

    // ** Player Sprites ** // 
    textures["mike_idle"] = LoadTexture("assets/mike_idle.png"); 
    textures["mike_run"] = LoadTexture("assets/mike_run.png"); 
    textures["mike_attack"] = LoadTexture("assets/mike_attack.png"); 
    textures["mike_dodge"] = LoadTexture("assets/mike_dodge.png"); 
    textures["mike_holding"] = LoadTexture("assets/mike_holding.png");

    textures["paul_idle"] = LoadTexture("assets/paul_idle.png"); 
    textures["paul_run"] = LoadTexture("assets/paul_run.png");
    textures["paul_attack"] = LoadTexture("assets/paul_attack.png");
    textures["paul_dodge"] = LoadTexture("assets/paul_dodge.png"); 
    textures["paul_holding"] = LoadTexture("assets/paul_holding.png"); 

    // ** Enemy Sprites ** //
    textures["enemy_moving"] = LoadTexture("assets/enemy_moving.png"); 
    textures["enemy_readying"] = LoadTexture("assets/enemy_readying.png"); 
    textures["enemy_chasing"] = LoadTexture("assets/enemy_chasing.png"); 

    // ** Stations ** //
    textures["station_idle"] = LoadTexture("assets/station_idle.png");
    textures["station_holding"] = LoadTexture("assets/station_holding.png");
    textures["station_processing"] = LoadTexture("assets/station_processing.png");
    textures["station_complete"] = LoadTexture("assets/station_complete.png");
    textures["station_delivery"] = LoadTexture("assets/station_delivery.png"); 

    // ** Spawners ** //
    textures["chest_circle"] = LoadTexture("assets/chest_circle.png");
    textures["chest_box"] = LoadTexture("assets/chest_box.png");
    textures["chest_wing"] = LoadTexture("assets/chest_wing.png");
    textures["chest_gun"] = LoadTexture("assets/chest_gun.png");
    textures["chest_arm"] = LoadTexture("assets/chest_arm.png");
    textures["chest_bulb"] = LoadTexture("assets/chest_bulb.png");
    textures["chest_micro"] = LoadTexture("assets/chest_micro.png");
    textures["chest_open"] = LoadTexture("assets/chest_open.png");

    // ** Item Components ** // 
    textures["comp_circle"] = LoadTexture("assets/comp_circle.png");
    textures["comp_box"] = LoadTexture("assets/comp_box.png");
    textures["comp_wing"] = LoadTexture("assets/comp_wing.png");
    textures["comp_gun"] = LoadTexture("assets/comp_gun.png");
    textures["comp_arm"] = LoadTexture("assets/comp_arm.png");
    textures["comp_bulb"] = LoadTexture("assets/comp_bulb.png");
    textures["comp_micro"] = LoadTexture("assets/comp_micro.png");
    
    // ** Item Complete Bots ** //
    textures["bot_wingbot"] = LoadTexture("assets/bot_wingbot.png");
    textures["bot_clawbot"] = LoadTexture("assets/bot_clawbot.png");
    textures["bot_gunbot"] = LoadTexture("assets/bot_gunbot.png");
    textures["bot_ballbot"] = LoadTexture("assets/bot_ballbot.png");
    textures["bot_angrybot"] = LoadTexture("assets/bot_angrybot.png");
    textures["bot_bombbot"] = LoadTexture("assets/bot_bombbot.png");
    textures["bot_junkbot"] = LoadTexture("assets/bot_junkbot.png");
}

Texture TextureManager::GetTexture(const std::string& name) {
    auto it = textures.find(name);
    if (it != textures.end()) return it->second;
    return textures["junk"]; 
}

void TextureManager::UnloadTextures() {
    for (auto& [key, texture] : textures) {
        UnloadTexture(texture);
    }
    textures.clear();
}
