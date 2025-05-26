#include <raylib.h>
#include <raymath.h>
#include <vector>
#include "Spawner.hpp"


void Spawner::Update(float delta_time) {
    current_state->Update(*this, delta_time);
}

void Spawner::Draw() {
    // based on last year 
    texture_frame = {0, 0, (float)texture_component.width, (float)texture_component.height};

    if (isSpawnerActive) {
        //DrawRectangleV(position, size, color);
        DrawTexturePro(
            texture_component,
            texture_frame,
            {position.x, position.y, size.x, size.x}, // 63, 45
            {0, 0}, // destination width and height /2 for center 
            0.0f,   
            WHITE
        );
    }

    else {
        //DrawRectangleV(position, size, color);
        DrawTexturePro(
            texture_component,
            texture_frame,
            {position.x, position.y, size.x, size.x}, //63, 45
            {0, 0}, // destination width and height /2 for center 
            0.0f,   
            WHITE
        );
    }
    

    DrawText(spawner_name.c_str(), position.x, position.y+70, 10, WHITE);
}

void Spawner::SetState(SpawnerState* new_state) {
    current_state = new_state;
    current_state->Enter(*this);
}

void Spawner::HandleCollision(Entity* other_entity, float delta_time) {
    current_state->HandleCollision(*this, other_entity, delta_time);
}

Spawner::Spawner(Vector2 pos, Vector2 size, std::string spawner_name) {
    position = pos;
    this->size = size; 
    this->spawner_name = spawner_name; 
    entity_type = "Spawner";
    texture_component;
    SetState(&spawnerIdle);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Spawner Idle
void SpawnerIdle::Enter(Spawner& spawner) {
    spawner.isSpawnerActive = false;
    spawner.color = ORANGE; 
    
    // *** Component Names Sprite Logic *** // 
    if (spawner.spawner_name=="circular-frame") {
        spawner.texture_component = TextureManager::GetTexture("chest_circle");
    }
    else if (spawner.spawner_name=="box-frame") {
        spawner.texture_component = TextureManager::GetTexture("chest_box");
    }

    else if (spawner.spawner_name=="wing") {
        spawner.texture_component = TextureManager::GetTexture("chest_wing");
    }

    else if (spawner.spawner_name=="gun") {
        spawner.texture_component = TextureManager::GetTexture("chest_gun");
    }

    else if (spawner.spawner_name=="arm") {
        spawner.texture_component = TextureManager::GetTexture("chest_arm");
    }

    else if (spawner.spawner_name=="bulb") {
        spawner.texture_component = TextureManager::GetTexture("chest_bulb");
    }

    else if (spawner.spawner_name=="micro") {
        spawner.texture_component = TextureManager::GetTexture("chest_micro");
    }
}

void SpawnerIdle::Update(Spawner& spawner, float delta_time) {

}

void SpawnerIdle::HandleCollision(Spawner& spawner, Entity* other_entity, float delta_time) {
    // *** COLLISION CHECK: SpawnerIdle to Player ***
    Rectangle spawner_rectangle = { spawner.position.x, spawner.position.y, spawner.size.x, spawner.size.y };
    bool isSpawnerIdleColliding = CheckCollisionCircleRec(other_entity->position, other_entity->radius, spawner_rectangle);
    
    if (isSpawnerIdleColliding && other_entity->entity_type=="Player") {
        spawner.SetState(&spawner.spawnerActive);
    }

}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Spawner Active
void SpawnerActive::Enter(Spawner& spawner) {
    spawner.color = RED; 
    spawner.isSpawnerActive = true;
    spawner.texture_component = TextureManager::GetTexture("chest_open");
    spawner.interaction_timer = 0.0f; 
}

void SpawnerActive::Update(Spawner& spawner, float delta_time) {
    
   

}

void SpawnerActive::HandleCollision(Spawner& spawner, Entity* other_entity, float delta_time) {
    // *** COLLISION CHECK: SpawnerActive to Player ***
    Rectangle spawner_rectangle = { spawner.position.x, spawner.position.y, spawner.size.x, spawner.size.y };
    bool isSpawnerIdleColliding = CheckCollisionCircleRec(other_entity->position, other_entity->radius, spawner_rectangle);

    if(isSpawnerIdleColliding && other_entity->entity_type=="Player" && other_entity->isInteractPressed) {
        std::cout << "PLAYER INTERACT: Item spawned here" << std::endl; 
        spawner.spawned_items.push_back(spawner.SpawnItem(spawner));
        std::cout << "PLAYER INTERACT: Number of Spawned Items - " <<  spawner.spawned_items.size() << std::endl;

    }
    
    if (!isSpawnerIdleColliding) {
        spawner.interaction_timer += delta_time;
        if (spawner.interaction_timer > 1.0f) {  // 1 second delay
            spawner.SetState(&spawner.spawnerIdle);
        }
    } else {
        spawner.interaction_timer = 0.0f;
    }

}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Item Spawner::SpawnItem(Spawner& spawner) {
    float item_radius = 30.0f;
    return Item(spawner.position, item_radius, spawner.spawner_name, "component");
}

void Spawner::Reset() {
    spawned_items.clear();              // Remove all spawned items
    interaction_timer = 0.0f;           // Reset interaction timer
    SetState(&spawnerIdle);             // Reset to idle state
}


