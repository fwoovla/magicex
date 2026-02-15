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
                TILEID id =  GetAutoTile(_tileset.sorted_tiles.dirt_tiles, _tileset, _tileset.lower_zone_grid, ZONE_DIRT, index);
                if(id != -1) {
                    
                    LDTKGridTile new_tile_ldtk;
                    new_tile_ldtk.px.push_back((float)x*_tileset.tile_grid_size);
                    new_tile_ldtk.px.push_back((float)y*_tileset.tile_grid_size);
                    //TILEID id = TILE_ID_DIRT_MID;
                    new_tile_ldtk.src.push_back( _tileset.tile_lookup[id].atlas_position.x);
                    new_tile_ldtk.src.push_back(_tileset.tile_lookup[id].atlas_position.y);
                    new_tile_ldtk.t = id;
                    
                    new_layer.grid_tiles.push_back(new_tile_ldtk);
                }
                else {
                    _tileset.lower_zone_grid[index] = ZONE_NONE;
                }

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

                TILEID id =  GetAutoTile(_tileset.sorted_tiles.path_tiles, _tileset, _tileset.upper_zone_grid, ZONE_PATH, index);
                if(id != TILE_ID_NONE) {
                    //TraceLog(LOG_INFO, "PATH zone  %i ", index);
                    
                    LDTKGridTile new_tile_ldtk;
                    
                    new_tile_ldtk.px.push_back((float)x*_tileset.tile_grid_size);
                    new_tile_ldtk.px.push_back((float)y*_tileset.tile_grid_size);
                    //TILEID id = TILE_ID_DIRT_MID;
                    new_tile_ldtk.src.push_back( _tileset.tile_lookup[id].atlas_position.x);
                    new_tile_ldtk.src.push_back(_tileset.tile_lookup[id].atlas_position.y);
                    new_tile_ldtk.t = id;
                    
                    new_layer.grid_tiles.push_back(new_tile_ldtk);
                }
                else {
                    _tileset.upper_zone_grid[index] = ZONE_NONE;
                    _tileset.collision_grid[index] = 0;
                }

            }

            if(_tileset.upper_zone_grid[index] == ZONE_BORDER) {
                //TraceLog(LOG_INFO, "BORDER zone  %i-  %i %i", index, x, y);
                TILEID id =  GetAutoTile(_tileset.sorted_tiles.border_tiles, _tileset, _tileset.upper_zone_grid, ZONE_BORDER, index);

                if(id == TILE_ID_NONE) {
                    _tileset.upper_zone_grid[index] = ZONE_NONE;
                    _tileset.collision_grid[index] = 0;
                    //TraceLog(LOG_INFO, "no tile found  %i", id);
                }
                else {

                    LDTKGridTile new_tile_ldtk;
                    
                    new_tile_ldtk.px.push_back((float)x*_tileset.tile_grid_size);
                    new_tile_ldtk.px.push_back((float)y*_tileset.tile_grid_size);
                    new_tile_ldtk.src.push_back( _tileset.tile_lookup[id].atlas_position.x);
                    new_tile_ldtk.src.push_back(_tileset.tile_lookup[id].atlas_position.y);
                    new_tile_ldtk.t = id;
                    
                    new_layer.grid_tiles.push_back(new_tile_ldtk);
                    _tileset.collision_grid[index] = 1;
                }
            }

            if(_tileset.upper_zone_grid[index] == ZONE_FENCE) { 

                TILEID id =  GetFenceTileTop(_tileset, _tileset.upper_zone_grid, x, y); //TILE_ID_FENCE_END_DOWN;

                if(id == TILE_ID_NONE) {
                    //_tileset.upper_zone_grid[index] = ZONE_NONE;
                    //_tileset.collision_grid[index] = 0;
                    //TraceLog(LOG_INFO, "no tile found  %i", id);
                }
                else {

                    LDTKGridTile new_tile_ldtk;
                    
                    new_tile_ldtk.px.push_back((float)x*_tileset.tile_grid_size);
                    new_tile_ldtk.px.push_back((float)y*_tileset.tile_grid_size);
                    new_tile_ldtk.src.push_back( _tileset.tile_lookup[id].atlas_position.x);
                    new_tile_ldtk.src.push_back(_tileset.tile_lookup[id].atlas_position.y);
                    new_tile_ldtk.t = id;
                    
                    new_layer.grid_tiles.push_back(new_tile_ldtk);
                    _tileset.collision_grid[index] = 1;
                }
            }

            if(_tileset.upper_zone_grid[index] == ZONE_FENCE_BOTTOM) { 
                TILEID id = GetFenceTileBottom(_tileset, _tileset.upper_zone_grid, x, y);

                if(id == TILE_ID_NONE) {
                    //_tileset.upper_zone_grid[index] = ZONE_NONE;
                    //_tileset.collision_grid[index] = 0;
                TraceLog(LOG_INFO, "no fence bottom tile found  %i", id);
                }
                else {
                    //TraceLog(LOG_INFO, "fence bottom tile found  %i", id);
                    LDTKGridTile new_tile_ldtk;
                    
                    new_tile_ldtk.px.push_back((float)x*_tileset.tile_grid_size);
                    new_tile_ldtk.px.push_back((float)y*_tileset.tile_grid_size);
                    new_tile_ldtk.src.push_back( _tileset.tile_lookup[id].atlas_position.x);
                    new_tile_ldtk.src.push_back(_tileset.tile_lookup[id].atlas_position.y);
                    new_tile_ldtk.t = id;
                    
                    new_layer.grid_tiles.push_back(new_tile_ldtk);
                }
            }
        }
    }

    _level.layer_instances.push_back(new_layer);
}




