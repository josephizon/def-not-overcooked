#ifndef ENTITY
#define ENTITY

#include <raylib.h>
#include <raymath.h>
#include "TextureManager.hpp"

class Entity {
public:
    // general
    int health;
    int player_id;
    Vector2 position;
    float radius;
    Color color;
    float invulnerable_timer;
    std::string entity_type; // used to distinguish what type of entity is being collided with for HandleCollission
    
    // for Player
    bool isInConveyerBelt;
    int bezierCurveIndex;
    bool isInteractPressed;
    void* holder_is = nullptr; // well technically shouldn't crashh but real HAHAHA 

    // for item
    std::string item_name; 
    std::string item_type; // robot or component
    bool isItemBeingHeld; // check for if ITEM itself is being held 
    bool isItemForDeletion; // checks if ITEM could be permanently deleted
    std::vector<int> ids_to_delete;
    

    // for station
    Vector2 size; // used for rectangle or square objects
    bool isStationProcessing; // check for if station is in StationProcessing state
    bool isStationComplete; // check for if station is in StationProcessing state
    bool isItemOnStation; // checks if there are any items colliding with the Station

    // for ScoreSystem,
    bool isItemOnScoringSystem;

    // for all


  

};


#endif