// Phil Cut Again?!
// Phil Artuz | Joseph Izon | Joerell Lao
// GDEV 42

#include <raylib.h>
#include <raymath.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

#include "PlayerStateMachine.cpp"
#include "EnemyStateMachine.cpp"
#include "StationStateMachine.cpp"
#include "ItemStateMachine.cpp"
#include "SpawnerStateMachine.cpp"
#include "tileSheetMapping.cpp"
#include "ScoreSystemMachine.cpp"
#include "TextureManager.cpp"
#include "Entity.hpp"
#include "LeaderBoardManager.hpp"
#include "LeaderBoardManager.cpp"
const int WINDOW_WIDTH(1280);
const int WINDOW_HEIGHT(720);
const float FPS(60.0f);

Rectangle camera_window = {(WINDOW_WIDTH / 2) - 150, (WINDOW_HEIGHT / 2) - 150, 300.0f, 300.0f};
float cam_drift = 5.0f;
float cam_zoom;
bool zoom_in;
Camera2D camera_view = {0};
//change for how long a level lasts
const float level_time_limit = 300.0f;

//for quitting
bool confirmQuit = false;
enum GameScene {
    TITLE,
    LEVELSELECT,
    LEVEL1,
    LEVEL2,
    LEVEL3,
    SCORESCREEN,
    LEADERBOARD
};

int selectedLevelIndex = 0;
int LastScoreRecorded;
GameScene currentScene = TITLE;
void MoveCamera(Camera2D* cam, Player* player, float delta_time) {
    float cam_push_x = 0.0f;
    float cam_push_y = 0.0f;
    float drift_x = Clamp(player->position.x - (camera_window.x + camera_window.x + camera_window.width) / 2, -cam_drift, cam_drift);
    float drift_y = Clamp(player->position.y - (camera_window.y + camera_window.y + camera_window.height) / 2, -cam_drift, cam_drift);

    //DrawRectangleLines(camera_window.x, camera_window.y, camera_window.width, camera_window.height, RED); 

    if (player->position.x > camera_window.x + camera_window.width) {
        if (cam->target.x + camera_window.width < 1150){
            cam_push_x = player->position.x - (camera_window.x + camera_window.width);
            cam->target.x += cam_push_x;
            camera_window.x += cam_push_x;
        }
    }
    else if (player->position.x < camera_window.x) {
        if (430 < cam->target.x){
            cam_push_x = player->position.x - camera_window.x;
            cam->target.x += cam_push_x;
            camera_window.x += cam_push_x;
        }
    }
    else {
        if (430 < cam->target.x && cam->target.x + camera_window.width < 1150){
            cam->target.x += drift_x;
            camera_window.x += drift_x;
        }
    }

    if (player->position.y > camera_window.y + camera_window.height) {
        if (cam->target.y + camera_window.height < 780){
            cam_push_y = player->position.y - (camera_window.y + camera_window.height);
            cam->target.y += cam_push_y;
            camera_window.y += cam_push_y;
        }
    }
    else if (player->position.y < camera_window.y) {
        if (245 < cam->target.y){
            cam_push_y = player->position.y - camera_window.y;
            cam->target.y += cam_push_y;
            camera_window.y += cam_push_y;
        }
    }
    else {
        if (245 < cam->target.y && cam->target.y + camera_window.height < 780){
            cam->target.y += drift_y;
            camera_window.y += drift_y;
        }
    }
    
}

void DrawTitleScreen(GameScene& currentScene) {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("NOT OVERCOOKED", WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 - 60, 50, WHITE);
    DrawText("Press ENTER to Start", WINDOW_WIDTH / 2 - 170, WINDOW_HEIGHT / 2 + 20, 30, GRAY);

    if (IsKeyPressed(KEY_ENTER)) {
        
        currentScene = LEVELSELECT;
    }
}

