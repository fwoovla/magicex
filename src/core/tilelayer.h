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


inline bool CheckCollisionWithLevel(BaseEntity *checker, CollisionResult &collision_result, int _range) {
    //================TILE COLLISION=========================

    LDTKLevel this_level = g_ldtk_maps.levels[g_game_data.current_map_index];
    LDTKLayerInstance *col_layer = nullptr;

    for (int l = 0; l < this_level.layer_instances.size(); l++) {
        if(this_level.layer_instances[l].type == "IntGrid") {
            col_layer = &this_level.layer_instances[l];
        }
    }

    if(col_layer == nullptr) {
        TraceLog(LOG_INFO, "no collision layer");
        return false;
    }

    int tile_size = col_layer->grid_size;
    float inv_tile_size = 1/(float)tile_size;
    int map_width = col_layer->c_wid;

    for(int y = -_range; y <=  _range; y++) {
        for(int x = -_range; x <= _range; x++) {

            int ix = (checker->position.x * inv_tile_size) + x;
            int iy = (checker->position.y * inv_tile_size) + y;
            int tile_index = iy * map_width + ix;
                //g_map_map[(Vector2){ix,iy}];
            int value = col_layer->int_grid[tile_index];

            //TraceLog(LOG_INFO, "CHECKING:ts:%i ivt:%0.2f (%0.2f, %0.2f)  (%i, %i) index: %i  id: %i", tile_size, inv_tile_size, checker->position.x, checker->position.y , ix, iy, tile_index, value);

            if(value == 1  ) {
                if(CheckCollisionCircleRec(Vector2Add(  checker->position,
                                                        checker->centered_offset),
                                                        checker->collision_radius,
                                                        {(float)ix * tile_size, (float)iy * tile_size, (float)tile_size,(float)tile_size}
                                                    )) {
                    //TraceLog(LOG_INFO, "COLLIDED  %i     %i %i", id, ix, iy);
                    return true;
                }
            }
        }
    } 
    TraceLog(LOG_INFO, "\n");

    return false;
}