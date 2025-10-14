#pragma once
#include "gamedefs.h"

/* struct Tile {
    Vector2 source_offset;
    int tile_id;
}; */

class TileLayer {

    public:
    TileLayer();
    void SetTiles(Texture2D &_tilesheet, Vector2 _tile_size);
    ~TileLayer();
    void Update();
    void Draw();

    Texture2D tilesheet;

    Vector2 tile_size;
    int rows;
    int cols;

    std::vector<Vector2> tile_lookup;

    int *level_tile_array;




};