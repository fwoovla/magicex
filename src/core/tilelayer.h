#pragma once
#include "gamedefs.h"


//extern MapData g_current_map_data;
//extern std::vector<std::vector<int>> g_current_ordered_layers;



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

    //MapData map_data;
    //std::vector<std::vector<int>> ordered_layers;
};


inline bool CheckCollisionWithLevel(BaseEntity *checker, CollisionResult &collision_result, int _range) {
    //================TILE COLLISION=========================
    //TraceLog(LOG_INFO, "CHECKER POS: %0.0f    %0.0f", checker->position.x * g_game_map_data.inv_tile_size, checker->position.y * g_game_map_data.inv_tile_size);
    //for (int l = 0; l < g_current_map_data.layers.size(); l++) {
/*         for(int y = -_range; y <=  _range; y++) {
            for(int x = -_range; x <= _range; x++) {

                int ix = (checker->position.x * g_current_map_data.inv_tile_size) + x;
                int iy = (checker->position.y * g_current_map_data.inv_tile_size) + y;
                int tile_index = iy * g_current_map_data.map_width + ix;
                //g_map_map[(Vector2){ix,iy}];
                int id = g_current_ordered_layers[g_current_map_data.collision_layer_index][ tile_index ];

                //TraceLog(LOG_INFO, "CHECKING: (%i, %i) index: %i  id: %i", ix, iy, tile_index, id);

                if(id != -1  ) {
                    int ts = g_current_map_data.tile_size;
                    if(CheckCollisionCircleRec(Vector2Add(checker->position, checker->centered_offset), checker->collision_radius, {(float)ix * ts, (float)iy * ts, (float)ts, (float)ts})) {
                        //TraceLog(LOG_INFO, "COLLIDED  %i     %i %i", id, ix, iy);
                        return true;
                    }
                }
            }
        //}
        } */
    //TraceLog(LOG_INFO, "\n");

    return false;
}