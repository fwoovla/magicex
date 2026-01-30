#include "../../core/scenerygen.h"

void PopulateTrees(LDTKLevel &_level, WorldGenTileSet &_tileset) {
    Vector2 temp_grid_size = {(_tileset.map_size.x*2), (_tileset.map_size.y*2)};
    std::vector<int> temp_grid;
    temp_grid.resize(temp_grid_size.x*temp_grid_size.y, SPRITE_ENVIRO_ERROR);

    int _grid_size = 8;

    int num_trees =_tileset.max_trees;

    for(int i = 0; i < num_trees; i++) {
        int x = GetRandomValue(4, (int)(temp_grid_size.x)-4);   
        int y = GetRandomValue(4, (int)(temp_grid_size.y)-4);

        int index = y * temp_grid_size.x + x;

        int zone_index = y/2 * (_tileset.map_size.x) + x/2;

        if(_tileset.upper_zone_grid[zone_index] == ZONE_NONE and _tileset.lower_zone_grid[zone_index] == ZONE_GRASS) {
            
            if(temp_grid[index] == SPRITE_ENVIRO_ERROR ) {
                
                int tree_id = GetRandomValue(SPRITE_ENVIRO_TREE_START +1, SPRITE_ENVIRO_TREE_END -1);
                
                temp_grid[index] = tree_id;
                
                LDTKEnvironmentData new_thing;
                new_thing.item_string = EnvironmentalIdToStr( tree_id );
                new_thing.position = {(float)(x * _grid_size), (float)(y * _grid_size)};
                _level.environment_data.push_back(new_thing);
                
                //TraceLog(LOG_INFO, "zone %i  id %i------------new grass %s  %0.0f %0.0f", _tileset.zone_grid[zone_index], StrToEnviroSpriteId(new_thing.item_string), new_thing.item_string.c_str(), new_thing.position.x, new_thing.position.y );
            }
        }
    }
}



void PopulateGrass(LDTKLevel &_level, WorldGenTileSet &_tileset) {

    Vector2 temp_grid_size = {(_tileset.map_size.x*2), (_tileset.map_size.y*2)};
    std::vector<int> temp_grid;
    temp_grid.resize(temp_grid_size.x*temp_grid_size.y, SPRITE_ENVIRO_ERROR);

    int _grid_size = 8;

    int num_grass = _tileset.max_grass;

    for(int i = 0; i < num_grass; i++) {
        int x = GetRandomValue(4, (int)(temp_grid_size.x)-4);   
        int y = GetRandomValue(4, (int)(temp_grid_size.y)-4);

        int index = y * temp_grid_size.x + x;

        int zone_index = y/2 * (_tileset.map_size.x) + x/2;

        if(_tileset.upper_zone_grid[zone_index] == ZONE_NONE and _tileset.lower_zone_grid[zone_index] == ZONE_GRASS) {
            
            if(temp_grid[index] == SPRITE_ENVIRO_ERROR ) {
                
                int grass_id = GetRandomValue(SPRITE_ENVIRO_GRASS_START +1, SPRITE_ENVIRO_GRASS_END -1);
                
                temp_grid[index] = grass_id;
                
                LDTKEnvironmentData new_thing;
                new_thing.item_string = EnvironmentalIdToStr( grass_id );
                //new_thing.item_string = "Grass2";
                new_thing.position = {(float)(x * _grid_size), (float)(y * _grid_size)};
                _level.environment_data.push_back(new_thing);
                
                //TraceLog(LOG_INFO, "zone %i  id %i------------new grass %s  %0.0f %0.0f", _tileset.zone_grid[zone_index], StrToEnviroSpriteId(new_thing.item_string), new_thing.item_string.c_str(), new_thing.position.x, new_thing.position.y );
            }
        }
    }
}