void GenerateStructuresLayer(LDTKLevel &_level, WorldGenTileSet &current_tileset, WorldGenTileSet &structure_tileset) {

    LDTKLayerInstance *entities_layer = nullptr;

    for(int l = 0; l < _level.layer_instances.size(); l++) {
        if(_level.layer_instances[l].type == "Entities") {
            entities_layer = &_level.layer_instances[l];
        }
    }

    if(entities_layer == nullptr) {
        TraceLog(LOG_INFO, "!!!!!   NO ENTITIES LAYER  !!!");
        return;
    }

    LDTKLayerInstance new_layer;

    new_layer.identifier = "Structures";
    new_layer.type = "Tiles";
    new_layer.tileset_def_uid = structure_tileset.uid;
    new_layer.grid_size = current_tileset.tile_grid_size;
    new_layer.c_wid = current_tileset.map_size.x;
    new_layer.c_hei = current_tileset.map_size.y;
    
    std::vector<std::string>  structure_names;

    for(auto &structure :structure_tileset.structure_lookup) {
        if(structure.first != "STARTINGSHELTER")
        structure_names.push_back(structure.first);
    }

    for(Vector2 structure_pos : current_tileset.structure_positions) {
        std::string name_choice;

        Vector2 s_pos = structure_pos;


        if(s_pos != current_tileset.structure_positions[0]) {
            name_choice = structure_names[GetRandomValue(0, structure_names.size() - 1 )];
        }
        else {
            name_choice = "STARTINGSHELTER";
            //s_pos.y -= 3;
        }


        int half_width = structure_tileset.structure_lookup[name_choice].structure_size.x/2;
        int half_height = structure_tileset.structure_lookup[name_choice].structure_size.y/2;

        Vector2 corner_offset = {s_pos.x - half_width, s_pos.y - half_height};

        for(WorldGenAutoTile tile : structure_tileset.structure_lookup[name_choice].structure_grid_tiles) { 
            
            LDTKGridTile new_tile_ldtk;
            new_tile_ldtk.px.push_back( (corner_offset.x * current_tileset.tile_grid_size) + tile.position.x );
            new_tile_ldtk.px.push_back( (corner_offset.y * current_tileset.tile_grid_size) + tile.position.y);
            new_tile_ldtk.src.push_back( tile.atlas_position.x);
            new_tile_ldtk.src.push_back(tile.atlas_position.y);
            new_tile_ldtk.t = tile.tile_id;
                
            new_layer.grid_tiles.push_back(new_tile_ldtk);
                
                
            int x = new_tile_ldtk.px[0]/current_tileset.tile_grid_size;
            int y = new_tile_ldtk.px[1]/current_tileset.tile_grid_size;

            if(x >= current_tileset.map_size.x) {
                x = current_tileset.map_size.x - 1;
            }
            if(y >= current_tileset.map_size.y) {
                y = current_tileset.map_size.y - 1;
            }

            int index = (y * current_tileset.map_size.x + x);


            TraceLog(LOG_INFO, "------tile %i   index %i at position %i %i",new_tile_ldtk.t, index, new_tile_ldtk.px[0], new_tile_ldtk.px[1]);


            if(tile.has_collision == true) {
                    //TraceLog(LOG_INFO, "structure tile");
                current_tileset.upper_zone_grid[index] = ZONE_STRUCTURE;
                current_tileset.lower_zone_grid[index] = ZONE_DIRT;
                current_tileset.collision_grid[index] = 1;
            }
            else {
                    //TraceLog(LOG_INFO, "shadow tile");
                //AWcurrent_tileset.upper_zone_grid[index] = ZONE_NONE;
                current_tileset.collision_grid[index] = 0;
            }



            for(int tile_tag = 0; tile_tag < structure_tileset.tile_tags.size(); tile_tag++) {
                if(structure_tileset.tile_tags[tile_tag].value_string == "HOUSE_TRANSITION") {
                    for(int i = 0; i < structure_tileset.tile_tags[tile_tag].tile_ids.size(); i++) {
                        if(new_tile_ldtk.t == structure_tileset.tile_tags[tile_tag].tile_ids[i]) {
                            //TraceLog(LOG_INFO, "      ---DOOR TILE FOUND---");

                            LDTKEntityInstance new_entity;
                            if(name_choice == "STARTINGSHELTER") {
                                new_entity.identifier = "ShelterTransition";
                            }
                            else {
                                new_entity.identifier = "HouseTransition";
                            }

                            TraceLog(LOG_INFO, "++++++------------NEW ENTITY %s", new_entity.identifier.c_str());

                            new_entity.iid = "housetransition_" + std::to_string( GetRandomValue(100, 10000));
                            new_entity.px.push_back(new_tile_ldtk.px[0]);
                            new_entity.px.push_back(new_tile_ldtk.px[1]);
                            new_entity.width = current_tileset.tile_grid_size;
                            new_entity.height = current_tileset.tile_grid_size * 2;

                            LDTKFieldInstance dest_map_field;
                            dest_map_field.identifier = "DestMapString";
                            
                            if(name_choice == "STARTINGSHELTER") {
                                dest_map_field.value_s = "StartingShelter";
                                //TraceLog(LOG_INFO, "++++++--------------------------------ENTITY FIELD %s", dest_map_field.identifier.c_str());
                            }
                            else {
                                TraceLog(LOG_INFO, "++++++--------- sub map names %i", g_sub_map_string_names.size());
                                for(std::string &map : g_sub_map_string_names) {
                                    TraceLog(LOG_INFO, "++++++--------- sub map %s", map.c_str());
                                }
                                std::string sub_map_choice = g_sub_map_string_names[GetRandomValue(0, g_sub_map_string_names.size()-1)];

                                dest_map_field.value_s = sub_map_choice;
                            }
                            

                            new_entity.field_instances.push_back(dest_map_field);

                            LDTKFieldInstance return_field;
                            return_field.identifier = "ReturnPosition";
                            return_field.value_v = {(float)new_tile_ldtk.px[0] / 2, (float)(new_tile_ldtk.px[1] / 2 + (current_tileset.tile_grid_size * 2))};
                            //TraceLog(LOG_INFO, "++++++--------------------------------Return FIELD %s  %0.0f %0.0f", return_field.identifier.c_str(), return_field.value_v.x, return_field.value_v.y);

                            new_entity.field_instances.push_back(return_field);

                            entities_layer->entity_instances.push_back(new_entity);
                        }
                    }
                }
            }
        }
        TraceLog(LOG_INFO, "++++++---------\n");
    }
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

            if(_tileset.collision_grid[index] == 1) {
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

    _level.layer_instances.push_back(new_layer);

    _tileset.collision_layer_index = _level.layer_instances.size() - 1;
}


void PlaceEntities(LDTKLevel &level, WorldGenTileSet &_tileset) {

    //shelter trantsition
    //lootables
    //mobs

    LDTKLayerInstance *entity_layer = nullptr;

    for(LDTKLayerInstance &layer : level.layer_instances) {
        if(layer.type == "Entities") {
            entity_layer = &layer;
        }
    }

    if(entity_layer == nullptr) {
        TraceLog(LOG_INFO, "no entities layer");
        return;
    }


    LDTKEntityInstance spawn_point;
    spawn_point.px.push_back( _tileset.structure_positions[0].x * _tileset.tile_grid_size);
    spawn_point.px.push_back( (_tileset.structure_positions[0].y + 4) * _tileset.tile_grid_size);
    spawn_point.identifier = "SpawnPoint";
    entity_layer->entity_instances.push_back(spawn_point);


}