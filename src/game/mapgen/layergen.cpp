#include "../../core/layergen.h"

void GenerateLowerTerrainLayer(LDTKLevel &_level, WorldGenTileSet &_tileset, Vector2 _map_size) {
    LDTKLayerInstance new_layer;

    new_layer.identifier = "TerrainLower";
    new_layer.type = "Tiles";
    new_layer.tileset_def_uid = _tileset.uid;
    new_layer.grid_size = _tileset.tile_grid_size;
    new_layer.c_wid = _map_size.x;
    new_layer.c_hei = _map_size.y;
    
    for(int y = 0; y < _map_size.y; y++) {
        for(int x = 0; x < _map_size.x; x++) {
            int index = y * _map_size.x + x;
            LDTKGridTile new_tile_ldtk;
            
            new_tile_ldtk.px.push_back((float)x*_tileset.tile_grid_size);
            new_tile_ldtk.px.push_back((float)y*_tileset.tile_grid_size);
            
            TILEID selected_tile = _tileset.sorted_tiles.grass_tiles[GetRandomValue(0, _tileset.sorted_tiles.grass_tiles.size() - 1)];
            
            new_tile_ldtk.src.push_back( _tileset.tile_lookup[selected_tile].atlas_position.x);
            new_tile_ldtk.src.push_back(_tileset.tile_lookup[selected_tile].atlas_position.y);
            new_tile_ldtk.t = selected_tile;
            
            new_layer.grid_tiles.push_back(new_tile_ldtk);

            if(_tileset.zone_grid[index] == ZONE_NONE) {
                _tileset.zone_grid[index] = ZONE_GRASS;
            }

            //TraceLog(LOG_INFO, "making tile   id: %i  px: %i %i    src  %i %i",tile_id, new_tile_ldtk.px[0], new_tile_ldtk.px[1], new_tile_ldtk.src[0], new_tile_ldtk.src[1]);
        }
    }
    //GenerateDirtPatch(new_layer, _tileset, _map_size);
    _level.layer_instances.push_back(new_layer);

}






