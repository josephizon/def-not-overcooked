#ifndef ITEM
#define ITEM

#include <raylib.h>
#include <raymath.h>
#include "Entity.hpp"

class Item;


class ItemState {
public:
    virtual ~ItemState() {}
    virtual void Enter(Item& item) = 0;
    virtual void Update(Item& item, float delta_time) = 0;
    virtual void HandleCollision(Item& item, Entity* other_entity) = 0;
};

// Base State Component for Item (solo item)
class ItemBase : public ItemState {
public:
    void Enter(Item& item);
    void Update(Item& item, float delta_time);
    void HandleCollision(Item& item, Entity* other_entity);
};

class ItemLaunch : public ItemState {
    public:
        void Enter(Item& item);
        void Update(Item& item, float delta_time);
        void HandleCollision(Item& item, Entity* other_entity);
};


// Base Item Class
class Item : public Entity {
public:

    Texture texture_component;
    Rectangle texture_frame;

    
    std::string item_type; 

    Item(Vector2 pos, float rad, std::string item_name, std::string item_type);

    void Update(float delta_time);

    void Draw();

    void SetState(ItemState* new_state);

    void HandleCollision(Entity* other_entity);

    ItemBase itemBase;
    ItemLaunch itemLaunch;
    int heldByPlayerIndex = -1; // ADD THIS LINE

private:
    ItemState* current_state;
};


#endif