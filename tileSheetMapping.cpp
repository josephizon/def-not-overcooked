#include <raylib.h>
#include <raymath.h>
#include <ostream>
#include <iostream>
#include <fstream>
#include <cstring>

#include "TileSheet.hpp"


tileSheet::tile::tile(Vector2 pos, float w, float h, bool c){
    position = pos;
    width = w;
    height = h;
    hasCollision = c;
}

tileSheet::tileSheet(int ts, std::string fn){
    tileSize = ts;
    fileName = fn;
    hasBezier = false;
}

void tileSheet::createTile(std::vector<tile>& tv, Vector2 pos, float w, float h, bool c){
    tile tileSprite = {pos, w, h, c};
    tv.push_back(tileSprite);
    //std::cout << "Tile Pos " << pos.x << ", " << pos.y << "| Size " << w << ", " << h << " | Collision " << (c ? "true" : "false") << std::endl;

}

void tileSheet::setupTileMap() {
    std::ifstream file(fileName); 

    std::string input;
    std::string xpos, ypos, width, height;
    std::string xpos_player, ypos_player;
    std::string enem_count, xpos_enemy, ypos_enemy; 

    while (true){
        file >> input;

        // IMAGE_NAME tilesheetile.png
        if (input == "IMAGE_NAME"){
            // Filename of the Tile map that will be used
            file >> tileMap;
        }

        // TILE_COUNT N
        else if (input == "TILE_COUNT"){
            file >> input;        
            int tileCount = stoi(input);

            // Handles the x, y, width and height of each Tile
            for (int i = 0; i < tileCount; i++){
                // Input from Files
                file >> xpos >> ypos >> width >> height >> input;
                Vector2 pos = {stof(xpos), stof(ypos)};

                // Convert boolean input into all lower case
                for (int x=0; x < input.size(); x++){
                    toupper(input[x]);
                }

                //std::cout << "Tile collision input: " << input << std::endl;

                // hasCollision will be true or false depending n the answer
                // False by default for now
                if (input == "true"){
                    createTile(tileVector, pos, stoi(width), stoi(height), true);
                }
                else if (input == "false"){
                    createTile(tileVector, pos, stoi(width), stoi(height), false);
                }
            }
        }

        // GRID X Y
        else if (input =="GRID"){
            file >> width >> height;
            
            xTileMapSize = stoi(width);
            yTileMapSize = stoi(height);
            // Creates the 2D Array
            grid = new int*[xTileMapSize];
            for (int i = 0; i < xTileMapSize; i++) {
                grid[i] = new int[yTileMapSize];
            }

            // Fills in the 2D Array
            for (int h = 0; h < yTileMapSize; h++) {
                for (int w = 0; w < xTileMapSize; w++) {
                    file >> input;
                    grid[w][h] = stoi(input);
                }
            }
        }

        // PLAYER_POSITION X Y
        else if (input =="PLAYER_POSITION"){
            file >> xpos_player >> ypos_player;
            player_position = {stof(xpos_player), stof(ypos_player)};
        }

        // ENEMY_COUNT enemy_count
        // else if (input =="ENEMY_COUNT"){
            
        //     file >> enem_count;
        //     enemy_count = stoi(enem_count);
        // }

        // ENEMY_POSITIONS 
        // X Y
        else if (input =="ENEMY_POSITIONS"){
            enemy_count = 3;
            enemy_position.clear();
            for (int i = 0; i < enemy_count; i++) {
                file >> xpos_enemy >> ypos_enemy;
                enemy_position.push_back({stof(xpos_enemy), stof(ypos_enemy)});
            }
        }
        else{
            break;
        }
    }
}

