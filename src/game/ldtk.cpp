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
                            TraceLog(LOG_INFO, "SHELTER LEVEL FOUND %i", level);
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
            
            for(int layer = 0; layer < mj["levels"][level]["layerInstances"].size(); layer++) {

                LDTKLayerInstance this_layer;

                this_layer.identifier = mj["levels"][level]["layerInstances"][layer]["__identifier"];
                this_layer.type = mj["levels"][level]["layerInstances"][layer]["__type"];

                if(this_layer.type == "IntGrid") {
                    this_layer.grid_size = mj["levels"][level]["layerInstances"][layer]["__gridSize"];
                    this_layer.c_hei = mj["levels"][level]["layerInstances"][layer]["__cHei"];
                    this_layer.c_wid = mj["levels"][level]["layerInstances"][layer]["__cWid"];
                    for (int _i = 0; _i < mj["levels"][level]["layerInstances"][layer]["intGridCsv"].size(); _i++){

                        this_layer.int_grid.push_back( mj["levels"][level]["layerInstances"][layer]["intGridCsv"][_i].get<int>());
                    }
                    TraceLog(LOG_INFO, "+++ void +++COLLISION LAYER ADDED %i %i", this_layer.int_grid.size(),  mj["levels"][level]["layerInstances"][layer]["intGridCsv"].size());
                }

                if(this_layer.type == "Tiles") {

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
                    }

                    TraceLog(LOG_INFO, "++++++TILES ADDED %i", this_layer.grid_tiles.size());
                }

                if(this_layer.type == "Entities") {
                    this_layer.grid_size = mj["levels"][level]["layerInstances"][layer]["__gridSize"];
                    this_layer.c_hei = mj["levels"][level]["layerInstances"][layer]["__cHei"];
                    this_layer.c_wid = mj["levels"][level]["layerInstances"][layer]["__cWid"];
                    
                    for (int entity = 0; entity < mj["levels"][level]["layerInstances"][layer]["entityInstances"].size(); entity++) {
                        LDTKEntityInstance new_entity;

                        new_entity.identifier = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["__identifier"];
                        TraceLog(LOG_INFO, "++++++--------------------------------ENTITY FOUND %s", new_entity.identifier.c_str());

                        new_entity.px.push_back(mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["px"][0]);
                        new_entity.px.push_back(mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["px"][1]);
                        new_entity.width = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["width"];
                        new_entity.height = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["height"];

                        if(new_entity.identifier == "LevelTransition" or new_entity.identifier == "ShelterTransition" or new_entity.identifier == "HouseTransition") {                            
                            
                            for(int _i = 0; _i < mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"].size(); _i++) {
                                LDTKFieldInstance new_field;

                                new_field.identifier = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"][_i]["__identifier"];
                                TraceLog(LOG_INFO, "++++++--------------------------------ENTITY field %s", new_field.identifier.c_str());

                                if(new_field.identifier == "DestMapString" ) {
                                    new_field.value_s = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"][_i]["__value"];
                                    TraceLog(LOG_INFO, "++++++entity dest string %s", new_field.value_s.c_str());
                                }

                                if(new_field.identifier == "ReturnPosition" ) {
                                    new_field.value_v.x = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"][_i]["__value"]["cx"];
                                    new_field.value_v.y = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"][_i]["__value"]["cy"];
                                    TraceLog(LOG_INFO, "++++++entity return position %0.2f %0.2f", new_field.value_v.x, new_field.value_v.y);
                                }
                                new_entity.field_instances.push_back(new_field);
                            }
                        }


                        if(new_entity.identifier == "ContainerEntity") {
                            TraceLog(LOG_INFO, "++++++--------------------------------CONTAINER ENTITY found");
                            for(int _i = 0; _i < mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"].size(); _i++) {
                                LDTKFieldInstance new_field;

                                new_field.identifier = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"][_i]["__identifier"];

                                if(new_field.identifier == "loot_table_id" ) {
                                    new_field.value_i = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"][_i]["__value"];
                                    TraceLog(LOG_INFO, "++++++--------------------------------CONTAINER loot table id %i", new_field.value_i);
                                }
                                if(new_field.identifier == "sprite_id" ) {
                                    new_field.value_i = mj["levels"][level]["layerInstances"][layer]["entityInstances"][entity]["fieldInstances"][_i]["__value"];
                                    TraceLog(LOG_INFO, "++++++--------------------------------CONTAINER sprite id id %i", new_field.value_i);
                                }
                                new_entity.field_instances.push_back(new_field);
                                TraceLog(LOG_INFO, "++++++--------------------------------CONTAINER ENTITY added");
                            }
                        }
                        
                        this_layer.entity_instances.push_back(new_entity);
                        TraceLog(LOG_INFO, "++++++--------------------------------ENTITY ADDED %i", this_layer.entity_instances.size());
                    }
                }
                this_level.layer_instances.push_back(this_layer);
                TraceLog(LOG_INFO, "++++++LAYERS FOUND %i/%i", this_level.layer_instances.size(), mj["levels"][level]["layerInstances"].size());
            }

            g_ldtk_maps.levels.push_back(this_level);
            TraceLog(LOG_INFO, "++++++LEVEL ADDED %i", g_ldtk_maps.levels.size());
        }
    } 
}


int LDTKDrawMap(Vector2 focus_position) {

    int map_index = g_game_data.current_map_index;
    if(g_game_data.is_in_sub_map) {
        map_index = g_game_data.sub_map_index;
    }
    //TraceLog(LOG_INFO, "drawing map index:  %i ", map_index);
    int tiles_drawn = 0;

    //invert layers for drawing
    int tilesheet_id = 0;

    LDTKLevel *this_level = &g_ldtk_maps.levels[map_index];

    for (int l = this_level->layer_instances.size() - 1; l >= 0; l--) {
        if(this_level->layer_instances[l].type == "IntGrid") {
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
                    {(float)tile_pos.x, (float)tile_pos.y,(float)g_viewport.tile_size, (float )g_viewport.tile_size},
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