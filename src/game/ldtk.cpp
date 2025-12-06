#include "../core/gamedefs.h"

void LDTKLoadTileSets(json &mj) {
    if(mj.contains("defs")) {
        TraceLog(LOG_INFO, "++DEFS FOUND");
        if(mj["defs"].contains("tilesets")) {
            TraceLog(LOG_INFO, "+++TILESETS FOUND");
            int num_tilesets = 0;
            for(int i = 0; i < mj["defs"]["tilesets"].size(); i++) {
                num_tilesets++;

                std::filesystem::path pathObj(mj["defs"]["tilesets"][i]["relPath"]);
                std::string texture_path = pathObj.filename().string();


                if(std::filesystem::exists( "assets/maps/ldtk/" + texture_path)) {
                    TileSheetData this_sheet = {
                        .uid = mj["defs"]["tilesets"][i]["uid"],
                        .texture = LoadTexture(("assets/maps/ldtk/" + texture_path).c_str())
                    };

                    g_ldtk_tilesheets[mj["defs"]["tilesets"][i]["uid"]] = this_sheet;
                    
                    TraceLog(LOG_INFO, "==========TILESETS TEXTURE LOADED %s", texture_path.c_str());
                }

                LDTKTileset this_tileset = {
                    .c_wid = mj["defs"]["tilesets"][i]["__cWid"],
                    .uid = mj["defs"]["tilesets"][i]["uid"],
                    .rel_path = mj["defs"]["tilesets"][i]["relPath"],
                    .px_wid = mj["defs"]["tilesets"][i]["pxWid"],
                    .px_hei = mj["defs"]["tilesets"][i]["pxHei"],
                    .tile_grid_size = mj["defs"]["tilesets"][i]["tileGridSize"]  
                    
                };
                for(int tag = 0; tag < mj["defs"]["tilesets"][i]["enumTags"].size(); tag++) {
                    LDTKEnumTag new_tag;
                    new_tag.value_string = mj["defs"]["tilesets"][i]["enumTags"][tag]["enumValueId"];
                    TraceLog(LOG_INFO, "====enum tag           ______________----------- %s", new_tag.value_string.c_str());
                    for(int tid = 0; tid <  mj["defs"]["tilesets"][i]["enumTags"][tag]["tileIds"].size(); tid++) {
                        int id = mj["defs"]["tilesets"][i]["enumTags"][tag]["tileIds"][tid];
                        new_tag.tile_ids.push_back(id);
                        TraceLog(LOG_INFO, "====enum tag    tileid       ______________----------- %i", id);

                    }
                    this_tileset.enum_tags.push_back(new_tag);
                    }
                
                g_ldtk_maps.tilesets.push_back(this_tileset);

            }
            TraceLog(LOG_INFO, "==========TILESETS LOADED %i", g_ldtk_maps.tilesets.size());
        }
    }
}