void tileSheet::draw(Texture tileSet){

    for (int y = 0; y < yTileMapSize; y++) {
        for (int x = 0; x < xTileMapSize; x++) {
            
            const float epsilon = 0.01f;
            Rectangle sprite = { tileVector[grid[x][y]].position.x + epsilon, 
                     tileVector[grid[x][y]].position.y + epsilon,
                     tileVector[grid[x][y]].width - 2 * epsilon, 
                     tileVector[grid[x][y]].height - 2 * epsilon };
            // Rectangle sprite = { tileVector[grid[x][y]].position.x , tileVector[grid[x][y]].position.y , 
            //                      tileVector[grid[x][y]].width , tileVector[grid[x][y]].height };
            Rectangle location = { (x * tileVector[grid[x][y]].width * tileSize) , (y * tileVector[grid[x][y]].height * tileSize), 
                tileVector[grid[x][y]].width * tileSize, tileVector[grid[x][y]].height * tileSize};

            
            DrawTexturePro(tileSet, sprite, location, { 0, 0 }, 0.0f, WHITE);
        }
    }

    if (hasBezier){
        for (auto& bv : bezierVector){
            // Draws the Start and End point
            for (size_t i = 0; i < bv.controlPointsList.size(); i++) {
                Color pointColor;
    
                if (i == 0) {
                    pointColor = GREEN;  
                }
                else if (i == bv.controlPointsList.size() - 1) {
                    pointColor = RED;  
                }
                else {
                    pointColor = {0, 0, 0, 0}; 
                } 
        
                DrawCircleV(bv.controlPointsList[i], bv.radius, pointColor);
            }
        
            // Draws the Bezier Curve
            for (size_t i = 0; i < bv.curvePointsList.size() - 1; i++) {
                //std::cout << bezier.curvePointsList[i].x << ", " << bezier.curvePointsList[i].y << std::endl;
                DrawLineV(bv.curvePointsList[i], bv.curvePointsList[i + 1], BLACK);
            }
        }
    }
}

void tileSheet::HandleCollision(Entity* other_entity) {
    // grid mapping similar to my exercise real
    // iterate through whole tile mapping

    // There should be conveyer belt in the level, and player should not currently be in one
    if (hasBezier && other_entity->entity_type == "Player" && !other_entity->isInConveyerBelt){
        bezierIndex = 0; 
        for (auto& bv : bezierVector){
            bool playerConveyerCollision = CheckCollisionCircles(bv.controlPointsList[0], bv.radius, other_entity->position, other_entity->radius);
            if(playerConveyerCollision){
                other_entity->isInConveyerBelt = true; 
                break;
            }
            else{
                bezierIndex++;
            }         
        }
    }

    if(hasBezier && other_entity->isInConveyerBelt && other_entity->entity_type == "Player"){
        if (other_entity->bezierCurveIndex < bezierVector[bezierIndex].curvePointsList.size() - 1 ) {
            Vector2 direction = Vector2Subtract(bezierVector[bezierIndex].curvePointsList[other_entity->bezierCurveIndex + 1], other_entity->position);
            other_entity->position = Vector2Add(other_entity->position, Vector2Scale(Vector2Normalize(direction), 2.0f));

            // Keeps the rectangle travelling on the bezier curve itself
            if (Vector2Length(direction) < 2.0f) {
                other_entity->bezierCurveIndex += 1;
            }  
        }
        else{       
            other_entity->isInConveyerBelt = false;
            other_entity->bezierCurveIndex = 0;
            bezierIndex = 0;
        }
    }
    
    if (!other_entity->isInConveyerBelt){
        for (int y = 0; y < yTileMapSize; y++) {
            for (int x = 0; x < xTileMapSize; x++) {
                tile& tile = tileVector[grid[x][y]];  
    
                // look for tiles in the grid with true (check actual grid not just one tile)
                if (tile.hasCollision) {
                    float actual_tile_x = x * tile.width * tileSize;
                    float actual_tile_y = y * tile.height * tileSize;
    
                    Rectangle actual_tile = { actual_tile_x, actual_tile_y, tile.width * tileSize, tile.height * tileSize };
    
                    if (CheckCollisionCircleRec(other_entity->position, other_entity->radius, actual_tile)) {
                        //std::cout << "Collision BENG" << " | Tile Pos: " << actual_tile_x << ", " << actual_tile_y << " | Tile Number in Sheet " << grid[x][y] << std::endl;
    
                        // GET COLLISION NORMAL OF PLAYER AND WALLS 
                        // closest point on the rectangle to the circle center
                        float closestX = Clamp(other_entity->position.x, actual_tile.x, actual_tile.x + actual_tile.width);
                        float closestY = Clamp(other_entity->position.y, actual_tile.y, actual_tile.y + actual_tile.height);
    
                        // collision normal 
                        Vector2 collision_normal = Vector2Subtract(other_entity->position, { closestX, closestY });
                        float distance = Vector2Length(collision_normal);
    
                        //  push the player back in the direction of the collision normal. 
                        if (distance == 0.0f) {
                            collision_normal = { 0.0f, -1.0f };  
                        } 
                        
                        else {
                            collision_normal = Vector2Normalize(collision_normal);
                        }
                        
    
                        collision_normal = Vector2Normalize(collision_normal);
                        float overlap = other_entity->radius - distance;
                        Vector2 push_vector = Vector2Scale(collision_normal, overlap);
    
                        other_entity->position = Vector2Add(other_entity->position, push_vector);
    
    
                    }
                }
            }
        }
    }
}

