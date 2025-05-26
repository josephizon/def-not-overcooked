#ifndef STATION
#define STATION

#include <raylib.h>
#include <raymath.h>
#include "Entity.hpp"
#include "Item.hpp"

class Station;


class StationState {
public:
    virtual ~StationState() {}
    virtual void Enter(Station& station) = 0;
    virtual void Update(Station& station, float delta_time) = 0;
    virtual void HandleCollision(Station& station, Entity* other_entity) = 0;
};

class StationIdle : public StationState {
public:
    void Enter(Station& station);
    void Update(Station& station, float delta_time);
    void HandleCollision(Station& station, Entity* other_entity);
};

class StationHolding : public StationState {
    public:
        void Enter(Station& station);
        void Update(Station& station, float delta_time);
        void HandleCollision(Station& station, Entity* other_entity);
    };

class StationProcessing : public StationState {
public:
    void Enter(Station& station);
    void Update(Station& station, float delta_time);
    void HandleCollision(Station& station, Entity* other_entity);
};


class StationComplete : public StationState {
    public:
        void Enter(Station& station);
        void Update(Station& station, float delta_time);
        void HandleCollision(Station& station, Entity* other_entity);
    };


// Base Station Class
class Station : public Entity {
public:
    Entity* heldItem = nullptr;

    bool isStationOccupied;
    float push_back_delay; 
    std::vector<Entity*> items_in_station; 
    std::vector<std::string> items_in_station_names; 

    std::vector<Item> item_complete_robot;

    // sprite stuff
    Texture texture_component;
    Rectangle texture_frame;

    Station(Vector2 pos, Vector2 size);

    void Update(float delta_time);

    void Draw();

    void SetState(StationState* new_state);

    void HandleCollision(Entity* other_entity);

    StationIdle stationIdle;
    StationHolding stationHolding;
    StationProcessing stationProcessing;
    StationComplete stationComplete; 

    void RemoveItems();
    
    void Reset();
    
    Item SpawnCompleteRobot(Station& station);
    

    

private:
    StationState* current_state;
};


#endif