void DrawLevelSelect(GameScene& currentScene, std::vector<Player>& players, tileSheet& tileMap, tileSheet& tileMap02, tileSheet& tileMap03,
    Station& station, ScoreSystem& ScoreSystem, std::vector<Spawner>& spawners) {

    const char* levelNames[] = { "Co-op 1", "Co-op 2", "SinglePlayer", "Leaderboard" };
    const int menuOptions = 4;  // 3 levels + 1 leaderboard

    // Input: navigate selection
    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
        selectedLevelIndex = (selectedLevelIndex - 1 + menuOptions) % menuOptions;
    }
    if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
        selectedLevelIndex = (selectedLevelIndex + 1) % menuOptions;
    }

    BeginDrawing();
    ClearBackground(BLACK);

    DrawText("Select a Level", WINDOW_WIDTH / 2 - 130, 100, 40, WHITE);

    for (int i = 0; i < menuOptions; ++i) {
        Color color = (i == selectedLevelIndex) ? YELLOW : WHITE;
        DrawText(levelNames[i], WINDOW_WIDTH / 2 - 50, 200 + i * 60, 30, color);
    }

    // Confirm selection
    if (IsKeyPressed(KEY_ENTER)) {
        LastScoreRecorded = 0;
        switch (selectedLevelIndex) {
            case 0: 
                camera_window = {tileMap.returnPlayerPosition().x - 150, tileMap.returnPlayerPosition().y -150, 300.0f, 300.0f};
                camera_view.target = tileMap.returnPlayerPosition();
                currentScene = LEVEL1; 
                break;
            case 1: 
                camera_window = {tileMap03.returnPlayerPosition().x - 150, tileMap03.returnPlayerPosition().y -150, 300.0f, 300.0f};
                camera_view.target = tileMap03.returnPlayerPosition();
                currentScene = LEVEL3; 
                break;
            case 2: 
                camera_window = {tileMap02.returnPlayerPosition().x - 150, tileMap02.returnPlayerPosition().y -150, 300.0f, 300.0f};
                camera_view.target = tileMap02.returnPlayerPosition();
                currentScene = LEVEL2; 
                break;
            case 3:
                currentScene = LEADERBOARD;
                break;
        }
    }

}