std::string tileSheet::returnTileSheetName(){
    return tileMap;
}

Vector2 tileSheet::returnPlayerPosition(){
    return player_position; 
}

// int tileSheet::returnEnemyCount(){
//     return enemy_count; 
// }

std::vector<Vector2> tileSheet::returnEnemyPositions(){
    return enemy_position; 
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ bezier Curve ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
tileSheet::BezierCurve::BezierCurve(int co, int s, int cp, int r){
    curveOrder = co;
    steps = s;
    controlPoints = cp;
    radius = r;
}

void tileSheet::BezierCurve::createPascalsTriangle(int R) {
    pascalTriangle.assign(R + 1, std::vector<int>(R + 1, 1));

    // every line loop
    for (int line = 0; line <= R; line++) {
        // number of integers are equal to the line number
        for (int i = 0; i <= line; i++) {
            // assign 1 to the start and end of line
            if (line == i || i == 0){
                pascalTriangle[line][i] = 1;
            }  
            // sum of above and left of above
            else {
                pascalTriangle[line][i] = pascalTriangle[line - 1][i - 1] + pascalTriangle[line - 1][i];
            }
            // reminder to self if i wanna see pascal triangle agen
        }
    }

}

Vector2 tileSheet::BezierCurve::computeBezierCurvePoints(float t) {
    Vector2 result = {0, 0};
    
    int R = curveOrder;
    for (int i = 0; i <= R; i++) {

        // multiply binomCo by s(1-t)^R-ith power times t^ith power 
        float binomialCoefficient = pascalTriangle[R][i];
        float term = binomialCoefficient * powf(1 - t, R - i) * powf(t, i);
        

        // P is basically an array with n+1 elements. This array is the control points, so each P is a different control point.
        // so basically P is an array with Control Points (N) elements.  
        result = Vector2Add(result, Vector2Scale(controlPointsList[i], term));
    }
    return result;
}

void tileSheet::BezierCurve::calculateCurve() {
    curvePointsList.clear();
    for (size_t i = 0; i <= steps; i++) {
        float t = (float)i / steps;
        curvePointsList.push_back(computeBezierCurvePoints(t)); 
    }
}

void tileSheet::AddBezierCurver(int co, int s, int cp, std::vector<Vector2>& cpv, int r){
    BezierCurve newBezier = {co, s, cp, r};
    
    for (int i = 0; i < cpv.size(); i++){
        newBezier.controlPointsList.push_back(cpv[i]);
    }

    newBezier.controlPointsList.resize(newBezier.controlPoints);

    // create triangle only once and call calculate only once at the start and whenever control points are adjusted
    newBezier.createPascalsTriangle(newBezier.curveOrder);
    newBezier.calculateCurve();

    bezierVector.push_back(newBezier);
    hasBezier = true;
    bezierIndex = 0;
}


// cd Documents/Acads/GDEV_42/
// g++ Main.cpp -o out -I raylib/ -L raylib/ -lraylib -lopengl32 -lgdi32 -lwinmm
// ./out