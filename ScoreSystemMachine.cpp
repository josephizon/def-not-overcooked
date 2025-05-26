#include <raylib.h>
#include <raymath.h>
#include <vector> 
#include <algorithm>
#include <string>

#include "ScoreSystem.hpp"

ScoreSystem::ScoreSystem(Vector2 pos, Vector2 s) {
    position = pos;
    size = s;
    createOrderInterval = 0.0f;
    playerPoints;
    max_frames = 10; 
    frame_speed = 7;
    current_frame = 0;
    frame_time = 1.0f / frame_speed;
    frame_counter = 0.0f;
    texture_station = TextureManager::GetTexture("station_delivery");
    station_per_frame = { 0.0f, 0.0f, (float)texture_station.width, ((float)texture_station.height / max_frames ) };   
}

void ScoreSystem::DrawStation(){
    // Draws anything related to the station
    //DrawRectangleV(position, size, GOLD);

    DrawTexturePro(
        texture_station,
        station_per_frame,                      
        {position.x, position.y, size.x, size.y},
        {0,0}, 
        0.0f,                      
        WHITE
    );

    DrawText("delivery station", position.x-5, position.y+63, 10, WHITE);
}

void ScoreSystem::Draw(){
    // Draws the Points of the Player   
    DrawRectangleV({0,0}, {460, (float)60 + (orderVector.size()*30)}, { 130, 130, 130, 50 });
    std::string playerInfo = TextFormat("Points: %i pts", playerPoints);
    DrawText(playerInfo.c_str(), 10, 10, 20, WHITE);

    // Draws the Current order in the list
    int orderY = 40;                        // Starting Y position for order text
    DrawText("~~~ Orders List ~~~", 10, orderY, 20, WHITE); // Header for the orders
        
    orderY += 30;                               // Move below the header

    size_t maxOrdersToShow = 4; // Maximum number of orders to display
    size_t ordersDisplayed = 0;

    for (size_t i = 0; i < orderVector.size(); i++){
        if (!orderVector[i].completed){
            // Display the first 4 orders
            if (ordersDisplayed < maxOrdersToShow){
                std::string orderText = TextFormat("Order: %s (%s)", orderVector[i].robot.c_str(), orderVector[i].recipe.c_str());
                DrawText(orderText.c_str(), 10, orderY, 20, orderVector[i].textColor);
                orderY += 25; // Increment Y for the next order
                ordersDisplayed++;
            }
        }
    }

                // If there are more than 4 orders, display the warning message
    if (ordersDisplayed == maxOrdersToShow && orderVector.size() > maxOrdersToShow){
        DrawText("Hurry up, there's more orders!", 10, orderY, 20, RED);
    }
}


void ScoreSystem::CreateOrder(const std::string& rob, float dur){
    RobotOrder newOrder(rob, dur);
    orderVector.push_back(newOrder);
}

void ScoreSystem::HandleCollision(Entity* other_entity){
    Rectangle station_rectangle = { position.x, position.y, size.x, size.y };   
    bool isStationColliding = CheckCollisionCircleRec(other_entity->position, other_entity->radius, station_rectangle);

    if (isStationColliding && !other_entity->isItemOnStation && other_entity->entity_type=="Item" && !other_entity->isItemBeingHeld) {
        other_entity->isItemOnScoringSystem = true;
    }

    if (!isStationColliding && other_entity->isItemOnStation && other_entity->entity_type=="Item") {
        other_entity->isItemOnScoringSystem = false;
    }

    if (other_entity->isItemOnScoringSystem && !other_entity->isItemBeingHeld){
        for (auto& ov : orderVector){
            // Checks if the Order in the Vector has been completed
            if (!ov.completed){
                // Checks if the player submitted a robot
                if (other_entity->item_name == ov.robot){
                    playerPoints += 50;
                    // Doubles the points player gets if they submitted an order fast
                    if (ov.currentTime < ov.duration/2){
                        playerPoints += 50;
                    }
                    ov.completed = true;
                }
                break;
            }
        }
        other_entity->isItemForDeletion = true;
        //other_entity->position = {-100, 0};
    }
}

void ScoreSystem::Update(float delta_time){
    // Creates Orders Overtime
    if (createOrderInterval < 0){
        switch (GetRandomValue(0, 5)){
            case 0:
                CreateOrder("wingbot", 45);
                break;
            case 1:
                CreateOrder("clawbot", 45);
                break;
            case 2:
                CreateOrder("gunbot", 45);
                break;
            case 3:
                CreateOrder("ballbot", 45);
                break;
            case 4:
              CreateOrder("angrybot", 45);
                break;
            case 5:
               CreateOrder("bombbot", 45);
                break;
        }
        createOrderInterval = GetRandomValue(15, 20);
    }
    else{
        createOrderInterval -= delta_time;
    }

    //  Update Robot Orders
    for (size_t i = 0; i < orderVector.size(); i++){
        if (!orderVector[i].completed){
            orderVector[i].currentTime += delta_time;

            if (orderVector[i].currentTime > orderVector[i].duration/3){
                orderVector[i].textColor = {255, 213, 128, 255};
            }
            else if (orderVector[i].currentTime > orderVector[i].duration*2/3){
                orderVector[i].textColor = {255, 176, 156, 255};
            }
            else {
                orderVector[i].textColor = {144, 238, 144, 255};
            }
        }
    }

    // ** Animation Frames ** //
    frame_counter += delta_time;
    if (frame_counter >= frame_time) {
        frame_counter = 0.0f;
        current_frame++;
        if (current_frame >= max_frames) {
            current_frame = 0;
        }
        
        station_per_frame.y = (float)current_frame * station_per_frame.height; 
    }
}
void ScoreSystem::Reset() {
    playerPoints = 0;
    orderVector.clear();
    createOrderInterval = 0.0f;
}