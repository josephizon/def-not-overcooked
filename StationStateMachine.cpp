// TODO:
// - now its the output nalang and combinatiosn from each item
// - updated the stations logic, it now always erases the item in items_in_station as long as its not colliding
// - stopped using find coz memory address unreliable had to use boolean value instead
#include <raylib.h>
#include <raymath.h>
#include <vector> 
#include <algorithm> 
#include "Station.hpp"



void Station::Update(float delta_time) {
    isStationOccupied = !items_in_station.empty();
    current_state->Update(*this, delta_time);
}

void Station::Draw() {  
    //DrawRectangleV(position, size, color);

    // based on last year 
    texture_frame = {0, 0, (float)texture_component.width, (float)texture_component.height};
    DrawTexturePro(
        texture_component,
        texture_frame,
        {position.x, position.y, size.x, size.y},
        {0, 0}, // destination width and height /2 for center 
        0.0f,   
        WHITE
);

    DrawText("crafting station", position.x-5, position.y+70, 10, WHITE);

}

void Station::SetState(StationState* new_state) {
    current_state = new_state;
    current_state->Enter(*this);
}

void Station::HandleCollision(Entity* other_entity) {
    current_state->HandleCollision(*this, other_entity);
}

Station::Station(Vector2 pos, Vector2 size) {
    this->position = pos;
    this->size = size;
    isStationOccupied = false;  
    entity_type = "Station";
    texture_component; 
    SetState(&stationIdle);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Station Idle
void StationIdle::Enter(Station& station) {
    station.isStationProcessing = false;
    station.isStationComplete = false; 
    station.color = WHITE;

    station.texture_component = TextureManager::GetTexture("station_idle");  
}

void StationIdle::Update(Station& station, float delta_time) {

    // *** Station Idle to Station Holding ***
    if (station.isStationOccupied) {
       station.SetState(&station.stationHolding);
    }
}

void StationIdle::HandleCollision(Station& station, Entity* other_entity) {

    // *** COLLISION CHECK: StationIdle to Item ***
    Rectangle station_rectangle = { station.position.x, station.position.y, station.size.x, station.size.y };
    bool isStationIdleColliding = CheckCollisionCircleRec(other_entity->position, other_entity->radius, station_rectangle);
    
    if (isStationIdleColliding && !other_entity->isItemOnStation && other_entity->entity_type=="Item") {
            other_entity->isItemOnStation = true;
            station.items_in_station.push_back(other_entity);
            // for (auto& entity : station.items_in_station) {
            //     // std::cout << "Pointer: " << entity << std::endl; 
            //     //std::cout << "isItemOnStation: " << entity->isItemOnStation << std::endl; 
            // }
            std::cout << "STATION IDLE: New item added to station | New Count: " << station.items_in_station.size() << std::endl;
            
    }

    
    if (!isStationIdleColliding && other_entity->isItemOnStation && other_entity->entity_type=="Item") {
        other_entity->isItemOnStation = false;
    }
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Station Holding
// only 1 item
void StationHolding::Enter(Station& station) {
    station.isStationProcessing = false;
    station.isStationComplete = false; 
    station.color = BLUE;

    station.texture_component = TextureManager::GetTexture("station_holding"); 
}

void StationHolding::Update(Station& station, float delta_time) {

    station.RemoveItems(); 

    // *** Station Holding to Station Idle ***
    if (!(station.isStationOccupied)) {
        station.SetState(&station.stationIdle);
    }

    // *** Station Holding to Station Processing ***
    if (station.items_in_station.size() >= 3) {
        station.SetState(&station.stationProcessing);
    }
    
    
}

void StationHolding::HandleCollision(Station& station, Entity* other_entity) {

    // *** COLLISION CHECK: StationHolding to Item ***
    Rectangle station_rectangle = { station.position.x, station.position.y, station.size.x, station.size.y };
    bool isStationHoldingColliding = CheckCollisionCircleRec(other_entity->position, other_entity->radius, station_rectangle); 

    if (isStationHoldingColliding && !other_entity->isItemOnStation && other_entity->entity_type=="Item") {
            other_entity->isItemOnStation = true;
            station.items_in_station.push_back(other_entity);
            // for (auto& entity : station.items_in_station) {
            //     // std::cout << "Pointer: " << entity << std::endl; 
            //     // std::cout << "isItemOnStation: " << entity->isItemOnStation << std::endl;  
            // }
            
            std::cout << "STATION HOLDING: New item added | New Count: " << station.items_in_station.size() << std::endl;
    }

    if (!isStationHoldingColliding && other_entity->isItemOnStation && other_entity->entity_type=="Item") {
        other_entity->isItemOnStation = false;
    }

    
    
    
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Station Processing
// for more than 1 item and you could start interacting
void StationProcessing::Enter(Station& station) {
    station.color = GREEN;
    station.texture_component = TextureManager::GetTexture("station_processing"); 
}

void StationProcessing::Update(Station& station, float delta_time) {
    station.RemoveItems(); 

    station.isStationProcessing = true; 
    //std:: cout << station.isStationProcessing  << std::endl;    

    // *** Station Processing  to Station Hold ***
    if (station.items_in_station.size() < 3) {
        station.SetState(&station.stationHolding);
    }


    
}

void StationProcessing::HandleCollision(Station& station, Entity* other_entity) {

    // *** COLLISION CHECK: StationProcessing to Item ***
    Rectangle station_rectangle = { station.position.x, station.position.y, station.size.x, station.size.y };
    bool isStationProcessingColliding = CheckCollisionCircleRec(other_entity->position, other_entity->radius, station_rectangle); 

    if (isStationProcessingColliding && !other_entity->isItemOnStation && other_entity->entity_type=="Item" ) {
            other_entity->isItemOnStation = true;
            station.items_in_station.push_back(other_entity);
            // for (auto& entity : station.items_in_station) {
            //     //std::cout << "Pointer: " << entity << std::endl; 
            //     //std::cout << "isItemOnStation: " << entity->isItemOnStation << std::endl; 
            // }
            std::cout << "STATION PROCESSING: New item added | New Count: " << station.items_in_station.size() << std::endl;
        
    }

    if (!isStationProcessingColliding && other_entity->isItemOnStation && other_entity->entity_type=="Item" ) {
        other_entity->isItemOnStation = false;
    }

    

    // *** Station Processing  to Station Complete ***
    if(isStationProcessingColliding && station.isStationComplete) {
          
        station.SetState(&station.stationComplete);
        
    }
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Station Complete
void StationComplete::Enter(Station& station) {
    station.texture_component = TextureManager::GetTexture("station_complete"); 
    station.color = BLACK;
    station.push_back_delay = 2.0f; 

    std::cout << "STATION COMPLETE: Enter | Items to delete: " << station.items_in_station.size() << std::endl;

    for (Entity* entity : station.items_in_station) {
        entity->isItemForDeletion = true; 
        
        station.items_in_station_names.push_back(entity->item_name); 
        std::cout << "Item Name Added: " << entity->item_name << std::endl; 
        // std::cout << "FLAGGING ITEM: " << entity << " " << entity->isItemForDeletion << std::endl;
    }

    // CHECK FOR WHAT NAMES GET PRINTED 
    for (auto& name: station.items_in_station_names) {
        std::cout <<  "items_in_station_names: " << name << std::endl;  
    }
    station.item_complete_robot.push_back(station.SpawnCompleteRobot(station));


}

void StationComplete::Update(Station& station, float delta_time) {
    station.push_back_delay -= delta_time; 
    

    if (station.push_back_delay <= 0.0f ) {
        station.items_in_station.clear(); 
        station.items_in_station_names.clear(); 

        // *** Station Complete to Station Idle *** //
        if (station.items_in_station.empty()) {
            station.SetState(&station.stationIdle);
        }

    }
    
    
}

void StationComplete::HandleCollision(Station& station, Entity* other_entity) {
    // *** COLLISION CHECK: StationComplete to Item ***
    Rectangle station_rectangle = { station.position.x, station.position.y, station.size.x, station.size.y };
    bool isStationCompleteColliding = CheckCollisionCircleRec(other_entity->position, other_entity->radius, station_rectangle); 

    if (isStationCompleteColliding && other_entity->isItemOnStation && other_entity->entity_type=="Item" ) {
            other_entity->isItemForDeletion = true; 
            std::cout << "FLAGGING EXTRA ITEMS: " << other_entity << " " << other_entity->isItemForDeletion << std::endl;
            
    }
    
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void Station::RemoveItems() {
    auto& items = items_in_station;

    // debugging purposes
    // std::vector<Entity*> to_remove;
    // for (Entity* e : items) {
    //     if (!e->isItemOnStation) {
    //         to_remove.push_back(e);
    //     }
    // }

    
    // for (Entity* e : to_remove) {
    //     std::cout << "STATION: Removing item -> " << e << std::endl; // or use e->id if you have one
    // }

    // Now erase them
    items.erase(std::remove_if(items.begin(), items.end(),
        [](Entity* e) {
            return !e->isItemOnStation;
        }), items.end());

    //std::cout << "STATION: Item removed | New Count: " << items.size() << std::endl;
}

Item Station::SpawnCompleteRobot(Station& station) {
    float item_radius = 30.0f;
    std::string robot_name;

    // this sorts it so that no matter wat arrangement it is is in the vector, its still right
    std::vector<std::string> names = station.items_in_station_names;
    std::sort(names.begin(), names.end());

    
    std::string combo_key = names[0] + "_" + names[1] + "_" + names[2];

    // *** Combos for the Items to Station Spawn*** // 
    if (combo_key == "circular-frame_wing_wing") {
        robot_name = "wingbot";
    } 
    
    else if (combo_key == "arm_arm_circular-frame") {
        robot_name = "clawbot";
    } 

    else if (combo_key == "arm_box-frame_wing") {
        robot_name = "gunbot";
    } 

    else if (combo_key == "box-frame_gun_wing") {
        robot_name = "ballbot";
    } 

    else if (combo_key == "box-frame_bulb_micro") {
        robot_name = "angrybot";
    } 
    
    else if (combo_key == "bulb_circular-frame_micro") {
        robot_name = "bombbot";
    } 
    
    else {
        robot_name = "junkbot";  // fallback
    }

    std::cout << "SPAWNING ROBOT: " << robot_name << std::endl;


    return Item(station.position, item_radius, robot_name, "robot");
}

void Station::Reset() {
    items_in_station.clear();
    items_in_station_names.clear();
    item_complete_robot.clear();
    heldItem = nullptr;
    isStationOccupied = false;
    isStationProcessing = false;
    isStationComplete = false;
    push_back_delay = 0.0f;

    SetState(&stationIdle);
}