void LDTKLoadMaps (json &mj) {

    TraceLog(LOG_INFO, "loading maps data");

    g_ldtk_maps.default_grid_size = mj["defaultGridSize"];

    if(mj.contains("levels")) {
        TraceLog(LOG_INFO, "(%i) LEVELS FOUND", mj["levels"].size());
        for(int level = 0; level < mj["levels"].size(); level++) {

            LDTKLevel this_level;

            if(mj["levels"][level].contains("fieldInstances")) {
                for (int field = 0; field < mj["levels"][level]["fieldInstances"].size(); field++) {
                    if(mj["levels"][level]["fieldInstances"][field]["__identifier"] == "is_shelter") {
                        if(mj["levels"][level]["fieldInstances"][field]["__value"] == true) {
                            g_game_data.shelter_map_index = level;
                            TraceLog(LOG_INFO, "SHELTER LEVEL FOUND at index: %i", level);
                            this_level.is_shelter = true;
                        }
                    }
                }
            }
            else {
                TraceLog(LOG_INFO, "(%i) is not SHELTER LEVEL", level);
            }
            
            this_level.identifier = mj["levels"][level]["identifier"];
            
            this_level.bg_color = mj["levels"][level]["__bgColor"];
            this_level.world_x = mj["levels"][level]["worldX"];
            this_level.world_y = mj["levels"][level]["worldY"];
            this_level.uid = mj["levels"][level]["uid"];
            this_level.px_wid = mj["levels"][level]["pxWid"];
            this_level.px_hei = mj["levels"][level]["pxHei"];

            TraceLog(LOG_INFO, "==================================================================");
            TraceLog(LOG_INFO, "                          NEW LEVEL");
            TraceLog(LOG_INFO, "                          %s", this_level.identifier.c_str());
            TraceLog(LOG_INFO, "==================================================================");
            
            for(int layer = 0; layer < mj["levels"][level]["layerInstances"].size(); layer++) {
                TraceLog(LOG_INFO, "                          ---------");
                
                LDTKLayerInstance this_layer;
                
                this_layer.identifier = mj["levels"][level]["layerInstances"][layer]["__identifier"];
                this_layer.type = mj["levels"][level]["layerInstances"][layer]["__type"];
                
                TraceLog(LOG_INFO, "--------------------------NEW LAYER------------------------------");
                TraceLog(LOG_INFO, "--------------------------%s------------------------------", this_layer.identifier.c_str());


                if(this_layer.type == "IntGrid") {
                    TraceLog(LOG_INFO, "++++++------------COLLISION LAYER FOUND %s", this_layer.identifier.c_str());
                    this_layer.grid_size = mj["levels"][level]["layerInstances"][layer]["__gridSize"];
                    this_layer.c_hei = mj["levels"][level]["layerInstances"][layer]["__cHei"];
                    this_layer.c_wid = mj["levels"][level]["layerInstances"][layer]["__cWid"];
                    for (int _i = 0; _i < mj["levels"][level]["layerInstances"][layer]["intGridCsv"].size(); _i++){

                        this_layer.int_grid.push_back( mj["levels"][level]["layerInstances"][layer]["intGridCsv"][_i].get<int>());
                    }
                    TraceLog(LOG_INFO, "++++++--------------------------------COLLISION LAYER ADDED %i", mj["levels"][level]["layerInstances"][layer]["intGridCsv"].size());
                }

                if(this_layer.type == "Tiles") {
                    TraceLog(LOG_INFO, "++++++--------------------------------GRID TILES FOUND %s", this_layer.identifier.c_str());
                    this_layer.grid_size = mj["levels"][level]["layerInstances"][layer]["__gridSize"];
                    this_layer.tileset_def_uid = mj["levels"][level]["layerInstances"][layer]["__tilesetDefUid"];
                    this_layer.c_hei = mj["levels"][level]["layerInstances"][layer]["__cHei"];
                    this_layer.c_wid = mj["levels"][level]["layerInstances"][layer]["__cWid"];

                    int size = mj["levels"][level]["layerInstances"][layer]["gridTiles"].size();
                    for (int i = 0; i < size; i++) {
                        LDTKGridTile empty_tile;
                        empty_tile.t = -1;
                        this_layer.grid_tiles.push_back(empty_tile );
                    }
                    

                    for( int tile = 0; tile < mj["levels"][level]["layerInstances"][layer]["gridTiles"].size(); tile++) {
                        LDTKGridTile this_tile;
                        this_tile.px.push_back(mj["levels"][level]["layerInstances"][layer]["gridTiles"][tile]["px"][0]);
                        this_tile.px.push_back(mj["levels"][level]["layerInstances"][layer]["gridTiles"][tile]["px"][1]);
                        this_tile.src.push_back(mj["levels"][level]["layerInstances"][layer]["gridTiles"][tile]["src"][0]);
                        this_tile.src.push_back(mj["levels"][level]["layerInstances"][layer]["gridTiles"][tile]["src"][1]);
                        this_tile.f = mj["levels"][level]["layerInstances"][layer]["gridTiles"][tile]["f"];
                        this_tile.a = mj["levels"][level]["layerInstances"][layer]["gridTiles"][tile]["a"];
                        this_tile.t = mj["levels"][level]["layerInstances"][layer]["gridTiles"][tile]["t"];
                        this_tile.d.push_back(mj["levels"][level]["layerInstances"][layer]["gridTiles"][tile]["d"][0]);
                        this_layer.grid_tiles[tile] = this_tile;

                        for(int ts = 0; ts < g_ldtk_maps.tilesets.size(); ts++) {
                            if(g_ldtk_maps.tilesets[ts].uid == this_layer.tileset_def_uid) {
                                for(int tag = 0; tag < g_ldtk_maps.tilesets[ts].enum_tags.size(); tag++) {
                                    std::string value_s = g_ldtk_maps.tilesets[ts].enum_tags[tag].value_string;
                                    //TraceLog(LOG_INFO, "          |||||||enum %s||||||", value_s.c_str());

                                    for(int tile = 0; tile < g_ldtk_maps.tilesets[ts].enum_tags[tag].tile_ids.size(); tile++) {
                                        if(this_tile.t == g_ldtk_maps.tilesets[ts].enum_tags[tag].tile_ids[tile]) {
                                            //TraceLog(LOG_INFO, "          |||||||tile id: %i has enum %s||||||", this_tile.t, value_s.c_str());
                                            LDTKEnvironmentData new_data;
                                            new_data.item_string = value_s;
                                            new_data.position = {(float)this_tile.px[0] + (g_ldtk_maps.default_grid_size/2), (float)this_tile.px[1] + (g_ldtk_maps.default_grid_size)};
                                            this_level.environment_data.push_back(new_data);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    TraceLog(LOG_INFO, "      |||||||# of enviro entities: %i", this_level.environment_data.size());
                    //TraceLog(LOG_INFO, "++++++--------------------------------GRID TILES ADDED %i", this_layer.grid_tiles.size());
                }

                if(this_layer.type == "Entities") {
                    this_layer.grid_size = mj["levels"][level]["layerInstances"][layer]["__gridSize"];
                    this_layer.c_hei = mj["levels"][level]["layerInstances"][layer]["__cHei"];
                    this_layer.c_wid = mj["levels"][level]["layerInstances"][layer]["__cWid"];
                    
                    for (int entity = 0; entity < mj["levels"][level]["layerInstances"][layer]["entityInstances"].size(); entity++) {
                        LDTKEntityInstance new_entity;

                        new_entity.identifier = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["__identifier"];
                        TraceLog(LOG_INFO, "++++++------------NEW ENTITY FOUND %s", new_entity.identifier.c_str());

                        new_entity.iid = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["iid"];
                        new_entity.px.push_back(mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["px"][0]);
                        new_entity.px.push_back(mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["px"][1]);
                        new_entity.width = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["width"];
                        new_entity.height = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["height"];

                        TraceLog(LOG_INFO, "++++++---------------------------ENTITY UID %s ", new_entity.iid.c_str());
                        //TraceLog(LOG_INFO, "++++++---------------------------ENTITY SIZE %i x %i", new_entity.width, new_entity.height);

                        if(new_entity.identifier == "LevelTransition" or new_entity.identifier == "ShelterTransition" or new_entity.identifier == "HouseTransition") {                            
                            
                            for(int _i = 0; _i < mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"].size(); _i++) {
                                LDTKFieldInstance new_field;

                                new_field.identifier = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"][_i]["__identifier"];
                                TraceLog(LOG_INFO, "++++++--------------------------------ENTITY FIELD %s", new_field.identifier.c_str());

                                if(new_field.identifier == "DestMapString" ) {
                                    new_field.value_s = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"][_i]["__value"];
                                    TraceLog(LOG_INFO, "++++++--------------------------------transition dest string %s", new_field.value_s.c_str());
                                }

                                if(new_field.identifier == "ReturnPosition" ) {
                                    new_field.value_v.x = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"][_i]["__value"]["cx"];
                                    new_field.value_v.y = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"][_i]["__value"]["cy"];
                                    //TraceLog(LOG_INFO, "++++++--------------------------------transition return position %0.2f %0.2f", new_field.value_v.x, new_field.value_v.y);
                                }
                                new_entity.field_instances.push_back(new_field);
                            }
                        }


                        if(new_entity.identifier == "PermContainerEntity") {
                            //new_entity.iid = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["iid"];
                            //TraceLog(LOG_INFO, "++++++--------------------------------PERM CONTAINER ENTITY found %s", new_entity.iid.c_str());
                            for(int _i = 0; _i < mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"].size(); _i++) {
                                LDTKFieldInstance new_field;

                                new_field.identifier = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"][_i]["__identifier"];
                                //TraceLog(LOG_INFO, "++++++--------------------------------ENTITY FIELD %s", new_field.identifier.c_str());
                                if(new_field.identifier == "loot_table_id" ) {
                                    new_field.value_i = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"][_i]["__value"];
                                    //TraceLog(LOG_INFO, "++++++--------------------------------loot table id %i", new_field.value_i);
                                }
                                if(new_field.identifier == "sprite_id" ) {
                                    new_field.value_i = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"][_i]["__value"];
                                    //TraceLog(LOG_INFO, "++++++--------------------------------sprite id id %i", new_field.value_i);
                                }
                                

                                new_entity.field_instances.push_back(new_field);
                            }
                        }

                        if(new_entity.identifier == "GroundContainerEntity") {
                            //new_entity.iid = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["iid"];
                            //TraceLog(LOG_INFO, "++++++----------------------GROUND CONTAINER ENTITY found %s", new_entity.iid.c_str());
                            for(int _i = 0; _i < mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"].size(); _i++) {
                                LDTKFieldInstance new_field;

                                new_field.identifier = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"][_i]["__identifier"];
                                //TraceLog(LOG_INFO, "++++++--------------------------------ENTITY FIELD %s", new_field.identifier.c_str());
                                if(new_field.identifier == "item_list" ) {
                                    for(int j = 0; j < mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"][_i]["__value"].size(); j++) {
                                        new_field.i_list.push_back(mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"][_i]["__value"][j].get<int>());
                                        //TraceLog(LOG_INFO, "++++++--------------------------------item added %i", mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"][_i]["__value"][j].get<int>());
                                    }
                                }
                                
                                new_entity.field_instances.push_back(new_field);
                                //TraceLog(LOG_INFO, "++++++--------------------------------GROUND CONTAINER ENTITY FIELD added");
                            }

                        }

                        if(new_entity.identifier == "ModuleEntity") {
                            TraceLog(LOG_INFO, "++++++----------------------MODULE ENTITY found %s", new_entity.iid.c_str());
                            for(int _i = 0; _i < mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"].size(); _i++) {
                                LDTKFieldInstance new_field;

                                new_field.identifier = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"][_i]["__identifier"];
                                if(new_field.identifier == "module_id" ) {
                                    new_field.value_i = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"][_i]["__value"];   
                                    TraceLog(LOG_INFO, "++++++--------------------------------ENTITY FIELD %s  value: %i", new_field.identifier.c_str(), new_field.value_i);
                                }
                                new_entity.field_instances.push_back(new_field);
                                TraceLog(LOG_INFO, "++++++--------------------------------MODULE ENTITY FIELD added");
                            }
                        }

                        if(new_entity.identifier == "MushroomZone") {
                            //TraceLog(LOG_INFO, "++++++----------------------MUSHROOM ZONE FOUND");
                            LDTKFieldInstance new_field;

                            new_field.identifier = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"][0]["__identifier"];
                            //TraceLog(LOG_INFO, "++++++--------------------------------ENTITY FIELD %s", new_field.identifier.c_str());
                            new_field.value_i = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"][0]["__value"];
                            //TraceLog(LOG_INFO, "++++++--------------------------------max mushrooms %i", new_field.value_i);

                            new_entity.field_instances.push_back(new_field);
                        }
                        
                        
                        this_layer.entity_instances.push_back(new_entity);
                        TraceLog(LOG_INFO, "++++++--------------------------------ENTITY ADDED %i", this_layer.entity_instances.size());
                    }
                }
                this_level.layer_instances.push_back(this_layer);
                TraceLog(LOG_INFO, "                    ++++++LAYER ADDED: %i ++++++", g_ldtk_maps.levels.size());
                TraceLog(LOG_INFO, "                          LAYERS: %i/%i\n\n", this_level.layer_instances.size(), mj["levels"][level]["layerInstances"].size());
            }

            g_ldtk_maps.levels.push_back(this_level);
            TraceLog(LOG_INFO, "                    ++++++LEVEL ADDED: %i ++++++\n\n", g_ldtk_maps.levels.size());
        }
    }
}


int LDTKDrawMap(Vector2 focus_position) {

    //int map_index = -1;

    int tiles_drawn = 0;

    //invert layers for drawing
    int tilesheet_id = 0;

    LevelData *level_data = nullptr;

    if(g_game_data.is_in_sub_map) {
        level_data = &g_sub_scene->level_data;
    }
    else {
        level_data = &g_current_scene->level_data;
    }

    if(level_data == nullptr) {
        return -1;
    }

    LDTKLevel *this_level = &g_ldtk_maps.levels[level_data->precalc.map_index];

    for (int l = this_level->layer_instances.size() - 1; l >= 0; l--) {
        if(this_level->layer_instances[l].type == "IntGrid") {
            break;
        }
        if(level_data->precalc.foreground_layer_index == l) {
            break;
        }
        tilesheet_id = this_level->layer_instances[l].tileset_def_uid;
        for(int tile = 0; tile < this_level->layer_instances[l].grid_tiles.size(); tile++) {

            LDTKGridTile *this_tile = &this_level->layer_instances[l].grid_tiles[tile];


            int tile_id = this_tile->t;

            int tile_x = this_tile->px[0] * g_viewport.inv_tile_size;
            int tile_y = this_tile->px[1] * g_viewport.inv_tile_size;

            if(tile_id == -1) {
                TraceLog(LOG_INFO, "GRID TILES||| is not valid tile.... x:  %i  y %i", tile_x, tile_y);
                break;
            }

            if((tile_x >= g_viewport.x_min) and (tile_x <= g_viewport.x_max) and (tile_y >= g_viewport.y_min) and (tile_y <= g_viewport.y_max)) {

                Vector2 tile_pos = {(float)this_tile->px[0], (float)this_tile->px[1]};
                Vector2 atlas_pos = {(float)this_tile->src[0], (float)this_tile->src[1]};

                Color color = WHITE;

                DrawTexturePro(
                    g_ldtk_tilesheets[tilesheet_id].texture,
                    {atlas_pos.x, atlas_pos.y, (float)g_viewport.tile_size, (float)g_viewport.tile_size},
                    {(float)tile_pos.x, (float)tile_pos.y,(float)g_viewport.tile_size+.1f, (float )g_viewport.tile_size +.1f},
                    {0,0},
                    0.0,
                    color
                );
                tiles_drawn++;
            }
        }
    }
    return tiles_drawn;
}



void LDTKDrawShadows(Vector2 focus_position) {

    LevelData *level_data = nullptr;

    if(g_game_data.is_in_sub_map) {
        level_data = &g_sub_scene->level_data;
    }
    else {
        level_data = &g_current_scene->level_data;
    }

    if(level_data == nullptr) {
        return;
    }

    LDTKLevel &this_level = g_ldtk_maps.levels[level_data->precalc.map_index];

    if(level_data->precalc.collision_layer_index == -1) {
        return;
    }
    LDTKLayerInstance &col_layer = this_level.layer_instances[level_data->precalc.collision_layer_index];

    int tile_size = level_data->precalc.tile_size;
    float inv_tile_size = level_data->precalc.inv_tile_size;
    int map_width = level_data->precalc.map_width;


    if(g_game_settings.show_debug) { //Draw Polys

        for(auto &poly : level_data->collision_polys) {

            int p_n = poly.points.size();
            if(p_n >= 3) {
                for(int i = 1; i < p_n - 1; ++i) {
                    DrawTriangle(
                        poly.points[0],
                        poly.points[i+1],
                        poly.points[i],
                        DARKRED
                    );
                } 
            }
        }
    }

    float extrudeDist = 1500.0f;
    Color shadowColor = {0, 0, 0, 5};
    //Color shadowColor = DARKERGRAY;
    for (auto &poly : level_data->collision_polys) {
        int p_n = poly.points.size();
         if(p_n > 2) {

            for (int i = 0; i < p_n; ++i) {
                 Vector2 v1 = poly.points[i];
                Vector2 v2 = poly.points[(i+1) % p_n];

                Vector2 edge = v2 - v1;

                Vector2 normal = { -edge.y, edge.x };


                float facing = Vector2DotProduct( normal, v1 - focus_position );

                if (facing > 0.0f) {
                    //TraceLog(LOG_INFO, "drawing triangle.... ");
                    Vector2 dir1 = Vector2Normalize(v1 - focus_position);
                    Vector2 dir2 = Vector2Normalize(v2 - focus_position);
                    Vector2 v1_ext = v1 + dir1 * extrudeDist;
                    Vector2 v2_ext = v2 + dir2 * extrudeDist;

                    if(g_game_settings.show_debug) {

                        DrawLineV(v1, v2, RED);
                        DrawLineV(v1, v1_ext, shadowColor);
                        DrawLineV(v2, v2_ext, shadowColor);
                    }

                    DrawTriangle({v1.x, v1.y}, {v2_ext.x, v2_ext.y}, {v2.x, v2.y}, shadowColor);
                    DrawTriangle({v1.x, v1.y}, {v1_ext.x, v1_ext.y}, {v2_ext.x, v2_ext.y}, shadowColor); 


                }

            }
        }
    }
}


