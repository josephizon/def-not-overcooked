#ifndef TileSheet
#define TileSheet

#include <raylib.h>
#include <raymath.h>
#include <fstream>
#include <vector>

#include "Entity.hpp"

class tileSheet;

class tileSheet{
public:
    struct tile: public Entity{
        float width = 0;
        float height = 0;
        bool hasCollision = false;

        tile(Vector2 pos, float w, float h, bool c);
    };

    struct BezierCurve {
        int curveOrder;
        int steps;
        int controlPoints;
        int radius;
        std::vector<std::vector<int>> pascalTriangle;
        std::vector<Vector2> controlPointsList;
        std::vector<Vector2> curvePointsList;
        int selectedControlPoint = -1;
    
        BezierCurve(int co, int s, int cp, int r);
        void createPascalsTriangle(int R);
        Vector2 computeBezierCurvePoints(float t);
    
        void calculateCurve();
    };

    // SETTINGS FILE STUFF
    int tileSize;
    //BezierCurve bezier;
    std::vector<BezierCurve> bezierVector;
    int bezierIndex; 
    bool hasBezier;
    std::string fileName, tileMap;
    std::vector<tile> tileVector;
    int xTileMapSize, yTileMapSize;
    int** grid = nullptr;

    tileSheet(int tileSize, std::string fileName);
    void createTile(std::vector<tile>& tv, Vector2 pos, float w, float h, bool c);
    void setupTileMap();
    void draw(Texture2D tileSet);
    std::string returnTileSheetName();

    // PLAYER AND ENEMY POSITION
    Vector2 player_position; 
    Vector2 returnPlayerPosition();

    int enemy_count; 
    //int returnEnemyCount();
    std::vector<Vector2> enemy_position; 
    std::vector<Vector2> returnEnemyPositions();

    void HandleCollision(Entity* other_entity);
    void AddBezierCurver(int co, int s, int cp, std::vector<Vector2>& cpv, int r);
};

#endif