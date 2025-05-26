// TO DO LIST
// IN PLAYER.HPP ADD CONTROL SCHEME TO BE ABLE TO HAVE PLAYER ONE AND TWO


#include <raylib.h>
#include <raymath.h>
#include "Player.hpp"


void Player::Update(float delta_time) {
    current_state->Update(*this, delta_time);

    if (pickup_cooldown > 0.0f) {
        //std::cout << pickup_cooldown << std::endl; 
        pickup_cooldown -= delta_time;
    }

    // ** Animation Frames ** //
    frame_counter += delta_time;
    if (frame_counter >= frame_time) {
        frame_counter = 0.0f;
        current_frame++;
        if (current_frame >= max_frames) {
            current_frame = 0;
        }
        player_per_frame.x = (float)current_frame * player_per_frame.width; 
    }
}

void Player::Draw() {
    // if (isHoldingItem){
    //     DrawCircleV(position, radius, color);
    // }
    // else{
    //     DrawCircleV(position, radius, color);  
    // }
    DrawTexturePro(
        texture_player,
        player_per_frame,                      
        { position.x, position.y, player_per_frame.width * 2, player_per_frame.height*2},
        { player_per_frame.width, player_per_frame.height}, // reminder: divide by 2 but since times 2 removed
        0.0f,                      
        WHITE
    );
}

void Player::SetState(PlayerState* new_state) {
    current_state = new_state;
    current_state->Enter(*this);
}

void Player::HandleCollision(Entity* other_entity) {
    current_state->HandleCollision(*this, other_entity);
    
}

Player::Player(Vector2 pos, float rad, float spd, int player_id, KeyBinds key_binds) {
    position = pos;
    radius = rad;
    speed = spd;
    this->player_id = player_id;
    this->key_binds = key_binds; 
    isHoldingItem = false; 
    pickup_cooldown = 0.0f;
    entity_type = "Player";
    isInConveyerBelt = false;
    bezierCurveIndex = 0;

    // ** Animation and Sprites ** //
    // more automatic version of the old code
    if (player_id == 1) {
        texture_player = TextureManager::GetTexture("mike_idle");
    }
    else if (player_id == 2) {
        texture_player = TextureManager::GetTexture("paul_idle");
    }
    max_frames = 7; 
    frame_speed = 10;
    current_frame = 0;
    frame_time = 1.0f / frame_speed;
    frame_counter = 0.0f;

    player_per_frame = { 0.0f, 0.0f, ((float)texture_player.width / max_frames), (float)texture_player.height };    

    isFacingLeft = false;
    
    SetState(&idleNotHolding);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Player Idle Not Holding
void PlayerIdleNotHolding::Enter(Player& player) {
    // UNPAIR ITEM AND PERSON
    player.color = WHITE;
    player.isInteractPressed = false; 
    player.max_frames = 7; 
    
    if (player.player_id == 1) {
        player.texture_player = TextureManager::GetTexture("mike_idle");
    }
    else if (player.player_id == 2) {
        player.texture_player = TextureManager::GetTexture("paul_idle");
    }
    
}

void PlayerIdleNotHolding::Update(Player& player, float delta_time) {
    // Misc
    if (player.invulnerable_timer > 0.0f) {
        player.invulnerable_timer -= delta_time;
    }

    // ***Idle Not Holding to Idle Holding***
    if (player.isHoldingItem){
        player.SetState(&player.idleHolding);
    }

    
    // ***Idle to Moving***
    if (IsKeyDown(player.key_binds.up) || IsKeyDown(player.key_binds.left) || IsKeyDown(player.key_binds.down) || IsKeyDown(player.key_binds.right)) {
        if (!player.isInConveyerBelt){
            player.SetState(&player.moving);
        }
    }

    
    // ***Idle to Interact***
    // if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    if (IsKeyDown(player.key_binds.interact)) {
        player.SetState(&player.interacting);
    }
}

