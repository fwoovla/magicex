#include "../../core/gamedefs.h"

int *level_tile_array;

TileLayer::TileLayer() {
    rows = 0;
    cols = 0;

}

TileLayer::~TileLayer() {
    delete level_tile_array;

}

void TileLayer::SetTiles(Texture2D &_tilesheet, Vector2 _tile_size) {

    tilesheet = _tilesheet;
    tile_size = _tile_size;

    rows = tilesheet.height / tile_size.y;
    cols = tilesheet.width / tile_size.x;
    
    level_tile_array = new int[g_map_width * g_map_height];

    TraceLog(LOG_INFO, "tile rows: %i   tile cols: %i", rows, cols);
    //get the source x and y for each tile
    
    for (int x = 0; x < cols; x ++) {
        for (int y = 0; y < rows; y++) {
            tile_lookup[y * cols + x] = {(float)x,(float)y};
            //level_tile_array[y * g_map_width + x] = 0;
        }
    }


/*     int index = 0;
    for(int x = 0; x < cols; x++) {
        for(int y = 0; y < rows; y++) {
            level_tile_array[y * cols + x] = index;
            index++;
        }      
    } */
}

void TileLayer::Update() {

}

void TileLayer::Draw() {
    //limit to viewport pls?
    for(int x = 0; x < cols; x++) {
        for(int y = 0; y < rows; y++) {

            DrawTexturePro(
                        tilesheet,
                        {tile_lookup[level_tile_array[y * cols + x]].x, tile_lookup[level_tile_array[y * cols + x]].y, tile_size.x, tile_size.y},
                        {(float)x*tile_size.x, (float)y*tile_size.y ,tile_size.x, tile_size.y},
                        {0,0},
                        0.0,
                        WHITE
                    );
        }
    }

}
