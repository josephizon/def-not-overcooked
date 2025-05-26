#include <raylib.h>
#include <raymath.h>
#include "Item.hpp"



void Item::Update(float delta_time) {
    current_state->Update(*this, delta_time);
}

void Item::Draw() {

    // based on last year 
    texture_frame = {0, 0, (float)texture_component.width, (float)texture_component.height};

    if(item_type=="component") {
        //DrawCircleV(position, radius, color);
        DrawTexturePro(
            texture_component,
            texture_frame,
            {position.x, position.y, radius, radius},
            {radius / 2, radius / 2}, // destination width and height /2 for center 
            0.0f,   
            WHITE
    );
    }

    else if (item_type=="robot") {
        //DrawCircleV(position, radius, color);
        DrawTexturePro(
            texture_component,
            texture_frame,
            {position.x, position.y, (float)texture_component.width, (float)texture_component.height},
            {(float)texture_component.width/2, (float)texture_component.height/2},
            0.0f,   
            WHITE
    );
    }

    
}

void Item::SetState(ItemState* new_state) {
    current_state = new_state;
    current_state->Enter(*this);
}

void Item::HandleCollision(Entity* other_entity) {
    current_state->HandleCollision(*this, other_entity);
}

Item::Item(Vector2 pos, float rad, std::string item_name, std::string item_type) {
    position = pos;
    radius = rad;
    this->item_name = item_name; 
    this->item_type = item_type; 
    isItemForDeletion = false; 
    entity_type = "Item";
    isItemBeingHeld = false;   
    SetState(&itemBase);
    
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Item Idle
void ItemBase::Enter(Item& item) {
    
    // *** Component Names Sprite Logic *** // 
    if (item.item_name=="circular-frame") {
        item.texture_component = TextureManager::GetTexture("comp_circle");
        item.color = RED; 
    }

    else if (item.item_name=="box-frame") {
        item.texture_component = TextureManager::GetTexture("comp_box");;
        item.color = ORANGE; 
    }

    else if (item.item_name=="wing") {
        item.texture_component = TextureManager::GetTexture("comp_wing");
        item.color = YELLOW; 
    }

    else if (item.item_name=="gun") {
        item.texture_component = TextureManager::GetTexture("comp_gun");;
        item.color = GREEN; 
        }

    else if (item.item_name=="arm") {
        item.texture_component = TextureManager::GetTexture("comp_arm");
        item.color = BLUE; 
    }
    else if (item.item_name=="bulb") {
        item.texture_component = TextureManager::GetTexture("comp_bulb");
        item.color = PURPLE; 
    }
    else if (item.item_name=="micro") {
        item.texture_component = TextureManager::GetTexture("comp_micro");
        item.color = PINK; 
    }


    // *** Complete Robots Sprite Logic *** // 
    else if (item.item_name=="wingbot") {
        item.texture_component = TextureManager::GetTexture("bot_wingbot");
        item.color = WHITE; 
    }

    else if (item.item_name=="clawbot") {
        item.texture_component = TextureManager::GetTexture("bot_clawbot");
        item.color = WHITE; 
    }

    else if (item.item_name=="gunbot") {
        item.texture_component = TextureManager::GetTexture("bot_gunbot");
        item.color = WHITE; 
    }
    else if (item.item_name=="ballbot") {
        item.texture_component = TextureManager::GetTexture("bot_ballbot");
        item.color = WHITE; 
    }
    else if (item.item_name=="angrybot") {
        item.texture_component = TextureManager::GetTexture("bot_angrybot");
        item.color = WHITE; 
    }
    else if (item.item_name=="bombbot") {
        item.texture_component = TextureManager::GetTexture("bot_bombot");
        item.color = WHITE; 
    } 

    else {
        item.texture_component = TextureManager::GetTexture("bot_junkbot");
        item.color = PURPLE; 
    }
    
}

void ItemBase::Update(Item& item, float delta_time) {


}

void ItemBase::HandleCollision(Item& item, Entity* other_entity) {

}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Item Launchg
void ItemLaunch::Enter(Item& item) {
    item.color = PURPLE;
}

void ItemLaunch::Update(Item& Item, float delta_time) {

}

void ItemLaunch::HandleCollision(Item& item, Entity* other_entity) {

}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