// Picking up Item Logic
void PlayerIdleNotHolding::HandleCollision(Player& player, Entity* other_entity) {
    bool isIdleNotHoldingColliding = CheckCollisionCircles(player.position, player.radius, other_entity->position, other_entity->radius);
    if ( isIdleNotHoldingColliding 
        && other_entity->entity_type=="Item" 
        && player.isHoldingItem == false ) {
        if (IsKeyPressed(player.key_binds.pickup) && player.pickup_cooldown <= 0.0f){
            player.heldItem = other_entity; 
            other_entity->isItemBeingHeld = true; 
            other_entity->holder_is = &player;
            player.isHoldingItem = true; 
            //std::cout<< "PLAYERIDLENOTHOLDING: tag for deletion - "<< other_entity << " " <<other_entity->isItemForDeletion <<std::endl; 
            std::cout<< "PLAYERIDLENOTHOLDING: Item Info - " << other_entity->item_name <<std::endl; 
            std::cout<< other_entity->item_name << std::endl; 
        }
    }
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Player Idle Holding
void PlayerIdleHolding::Enter(Player& player) {
    // ADD CODE HOLD ITEM, PAIR ITEM TO PERSON
    player.color = BLACK;
    player.isInteractPressed = false; 
    player.max_frames = 4; 

    if (player.player_id == 1) {
        player.texture_player = TextureManager::GetTexture("mike_holding");
    }
    else if (player.player_id == 2) {
        player.texture_player = TextureManager::GetTexture("paul_holding");
    }
}

void PlayerIdleHolding::Update(Player& player, float delta_time) {
    // ***Idle Holding to Interacting***
    //if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    if (IsKeyDown(player.key_binds.interact)) {
        player.SetState(&player.interacting);
    }
    
    // ***Idle Holding to Idle Not Holding***
    if (IsKeyPressed(player.key_binds.pickup) && player.pickup_cooldown <= 0.0f ){
        //  std::cout << "PLAYER: not holding item" << std::endl;
        player.heldItem->position = player.position;
        player.heldItem->isItemBeingHeld = false; 
        player.heldItem->holder_is = nullptr; 
        player.isHoldingItem = false;
        player.heldItem = nullptr;
        player.pickup_cooldown = 0.5f; 
        player.SetState(&player.idleNotHolding);
    }

    // ***Idle Holding to Moving***
    if (IsKeyDown(player.key_binds.up) || IsKeyDown(player.key_binds.left) || IsKeyDown(player.key_binds.down) || IsKeyDown(player.key_binds.right)) {
        if (!player.isInConveyerBelt){
            player.SetState(&player.moving);
        }
    }

    // Misc
    if (player.invulnerable_timer > 0.0f) {
        player.invulnerable_timer -= delta_time;
    }
}

void PlayerIdleHolding::HandleCollision(Player& player, Entity* other_entity) {
    if (CheckCollisionCircles(player.position, player.radius, other_entity->position, other_entity->radius) && player.invulnerable_timer <= 0.0f && player.isHoldingItem == true && other_entity->entity_type=="enemy") {
        player.heldItem->position = player.position;
        player.heldItem->isItemBeingHeld = false; 
        player.heldItem->holder_is = nullptr;
        player.isHoldingItem = false;
        player.heldItem = nullptr;
        player.pickup_cooldown = 3.0f; 
    }
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Player Moving
void PlayerMoving::Enter(Player& player) {
    player.color = YELLOW;
    if (player.player_id == 1) {
        player.texture_player = TextureManager::GetTexture("mike_run");
    }
    else if (player.player_id == 2) {
        player.texture_player = TextureManager::GetTexture("paul_run");
    }
}

void PlayerMoving::Update(Player& player, float delta_time) {
    // Checks first if in conveyer belt, overides movement if thats the case
    

    // ***Moving***
    player.velocity = {0, 0};

    if (!player.isInConveyerBelt){
        if (IsKeyDown(player.key_binds.up) && player.position.y > 0 + player.radius) {
            player.velocity.y -= 1.0f;
        }
        if (IsKeyDown(player.key_binds.left) && player.position.x > 0 + player.radius) {
            player.velocity.x -= 1.0f;
            player.isFacingLeft = true;
        }
        if (IsKeyDown(player.key_binds.down) && player.position.y < 720 - player.radius) {
            player.velocity.y += 1.0f;
        }
        if (IsKeyDown(player.key_binds.right) && player.position.x < 1280 - player.radius) {
            player.velocity.x += 1.0f;
            player.isFacingLeft = false;
        }
    }

    player.velocity = Vector2Scale(Vector2Normalize(player.velocity), player.speed * delta_time);
    player.position = Vector2Add(player.position, player.velocity);

    // MISC
    if (player.invulnerable_timer > 0.0f) {
        player.invulnerable_timer -= delta_time;
    }

    // ***Moving to Dashing***
    if (IsKeyPressed(player.key_binds.dash) && Vector2Length(player.velocity) > 0) {
        player.velocity = Vector2Normalize(player.velocity);
        player.SetState(&player.dashing);
    }

    // ***Moving to Interact***
    // if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    if (IsKeyDown(player.key_binds.interact)) {
        player.SetState(&player.interacting);
    }

    // ***Moving to Idle Not Holding***
    if(Vector2Length(player.velocity) == 0 && !player.isHoldingItem) {
        player.SetState(&player.idleNotHolding);
    }

    // ***Moving to Idle Holding***
    if(Vector2Length(player.velocity) == 0 && player.isHoldingItem) {
        player.SetState(&player.idleHolding);
    }
}


