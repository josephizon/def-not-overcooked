#ifndef SCORE_SYSTEM_MACHINE_HPP
#define SCORE_SYSTEM_MACHINE_HPP

#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <string>
#include "Entity.hpp" 

class ScoreSystem: public Entity{
public:
    struct RobotOrder {
        std::string robot;
        std::string recipe;
        float currentTime;
        float duration;
        Color textColor;
        bool completed;

        RobotOrder(std::string rob, float dur){
            robot = rob;
            duration = dur;
            currentTime = 0.0f;
            completed = false;
            textColor = { 144, 238, 144, 255 };
            if (rob == "wingbot") {
                recipe = "circular-frame wing wing";
            } 
            else if (rob == "clawbot") {
                recipe = "circular-frame arm arm";
            } 
            else if (rob == "gunbot") {
                recipe = "box-frame arm wing";
            } 
            else if (rob == "ballbot") {
                recipe = "box-frame gun wing";
            } 
            else if (rob == "angrybot") {
                recipe = "box-frame bulb micro";
            } 
            else if (rob == "bombbot") {
                recipe = "circular-frame bulb micro";
            } 
        }
    };

    Vector2 position;
    Vector2 size;
    std::vector<RobotOrder> orderVector;
    float createOrderInterval;
    int playerPoints = 0;

    ScoreSystem(Vector2 pos, Vector2 size);

    void Draw();
    void DrawStation();
    void CreateOrder(const std::string& robot, float duration);
    void HandleCollision(Entity* other_entity);
    void Update(float delta_time);
    void Reset();
    // ** Sprite and Animations Stuff ** //
    Texture texture_station; 
    Rectangle station_per_frame; // per frame reference
    int current_frame;
    float frame_time;
    float frame_counter; // 
    int max_frames; // number of frames in the spritesheet
    int frame_speed; // how fast frame go brom

private:
    
};

#endif