void DrawScoreScreen(GameScene& currentScene) {
    BeginDrawing();
    ClearBackground(BLACK);
    std::string scoreText = "YOU GOT " + std::to_string(LastScoreRecorded);
    DrawText(scoreText.c_str(), WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 - 60, 50, WHITE);
    DrawText("Press ENTER to Continue...", WINDOW_WIDTH / 2 - 170, WINDOW_HEIGHT / 2 + 20, 30, GRAY);

    if (IsKeyPressed(KEY_ENTER)) {
        
        currentScene = LEVELSELECT;
    }
}
void DrawLeaderboard(GameScene& currentScene) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        currentScene = LEVELSELECT;
    }

    BeginDrawing();
    ClearBackground(BLACK);

    DrawText("Leaderboard", WINDOW_WIDTH / 2 - 80, 50, 40, WHITE);

    int columnX[] = { 100, WINDOW_WIDTH / 2 - 50, WINDOW_WIDTH - 250 };
    const char* levelTitles[] = { "Level 1", "Level 2", "Level 3" };

    for (int i = 0; i < 3; ++i) {
        DrawText(levelTitles[i], columnX[i], 120, 30, YELLOW);

        for (int j = 0; j < levelScores[i].size(); ++j) {
            std::string scoreText = std::to_string(j + 1) + ". " + std::to_string(levelScores[i][j]);
            DrawText(scoreText.c_str(), columnX[i], 160 + j * 40, 25, WHITE);
        }
    }
    DrawText("Press ESC to return", WINDOW_WIDTH / 2 - 170, WINDOW_HEIGHT / 2 + 100, 30, GRAY);
}
void UpdateAndDrawLevel1(float delta_time, GameScene& currentScene, bool& game_ongoing, bool& enemy_lose,
    std::vector<Player>& players, std::vector<Enemy>& enemies, tileSheet& tileMap, Station& station,
    ScoreSystem& ScoreSystem, std::vector<Spawner>& spawners, Texture& tileSet, Camera2D& camera_view, float& level_timer) {
    
    level_timer += delta_time;

    static bool confirmQuit = false;

    bool timeout = level_timer >= level_time_limit;

    if (!confirmQuit && (IsKeyPressed(KEY_ESCAPE) || timeout)) {
        confirmQuit = true;
    }
    if (game_ongoing) {
        for (auto& enemy : enemies) {
            if(enemy.active) {
                enemy_lose = false;
                enemy.Update(delta_time);

                for (auto& player: players) {
                    enemy.HandleCollision(&player);
                    player.HandleCollision(&enemy);
                }
                tileMap.HandleCollision(&enemy);
            }
        }

        for (auto& player: players) {
            player.Update(delta_time);
            player.HandleCollision(&station);
            tileMap.HandleCollision(&player);
        }
        
        station.Update(delta_time);
        ScoreSystem.Update(delta_time);

        

        for (auto& spawner : spawners) {
            for (auto& player: players) {
                player.HandleCollision(&spawner);
                spawner.HandleCollision(&player, delta_time);
            }
           
            spawner.Update(delta_time);
            

            for (auto& item : spawner.spawned_items) {
                for (auto& player: players) {
                    player.HandleCollision(&item);
                }
                
                station.HandleCollision(&item);
            }

            auto& items = spawner.spawned_items;
            items.erase(std::remove_if(items.begin(), items.end(), [](const Item& item) {
                return item.isItemForDeletion;
            }), items.end());
        }

        for (auto& complete_robot : station.item_complete_robot) {
            for (auto& player: players) {
                player.HandleCollision(&complete_robot);
            }
            
            ScoreSystem.HandleCollision(&complete_robot);

            auto& items = station.item_complete_robot;
            items.erase(std::remove_if(items.begin(), items.end(), [](const Item& item) {
                return item.isItemForDeletion;
            }), items.end());
        }
    }

    

    BeginDrawing();
    BeginMode2D(camera_view);
    ClearBackground(BLACK);

    

    tileMap.draw(tileSet);
    MoveCamera(&camera_view, &players[0], delta_time);
    station.Draw();
    ScoreSystem.DrawStation();


    if (game_ongoing) {
        for (auto& spawner : spawners) {
            spawner.Draw();
            for (auto& item : spawner.spawned_items) {
                if (item.isItemBeingHeld) {
                    Player* p = static_cast<Player*>(item.holder_is);
                    if (p) {
                        item.position.x = p->position.x;
                        item.position.y = p->position.y - 40 ;
                    }
                    // item.position.x = player.position.x;
                    // item.position.y = player.position.y - 40;
                    
                }
                item.Draw();
            }
        }

        for (auto& complete_robot : station.item_complete_robot) {

            if (complete_robot.isItemBeingHeld) {
                Player* p = static_cast<Player*>(complete_robot.holder_is);
                if(p) {
                    complete_robot.position.x = p->position.x;
                    complete_robot.position.y = p->position.y - 40;
                }
            }
            complete_robot.Draw();
        }

        for(auto& player: players) {
            player.Draw();
        }

        for (auto& enemy : enemies) {
            if (enemy.active) {
                enemy.Draw();
            }
        }
    }

   EndMode2D(); 
   // Dtimer
   int seconds_left = (int)(level_time_limit - level_timer);
   if (seconds_left < 0) seconds_left = 0;
   std::string timerText = "Time: " + std::to_string(seconds_left);
   int textWidth = MeasureText(timerText.c_str(), 30);
   DrawText(timerText.c_str(), WINDOW_WIDTH - textWidth - 50, 20, 30, WHITE);
   
    ScoreSystem.Draw();
    //Pause Confirmation Dialog and Resets
    if (confirmQuit) {
        // Draw pause overlay
        LastScoreRecorded = ScoreSystem.playerPoints;

        DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Fade(BLACK, 0.5f));
        DrawRectangle(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 - 60, 400, 120, DARKGRAY);
        DrawText("Are you sure you want to quit?", WINDOW_WIDTH / 2 - 180, WINDOW_HEIGHT / 2 - 40, 20, WHITE);
        DrawText("Press Y to quit | N to cancel", WINDOW_WIDTH / 2 - 160, WINDOW_HEIGHT / 2, 20, LIGHTGRAY);


        if (IsKeyPressed(KEY_Y) || timeout) {
            
            // Quit: Reset and return to Level Select
            if(timeout)
            {
                AddScoreToLeaderboard(selectedLevelIndex, LastScoreRecorded);
                currentScene = SCORESCREEN;
            }
            else
            {
                currentScene = LEVELSELECT;
            }

            game_ongoing = true;
            enemy_lose = false;
            level_timer = 0.0f;
            
            station.Reset();
            ScoreSystem.Reset();
            for (auto& spawner : spawners) {
                spawner.Reset();
            }

            players[0].position = tileMap.returnPlayerPosition();
            players[1].position = {tileMap.returnPlayerPosition().x + 100, tileMap.returnPlayerPosition().y };
            confirmQuit = false;
            return;
        }
        else if (IsKeyPressed(KEY_N)) {
            // Cancel quit
            confirmQuit = false;
        }

        return;  // Skip rest of update/draw while paused for confirm
    }
}