void PlayerMoving::HandleCollision(Player& player, Entity* other_entity) {
    if (CheckCollisionCircles(player.position, player.radius, other_entity->position, other_entity->radius) && player.invulnerable_timer <= 0.0f && player.isHoldingItem == true && other_entity->entity_type=="enemy") {
        player.heldItem->position = player.position;
        player.heldItem->isItemBeingHeld = false; 
        player.heldItem->holder_is = nullptr; 
        player.isHoldingItem = false;
        player.heldItem = nullptr;
        player.pickup_cooldown = 3.0f; 
    }
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Player Dashing
void PlayerDashing::Enter(Player& player) {
    player.color = LIME;

    dodge_direction = Vector2Scale(player.velocity, 1200.0f);
    player.acceleration = dodge_direction;

    // ** 20 percent chance to drop item ** //
    if (GetRandomValue(0, 99) < 20 && player.isHoldingItem) { // 50% chance
        player.heldItem->position = player.position;
        player.heldItem->isItemBeingHeld = false;
        player.isHoldingItem = false;
        player.heldItem->holder_is = nullptr; 
        player.heldItem = nullptr;
        player.pickup_cooldown = 0.5f;
        std::cout << "ITEM DROPPED" << std::endl; 
    }


    if (player.player_id == 1) {
        player.texture_player = TextureManager::GetTexture("mike_dodge");
    }
    else if (player.player_id == 2) {
        player.texture_player = TextureManager::GetTexture("paul_dodge");
    }
}

void PlayerDashing::Update(Player& player, float delta_time) {
    // ***Dashing***
    player.velocity = Vector2Add(player.velocity, player.acceleration);
    player.velocity = Vector2Subtract(player.velocity, Vector2Scale(player.velocity, 5.0f * delta_time));

    // Cannot Dash Beyond Screen Border
    if (0 + player.radius < player.position.y + (player.velocity.y * delta_time) 
        && player.position.y + (player.velocity.y * delta_time) < 720 - player.radius) {

        player.position.y = player.position.y + (player.velocity.y * delta_time);
        
    }
    if (0 + player.radius < player.position.x + (player.velocity.x * delta_time) 
        && player.position.x + (player.velocity.x * delta_time) < 1280 - player.radius) {
        player.position.x = player.position.x + (player.velocity.x * delta_time);
    }
    
    // ***Dashing to Idle Holding***
    if(Vector2Length(player.velocity) < 50.0f && player.isHoldingItem) {
        player.velocity = Vector2Zero();
        player.SetState(&player.idleHolding);
    }

    // ***Dashing to Idle Not Holding***
    if(Vector2Length(player.velocity) < 50.0f && !player.isHoldingItem) {
        player.velocity = Vector2Zero();
        player.SetState(&player.idleNotHolding);
    }

    player.acceleration = Vector2Zero();

    // MISC
    if (player.invulnerable_timer > 0.0f) {
        player.invulnerable_timer -= delta_time;
    }

    
}

void PlayerDashing::HandleCollision(Player& player, Entity* other_entity) {
    // COMMENT: I think player should be rewarded when they are going to "dodge" enemies by dashing through them
    //          UNSURE because it feels abusable unless theres a timer
    /*
    if (CheckCollisionCircles(player.position, player.radius, other_entity->position, other_entity->radius) && player.invulnerable_timer <= 0.0f && player.isHoldingItem == true && other_entity->entity_type=="enemy") {
        player.heldItem->position = player.position;
        player.heldItem->isItemBeingHeld = false; 
        player.isHoldingItem = false;
        player.heldItem = nullptr;
        player.pickup_cooldown = 0.5f; 
    }*/
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Player Interacting
void PlayerInteracting::Enter(Player& player) {
    player.color = RED;
    player.isInteractPressed = true; 
    player.interaction_timer = 0.0f;  // reset the interaction timer when entering the state

    if (player.player_id == 1) {
        player.texture_player = TextureManager::GetTexture("mike_attack");
    }
    else if (player.player_id == 2) {
        player.texture_player = TextureManager::GetTexture("paul_attack");
    }
}

void PlayerInteracting::Update(Player& player, float delta_time) {
    player.interaction_timer += delta_time;

    // flip on off for isInteractPressed so we dont spawn a bajillion at once
    if (player.isInteractPressed) {
        player.isInteractPressed = false;
    }

    // ***Interacting to Idle Not Holding***
    if ((!IsKeyDown(player.key_binds.interact)) && !player.isHoldingItem) {
        player.SetState(&player.idleNotHolding);
    }

    // ***Interacting to Idle Holding***
    if ((!IsKeyDown(player.key_binds.interact)) && player.isHoldingItem) {
        player.SetState(&player.idleHolding);
    }

    
    // MISC
    if (player.invulnerable_timer > 0.0f) {
        player.invulnerable_timer -= delta_time;
    }
}

void PlayerInteracting::HandleCollision(Player& player, Entity* other_entity) {
    Rectangle entity_rectangle = { other_entity->position.x, other_entity->position.y, other_entity->size.x, other_entity->size.y };
    bool isPlayerInteractingColliding = CheckCollisionCircleRec(player.position, player.radius, entity_rectangle); 

    // *** PlayerInteracting is Colliding with Station Entity *** //
    if (isPlayerInteractingColliding  &&

        player.interaction_timer >= 3.0f && 
        other_entity->entity_type=="Station") {

        // checks if the player is colliding and the station state is StationProcessing
        other_entity->isStationComplete = true; 
        std::cout << "PLAYER INTERACTING: complete status:" << other_entity->isStationComplete << std::endl;

        player.SetState(&player.idleNotHolding);
    }

}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
