#include "../../core/scenerygen.h"

void PopulateTrees(LDTKLevel &_level, WorldGenTileSet &_tileset) {
    Vector2 temp_grid_size = {(_tileset.map_size.x*2), (_tileset.map_size.y*2)};
    std::vector<int> temp_grid;
    temp_grid.resize(temp_grid_size.x*temp_grid_size.y, SPRITE_ENVIRO_ERROR);

    int _grid_size = 8;


    int free_tiles = 0;

    for(int tile : _tileset.upper_zone_grid) {
        if(tile == ZONE_NONE) {
            free_tiles++;
        }
    }


    int num_trees = free_tiles * _tileset.tree_coverage;

    for(int i = 0; i < num_trees; i++) {
        int x = GetRandomValue(4, (int)(temp_grid_size.x)-4);   
        int y = GetRandomValue(4, (int)(temp_grid_size.y)-4);

        int index = y * temp_grid_size.x + x;

        int zone_index = y/2 * (_tileset.map_size.x) + x/2;

        if(_tileset.upper_zone_grid[zone_index] == ZONE_NONE and _tileset.lower_zone_grid[zone_index] == ZONE_GRASS) {
            
            if(temp_grid[index] == SPRITE_ENVIRO_ERROR ) {
                
                //_tileset.collision_grid[zone_index] = 1;

                int tree_id = GetRandomValue(SPRITE_ENVIRO_TREE_START +1, SPRITE_ENVIRO_TREE_END -1);
                
                temp_grid[index] = tree_id;
                
                LDTKEnvironmentData new_thing;
                new_thing.item_string = EnvironmentalIdToStr( tree_id );
                new_thing.position = {(float)(x * _grid_size), (float)((y+1) * _grid_size)};
                _level.environment_data.push_back(new_thing);
                
                //TraceLog(LOG_INFO, "zone %i  id %i------------new grass %s  %0.0f %0.0f", _tileset.zone_grid[zone_index], StrToEnviroSpriteId(new_thing.item_string), new_thing.item_string.c_str(), new_thing.position.x, new_thing.position.y );
            }
        }
    }
}



void PopulateDeccoEntities(LDTKLevel &_level, WorldGenTileSet &current_tileset, WorldGenTileSet &structure_tileset) {


    std::vector<std::string>  structure_names;

    for(auto &structure :structure_tileset.entity_decco_lookup) {
        structure_names.push_back(structure.first);
    }


    for(WorldGenDeccoEntityData &decco_entity : current_tileset.entity_decco_data) {

        std::string name_choice;

        Vector2 s_pos = decco_entity.position;
        name_choice = decco_entity.decco_name;

        int half_width = structure_tileset.entity_decco_lookup[name_choice].structure_size.x/2;
        int half_height = structure_tileset.entity_decco_lookup[name_choice].structure_size.y/2;

        Vector2 corner_offset = {s_pos.x - half_width, s_pos.y - half_height};
        Vector2 bottom_offset = {s_pos.x + half_width, s_pos.y + (half_height*2)};

        for(WorldGenAutoTile &tile : structure_tileset.entity_decco_lookup[name_choice].structure_grid_tiles) {
            LDTKGridTile new_tile_ldtk;
            new_tile_ldtk.px.push_back( (corner_offset.x * current_tileset.tile_grid_size) + tile.position.x );
            new_tile_ldtk.px.push_back( (corner_offset.y * current_tileset.tile_grid_size) + tile.position.y );
            new_tile_ldtk.src.push_back( tile.atlas_position.x);
            new_tile_ldtk.src.push_back(tile.atlas_position.y);
            new_tile_ldtk.t = tile.tile_id;

            int x = new_tile_ldtk.px[0]/current_tileset.tile_grid_size;
            int y = new_tile_ldtk.px[1]/current_tileset.tile_grid_size;

            if(x <  1) {x = 1;}
            if(y <  1) {y = 1;}
            if(x >= current_tileset.map_size.x) {x = current_tileset.map_size.x - 1;}
            if(y >= current_tileset.map_size.y) {y = current_tileset.map_size.y - 1;}

            int index = (y * current_tileset.map_size.x + x);

            TraceLog(LOG_INFO, "------fake decco entity tile %i   index %i at position %i %i",new_tile_ldtk.t, index, new_tile_ldtk.px[0], new_tile_ldtk.px[1]);

            if(tile.has_collision == true) {
                    //TraceLog(LOG_INFO, "structure tile");
                current_tileset.upper_zone_grid[index] = ZONE_STRUCTURE;
                //current_tileset.lower_zone_grid[index] = ZONE_DIRT;
                current_tileset.collision_grid[index] = 1;
            }
            else {
                    //TraceLog(LOG_INFO, "shadow tile");
                //AWcurrent_tileset.upper_zone_grid[index] = ZONE_NONE;
                current_tileset.collision_grid[index] = 0;
            }
            //int decco_id = name_choice;
        }
        LDTKEnvironmentData new_thing;

        new_thing.item_string = name_choice;
        new_thing.position = {(float)(s_pos.x *current_tileset.tile_grid_size ), (float)((s_pos.y+2) * current_tileset.tile_grid_size)};
        _level.environment_data.push_back(new_thing);
        TraceLog(LOG_INFO, "DECCO ENTIY, %s", new_thing.item_string.c_str());
    }
}




void AddMushroomZones(LDTKLevel &_level, WorldGenTileSet &_tileset) {



    for(Rectangle patch : _tileset.ruins_rects) {
        LDTKEntityInstance new_shroom_zone;
        new_shroom_zone.identifier = "MushroomZone";


        new_shroom_zone.iid = "shroomzone_" + std::to_string(GetRandomValue(100, 1000000));

        new_shroom_zone.px.push_back(patch.x * _tileset.tile_grid_size);
        new_shroom_zone.px.push_back(patch.y * _tileset.tile_grid_size);
        new_shroom_zone.width = patch.width * _tileset.tile_grid_size;
        new_shroom_zone.height = patch.height * _tileset.tile_grid_size;
        LDTKFieldInstance new_field;

        new_field.identifier = "max_mushrooms";
        new_field.value_i = GetRandomValue(1*patch.width, 1*patch.height);
        new_shroom_zone.field_instances.push_back(new_field);
        _level.layer_instances[_tileset.collision_layer_index].entity_instances.push_back(new_shroom_zone);

        
        //TraceLog(LOG_INFO, "shroom zone");
    }

}





void PopulateGrass(LDTKLevel &_level, WorldGenTileSet &_tileset) {

    Vector2 temp_grid_size = {(_tileset.map_size.x*2), (_tileset.map_size.y*2)};
    std::vector<int> temp_grid;
    temp_grid.resize(temp_grid_size.x*temp_grid_size.y, SPRITE_ENVIRO_ERROR);

    int _grid_size = 8;

    int free_tiles = 0;

    for(int tile : _tileset.upper_zone_grid) {
        if(tile == ZONE_NONE) {
            free_tiles++;
        }
    }


    int num_grass = free_tiles * _tileset.grass_coverage;

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


