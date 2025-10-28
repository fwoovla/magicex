#include "../../core/gamedefs.h"


TileLayer::TileLayer() {

}

TileLayer::~TileLayer() {

}


void TileLayer::Update() {

}

void TileLayer::Draw() {

    double time = GetTime();

    g_debug_data.tiles_drawn =  LDTKDrawMap(g_current_player->position);

    g_debug_data.tile_dt = GetTime() - time;

    //TraceLog(LOG_INFO, "dt tile draw %f", g_debug_data.tile_dt);
    //TraceLog(LOG_INFO, "tiles drawn %i", g_debug_data.tiles_drawn);

}


bool CheckCollisionWithLevel(BaseEntity *checker, CollisionResult &collision_result, int _range) {
    //================TILE COLLISION=========================
    bool collided = false;

    int map_index = g_game_data.current_map_index;
    if(g_game_data.is_in_sub_map) {
        map_index = g_game_data.sub_map_index;
    }

    LDTKLevel this_level = g_ldtk_maps.levels[map_index];
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

    Vector2 c_pos = Vector2Add(checker->position, checker->centered_offset);
    Vector2 c_pos_i = Vector2Add(checker->position, checker->centered_offset) * inv_tile_size;


    int c_pos_x_i = (checker->position.x + checker->centered_offset.x) * inv_tile_size;
    int c_pos_y_i = (checker->position.y + checker->centered_offset.y) * inv_tile_size;


    float c_rad = checker->collision_radius;
    
    int left_i = (c_pos.x - c_rad) * inv_tile_size;
    int right_i = (c_pos.x + c_rad) * inv_tile_size;
    int top_i = (c_pos.y - c_rad) * inv_tile_size;
    int bottom_i = (c_pos.y + c_rad) * inv_tile_size;

    //TraceLog(LOG_INFO, "checker pos %i, %i", c_pos_x_i, c_pos_y_i);

    //check x direction
    //left
    int index = c_pos_y_i * map_width + left_i;
    int value = col_layer->int_grid[index];
    //TraceLog(LOG_INFO, "checking l %i, %i  | index %i   | value %i", left_i, c_pos_y_i , index, value);
    
    if(value == 1) {
        if(CheckCollisionCircleRec( c_pos, c_rad, { (float)left_i * tile_size, (float)c_pos_y_i * tile_size, (float)tile_size, (float)tile_size } )) {
            //TraceLog(LOG_INFO, "COLLISON LEFT");
            collision_result.collision_dir.x = -1;
            collided =  true;
        }
    }


    //right
    index = c_pos_y_i * map_width + right_i;
    value = col_layer->int_grid[index];

    //TraceLog(LOG_INFO, "checking r %i, %i  | index %i   | value %i", right_i, c_pos_y_i , index, value);

    if(value == 1) {
        if(CheckCollisionCircleRec( c_pos, c_rad, { (float)right_i * tile_size, (float)c_pos_y_i * tile_size, (float)tile_size, (float)tile_size } )) {
            //TraceLog(LOG_INFO, "COLLISON RIGHT");
            collision_result.collision_dir.x = 1;
            collided =  true;
        }
    }

    //check y direction

    //top
    index = top_i * map_width + c_pos_x_i;
    value = col_layer->int_grid[index];

    //TraceLog(LOG_INFO, "checking t %i, %i  | index %i   | value %i", c_pos_x_i, top_i , index, value);

    if(value == 1) {
        if(CheckCollisionCircleRec( c_pos, c_rad, { (float)c_pos_x_i * tile_size, (float)top_i * tile_size, (float)tile_size, (float)tile_size } )) {
            //TraceLog(LOG_INFO, "COLLISON TOP");
            collision_result.collision_dir.y = -1;
            collided =  true;
        }
    }

    //bottom
    index = bottom_i * map_width + c_pos_x_i;
    value = col_layer->int_grid[index];
    
    //TraceLog(LOG_INFO, "checking b %i, %i  | index %i   | value %i", c_pos_x_i, bottom_i, index, value);

    if(value == 1) {
        if(CheckCollisionCircleRec( c_pos, c_rad, { (float)c_pos_x_i * tile_size, (float)bottom_i * tile_size, (float)tile_size, (float)tile_size } )) {
            //TraceLog(LOG_INFO, "COLLISON BOTTOM");
            collision_result.collision_dir.y = 1;
            collided =  true;
        }
    }

    //TraceLog(LOG_INFO, "\n");
    return collided;
}
