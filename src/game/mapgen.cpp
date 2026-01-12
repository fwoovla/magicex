#include "../core/gamedefs.h"


void GenerateWorldGenTilesets(std::string _path) {

    TraceLog(LOG_INFO, "++++++++++++++++++++++++++++++++ LOADING MAPGEN DATA....  (%s) ++++++++++++++++++++++++++\n", _path.c_str());

    std::ifstream mfile(_path);
    if (!mfile.is_open()) {
        TraceLog(LOG_INFO, "-CANNOT OPEN FILE");
        return;
    }
    
    json j;
    mfile>>j;
    TraceLog(LOG_INFO, "+FOUND MAP FILE");

    if(j.contains("defs")) {

        if(j["defs"].contains("tilesets")) {
            TraceLog(LOG_INFO, "+++TILESETS FOUND");
            int num_tilesets = 0;

            for(int i = 0; i < j["defs"]["tilesets"].size(); i++) {
                num_tilesets++;

                //int uid = GetRandomValue(100, 1000);
                int uid = j["defs"]["tilesets"][i]["uid"];

                std::filesystem::path pathObj(j["defs"]["tilesets"][i]["relPath"]);
                std::string texture_path = pathObj.filename().string();

                if(std::filesystem::exists( "assets/maps/ldtk/" + texture_path)) {
                    TileSheetData this_sheet = {
                        .uid = uid,
                        .texture = LoadTexture(("assets/maps/ldtk/" + texture_path).c_str())
                    };

                    g_ldtk_tilesheets[this_sheet.uid] = this_sheet;
                    
                    TraceLog(LOG_INFO, "==========TILESETS TEXTURE LOADED %s tileset uid %i", texture_path.c_str(), uid);

                    WorldGenTileSet this_tileset;

                    this_tileset.c_wid = j["defs"]["tilesets"][i]["__cWid"];
                    this_tileset.uid = uid;
                    this_tileset.rel_path = j["defs"]["tilesets"][i]["relPath"];
                    this_tileset.px_wid = j["defs"]["tilesets"][i]["pxWid"];
                    this_tileset.px_hei = j["defs"]["tilesets"][i]["pxHei"];
                    this_tileset.tile_grid_size = j["defs"]["tilesets"][i]["tileGridSize"] ;

                    for(int tag = 0; tag < j["defs"]["tilesets"][i]["enumTags"].size(); tag++) {
                        LDTKEnumTag new_tag;
                        new_tag.value_string = j["defs"]["tilesets"][i]["enumTags"][tag]["enumValueId"];
                        TraceLog(LOG_INFO, "====enum tag           _____________ %s", new_tag.value_string.c_str());
                        for(int tid = 0; tid <  j["defs"]["tilesets"][i]["enumTags"][tag]["tileIds"].size(); tid++) {
                            int id = j["defs"]["tilesets"][i]["enumTags"][tag]["tileIds"][tid];
                            new_tag.tile_ids.push_back(id);
                            TraceLog(LOG_INFO, "====tile id         _____________ %i", id);
                        }
                        this_tileset.tile_tags.push_back(new_tag);
                        TraceLog(LOG_INFO, "====tiles tags loaded        _____________ %i", this_tileset.tile_tags.size());
                    }

                    g_worldgen_tilesets.push_back(this_tileset);

                    for(int set = 0; set< g_worldgen_tilesets.size(); set++) {
                        TraceLog(LOG_INFO, "set # %i", set);
                        TraceLog(LOG_INFO, "-----uid %i", g_worldgen_tilesets[set].uid);
                        TraceLog(LOG_INFO, "-----tile tag size %i", g_worldgen_tilesets[set].tile_tags.size());
                        for(int tag = 0; tag < g_worldgen_tilesets[set].tile_tags.size(); tag++) {
                            TraceLog(LOG_INFO, "------------id %i", g_worldgen_tilesets[set].tile_tags[tag].tile_ids[0]);
                        }
                    }
                }
            }
        }
    }


    if(j.contains("levels")) {
        TraceLog(LOG_INFO, "(%i) LEVELS FOUND", j["levels"].size());
        for(int level = 0; level < j["levels"].size(); level++) {
            std::string level_name = j["levels"][level]["identifier"];
            TraceLog(LOG_INFO, "LEVEL FOUND %s", level_name.c_str());
            

            for(int layer = 0; layer < j["levels"][level]["layerInstances"].size(); layer++) {
                TraceLog(LOG_INFO, "                          ---------");
                
                LDTKLayerInstance this_layer;
                
                this_layer.identifier = j["levels"][level]["layerInstances"][layer]["__identifier"];
                this_layer.type = j["levels"][level]["layerInstances"][layer]["__type"];
                
                TraceLog(LOG_INFO, "--------------------------NEW LAYER------------------------------");
                TraceLog(LOG_INFO, "--------------------------%s------------------------------", this_layer.identifier.c_str());

                if(this_layer.type == "Tiles") {
                    this_layer.grid_size = j["levels"][level]["layerInstances"][layer]["__gridSize"];
                    this_layer.tileset_def_uid = j["levels"][level]["layerInstances"][layer]["__tilesetDefUid"];
                    this_layer.c_hei = j["levels"][level]["layerInstances"][layer]["__cHei"];
                    this_layer.c_wid = j["levels"][level]["layerInstances"][layer]["__cWid"];
                    TraceLog(LOG_INFO, "++++++--------------------------------GRID TILES FOUND %s tileset uid %i", this_layer.identifier.c_str(), this_layer.tileset_def_uid);
                    
                    WorldGenTileSet *this_tileset = nullptr;
                    
                    for(int ts = 0; ts < g_worldgen_tilesets.size(); ts++) {
                        if(g_worldgen_tilesets[ts].uid == this_layer.tileset_def_uid) {
                            this_tileset = &g_worldgen_tilesets[ts];
                            TraceLog(LOG_INFO, "++++++----tileset %i has %i tile tags", g_worldgen_tilesets[ts].uid, g_worldgen_tilesets[ts].tile_tags.size());
                        }
                    }

                    if(this_tileset != nullptr) {
                        //this_tileset->tile_lookup.resize(j["levels"][level]["layerInstances"][layer]["gridTiles"].size(), {-1, -1});
    
                        for( int tile = 0; tile < j["levels"][level]["layerInstances"][layer]["gridTiles"].size(); tile++) {
                            Vector2 atlas_pos;
                            atlas_pos.x = j["levels"][level]["layerInstances"][layer]["gridTiles"][tile]["src"][0];
                            atlas_pos.y = j["levels"][level]["layerInstances"][layer]["gridTiles"][tile]["src"][1];
                        
                            int tile_id = j["levels"][level]["layerInstances"][layer]["gridTiles"][tile]["t"];
    
                            //TraceLog(LOG_INFO, "tile id  %i  %i", tile_id, this_tileset->tile_tags.size());
                            
                            for(int tile_tag = 0; tile_tag < this_tileset->tile_tags.size(); tile_tag++) {
                                //TraceLog(LOG_INFO, "looking for tile id  %i  ?  %i", tile_id, this_tileset->tile_tags[tile_tag].tile_ids[0]);
                                if(tile_id == this_tileset->tile_tags[tile_tag].tile_ids[0]) {
                                    this_tileset->tile_lookup[tile_id] = atlas_pos;
                                    TraceLog(LOG_INFO, "tile found");
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    TraceLog(LOG_INFO, "---------------WORLD GEN TILES LOADED -------------");
    for(int set = 0; set< g_worldgen_tilesets.size(); set++) {
        TraceLog(LOG_INFO, "set # %i", set);
        TraceLog(LOG_INFO, "-----uid %i", g_worldgen_tilesets[set].uid);
        TraceLog(LOG_INFO, "-----loaded tiles %i", g_worldgen_tilesets[set].tile_lookup.size());
        for(int tile = 0; tile < g_worldgen_tilesets[set].tile_lookup.size(); tile++) {
            if(g_worldgen_tilesets[set].tile_lookup[tile] != Vector2{-1, -1}) {
                TraceLog(LOG_INFO, "------------tile %i %0.0f  %0.0f", tile, g_worldgen_tilesets[set].tile_lookup[tile].x, g_worldgen_tilesets[set].tile_lookup[tile].y);
            }
        }
    }

}

void GenerateMap(LDTKLevel &new_level, int tileset_uid) {

    WorldGenTileSet *this_tilset = nullptr;

    for(int ts = 0; ts < g_worldgen_tilesets.size(); ts++) {
        if(g_worldgen_tilesets[ts].uid == tileset_uid) {
            this_tilset = &g_worldgen_tilesets[ts];
        }
    }

    if(this_tilset == nullptr) {
        TraceLog(LOG_INFO, "could not find tileset %i", tileset_uid);
    }

    Vector2 map_size = {10, 10};   
    
    new_level.identifier = "WORLD GEN LEVEL";
    new_level.px_wid = map_size.x * this_tilset->tile_grid_size;
    new_level.px_hei = map_size.y * this_tilset->tile_grid_size;

    LDTKLayerInstance terrain_lower_ldtk;
    terrain_lower_ldtk.identifier = "TerrainLower";
    terrain_lower_ldtk.type = "Tiles";
    terrain_lower_ldtk.tileset_def_uid = this_tilset->uid;
    terrain_lower_ldtk.grid_size = this_tilset->tile_grid_size;
    terrain_lower_ldtk.c_wid = map_size.x;
    terrain_lower_ldtk.c_hei = map_size.y;
    
    for(int x = 0; x < map_size.x; x++) {
        for(int y = 0; y < map_size.y; y++) {
            
            LDTKGridTile new_tile_ldtk;
            
            new_tile_ldtk.px.push_back((float)x*this_tilset->tile_grid_size);//x*this_tilset->tile_grid_size;
            new_tile_ldtk.px.push_back((float)y*this_tilset->tile_grid_size);//y*this_tilset->tile_grid_size;
            
            
            int tile_id = TILE_ID_GRASS_1;//GetRandomValue(0, TILE_ID_MAX -1);
            //TraceLog(LOG_INFO, "tile id  %i   lookup %0.0f %0.0f", tile_id, x, y);
            
            new_tile_ldtk.src.push_back( this_tilset->tile_lookup[tile_id].x);
            new_tile_ldtk.src.push_back(this_tilset->tile_lookup[tile_id].y);
            new_tile_ldtk.t = tile_id;
            
            terrain_lower_ldtk.grid_tiles.push_back(new_tile_ldtk);

            TraceLog(LOG_INFO, "making tile %i %i    src  %i %i", new_tile_ldtk.px[0], new_tile_ldtk.px[1], new_tile_ldtk.src[0], new_tile_ldtk.src[1]);
        }
    }
            
    new_level.layer_instances.push_back(terrain_lower_ldtk);
            
    LDTKLayerInstance eneities_ldtk;

    eneities_ldtk.identifier = "Entities";
    eneities_ldtk.type = "Entities";
    eneities_ldtk.tileset_def_uid = -1;
    eneities_ldtk.grid_size = this_tilset->tile_grid_size;
    eneities_ldtk.c_wid = map_size.x;
    eneities_ldtk.c_hei = map_size.y;

    LDTKEntityInstance spawn_point;
    spawn_point.px.push_back(5*this_tilset->tile_grid_size);
    spawn_point.px.push_back(5*this_tilset->tile_grid_size);
    spawn_point.identifier = "SpawnPoint";
    eneities_ldtk.entity_instances.push_back(spawn_point);


    new_level.layer_instances.push_back(eneities_ldtk);


    LDTKLayerInstance collision_ldtk;
    
    collision_ldtk.identifier = "Collision";
    collision_ldtk.type = "IntGrid";
    collision_ldtk.tileset_def_uid = -1;
    collision_ldtk.grid_size = this_tilset->tile_grid_size;
    collision_ldtk.c_wid = map_size.x;
    collision_ldtk.c_hei = map_size.y;
    collision_ldtk.int_grid.resize(map_size.x*map_size.y, -1);
    
    new_level.layer_instances.push_back(collision_ldtk);
}