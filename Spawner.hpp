#ifndef SPAWNER
#define SPAWNER

#include <raylib.h>
#include <raymath.h>
#include <vector>
#include "Item.hpp"
#include "Entity.hpp"

class Spawner;

class SpawnerState {
    public:
        virtual ~SpawnerState() {}
        virtual void Enter(Spawner& spawner) = 0;
        virtual void Update(Spawner& spawner, float delta_time) = 0;
        virtual void HandleCollision(Spawner& spawner, Entity* other_entity, float delta_time) = 0;
    };
    

class SpawnerIdle : public SpawnerState {
    public:
        void Enter(Spawner& spawner);
        void Update(Spawner& spawner, float delta_time);
        void HandleCollision(Spawner& spawner, Entity* other_entity, float delta_time);
    };

class SpawnerActive : public SpawnerState {
    public:
        void Enter(Spawner& spawner);
        void Update(Spawner& spawner, float delta_time);
        void HandleCollision(Spawner& spawner, Entity* other_entity, float delta_time);
    };
    
    

class Spawner : public Entity { 
public:

    float interaction_timer; // how long player stays in interaction with the spawner
    std::vector<Item> spawned_items; // list of items spawned by spawner
    std::string spawner_name; //name of item that this spawner spawns
    bool isSpawnerActive; 

    // sprite stuff
    Texture texture_component; 
    Rectangle texture_frame;

    Spawner(Vector2 pos, Vector2 size, std::string spawner_name);

    void Update(float delta_time);

    void Draw();

    void SetState(SpawnerState* new_state);

    void HandleCollision(Entity* other_entity, float delta_time);
    
    void Reset();
    SpawnerIdle spawnerIdle;
    SpawnerActive spawnerActive;

    Item SpawnItem(Spawner& spawner);

private:
    SpawnerState* current_state;

};


#endif