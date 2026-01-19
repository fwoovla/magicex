#include "../../core/layergen.h"

void GenerateLowerTerrainLayer(LDTKLevel &_level, WorldGenTileSet &_tileset) {
    LDTKLayerInstance new_layer;

    new_layer.identifier = "TerrainLower";
    new_layer.type = "Tiles";
    new_layer.tileset_def_uid = _tileset.uid;
    new_layer.grid_size = _tileset.tile_grid_size;
    new_layer.c_wid = _tileset.map_size.x;
    new_layer.c_hei = _tileset.map_size.y;
    
    for(int y = 0; y < _tileset.map_size.y; y++) {
        for(int x = 0; x < _tileset.map_size.x; x++) {

            int index = y * _tileset.map_size.x + x;

            if(_tileset.lower_zone_grid[index] == ZONE_GRASS) {

                
                LDTKGridTile new_tile_ldtk;
                
                new_tile_ldtk.px.push_back((float)x*_tileset.tile_grid_size);
                new_tile_ldtk.px.push_back((float)y*_tileset.tile_grid_size);
                
                TILEID selected_tile = _tileset.sorted_tiles.grass_tiles[GetRandomValue(0, _tileset.sorted_tiles.grass_tiles.size() - 1)];
                
                new_tile_ldtk.src.push_back( _tileset.tile_lookup[selected_tile].atlas_position.x);
                new_tile_ldtk.src.push_back(_tileset.tile_lookup[selected_tile].atlas_position.y);
                new_tile_ldtk.t = selected_tile;
                
                new_layer.grid_tiles.push_back(new_tile_ldtk);
            }


            if(_tileset.lower_zone_grid[index] == ZONE_DIRT) {
                LDTKGridTile new_tile_ldtk;
                
                new_tile_ldtk.px.push_back((float)x*_tileset.tile_grid_size);
                new_tile_ldtk.px.push_back((float)y*_tileset.tile_grid_size);
                TILEID id =  GetAutoTile(_tileset.sorted_tiles.dirt_tiles, _tileset, _tileset.lower_zone_grid, ZONE_DIRT, index);
                //TILEID id = TILE_ID_DIRT_MID;
                new_tile_ldtk.src.push_back( _tileset.tile_lookup[id].atlas_position.x);
                new_tile_ldtk.src.push_back(_tileset.tile_lookup[id].atlas_position.y);
                new_tile_ldtk.t = id;
                
                new_layer.grid_tiles.push_back(new_tile_ldtk);

            }
            //TraceLog(LOG_INFO, "making tile   id: %i  px: %i %i    src  %i %i",tile_id, new_tile_ldtk.px[0], new_tile_ldtk.px[1], new_tile_ldtk.src[0], new_tile_ldtk.src[1]);
        }
    }
    //GenerateDirtPatch(new_layer, _tileset, _map_size);
    _level.layer_instances.push_back(new_layer);

}




