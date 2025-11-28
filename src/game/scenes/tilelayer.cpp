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

}


bool CollideAndSlide(BaseEntity *checker, CollisionResult &collision_result, int _range) {
    //================TILE COLLISION=========================
    bool collided = false;

    LDTKLevel &this_level = g_ldtk_maps.levels[g_current_scene->level_data.precalc.map_index];

    if(g_current_scene->level_data.precalc.collision_layer_index == -1) {
        return false;
    }
    
    LDTKLayerInstance &col_layer = this_level.layer_instances[g_current_scene->level_data.precalc.collision_layer_index];

    int tile_size = g_current_scene->level_data.precalc.tile_size;
    float inv_tile_size = g_current_scene->level_data.precalc.inv_tile_size;
    int map_width = g_current_scene->level_data.precalc.map_width;

    Vector2 checker_pos = Vector2Add(checker->position, checker->centered_offset);

    int cell_pos_x_i = (checker->position.x + checker->centered_offset.x) * inv_tile_size;
    int cell_pos_y_i = (checker->position.y + checker->centered_offset.y) * inv_tile_size;


    float c_rad = checker->collision_radius;
    
    int left_i = (checker_pos.x - c_rad) * inv_tile_size;
    int right_i = (checker_pos.x + c_rad) * inv_tile_size;
    int top_i = (checker_pos.y - c_rad) * inv_tile_size;
    int bottom_i = (checker_pos.y + c_rad) * inv_tile_size;

    //TraceLog(LOG_INFO, "checker pos %i, %i", c_pos_x_i, c_pos_y_i);

    //check x direction
    //left
    int index = cell_pos_y_i * map_width + left_i;
    int value = col_layer.int_grid[index];
    //TraceLog(LOG_INFO, "checking l %i, %i  | index %i   | value %i", left_i, c_pos_y_i , index, value);
    if(value == 1) {
        if(CheckCollisionCircleRec( checker_pos, c_rad, { (float)left_i * tile_size, (float)cell_pos_y_i * tile_size, (float)tile_size, (float)tile_size } )) {
            //TraceLog(LOG_INFO, "COLLISON LEFT");
            collision_result.collision_dir.x = -1;
            collided =  true;
        }
    }
    //right
    index = cell_pos_y_i * map_width + right_i;
    value = col_layer.int_grid[index];
    //TraceLog(LOG_INFO, "checking r %i, %i  | index %i   | value %i", right_i, c_pos_y_i , index, value);
    if(value == 1) {
        if(CheckCollisionCircleRec( checker_pos, c_rad, { (float)right_i * tile_size, (float)cell_pos_y_i * tile_size, (float)tile_size, (float)tile_size } )) {
            //TraceLog(LOG_INFO, "COLLISON RIGHT");
            collision_result.collision_dir.x = 1;
            collided =  true;
        }
    }
    //check y direction
    //top
    index = top_i * map_width + cell_pos_x_i;
    value = col_layer.int_grid[index];
    //TraceLog(LOG_INFO, "checking t %i, %i  | index %i   | value %i", c_pos_x_i, top_i , index, value);
    if(value == 1) {
        if(CheckCollisionCircleRec( checker_pos, c_rad, { (float)cell_pos_x_i * tile_size, (float)top_i * tile_size, (float)tile_size, (float)tile_size } )) {
            //TraceLog(LOG_INFO, "COLLISON TOP");
            collision_result.collision_dir.y = -1;
            collided =  true;
        }
    }
    //bottom
    index = bottom_i * map_width + cell_pos_x_i;
    value = col_layer.int_grid[index];

    //TraceLog(LOG_INFO, "checking b %i, %i  | index %i   | value %i", c_pos_x_i, bottom_i, index, value);
    if(value == 1) {
        if(CheckCollisionCircleRec( checker_pos, c_rad, { (float)cell_pos_x_i * tile_size, (float)bottom_i * tile_size, (float)tile_size, (float)tile_size } )) {
            //TraceLog(LOG_INFO, "COLLISON BOTTOM");
            collision_result.collision_dir.y = 1;
            collided =  true;
        }
    }
    //TraceLog(LOG_INFO, "\n");
    return collided;
}



bool CollideWithTile(BaseEntity *checker, CollisionResult &collision_result) {
    
    bool collided = false;

    LDTKLevel &this_level = g_ldtk_maps.levels[g_current_scene->level_data.precalc.map_index];

    if(g_current_scene->level_data.precalc.collision_layer_index == -1) {
        return false;
    }
    LDTKLayerInstance &col_layer = this_level.layer_instances[g_current_scene->level_data.precalc.collision_layer_index];



    int tile_size = g_current_scene->level_data.precalc.tile_size;
    float inv_tile_size = g_current_scene->level_data.precalc.inv_tile_size;
    int map_width = g_current_scene->level_data.precalc.map_width;

    Vector2 checker_pos = Vector2Add(checker->position, checker->centered_offset);

    int cell_pos_x_i = (checker->position.x + checker->centered_offset.x) * inv_tile_size;
    int cell_pos_y_i = (checker->position.y + checker->centered_offset.y) * inv_tile_size;

    int cell = (checker_pos.x) * inv_tile_size;
    int index = cell_pos_y_i * map_width + cell;
    int value = col_layer.int_grid[index];

    if(value == 1) {
        //if(CheckCollisionCircleRec( {1,1}, 0.1, {10,10,1,1})) {
        TraceLog(LOG_INFO, "radius %f", checker->collision_radius);
        if(CheckCollisionCircleRec( checker_pos, checker->collision_radius, { (float)cell_pos_x_i * tile_size, (float)cell_pos_y_i * tile_size, (float)tile_size, (float)tile_size } )) {
            collided =  true;
        }
    }

    return collided;
}