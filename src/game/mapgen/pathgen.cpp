#include "../../core/pathgen.h"


void GenerateMapPaths(LDTKLayerInstance &_layer, WorldGenTileSet &_tileset, Vector2 _map_size) {
    std::vector<int> temp_grid;
    temp_grid.resize(_map_size.x*_map_size.y, TILE_ID_NONE);

    for(int len = 0; len < 50; len++) {

        for(auto &worm : _tileset.path_worms) {
            TraceLog(LOG_INFO, "worm pos %0.0f %0.0f", worm.position.x, worm.position.y);
            int index = worm.position.y * _map_size.x + worm.position.x;
            temp_grid[index] = 1;
            _tileset.zone_grid[index] = ZONE_PATH;

            if(GetRandomValue(0, 100) > 50 and (worm.position.x > 1 and worm.position.x < _map_size.x-1)) { //do x
                if(worm.locked_dir.x != 0) { //base locked
                    worm.position.x += worm.locked_dir.x * -1;
                    worm.last_locked_dir = {worm.locked_dir.x,0};
                    //TraceLog(LOG_INFO, "move x %0.0f", worm.locked_dir.x*-1);
                }
                else { //not base locked
                    if(GetRandomValue(0, 100) > 50) {

                        if(worm.last_locked_dir.x < 0.0f) {
                            worm.position.x += 1;
                            worm.last_locked_dir = {-1,0};
                            //TraceLog(LOG_INFO, "right");
                        }
                        else {
                            worm.position.x += -1;
                            worm.last_locked_dir = {1,0};
                            //TraceLog(LOG_INFO, "left");
                        }
                    }   
                    else {
                        if(worm.last_locked_dir.x > 0.0f) {
                            worm.position.x += -1;
                            worm.last_locked_dir = {1,0};
                            //TraceLog(LOG_INFO, "left");
                        }
                        else {
                            worm.position.x += 1;
                            worm.last_locked_dir = {-1,0};
                            //TraceLog(LOG_INFO, "right"); 
                        }
                    }    
                }
            }

            else if(worm.position.y > 1 and worm.position.y < _map_size.y-1) { //do y
                if(worm.locked_dir.y != 0) {
                    worm.position.y += worm.locked_dir.y * -1;
                    worm.last_locked_dir = {0,worm.locked_dir.y};
                    //TraceLog(LOG_INFO, "move y %0.0f", worm.locked_dir.y*-1);
                }
                else {
                    if(GetRandomValue(0, 100) > 50) {
                        if(worm.last_locked_dir.y < 0.0f) {
                            worm.position.y += 1;
                            worm.last_locked_dir = {0,-1};
                            //TraceLog(LOG_INFO, "down");
                        }
                        else {
                            worm.position.y += -1;
                            worm.last_locked_dir = {0,1};
                            //TraceLog(LOG_INFO, "up");
                        }
                    }  
                    else {
                        if(worm.last_locked_dir.y > 0.0f) {
                            worm.position.y += -1;
                            worm.last_locked_dir = {0,1};
                            //TraceLog(LOG_INFO, "up");
                        }
                        else {
                            worm.position.y += 1;
                            worm.last_locked_dir = {0,-1};
                            //TraceLog(LOG_INFO, "down");
                        }
                    }   
                }
            }         
        }
    }

//resolve autotile
    for(int tile = 0; tile < temp_grid.size(); tile++) {
        int x = tile%(int)_map_size.x;
        int y = tile/(int)_map_size.x;

        if(x < 2 or y < 2 or x > _map_size.x-2 or y > _map_size.y-2) {
            continue;
        }

        
        if(temp_grid[tile] != -1) {
            
            LDTKGridTile new_tile_ldtk;
            
            new_tile_ldtk.px.push_back(x*_tileset.tile_grid_size);
            new_tile_ldtk.px.push_back(y*_tileset.tile_grid_size);
            
            int index = y * _map_size.x + x;
            int index_right = y * _map_size.x + (x+1);
            int index_left = y * _map_size.x + (x-1);
            int index_up = (y-1) * _map_size.x + x;
            int index_down = (y+1) * _map_size.x + x;
            
            bool this_tile_marked_sides[4] = {false, false, false, false};
            
            if (temp_grid[index_up] != TILE_ID_NONE) {
                this_tile_marked_sides[TILESIDE_UP] = true;
            }
            if (temp_grid[index_right] != TILE_ID_NONE) {
                this_tile_marked_sides[TILESIDE_RIGHT] = true;
            }
            if (temp_grid[index_down] != TILE_ID_NONE) {
                this_tile_marked_sides[TILESIDE_DOWN] = true;
            }
            if (temp_grid[index_left] != TILE_ID_NONE) {
                this_tile_marked_sides[TILESIDE_LEFT] = true;
            }

            TILEID tile_id = TILE_ID_PATH_SINGLE;

            for(auto &tile : _tileset.tile_lookup) {
                if(tile.first > TILE_ID_PATH_START and tile.first < TILE_ID_PATH_END) {
                    //TraceLog(LOG_INFO, "PATH TILE ? %i ", tile.first);

                    bool up_ok = false;
                    bool right_ok = false;
                    bool down_ok = false;
                    bool left_ok = false;
    
                    if(tile.second.marked_sides[TILESIDE_UP] == true and this_tile_marked_sides[TILESIDE_UP] == true) {
                        up_ok = true;
                    }
                    if(tile.second.marked_sides[TILESIDE_UP] == false and this_tile_marked_sides[TILESIDE_UP] == false) {
                        up_ok = true;
                    }

                    if(tile.second.marked_sides[TILESIDE_RIGHT] == true and this_tile_marked_sides[TILESIDE_RIGHT] == true) {
                        right_ok = true;
                    }
                    if(tile.second.marked_sides[TILESIDE_RIGHT] == false and this_tile_marked_sides[TILESIDE_RIGHT] == false) {
                        right_ok = true;
                    }


                    if(tile.second.marked_sides[TILESIDE_DOWN] == true and this_tile_marked_sides[TILESIDE_DOWN] == true) {
                        down_ok = true;
                    }
                    if(tile.second.marked_sides[TILESIDE_DOWN] == false and this_tile_marked_sides[TILESIDE_DOWN] == false) {
                        down_ok = true;
                    }


                    if(tile.second.marked_sides[TILESIDE_LEFT] == true and this_tile_marked_sides[TILESIDE_LEFT] == true) {
                        left_ok = true;
                    }
                    if(tile.second.marked_sides[TILESIDE_LEFT] == false and this_tile_marked_sides[TILESIDE_LEFT] == false) {
                        left_ok = true;
                    }


                    if(up_ok and right_ok and down_ok and left_ok) {
                        tile_id = tile.first;
                        //TraceLog(LOG_INFO, "PATH TILE found %i ", tile.first);
                    }
                }
            }
            
            
            new_tile_ldtk.src.push_back( _tileset.tile_lookup[tile_id].atlas_position.x);
            new_tile_ldtk.src.push_back(_tileset.tile_lookup[tile_id].atlas_position.y);
            new_tile_ldtk.t = tile_id;
                        
            //TraceLog(LOG_INFO, "PATH TILE  tile id  %i   pos %i %i   index %i", new_tile_ldtk.t, new_tile_ldtk.px[0], new_tile_ldtk.px[1], index);
            _layer.grid_tiles.push_back(new_tile_ldtk);

            _tileset.zone_grid[index] = ZONE_PATH;
            _tileset.zone_grid[index_up] = ZONE_PATH;
            _tileset.zone_grid[index_right] = ZONE_PATH;
            _tileset.zone_grid[index_down] = ZONE_PATH;
            _tileset.zone_grid[index_left] = ZONE_PATH;
        }

    }
}