void GenerateUpperTerrainLayer(LDTKLevel &_level, WorldGenTileSet &_tileset) {
    LDTKLayerInstance new_layer;
    new_layer.identifier = "TerrainUpper";
    new_layer.type = "Tiles";
    new_layer.tileset_def_uid = _tileset.uid;
    new_layer.grid_size = _tileset.tile_grid_size;
    new_layer.c_wid = _tileset.map_size.x;
    new_layer.c_hei = _tileset.map_size.y;
    

    for(int y = 0; y < _tileset.map_size.y; y++) {
        for(int x = 0; x < _tileset.map_size.x; x++) {

            int index = y * _tileset.map_size.x + x;

            if(_tileset.upper_zone_grid[index] == ZONE_PATH) {

                //TraceLog(LOG_INFO, "PATH zone  %i ", index);

                LDTKGridTile new_tile_ldtk;
                
                new_tile_ldtk.px.push_back((float)x*_tileset.tile_grid_size);
                new_tile_ldtk.px.push_back((float)y*_tileset.tile_grid_size);
                TILEID id =  GetAutoTile(_tileset.sorted_tiles.path_tiles, _tileset, _tileset.upper_zone_grid, ZONE_PATH, index);
                //TILEID id = TILE_ID_DIRT_MID;
                new_tile_ldtk.src.push_back( _tileset.tile_lookup[id].atlas_position.x);
                new_tile_ldtk.src.push_back(_tileset.tile_lookup[id].atlas_position.y);
                new_tile_ldtk.t = id;
                
                new_layer.grid_tiles.push_back(new_tile_ldtk);

            }

            if(_tileset.upper_zone_grid[index] == ZONE_BORDER) {
                //TraceLog(LOG_INFO, "BORDER zone  %i-  %i %i", index, x, y);

                LDTKGridTile new_tile_ldtk;
                
                new_tile_ldtk.px.push_back((float)x*_tileset.tile_grid_size);
                new_tile_ldtk.px.push_back((float)y*_tileset.tile_grid_size);
                TILEID id =  GetAutoTile(_tileset.sorted_tiles.border_tiles, _tileset, _tileset.upper_zone_grid, ZONE_BORDER, index);
                new_tile_ldtk.src.push_back( _tileset.tile_lookup[id].atlas_position.x);
                new_tile_ldtk.src.push_back(_tileset.tile_lookup[id].atlas_position.y);
                new_tile_ldtk.t = id;
                
                new_layer.grid_tiles.push_back(new_tile_ldtk);
            }
            if(_tileset.upper_zone_grid[index] == ZONE_STRUCTURE) {
                LDTKGridTile new_tile_ldtk;
                
                new_tile_ldtk.px.push_back((float)x*_tileset.tile_grid_size);
                new_tile_ldtk.px.push_back((float)y*_tileset.tile_grid_size);
                TILEID id =  TILE_ID_BORDER_MID;
                new_tile_ldtk.src.push_back( _tileset.tile_lookup[id].atlas_position.x);
                new_tile_ldtk.src.push_back(_tileset.tile_lookup[id].atlas_position.y);
                new_tile_ldtk.t = id;
                
                new_layer.grid_tiles.push_back(new_tile_ldtk);
            }
        }
    }


    
    _level.layer_instances.push_back(new_layer);
}




void GenerateStructuresLayer(LDTKLevel &_level, WorldGenTileSet &_tileset) {
    LDTKLayerInstance new_layer;

    _level.layer_instances.push_back(new_layer);
}



void GenerateCollisionLayer(LDTKLevel &_level, WorldGenTileSet &_tileset) {
    LDTKLayerInstance new_layer;
    
    new_layer.identifier = "Collision";
    new_layer.type = "IntGrid";
    new_layer.tileset_def_uid = -1;
    new_layer.grid_size = _tileset.tile_grid_size;
    new_layer.c_wid = _tileset.map_size.x;
    new_layer.c_hei = _tileset.map_size.y;
    new_layer.int_grid.resize(_tileset.map_size.x*_tileset.map_size.y, 0);

    int cols = _tileset.map_size.x;

    for(int y = 0; y < _tileset.map_size.y; y++) {
        for(int x = 0; x < _tileset.map_size.x; x++) { 
            int index = y * cols + x;

            if(_tileset.upper_zone_grid[index] == ZONE_BORDER) {
                new_layer.int_grid[index] = 1;
                //TraceLog(LOG_INFO, "BORdER TILE found %i %i", x, y);
            }
        }
    }
    
    _level.layer_instances.push_back(new_layer);
}


void GenerateEntitiesLayer(LDTKLevel &_level, WorldGenTileSet &_tileset) {
    LDTKLayerInstance new_layer;
    new_layer.identifier = "Entities";
    new_layer.type = "Entities";
    new_layer.tileset_def_uid = -1;
    new_layer.grid_size = _tileset.tile_grid_size;
    new_layer.c_wid = _tileset.map_size.x;
    new_layer.c_hei = _tileset.map_size.y;

    LDTKEntityInstance spawn_point;
    spawn_point.px.push_back(5*_tileset.tile_grid_size);
    spawn_point.px.push_back(5*_tileset.tile_grid_size);
    spawn_point.identifier = "SpawnPoint";
    new_layer.entity_instances.push_back(spawn_point);
    _level.layer_instances.push_back(new_layer);


}