int main() {

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Artuz_Izon_Lao_FinalProject");
    
    SetExitKey(0); // disables ESC key from automatically closing the window
    SetTargetFPS(FPS);

    TextureManager texture_manager; 
    texture_manager.LoadTextures(); 

    bool game_ongoing = true;
    bool enemy_lose = false;

    float level_timer = 0.0f;

    LoadLeaderboard("texts/leaderboard.txt");

    // SETUP AND LOAD TILE MAP
    tileSheet tileMap(5, "texts/RobotsLevel1.txt");
    tileMap.setupTileMap();

    tileSheet tileMap02(5, "texts/RobotsLevel2.txt");
    tileMap02.setupTileMap();

    tileSheet tileMap03(5, "texts/RobotsLevel3.txt");
    tileMap03.setupTileMap();

    //Level 2 Conveyor
    Vector2 v1 = {275, 50};     // Start
    Vector2 v2 = {1500, 50};    // Bigger S Curve: {1500, 60}    Smaller Curve: {750, 50}
    Vector2 v3 = {-100, 250};   // Bigger S Curve: {-100, 200}   Smaller Curve: {500, 250}
    Vector2 v4 = {825, 250};    // End
    std::vector<Vector2> cpv = {v1, v2, v3, v4};
    tileMap02.AddBezierCurver(3, 100, 4, cpv, 5);
    //tileMap03.AddBezierCurver(3, 100, 4, cpv, 5);
    v1 = {1160, 515};       // Start
    v2 = {1150, 650};
    v3 = {1025, 675}; 
    v4 = {850, 640};        // End
    cpv = {v1, v2, v3, v4};
    tileMap02.AddBezierCurver(3, 100, 4, cpv, 5);
    //tileMap03.AddBezierCurver(3, 100, 4, cpv, 5);


    // Players Inialization
    KeyBinds player_one_binds = { KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE, KEY_F, KEY_E };
    KeyBinds player_two_binds = { KEY_U, KEY_J, KEY_H, KEY_K, KEY_PERIOD, KEY_L, KEY_I };
    std::vector<Player> players;    
    std::vector<Player> players2;
    std::vector<Player> players3;
    Player player1({tileMap.returnPlayerPosition().x, tileMap.returnPlayerPosition().y }, 30.0f, 150.0f, 1, player_one_binds); //640 360
    Player player2({tileMap.returnPlayerPosition().x + 100, tileMap.returnPlayerPosition().y }, 30.0f, 150.0f, 2, player_two_binds); //640 360

    Player player21({tileMap02.returnPlayerPosition().x, tileMap02.returnPlayerPosition().y }, 30.0f, 150.0f, 1, player_one_binds); //640 360
    Player player22({tileMap02.returnPlayerPosition().x + 300, tileMap02.returnPlayerPosition().y }, 30.0f, 150.0f, 2, player_two_binds); //640 360

    
    Player player31({tileMap03.returnPlayerPosition().x, tileMap03.returnPlayerPosition().y }, 30.0f, 150.0f, 1, player_one_binds); //640 360
    Player player32({tileMap03.returnPlayerPosition().x + 100, tileMap03.returnPlayerPosition().y }, 30.0f, 150.0f, 2, player_two_binds); //640 360


    players.push_back(player1);
    players.push_back(player2);

    players2.push_back(player21);
    //players2.push_back(player22);
        
    players3.push_back(player31);
    players3.push_back(player32);
    
    //Enemy Inialization
    //int enemy_count = tileMap.returnEnemyCount();
    std::vector<Enemy> enemies;
    std::vector<Enemy> enemies03;
    std::vector<Vector2> enemy_positions = tileMap03.returnEnemyPositions();

    Enemy enemy01(enemy_positions[0], 100.0f, 15.0f, 100.0f, 250.0f, 50.0f, 2);
    Enemy enemy02(enemy_positions[1], 100.0f, 15.0f, 100.0f, 250.0f, 50.0f, 2);
    //Enemy enemy03(enemy_positions[2], 100.0f, 15.0f, 100.0f, 250.0f, 50.0f, 2);

    enemies03.push_back(enemy01);
    enemies03.push_back(enemy02);

    // Station Initialization
    Station station({605, 305}, {70, 70});
    ScoreSystem score_system01({605, 5}, {70, 70});

    Station station02({245, 325}, {70, 70});
    ScoreSystem score_system02({1125, 5}, {70, 70});

    Station station03({1125, 300}, {70, 70});
    ScoreSystem score_system03({1125, 5}, {70, 70});


    // Spawner Initialization
    std::vector<Spawner> spawners;
    std::vector<Spawner> spawners2;
    std::vector<Spawner> spawners3;

    Spawner spawner01({328.5, 105.5}, {65, 61}, "circular-frame");
    Spawner spawner02({885.5, 175.5}, {65, 61}, "box-frame" );
    Spawner spawner03({328.5, 255.5}, {65, 61}, "arm" );
    Spawner spawner04({885.5, 325.5}, {65, 61}, "wing" );
    Spawner spawner05({328.5, 405.5}, {65, 61}, "gun" );
    Spawner spawner06({885.5, 475.5}, {65, 61}, "bulb" );
    Spawner spawner07({328.5, 555.5}, {65, 61}, "micro" );

    Spawner spawner21({407.5, 174.5}, {65, 61}, "circular-frame");
    Spawner spawner22({647.5, 489.5}, {65, 61}, "box-frame" );
    Spawner spawner23({247.5, 639.5}, {65, 61}, "arm" );
    Spawner spawner24({487.5, 329.5}, {65, 61}, "wing" );
    Spawner spawner25({647.5, 69.5}, {65, 61}, "gun" );
    Spawner spawner26({7.5, 409.5}, {65, 61}, "bulb" );
    Spawner spawner27({167.5, 89.5}, {65, 61}, "micro" );

    spawners.push_back(spawner01);
    spawners.push_back(spawner02);
    spawners.push_back(spawner03);
    spawners.push_back(spawner04);
    spawners.push_back(spawner05);
    spawners.push_back(spawner06);
    spawners.push_back(spawner07);

    spawners2.push_back(spawner21);
    spawners2.push_back(spawner22);
    spawners2.push_back(spawner23);
    spawners2.push_back(spawner24);
    spawners2.push_back(spawner25);
    spawners2.push_back(spawner26);
    spawners2.push_back(spawner27);
    

    camera_view.target = player1.position;
    camera_view.offset = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
    camera_view.zoom = 1.5f;

    Texture tileSet = LoadTexture(tileMap.returnTileSheetName().c_str());

    while (!WindowShouldClose()) {
        float delta_time = GetFrameTime();

        switch (currentScene) {
            case TITLE:
                DrawTitleScreen(currentScene);
                break;
            case LEVELSELECT:
                DrawLevelSelect(currentScene, players, tileMap, tileMap02, tileMap03, station, score_system01, spawners);
                break;
            case LEVEL1:
                UpdateAndDrawLevel1(delta_time, currentScene, game_ongoing, enemy_lose, players, enemies, tileMap, station, score_system01, spawners, tileSet, camera_view, level_timer);
                break;
            
            case LEVEL2:
                UpdateAndDrawLevel1(delta_time, currentScene, game_ongoing, enemy_lose, players2, enemies, tileMap02, station02, score_system02, spawners2, tileSet, camera_view, level_timer);
                break;
            case LEVEL3:
                UpdateAndDrawLevel1(delta_time, currentScene, game_ongoing, enemy_lose, players3, enemies03, tileMap03, station03, score_system03, spawners2, tileSet, camera_view, level_timer);
                break;
            case SCORESCREEN:
                DrawScoreScreen(currentScene);
                break;
            case LEADERBOARD:
                DrawLeaderboard(currentScene);
                break;
        }

        EndDrawing();
    }

    TextureManager::UnloadTextures(); 
    CloseWindow();
    return 0;
}
