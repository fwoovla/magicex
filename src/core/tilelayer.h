#pragma once
#include "gamedefs.h"


extern MapData g_game_map_data;
//extern std::vector<int> g_ordered_map_data;
extern std::vector<std::vector<int>> g_ordered_map_layers;



class TileLayer {

    public:
    TileLayer();
    void SetTiles(Texture2D &_tilesheet, int _map_index);
    ~TileLayer();
    void Update();
    void Draw();

    Texture2D tilesheet;
    std::vector<Vector2> tile_atlas;
    int map_index;
};


inline bool CheckCollisionWithLevel(BaseEntity *checker, CollisionResult &collision_result, int _range) {
    //================TILE COLLISION=========================
    //TraceLog(LOG_INFO, "CHECKER POS: %0.0f    %0.0f", checker->position.x * g_game_map_data.inv_tile_size, checker->position.y * g_game_map_data.inv_tile_size);
    for (int l = 0; l < g_game_map_data.layers.size(); l++) {
        for(int y = -_range; y <=  _range; y++) {
            for(int x = -_range; x <= _range; x++) {

                int ix = (checker->position.x * g_game_map_data.inv_tile_size) + x;
                int iy = (checker->position.y * g_game_map_data.inv_tile_size) + y;
                int tile_index = iy * g_game_map_data.map_width + ix;
                //g_map_map[(Vector2){ix,iy}];
                int id = g_ordered_map_layers[l][ tile_index ];

                //TraceLog(LOG_INFO, "CHECKING: (%i, %i) index: %i  id: %i", ix, iy, tile_index, id);

                if(id == 31  ) {
                    
                    if(CheckCollisionCircleRec(checker->position, checker->collision_radius, {(float)ix * g_game_map_data.tile_size, (float)iy * g_game_map_data.tile_size, (float)g_game_map_data.tile_size, (float)g_game_map_data.tile_size})) {
                        //TraceLog(LOG_INFO, "COLLIDED  %i     %i %i", id, ix, iy);
                        return true;
                    }
                }
            }
        }
    }
    //TraceLog(LOG_INFO, "\n");

    return false;
}