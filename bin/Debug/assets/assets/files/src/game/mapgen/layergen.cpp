#include "../../core/gamedefs.h"

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
                    _tileset.lower_zone_grid[index] = ZONE_GRASS;
                }
            }
            //TraceLog(LOG_INFO, "making tile   id: %i  px: %i %i    src  %i %i",tile_id, new_tile_ldtk.px[0], new_tile_ldtk.px[1], new_tile_ldtk.src[0], new_tile_ldtk.src[1]);
        }
    }
    _level.layer_instances.push_back(new_layer);

}


void GenerateDeccoLayer(LDTKLevel &_level, WorldGenTileSet &current_tileset, WorldGenTileSet &structure_tileset) {

    LDTKLayerInstance new_layer;

    new_layer.identifier = "LowerDeccoLayer";
    new_layer.type = "Tiles";
    new_layer.tileset_def_uid = structure_tileset.uid;
    new_layer.grid_size = current_tileset.tile_grid_size;
    new_layer.c_wid = current_tileset.map_size.x;
    new_layer.c_hei = current_tileset.map_size.y;


    std::vector<std::string>  structure_names;

    for(auto &structure :structure_tileset.layer_decco_lookup) {
        structure_names.push_back(structure.first);
    }

    for(WorldGenDeccoEntityData &layer_entity : current_tileset.layer_decco_data) {

        std::string name_choice;

        Vector2 s_pos = layer_entity.position;
        name_choice = layer_entity.decco_name;

        int half_width = structure_tileset.layer_decco_lookup[name_choice].structure_size.x/2;
        int half_height = structure_tileset.layer_decco_lookup[name_choice].structure_size.y/2;

        Vector2 corner_offset = {s_pos.x - half_width, s_pos.y - half_height};
        Vector2 bottom_offset = {s_pos.x + half_width, s_pos.y + (half_height*2)};

        for(WorldGenAutoTile &tile : structure_tileset.layer_decco_lookup[name_choice].structure_grid_tiles) {
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

            //TraceLog(LOG_INFO, "------tile %i   index %i at position %i %i",new_tile_ldtk.t, index, new_tile_ldtk.px[0], new_tile_ldtk.px[1]);

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
        }
    }
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


            TILEID tile_id = TILE_ID_NONE;

            if(_tileset.upper_zone_grid[index] == ZONE_ROAD) {
                tile_id = GetAutoTile(_tileset.sorted_tiles.road_tiles, _tileset, _tileset.upper_zone_grid, ZONE_ROAD, index);
            }

            else if(_tileset.upper_zone_grid[index] == ZONE_ROAD) {
                tile_id =  GetAutoTile(_tileset.sorted_tiles.road_tiles, _tileset, _tileset.upper_zone_grid, ZONE_ROAD, index);
            }

            else if(_tileset.upper_zone_grid[index] == ZONE_PATH) {
                tile_id =  GetAutoTile(_tileset.sorted_tiles.path_tiles, _tileset, _tileset.upper_zone_grid, ZONE_PATH, index);
            }

            else if(_tileset.upper_zone_grid[index] == ZONE_BORDER) {
                tile_id =  GetAutoTile(_tileset.sorted_tiles.border_tiles, _tileset, _tileset.upper_zone_grid, ZONE_BORDER, index);
                if(tile_id != TILE_ID_NONE) {
                    _tileset.collision_grid[index] = 1;     
                }
                else {
                    _tileset.collision_grid[index] = 0;
                }
            }

            else if(_tileset.upper_zone_grid[index] == ZONE_FENCE) { 
                tile_id =  GetFenceTileTop(_tileset, _tileset.upper_zone_grid, x, y); //TILE_ID_FENCE_END_DOWN;
            }

            else if(_tileset.upper_zone_grid[index] == ZONE_FENCE_BOTTOM) { 
                tile_id = GetFenceTileBottom(_tileset, _tileset.upper_zone_grid, x, y);

                if(tile_id != TILE_ID_NONE) {
                    _tileset.collision_grid[index] = 0;     
                }
            }

            if(tile_id != TILE_ID_NONE) {
                LDTKGridTile _new_tile_ldtk;
                
                _new_tile_ldtk.px.push_back((float)x*_tileset.tile_grid_size);
                _new_tile_ldtk.px.push_back((float)y*_tileset.tile_grid_size);
                //TILEID id = TILE_ID_DIRT_MID;
                _new_tile_ldtk.src.push_back( _tileset.tile_lookup[tile_id].atlas_position.x);
                _new_tile_ldtk.src.push_back(_tileset.tile_lookup[tile_id].atlas_position.y);
                _new_tile_ldtk.t = tile_id;
                new_layer.grid_tiles.push_back(_new_tile_ldtk);
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
    

    for(StructurePatch &s_patch : current_tileset.wg_plan.poi_patches) {
    //for(Vector2 structure_pos : current_tileset.wg_plan.structure_positions) {

        int fx = (int)s_patch.center.x;
        int fy = (int)s_patch.center.y;


        Vector2 s_pos = {(float)fx, (float)fy};

        std::vector<std::string>  structure_names;
        std::unordered_map<std::string, WorldGenStructureData> *this_lookup;

        TraceLog(LOG_INFO, "++++++------------NEW STRUCTURE DECCO patch id %s", s_patch.id.c_str());
        if(s_patch.id == "house") {
            this_lookup = &structure_tileset.house_lookup;            
        }
        if(s_patch.id == "spawn") {
            this_lookup = &structure_tileset.spawn_lookup;
            current_tileset.wg_plan.spawn_position = s_pos;
        }
        if(s_patch.id == "ruins") {
            this_lookup = &structure_tileset.ruins_lookup;
        }
        if(s_patch.id == "exit") {
            this_lookup = &structure_tileset.exit_lookup;
        }

        for(auto &structure : *this_lookup) {
            structure_names.push_back(structure.first);
        }

        std::string name_choice = structure_names[GetRandomValue(0, structure_names.size() - 1 )];

        std::string decco_entity = ChooseDeccoEntity((*this_lookup)[name_choice].decco_base, structure_tileset);
        TraceLog(LOG_INFO, "++++++------------NEW STRUCTURE DECCO ENTITY %s", decco_entity.c_str());

        if(decco_entity != "NONE") {
            WorldGenDeccoEntityData new_decco_entity;
            new_decco_entity.decco_name = decco_entity;
            new_decco_entity.position = {s_patch.rect.x + (s_patch.rect.width/2), s_patch.rect.y + (s_patch.rect.height/2)};

            current_tileset.entity_decco_data.push_back(new_decco_entity);
        }

        if((*this_lookup)[name_choice].decco_base != "")

        std::string decco_base = (*this_lookup)[name_choice].decco_base;

        int half_width = (*this_lookup)[name_choice].structure_size.x/2;
        int half_height = (*this_lookup)[name_choice].structure_size.y/2;


        Vector2 corner_offset = {s_pos.x - half_width, s_pos.y - half_height};

        for(WorldGenAutoTile tile : (*this_lookup)[name_choice].structure_grid_tiles) { 
            

            int px = (int)((corner_offset.x * current_tileset.tile_grid_size) + tile.position.x);
            int py = (int)((corner_offset.y * current_tileset.tile_grid_size) + tile.position.y);

            LDTKGridTile new_tile_ldtk;
            new_tile_ldtk.px.push_back( px );
            new_tile_ldtk.px.push_back( py );
            new_tile_ldtk.src.push_back( tile.atlas_position.x);
            new_tile_ldtk.src.push_back(tile.atlas_position.y);
            new_tile_ldtk.t = tile.tile_id;
                
            new_layer.grid_tiles.push_back(new_tile_ldtk);
                
                
            int x = px/current_tileset.tile_grid_size;
            int y = py/current_tileset.tile_grid_size;

            if(x <  1) {x = 1;}
            if(y <  1) {y = 1;}
            if(x >= current_tileset.map_size.x) {x = current_tileset.map_size.x - 1;}
            if(y >= current_tileset.map_size.y) {y = current_tileset.map_size.y - 1;}

            int index = (y * current_tileset.map_size.x + x);

            if(tile.has_collision == true) {
                //TraceLog(LOG_INFO, "structure tile");
                current_tileset.upper_zone_grid[index] = ZONE_STRUCTURE;
                current_tileset.lower_zone_grid[index] = ZONE_DIRT;
                current_tileset.collision_grid[index] = 1;
            }
            else {
                //TraceLog(LOG_INFO, "shadow tile");
                current_tileset.upper_zone_grid[index] = ZONE_NONE;
                current_tileset.lower_zone_grid[index] = ZONE_DIRT;
                current_tileset.collision_grid[index] = 0;
            }



            for(int tile_tag = 0; tile_tag < structure_tileset.tile_tags.size(); tile_tag++) {
                if(structure_tileset.tile_tags[tile_tag].value_string == "HOUSE_TRANSITION") {
                    for(int i = 0; i < structure_tileset.tile_tags[tile_tag].tile_ids.size(); i++) {
                        if(new_tile_ldtk.t == structure_tileset.tile_tags[tile_tag].tile_ids[i]) {
                            //TraceLog(LOG_INFO, "      ---DOOR TILE FOUND---");

                            LDTKEntityInstance new_entity;
                            
                            new_entity.identifier = "HouseTransition";

                            //TraceLog(LOG_INFO, "++++++------------NEW ENTITY %s", new_entity.identifier.c_str());

                            new_entity.iid = "housetransition_" + std::to_string( GetRandomValue(100, 10000));
                            new_entity.px.push_back(new_tile_ldtk.px[0]);
                            new_entity.px.push_back(new_tile_ldtk.px[1]);
                            new_entity.width = current_tileset.tile_grid_size;
                            new_entity.height = current_tileset.tile_grid_size * 2;

                            LDTKFieldInstance dest_map_field;
                            dest_map_field.identifier = "DestMapString";
                            
                            if(name_choice.rfind("EXIT", 0) == 0) {
                                new_entity.identifier = "ShelterTransition";
                                dest_map_field.value_s = "StartingShelter";
                                //TraceLog(LOG_INFO, "++++++--------------------------------ENTITY FIELD %s", dest_map_field.identifier.c_str());
                            }
                            else {
                                //TraceLog(LOG_INFO, "++++++--------- sub map names %i", g_sub_map_string_names.size());
                                for(std::string &map : g_sub_map_string_names) {
                                    //TraceLog(LOG_INFO, "++++++--------- sub map %s", map.c_str());
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
        //TraceLog(LOG_INFO, "++++++---------\n");
    }
     _level.layer_instances.push_back(new_layer);
}
            

void GenerateCollisionLayer(LDTKLevel &_level, WorldGenTileSet &_tileset) {

    for(int y = 0; y < _tileset.map_size.y; y++) {
        for(int x = 0; x < _tileset.map_size.x; x++) {
            int index = y * _tileset.map_size.x + x;
            _tileset.lower_zone_grid[index] = ZONE_GRASS;
            
            if(x == 0 or y == 0 or x == _tileset.map_size.x-1 or y == _tileset.map_size.y-1) {              
                _tileset.upper_zone_grid[index] = ZONE_BORDER;
                _tileset.collision_grid[index] = 1;
            }
        }
    }

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
    spawn_point.px.push_back( _tileset.wg_plan.spawn_position.x * _tileset.tile_grid_size);
    spawn_point.px.push_back( (_tileset.wg_plan.spawn_position.y) * _tileset.tile_grid_size);
    spawn_point.identifier = "SpawnPoint";
    entity_layer->entity_instances.push_back(spawn_point);

    //place exit triggers
    /* for(int exit  = 0; exit < _tileset.wg_plan.exit_positions.size(); exit++) {
        if(exit < _tileset.wg_plan.exit_dest_strings.size()) {

            LDTKEntityInstance new_entity;
            new_entity.identifier = "LevelTransition";
            new_entity.iid = "leveltransition_" + std::to_string( GetRandomValue(100, 10000));
            
            new_entity.px.push_back( (int)_tileset.wg_plan.exit_positions[exit].x * _tileset.tile_grid_size);
            new_entity.px.push_back( (int)_tileset.wg_plan.exit_positions[exit].y * _tileset.tile_grid_size);
            new_entity.width = _tileset.tile_grid_size;
            new_entity.height = _tileset.tile_grid_size * 2;
            LDTKFieldInstance new_field;
            new_field.identifier = "DestMapString";
            new_field.value_s = _tileset.wg_plan.exit_dest_strings[exit];

            TraceLog(LOG_INFO, "++++++------------NEW ENTITY %s", new_field.value_s.c_str());

            new_entity.field_instances.push_back(new_field);

            entity_layer->entity_instances.push_back(new_entity);
            
        }
    } */

}


void PlaceCreatureEntities(LDTKLevel &level, WorldGenTileSet &_tileset) {


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


    for(int index = 0; index < _tileset.spawn_zone_grid.size(); index++) {
        if(_tileset.spawn_zone_grid[index] == ZONE_CREATURE) {
            int x = index%(int)_tileset.map_size.x;
            int y = index/(int)_tileset.map_size.x;

            LDTKEntityInstance new_creature;
            new_creature.identifier = "CreatureEntity";


            new_creature.iid = "creature_" + std::to_string(GetRandomValue(100, 1000000));

            new_creature.px.push_back(x * _tileset.tile_grid_size);
            new_creature.px.push_back(y * _tileset.tile_grid_size);
            new_creature.width = _tileset.tile_grid_size;
            new_creature.height = _tileset.tile_grid_size;
            LDTKFieldInstance new_field;

            new_field.identifier = "CreatureType";
            new_field.value_i = CREATURE_BUNNY;
            new_creature.field_instances.push_back(new_field);
            entity_layer->entity_instances.push_back(new_creature);
            entity_layer->entity_instances.push_back(new_creature);

            TraceLog(LOG_INFO, "ADDING CREATURE DATA");
        }
    }


}



void GenerateTerrainZones(LDTKLevel &level, WorldGenTileSet &_tileset) {
    for(WorldGenBiome &biome : _tileset.wg_plan.biomes) {
        for(int y = biome.rect.y; y < biome.rect.y + biome.rect.height; y++) {
            for(int x = biome.rect.x; x < biome.rect.x + biome.rect.width; x++) {
                int index = y * _tileset.map_size.x + x;

                if (biome.type == BIOME_PLAINS) {
                    _tileset.lower_zone_grid[index] = ZONE_GRASS;
                }
                else if(biome.type == BIOME_FORREST) {
                    _tileset.lower_zone_grid[index] = ZONE_GRASS;
                }
                else if(biome.type == BIOME_HILLS) {
                    _tileset.lower_zone_grid[index] = ZONE_GRASS;
                    //_tileset.upper_zone_grid[index] = ZONE_BORDER;
                }

            }

        }
    }
}



void ShapeHills(LDTKLevel &level, WorldGenTileSet &_tileset) {

    std::vector<Rectangle> hill_rects;

    for(WorldGenBiome &biome : _tileset.wg_plan.biomes) {
        if(biome.type == BIOME_HILLS) {
            hill_rects.push_back(biome.rect);
        }
    }

    


    for(Rectangle &rect : hill_rects) {
        for(int y = rect.y; y < rect.y + rect.height; y++) {
            for(int x = rect.x; x < rect.x + rect.width; x++) {
                int index = y * _tileset.map_size.x + x;
                _tileset.upper_zone_grid[index] = ZONE_BORDER;

            }
        }
    }
}




std::string ChooseDeccoEntity(std::string base_decco_string, WorldGenTileSet &structure_tileset) {
    std::string return_string = "NONE";
    std::vector<std::string> available_choices;

    for(auto &structure : structure_tileset.entity_decco_lookup) {
        TraceLog(LOG_INFO, "structuire field   %s", base_decco_string.c_str());
        if(structure.first.rfind(base_decco_string, 0)== 0) {
            available_choices.push_back(structure.first);
        }
    }
    if(available_choices.size() > 0) {
        return_string = available_choices[GetRandomValue(0, available_choices.size() - 1)];
    }
    return return_string;
}



void GenerateDebugVisuals(LDTKLevel &_level, WorldGenTileSet &_tileset) {
    LDTKLayerInstance new_layer;
    new_layer.identifier = "TerrainDebug";
    new_layer.type = "Tiles";
    new_layer.tileset_def_uid = _tileset.uid;
    new_layer.grid_size = _tileset.tile_grid_size;
    new_layer.c_wid = _tileset.map_size.x;
    new_layer.c_hei = _tileset.map_size.y;
    _level.layer_instances.push_back(new_layer);
}



