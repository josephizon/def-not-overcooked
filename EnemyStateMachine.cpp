#include <raylib.h>
#include <raymath.h>
#include <iostream>

#include "Enemy.hpp"


void Enemy::Update(float delta_time) {
    current_state->Update(*this, delta_time);

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

void Enemy::Draw() {
    //DrawRectanglePro({position.x, position.y, 30.0f, 30.0f}, {15.0f, 15.0f}, rotation, color);
    DrawTexturePro(
        texture_player,
        player_per_frame,                      
        { position.x, position.y, player_per_frame.width * 2, player_per_frame.height*2},
        { player_per_frame.width, player_per_frame.height}, // reminder: divide by 2 but since times 2 removed
        rotation,                      
        WHITE
    );

    //DrawCircleLines(position.x, position.y, ready_attack_radius, RED);
    //DrawCircleLines(position.x, position.y, detection_radius, ORANGE);
    //DrawCircleLines(position.x, position.y, aggro_radius, YELLOW);
}

void Enemy::SetState(EnemyState* new_state) {
    current_state = new_state;
    current_state->Enter(*this);
}

void Enemy::HandleCollision(Entity* other_entity) {
    current_state->HandleCollision(*this, other_entity);
}

Enemy::Enemy(Vector2 pos, float spd, float rad, float d_radius, float a_radius, float r_radius, int hp) {
    position = pos;
    speed = spd;
    radius = rad;
    detection_radius = d_radius;
    aggro_radius = a_radius;
    ready_attack_radius = r_radius;
    health = hp;
    rotation = 0.0f;
    active = true;
    entity_type = "enemy";

    texture_player = TextureManager::GetTexture("enemy_moving");
    max_frames = 6; 
    frame_speed = 10;
    current_frame = 0;
    frame_time = 1.0f / frame_speed;
    frame_counter = 0.0f;
    player_per_frame = { 0.0f, 0.0f, ((float)texture_player.width / max_frames), (float)texture_player.height };    
    
    SetState(&wandering);
}

void EnemyWandering::Enter(Enemy& enemy) {
    enemy.color = VIOLET;
    change_direction_cooldown = GetRandomValue(1, 3);
    move_direction.x = GetRandomValue(-100.0f, 100.0f) / 100.0f;
    move_direction.y = GetRandomValue(-100.0f, 100.0f) / 100.0f;

    move_direction = Vector2Normalize(move_direction);

    enemy.entity_following = nullptr;

    enemy.rotation = 0.0f;

    enemy.texture_player = TextureManager::GetTexture("enemy_moving");
    enemy.max_frames = 6;
}

void EnemyChasing::Enter(Enemy& enemy) {
    enemy.color = YELLOW;
    enemy.texture_player = TextureManager::GetTexture("enemy_chasing");
    enemy.max_frames = 4;
}

void EnemyReady::Enter(Enemy& enemy) {
    enemy.color = ORANGE;
    ready_timer = 2.0f;
    enemy.texture_player = TextureManager::GetTexture("enemy_readying");
    enemy.max_frames = 15; 
}

void EnemyAttacking::Enter(Enemy& enemy) {
    enemy.color = RED;
    attack_direction = Vector2Scale(Vector2Normalize(Vector2Subtract(enemy.entity_following->position, enemy.position)), 800.0f);
    enemy.acceleration = attack_direction;
    
}


void EnemyWandering::Update(Enemy& enemy, float delta_time) {
    enemy.velocity = Vector2Zero();

    if (change_direction_cooldown <= 0.0f) {
        move_direction.x = GetRandomValue(-100.0f, 100.0f) / 100.0f;
        move_direction.y = GetRandomValue(-100.0f, 100.0f) / 100.0f;

        move_direction = Vector2Normalize(move_direction);
        change_direction_cooldown = GetRandomValue(1, 3);

       
    }
    else {
        change_direction_cooldown -= delta_time;
    }

    enemy.velocity = Vector2Scale(move_direction, 50.0f);
    
    if (0 + enemy.radius+2 < enemy.position.y + (enemy.velocity.y * delta_time) 
        && enemy.position.y + (enemy.velocity.y * delta_time) < 720 - enemy.radius) {
        enemy.position.y = enemy.position.y + (enemy.velocity.y * delta_time); 
    }
    if (0 + enemy.radius < enemy.position.x + (enemy.velocity.x * delta_time) 
        && enemy.position.x + (enemy.velocity.x * delta_time) < 1280 - enemy.radius) {
        enemy.position.x = enemy.position.x + (enemy.velocity.x * delta_time);
    }

    //enemy.position = Vector2Add(enemy.position, Vector2Scale(enemy.velocity, delta_time));


    if (enemy.invulnerable_timer > 0.0f) {
        enemy.invulnerable_timer -= delta_time;
    }

    if (enemy.health <= 0) {
        enemy.active = false;
    }
}

void EnemyWandering::HandleCollision(Enemy& enemy, Entity* other_entity) {
    if (CheckCollisionCircles(enemy.position, enemy.detection_radius, other_entity->position, other_entity->radius)) {
        enemy.entity_following = other_entity;
        enemy.SetState(&enemy.chasing);
    }
}

void EnemyChasing::Update(Enemy& enemy, float delta_time) {
    enemy.velocity = Vector2Subtract(enemy.entity_following->position, enemy.position);
    enemy.velocity = Vector2Normalize(enemy.velocity);

    enemy.rotation = Vector2Angle({0.0f, -1.0f}, enemy.velocity) * 360 / PI;

    enemy.velocity = Vector2Scale(enemy.velocity, 100.0f);

    if (0 + enemy.radius+2 < enemy.position.y + (enemy.velocity.y * delta_time) 
        && enemy.position.y + (enemy.velocity.y * delta_time) < 720 - enemy.radius) {
        enemy.position.y = enemy.position.y + (enemy.velocity.y * delta_time); 
    }
    if (0 + enemy.radius < enemy.position.x + (enemy.velocity.x * delta_time) 
        && enemy.position.x + (enemy.velocity.x * delta_time) < 1280 - enemy.radius) {
        enemy.position.x = enemy.position.x + (enemy.velocity.x * delta_time);
    }
    //enemy.position = Vector2Add(enemy.position, Vector2Scale(enemy.velocity, delta_time));

    if (enemy.invulnerable_timer > 0.0f) {
        enemy.invulnerable_timer -= delta_time;
    }

    if (enemy.health <= 0) {
        enemy.active = false;
    }
}

void EnemyChasing::HandleCollision(Enemy& enemy, Entity* other_entity) {
    if (CheckCollisionCircles(enemy.position, enemy.ready_attack_radius, other_entity->position, other_entity->radius)) {
        enemy.SetState(&enemy.ready);
    }
    
    if (!CheckCollisionCircles(enemy.position, enemy.aggro_radius, other_entity->position, other_entity->radius)) {
        enemy.SetState(&enemy.wandering);
    }
}

void EnemyReady::Update(Enemy& enemy, float delta_time) {
    aim_direction = Vector2Subtract(enemy.entity_following->position, enemy.position);
    aim_direction = Vector2Normalize(aim_direction);

    enemy.rotation = Vector2Angle({0.0f, -1.0f}, aim_direction) * 360 / PI;

    if (!(ready_timer <= 0.0f)) {
        ready_timer -= delta_time;
    }
    else {
        enemy.SetState(&enemy.attacking);
    }

    if (enemy.invulnerable_timer > 0.0f) {
        enemy.invulnerable_timer -= delta_time;
    }

    if (enemy.health <= 0) {
        enemy.active = false;
    }
}

void EnemyReady::HandleCollision(Enemy& enemy, Entity* other_entity) {
}

void EnemyAttacking::Update(Enemy& enemy, float delta_time) {
    enemy.velocity = Vector2Add(enemy.velocity, enemy.acceleration);
    enemy.velocity = Vector2Subtract(enemy.velocity, Vector2Scale(enemy.velocity, 5.0f * delta_time));
    
    if (0 + enemy.radius+2 < enemy.position.y + (enemy.velocity.y * delta_time) 
        && enemy.position.y + (enemy.velocity.y * delta_time) < 720 - enemy.radius) {
        enemy.position.y = enemy.position.y + (enemy.velocity.y * delta_time); 
    }
    if (0 + enemy.radius < enemy.position.x + (enemy.velocity.x * delta_time) 
        && enemy.position.x + (enemy.velocity.x * delta_time) < 1280 - enemy.radius) {
        enemy.position.x = enemy.position.x + (enemy.velocity.x * delta_time);
    }

    if(Vector2Length(enemy.velocity) < 50.0f) {
        enemy.velocity = Vector2Zero();
        enemy.SetState(&enemy.wandering);
    }

    enemy.acceleration = Vector2Zero();

    if (enemy.invulnerable_timer > 0.0f) {
        enemy.invulnerable_timer -= delta_time;
    }
}

void EnemyAttacking::HandleCollision(Enemy& enemy, Entity* other_entity) {
}