void GenerateUpperTerrainLayer(LDTKLevel &_level, WorldGenTileSet &_tileset, Vector2 _map_size) {
    LDTKLayerInstance new_layer;
    new_layer.identifier = "TerrainUpper";
    new_layer.type = "Tiles";
    new_layer.tileset_def_uid = _tileset.uid;
    new_layer.grid_size = _tileset.tile_grid_size;
    new_layer.c_wid = _map_size.x;
    new_layer.c_hei = _map_size.y;
    
    for(int y = 0; y < _map_size.y; y++) {
        for(int x = 0; x < _map_size.x; x++) {
            if(x == 0 or y == 0 or x == _map_size.x-1 or y == _map_size.y-1) {              
                int index = y * _map_size.x + x;

                _tileset.zone_grid[index] = ZONE_BORDER;

                
                //TraceLog(LOG_INFO, "making border tile id: %i  px %i %i    src  %i %i", tile_id, new_tile_ldtk.px[0], new_tile_ldtk.px[1], new_tile_ldtk.src[0], new_tile_ldtk.src[1]);
            }
        }
    }


    for(int tile = 0; tile < _tileset.zone_grid.size(); tile++) {
        int x = tile%(int)_map_size.x;
        int y = tile/(int)_map_size.x;

        if(x < 0 or y < 0 or x > _map_size.x-1 or y > _map_size.y-1) {
            continue;
        }

        
        if(_tileset.zone_grid[tile] == ZONE_BORDER) {
            
                        
            int index = y * _map_size.x + x;
            int index_right = y * _map_size.x + (x+1);
            if(x+1 >= _map_size.x-1) {
                index_right = y * _map_size.x + x;
            }

            int index_left = y * _map_size.x + (x-1);
            if(x-1 < 0) {
                index_left = y * _map_size.x + x;
            }

            int index_up = (y-1) * _map_size.x + x;
            if(y-1 < 0) {
                index_up = y * _map_size.x + x;
            }

            int index_down = (y+1) * _map_size.x + x;
            if(y+1 >= _map_size.y-1) {
                index_down = y * _map_size.x + x;
            }
            
            bool this_tile_marked_sides[4] = {false, false, false, false};
            
            if (_tileset.zone_grid[index_up] == ZONE_BORDER) {
                this_tile_marked_sides[TILESIDE_UP] = true;
            }
            if (_tileset.zone_grid[index_right] == ZONE_BORDER) {
                this_tile_marked_sides[TILESIDE_RIGHT] = true;
            }
            if (_tileset.zone_grid[index_down] == ZONE_BORDER) {
                this_tile_marked_sides[TILESIDE_DOWN] = true;    
            }
            if (_tileset.zone_grid[index_left] == ZONE_BORDER) {
                this_tile_marked_sides[TILESIDE_LEFT] = true;
            }

            TILEID tile_id = TILE_ID_BORDER_SINGLE;

            for(auto &tile : _tileset.tile_lookup) {
                if(tile.first > TILE_ID_BORDER_START and tile.first < TILE_ID_BORDER_END) {
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
            
            LDTKGridTile new_tile_ldtk;

            new_tile_ldtk.px.push_back((float)x*_tileset.tile_grid_size);
            new_tile_ldtk.px.push_back((float)y*_tileset.tile_grid_size);
                
                        
            new_tile_ldtk.src.push_back( _tileset.tile_lookup[tile_id].atlas_position.x);
            new_tile_ldtk.src.push_back(_tileset.tile_lookup[tile_id].atlas_position.y);
            new_tile_ldtk.t = tile_id;
                
            new_layer.grid_tiles.push_back(new_tile_ldtk);

            //new_tile_ldtk.src.push_back( _tileset.tile_lookup[tile_id].atlas_position.x);
            //new_tile_ldtk.src.push_back(_tileset.tile_lookup[tile_id].atlas_position.y);
            //new_tile_ldtk.t = tile_id;
                        
            //TraceLog(LOG_INFO, "PATH TILE  tile id  %i   pos %i %i   index %i", new_tile_ldtk.t, new_tile_ldtk.px[0], new_tile_ldtk.px[1], index);
            new_layer.grid_tiles.push_back(new_tile_ldtk);
        }
    }
    //GenerateMapPaths(new_layer, _tileset, _map_size);
    _level.layer_instances.push_back(new_layer);
}




void GenerateStructuresLayer(LDTKLevel &_level, WorldGenTileSet &_tileset, Vector2 _map_size) {
    LDTKLayerInstance new_layer;

    _level.layer_instances.push_back(new_layer);
}



void GenerateCollisionLayer(LDTKLevel &_level, WorldGenTileSet &_tileset, Vector2 _map_size) {
    LDTKLayerInstance new_layer;
    
    new_layer.identifier = "Collision";
    new_layer.type = "IntGrid";
    new_layer.tileset_def_uid = -1;
    new_layer.grid_size = _tileset.tile_grid_size;
    new_layer.c_wid = _map_size.x;
    new_layer.c_hei = _map_size.y;
    new_layer.int_grid.resize(_map_size.x*_map_size.y, 0);

    int cols = _map_size.x;

    for(int y = 0; y < _map_size.y; y++) {
        for(int x = 0; x < _map_size.x; x++) { 
            int index = y * cols + x;

            if(_tileset.zone_grid[index] == ZONE_BORDER) {
                new_layer.int_grid[index] = 1;
                TraceLog(LOG_INFO, "BORdER TILE found %i %i", x, y);
            }
        }
    }
    
    _level.layer_instances.push_back(new_layer);
}


void GenerateEntitiesLayer(LDTKLevel &_level, WorldGenTileSet &_tileset, Vector2 _map_size) {
    LDTKLayerInstance new_layer;
    new_layer.identifier = "Entities";
    new_layer.type = "Entities";
    new_layer.tileset_def_uid = -1;
    new_layer.grid_size = _tileset.tile_grid_size;
    new_layer.c_wid = _map_size.x;
    new_layer.c_hei = _map_size.y;

    LDTKEntityInstance spawn_point;
    spawn_point.px.push_back(5*_tileset.tile_grid_size);
    spawn_point.px.push_back(5*_tileset.tile_grid_size);
    spawn_point.identifier = "SpawnPoint";
    new_layer.entity_instances.push_back(spawn_point);
    _level.layer_instances.push_back(new_layer);


}