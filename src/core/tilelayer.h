#pragma once
#include "gamedefs.h"

class TileLayer {

    public:
    TileLayer();
    void SetTiles(Texture2D &_tilesheet, int _map_index);
    ~TileLayer();
    void Update();
    void Draw();

    Texture2D tile_texture;
    std::vector<Vector2> tile_sheet;
    int map_index;

};

bool CheckCollisionWithLevel(BaseEntity *checker, CollisionResult &collision_result, int _range);

