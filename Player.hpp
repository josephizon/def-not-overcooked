#ifndef PLAYER
#define PLAYER

#include <raylib.h>
#include <raymath.h>
#include "Entity.hpp"

class Player;


// control stuff 

struct KeyBinds {
    KeyboardKey up;
    KeyboardKey down;
    KeyboardKey left;
    KeyboardKey right;
    KeyboardKey dash;
    KeyboardKey interact;
    KeyboardKey pickup;
};


// Base Player State
// All Player States inherit this interface
class PlayerState {
public:
    virtual ~PlayerState() {}
    virtual void Enter(Player& player) = 0;
    virtual void Update(Player& player, float delta_time) = 0;
    virtual void HandleCollision(Player& player, Entity* other_entity) = 0;
};

class PlayerIdleNotHolding : public PlayerState {
public:
    void Enter(Player& player);
    void Update(Player& player, float delta_time);
    void HandleCollision(Player& player, Entity* other_entity);
};

class PlayerIdleHolding : public PlayerState {
    public:
        void Enter(Player& player);
        void Update(Player& player, float delta_time);
        void HandleCollision(Player& player, Entity* other_entity);
    };

class PlayerMoving : public PlayerState {
public:
    void Enter(Player& player);
    void Update(Player& player, float delta_time);
    void HandleCollision(Player& player, Entity* other_entity);
};


class PlayerDashing : public PlayerState {
public:
    Vector2 dodge_direction;
    void Enter(Player& player);
    void Update(Player& player,  float delta_time);
    void HandleCollision(Player& player, Entity* other_entity);
};

class PlayerInteracting : public PlayerState {
    public:
        void Enter(Player& player);
        void Update(Player& player, float delta_time);
        void HandleCollision(Player& player, Entity* other_entity);
    };


// Base Player Class
class Player : public Entity {
public:
    Vector2 velocity;
    Vector2 acceleration;
    float speed;
    bool isHoldingItem; // checks if player is holding item
    bool isInConveyerBelt;
    Entity* heldItem = nullptr; // stores the item player is holding
    float pickup_cooldown;  // to counter the same bind for pick up and let go
    float interaction_timer; // how long player has to hold down interact button 
    KeyBinds key_binds; 

    Player(Vector2 pos, float rad, float spd, int player_id, KeyBinds key_binds);

    void Update(float delta_time);
 
    void Draw();

    void SetState(PlayerState* new_state);

    void HandleCollision(Entity* other_entity);

    PlayerIdleNotHolding idleNotHolding;
    PlayerIdleHolding idleHolding;
    PlayerMoving moving;
    PlayerDashing dashing;
    PlayerInteracting interacting;

    // ** Sprite and Animations Stuff ** //
    Texture texture_player; 
    Rectangle player_per_frame; // per frame reference
    int current_frame;
    float frame_time;
    float frame_counter; // 
    int max_frames; // number of frames in the spritesheet
    int frame_speed; // how fast frame go brom

    bool isFacingLeft;
    float flip; 

private:
    PlayerState* current_state;
};


#endif