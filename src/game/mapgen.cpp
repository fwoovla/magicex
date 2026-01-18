#include "../core/gamedefs.h"
#include "../core/layergen.h"
#include "../core/pathgen.h"

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
                    this_tileset.c_hei = j["defs"]["tilesets"][i]["__cHei"];
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

                }
            }
        }
        for(int set = 0; set< g_worldgen_tilesets.size(); set++) {
            TraceLog(LOG_INFO, "set # %i", set);
            TraceLog(LOG_INFO, "-----uid %i", g_worldgen_tilesets[set].uid);
            TraceLog(LOG_INFO, "-----tile tag size %i", g_worldgen_tilesets[set].tile_tags.size());
            for(int tag = 0; tag < g_worldgen_tilesets[set].tile_tags.size(); tag++) {
                for(int id = 0; id < g_worldgen_tilesets[set].tile_tags[tag].tile_ids.size(); id++) {
                    TraceLog(LOG_INFO, "------------id %i %i", tag, g_worldgen_tilesets[set].tile_tags[tag].tile_ids[id]);
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
                                if(this_tileset->tile_tags[tile_tag].tile_ids.size() > 0) {

                                    //TraceLog(LOG_INFO, "looking for tile id  %i  ?  %i", tile_id, this_tileset->tile_tags[tile_tag].tile_ids[0]);
                                    if(tile_id == this_tileset->tile_tags[tile_tag].tile_ids[0]) {
                                        int alatered_id = StrToTileId(this_tileset->tile_tags[tile_tag].value_string);
                                        WorldGenAutoTile new_tile;
                                        new_tile.atlas_position = atlas_pos;
                                        new_tile.position = {0,0};
                                        new_tile.tile_id = alatered_id;
                                        new_tile.marked_sides[TILESIDE_UP] = false;
                                        new_tile.marked_sides[TILESIDE_RIGHT] = false;
                                        new_tile.marked_sides[TILESIDE_DOWN] = false;
                                        new_tile.marked_sides[TILESIDE_LEFT] = false;
                                        new_tile.marked_sides[TILESIDE_UPPER_RIGHT] = false;
                                        new_tile.marked_sides[TILESIDE_UPPER_LEFT] = false;
                                        new_tile.marked_sides[TILESIDE_LOWER_RIGHT] = false;
                                        new_tile.marked_sides[TILESIDE_LOWER_LEFT] = false;


                                        if(alatered_id > TILE_ID_BORDER_START and alatered_id < TILE_ID_BORDER_END) {
                                           if(alatered_id == TILE_ID_DIRT_SINGLE) {
                                                new_tile.marked_sides[TILESIDE_UP] = 0;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 0;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 0;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 0;
                                            }
                                            if(alatered_id == TILE_ID_BORDER_MID) {
                                                new_tile.marked_sides[TILESIDE_UP] = 1;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 1;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 1;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 1;
                                            }
                                            if(alatered_id == TILE_ID_BORDER_RIGHT_DOWN) {
                                                new_tile.marked_sides[TILESIDE_UP] = 0;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 1;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 1;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 0;
                                            }
                                            if(alatered_id == TILE_ID_BORDER_RIGHT_DOWN_LEFT) {
                                                new_tile.marked_sides[TILESIDE_UP] = 0;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 1;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 1;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 1;
                                            }
                                            if(alatered_id == TILE_ID_BORDER_DOWN_LEFT) {
                                                new_tile.marked_sides[TILESIDE_UP] = 0;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 0;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 1;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 1;
                                            }
                                            if(alatered_id == TILE_ID_BORDER_UP_DOWN_LEFT) {
                                                new_tile.marked_sides[TILESIDE_UP] = 1;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 0;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 1;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 1;
                                            }
                                            if(alatered_id == TILE_ID_BORDER_UP_LEFT) {
                                                new_tile.marked_sides[TILESIDE_UP] = 1;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 0;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 0;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 1;
                                            }
                                            if(alatered_id == TILE_ID_BORDER_UP_RIGHT_LEFT) {
                                                new_tile.marked_sides[TILESIDE_UP] = 1;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 1;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 0;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 1;
                                            }
                                            if(alatered_id == TILE_ID_BORDER_RIGHT_UP) {
                                                new_tile.marked_sides[TILESIDE_UP] = 1;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 1;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 0;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 0;
                                            }
                                            if(alatered_id == TILE_ID_BORDER_UP_RIGHT_DOWN) {
                                                new_tile.marked_sides[TILESIDE_UP] = 1;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 1;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 1;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 0;
                                            }
                                        }

                                        if(alatered_id > TILE_ID_DIRT_START and alatered_id < TILE_ID_DIRT_END) {
                                           if(alatered_id == TILE_ID_DIRT_SINGLE) {
                                                new_tile.marked_sides[TILESIDE_UP] = 0;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 0;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 0;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 0;
                                            }
                                            if(alatered_id == TILE_ID_DIRT_MID) {
                                                new_tile.marked_sides[TILESIDE_UP] = 1;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 1;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 1;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 1;
                                            }
                                            if(alatered_id == TILE_ID_DIRT_RIGHT_DOWN) {
                                                new_tile.marked_sides[TILESIDE_UP] = 0;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 1;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 1;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 0;
                                            }
                                            if(alatered_id == TILE_ID_DIRT_RIGHT_DOWN_LEFT) {
                                                new_tile.marked_sides[TILESIDE_UP] = 0;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 1;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 1;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 1;
                                            }
                                            if(alatered_id == TILE_ID_DIRT_DOWN_LEFT) {
                                                new_tile.marked_sides[TILESIDE_UP] = 0;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 0;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 1;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 1;
                                            }
                                            if(alatered_id == TILE_ID_DIRT_UP_DOWN_LEFT) {
                                                new_tile.marked_sides[TILESIDE_UP] = 1;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 0;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 1;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 1;
                                            }
                                            if(alatered_id == TILE_ID_DIRT_UP_LEFT) {
                                                new_tile.marked_sides[TILESIDE_UP] = 1;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 0;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 0;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 1;
                                            }
                                            if(alatered_id == TILE_ID_DIRT_UP_RIGHT_LEFT) {
                                                new_tile.marked_sides[TILESIDE_UP] = 1;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 1;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 0;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 1;
                                            }
                                            if(alatered_id == TILE_ID_DIRT_RIGHT_UP) {
                                                new_tile.marked_sides[TILESIDE_UP] = 1;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 1;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 0;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 0;
                                            }
                                            if(alatered_id == TILE_ID_DIRT_UP_RIGHT_DOWN) {
                                                new_tile.marked_sides[TILESIDE_UP] = 1;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 1;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 1;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 0;
                                            }
                                        }

                                        if(alatered_id > TILE_ID_PATH_START and alatered_id < TILE_ID_PATH_END) {
                                            if(alatered_id == TILE_ID_PATH_MID) {
                                                new_tile.marked_sides[TILESIDE_UP] = 1;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 1;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 1;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 1;
                                            }
                                            if(alatered_id == TILE_ID_PATH_END_UP) {
                                                new_tile.marked_sides[TILESIDE_UP] = 0;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 0;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 1;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 0;
                                            }
                                            if(alatered_id == TILE_ID_PATH_END_RIGHT) {
                                                new_tile.marked_sides[TILESIDE_UP] = 0;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 0;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 0;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 1;
                                            }
                                            if(alatered_id == TILE_ID_PATH_END_DOWN) {
                                                new_tile.marked_sides[TILESIDE_UP] = 1;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 0;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 0;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 0;
                                            }
                                            if(alatered_id == TILE_ID_PATH_END_LEFT) {
                                                new_tile.marked_sides[TILESIDE_UP] = 0;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 1;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 0;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 0;
                                            }
                                            if(alatered_id == TILE_ID_PATH_SINGLE) {
                                                new_tile.marked_sides[TILESIDE_UP] = 0;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 0;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 0;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 0;
                                            }
                                            if(alatered_id == TILE_ID_PATH_UP_RIGHT) {
                                                new_tile.marked_sides[TILESIDE_UP] = 1;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 1;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 0;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 0;
                                            }
                                            if(alatered_id == TILE_ID_PATH_UP_DOWN) {
                                                new_tile.marked_sides[TILESIDE_UP] = 1;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 0;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 1;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 0;
                                            }
                                            if(alatered_id == TILE_ID_PATH_UP_LEFT) {
                                                new_tile.marked_sides[TILESIDE_UP] = 1;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 0;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 0;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 1;
                                            }
                                            if(alatered_id == TILE_ID_PATH_UP_RIGHT_DOWN) {
                                                new_tile.marked_sides[TILESIDE_UP] = 1;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 1;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 1;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 0;
                                            }
                                            if(alatered_id == TILE_ID_PATH_UP_DOWN_LEFT) {
                                                new_tile.marked_sides[TILESIDE_UP] = 1;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 0;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 1;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 1;
                                            }
                                            if(alatered_id == TILE_ID_PATH_RIGHT_LEFT) {
                                                new_tile.marked_sides[TILESIDE_UP] = 0;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 1;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 0;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 1;
                                            }
                                            if(alatered_id == TILE_ID_PATH_UP_RIGHT_LEFT) {
                                                new_tile.marked_sides[TILESIDE_UP] = 1;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 1;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 0;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 1;
                                            }
                                            if(alatered_id == TILE_ID_PATH_RIGHT_DOWN_LEFT) {
                                                new_tile.marked_sides[TILESIDE_UP] = 0;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 1;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 1;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 1;
                                            }
                                            if(alatered_id == TILE_ID_PATH_DOWN_LEFT) {
                                                new_tile.marked_sides[TILESIDE_UP] = 0;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 0;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 1;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 1;
                                            }
                                            if(alatered_id == TILE_ID_PATH_RIGHT_DOWN) {
                                                new_tile.marked_sides[TILESIDE_UP] = 0;
                                                new_tile.marked_sides[TILESIDE_RIGHT] = 1;
                                                new_tile.marked_sides[TILESIDE_DOWN] = 1;
                                                new_tile.marked_sides[TILESIDE_LEFT] = 0;
                                            }
                                        }
                                        
                                        this_tileset->tile_lookup[(TILEID)alatered_id] = new_tile;
                                        TraceLog(LOG_INFO, "tile found id: %i  %0.0f  %0.0f", alatered_id, atlas_pos.x, atlas_pos.y);
                                    }
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
        
        for(auto &tile : g_worldgen_tilesets[set].tile_lookup) {
            if(tile.second.atlas_position.x >= 0 and tile.second.atlas_position.y >= 0) {
                TraceLog(LOG_INFO, "------------tile %i %0.0f  %0.0f", tile.first, tile.second.atlas_position.x, tile.second.atlas_position.y);
                for(auto side : tile.second.marked_sides) {
                    TraceLog(LOG_INFO, "------------marked side %i", side);
                }
            }
        }
    }
}

void GenerateMap(LDTKLevel &new_level, int tileset_uid, Vector2 _map_size) {

    WorldGenTileSet *this_tilset = nullptr;

    for(int ts = 0; ts < g_worldgen_tilesets.size(); ts++) {
        if(g_worldgen_tilesets[ts].uid == tileset_uid) {
            this_tilset = &g_worldgen_tilesets[ts];
        }
    }

    if(this_tilset == nullptr) {
        TraceLog(LOG_INFO, "could not find tileset %i", tileset_uid);
    
    }

    for(auto &tile : this_tilset->tile_lookup) {
        TILEID tile_id = tile.first;

        if(tile_id > TILE_ID_GRASS_START and tile_id < TILE_ID_GRASS_END) {
            this_tilset->sorted_tiles.grass_tiles.push_back(tile_id);
        }
        if(tile_id > TILE_ID_BORDER_START and tile_id < TILE_ID_DIRT_END) {
            this_tilset->sorted_tiles.border_tiles.push_back(tile_id);
        }
        if(tile_id > TILE_ID_PATH_MID and tile_id < TILE_ID_PATH_END) {
            this_tilset->sorted_tiles.path_tiles.push_back(tile_id);
        }
        if(tile_id > TILE_ID_DIRT_START and tile_id < TILE_ID_DIRT_END) {
            this_tilset->sorted_tiles.path_tiles.push_back(tile_id);
        }

    }

    this_tilset->num_paths = 3;

    for(int path = 0; path < this_tilset->num_paths; path++) {
        PathWorm worm;
        worm.locked_dir = {0,0};
        worm.position = { _map_size.x/2, _map_size.y/2 };

        if(GetRandomValue(0, 100) > 50) {
            if(GetRandomValue(0, 100) > 50) {
                worm.locked_dir.x = 1;
                
                TraceLog(LOG_INFO, "locked right");
            }
            else {
                worm.locked_dir.x = -1;
                TraceLog(LOG_INFO, "locked left");
            }
        }
        else {
            if(GetRandomValue(0, 100) > 50) {
                worm.locked_dir.y = 1;
                TraceLog(LOG_INFO, "locked down");
            }
            else {
                worm.locked_dir.y = -1;
                TraceLog(LOG_INFO, "locked up");
            }       
        }
        worm.last_locked_dir = worm.locked_dir;
        this_tilset->path_worms.push_back(worm);
    }

    this_tilset->zone_grid.resize((int)_map_size.x * (int)_map_size.y, ZONE_NONE);


    new_level.identifier = "WORLD GEN LEVEL";
    new_level.px_wid = _map_size.x * this_tilset->tile_grid_size;
    new_level.px_hei = _map_size.y * this_tilset->tile_grid_size;


    //grass layer                           X
    //border                                X
    //structure areas                       -
    //structure positions                   -
    //structure tiles                       -
    //dirt patches                          x
    //paths connect structure areas         -
    //obsticles                             -
    //trees                                 -
    //grass                                 X
    //spawn point                           X
    //transition areas                      -
    //lootables                             -
    //mobs                                  -

    GenerateEntitiesLayer(new_level, *this_tilset, _map_size);

    GenerateStructuresLayer(new_level, *this_tilset, _map_size);

    GenerateUpperTerrainLayer(new_level, *this_tilset, _map_size);

    GenerateLowerTerrainLayer(new_level, *this_tilset, _map_size);

    for(LDTKLayerInstance &layer : new_level.layer_instances) {
        if(layer.identifier == "TerrainUpper") {
            GenerateMapPaths(layer, *this_tilset, _map_size);
        }
    }

    for(LDTKLayerInstance &layer : new_level.layer_instances) {
        if(layer.identifier == "TerrainLower") {
            GenerateDirtPatch(layer, *this_tilset, _map_size);
        }
    }

    GenerateCollisionLayer(new_level, *this_tilset, _map_size);

    GenerateGrass(new_level, *this_tilset, _map_size);

}















void GenerateGrass(LDTKLevel &_level, WorldGenTileSet &_tileset, Vector2 _map_size) {

    Vector2 temp_grid_size = {(_map_size.x*2), (_map_size.y*2)};
    std::vector<int> temp_grid;
    temp_grid.resize(temp_grid_size.x*temp_grid_size.y, SPRITE_ENVIRO_ERROR);

    int _grid_size = 8;

    int num_grass = 4000;

    for(int i = 0; i < num_grass; i++) {
        int x = GetRandomValue(4, (int)(temp_grid_size.x)-4);   
        int y = GetRandomValue(4, (int)(temp_grid_size.y)-4);

        int index = y * temp_grid_size.x + x;

        int zone_index = y/2 * (_map_size.x) + x/2;

        if(_tileset.zone_grid[zone_index] != ZONE_PATH and _tileset.zone_grid[zone_index] != ZONE_DIRT) {
            
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






//needs fully filled grid_tiles
void GenerateDirtPatch(LDTKLayerInstance &_layer, WorldGenTileSet &_tileset, Vector2 _map_size) {
    int num_patches = _layer.c_wid / 10;

    std::vector<int> temp_grid;
    temp_grid.resize(_map_size.x*_map_size.y, TILE_ID_NONE);


    std::vector<Rectangle> dirt_rects;

    for(int patch = 0; patch < num_patches; patch++ ) {
        Rectangle new_rect;
        new_rect.x = (float)GetRandomValue(1, _map_size.x-5);
        new_rect.y = (float)GetRandomValue(1, _map_size.y-5);
        new_rect.width = (float)GetRandomValue(2, 15);
        new_rect.height = (float)GetRandomValue(2, 15);

        dirt_rects.push_back(new_rect);
    }

    for(Rectangle &patch : dirt_rects) {
        //TraceLog(LOG_INFO, "DIRT RECT  x %0.0f y %0.0f  w %0.0f  h %0.0f", patch.x, patch.y, patch.width + patch.x, patch.height + patch.y);
        for(int y = (int)patch.y; y < (int)patch.height + (int)patch.y; y++) {
            for(int x = (int)patch.x; x < (int)patch.width + (int)patch.x; x++) {
                int index = y * (int)_map_size.x + x;
                if(index < temp_grid.size()-1) {
                    temp_grid[index] = 1;
                    _tileset.zone_grid[index] = ZONE_DIRT;
                }
            }
        }
    }

    for(int tile_index = 0; tile_index < temp_grid.size(); tile_index++) {
        int x = tile_index%(int)_map_size.x;
        int y = tile_index/(int)_map_size.x;

        if(x < 1 or y < 1 or x > _map_size.x-1 or y > _map_size.y-1) {
            continue;
        }

        if(temp_grid[tile_index] != -1) {
            int index_right = y * _map_size.x + (x+1);
            int index_left = y * _map_size.x + (x-1);
            int index_up = (y-1) * _map_size.x + x;
            int index_down = (y+1) * _map_size.x + x;

            int index_ur = (y-1) * _map_size.x + (x+1);
            int index_ul = (y-1) * _map_size.x + (x-1);
            int index_lr = (y+1) * _map_size.x + (x+1);
            int index_ll = (y+1) * _map_size.x + (x-1);

            
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


            TILEID tile_id = TILE_ID_DIRT_SINGLE;

            for(auto &tile : _tileset.tile_lookup) {
                if(tile.first > TILE_ID_DIRT_START and tile.first < TILE_ID_DIRT_END) {
                    //TraceLog(LOG_INFO, "dirt TILE ? %i ", tile.first);

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
                        //TraceLog(LOG_INFO, "dirt TILE found %i ", tile.first);
                    }
                }
            }
            
            _layer.grid_tiles[tile_index].src[0] = _tileset.tile_lookup[tile_id].atlas_position.x;
            _layer.grid_tiles[tile_index].src[1] = _tileset.tile_lookup[tile_id].atlas_position.y;
            _layer.grid_tiles[tile_index].t = tile_id;

        }
    }

    //TraceLog(LOG_INFO, "tile  index %i  x %i y %i", index, x, y);
    //_layer.grid_tiles[index].src[0] = _tileset.tile_lookup[TILE_ID_DIRT_MID].atlas_position.x;
    //_layer.grid_tiles[index].src[1] = _tileset.tile_lookup[TILE_ID_DIRT_MID].atlas_position.y;
}



TILEID StrToTileId(const std::string& s) {

    static const std::unordered_map<std::string, TILEID> lookup_table = {
        {"TILE_ID_GRASS_1",                      TILEID::TILE_ID_GRASS_1},
        {"TILE_ID_GRASS_2",                      TILEID::TILE_ID_GRASS_2},
        {"TILE_ID_GRASS_3",                      TILEID::TILE_ID_GRASS_3},
        {"TILE_ID_GRASS_4",                      TILEID::TILE_ID_GRASS_4},
        {"TILE_ID_BORDER",                       TILEID::TILE_ID_BORDER},
        {"TILE_ID_PATH_MID",                     TILEID::TILE_ID_PATH_MID},

        {"TILE_ID_PATH_END_LEFT",                TILEID::TILE_ID_PATH_END_LEFT},
        {"TILE_ID_PATH_END_RIGHT",               TILEID::TILE_ID_PATH_END_RIGHT},
        {"TILE_ID_PATH_END_UP",                  TILEID::TILE_ID_PATH_END_UP},
        {"TILE_ID_PATH_END_DOWN",                TILEID::TILE_ID_PATH_END_DOWN},

        {"TILE_ID_PATH_SINGLE",                  TILEID::TILE_ID_PATH_SINGLE},

        {"TILE_ID_PATH_UP_RIGHT",                 TILEID::TILE_ID_PATH_UP_RIGHT},
        {"TILE_ID_PATH_UP_DOWN",                  TILEID::TILE_ID_PATH_UP_DOWN},
        {"TILE_ID_PATH_UP_LEFT",                  TILEID::TILE_ID_PATH_UP_LEFT},
        {"TILE_ID_PATH_UP_RIGHT_DOWN",            TILEID::TILE_ID_PATH_UP_RIGHT_DOWN},
        {"TILE_ID_PATH_UP_DOWN_LEFT",             TILEID::TILE_ID_PATH_UP_DOWN_LEFT},
        {"TILE_ID_PATH_RIGHT_LEFT",               TILEID::TILE_ID_PATH_RIGHT_LEFT},
        {"TILE_ID_PATH_UP_RIGHT_LEFT",            TILEID::TILE_ID_PATH_UP_RIGHT_LEFT},
        {"TILE_ID_PATH_RIGHT_DOWN_LEFT",          TILEID::TILE_ID_PATH_RIGHT_DOWN_LEFT},
        {"TILE_ID_PATH_DOWN_LEFT",                TILEID::TILE_ID_PATH_DOWN_LEFT},
        {"TILE_ID_PATH_RIGHT_DOWN",               TILEID::TILE_ID_PATH_RIGHT_DOWN},

        {"TILE_ID_DIRT_SINGLE",                 TILEID::TILE_ID_DIRT_SINGLE},
        {"TILE_ID_DIRT_MID",                    TILEID::TILE_ID_DIRT_MID},
        {"TILE_ID_DIRT_RIGHT_DOWN",             TILEID::TILE_ID_DIRT_RIGHT_DOWN},
        {"TILE_ID_DIRT_RIGHT_DOWN_LEFT",        TILEID::TILE_ID_DIRT_RIGHT_DOWN_LEFT},
        {"TILE_ID_DIRT_DOWN_LEFT",              TILEID::TILE_ID_DIRT_DOWN_LEFT},
        {"TILE_ID_DIRT_UP_DOWN_LEFT",           TILEID::TILE_ID_DIRT_UP_DOWN_LEFT},
        {"TILE_ID_DIRT_UP_LEFT",                TILEID::TILE_ID_DIRT_UP_LEFT},
        {"TILE_ID_DIRT_UP_RIGHT_LEFT",          TILEID::TILE_ID_DIRT_UP_RIGHT_LEFT},
        {"TILE_ID_DIRT_RIGHT_UP",               TILEID::TILE_ID_DIRT_RIGHT_UP},
        {"TILE_ID_DIRT_UP_RIGHT_DOWN",          TILEID::TILE_ID_DIRT_UP_RIGHT_DOWN},

        {"TILE_ID_BORDER_SINGLE",                 TILEID::TILE_ID_BORDER_SINGLE},
        {"TILE_ID_BORDER_MID",                    TILEID::TILE_ID_BORDER_MID},
        {"TILE_ID_BORDER_RIGHT_DOWN",             TILEID::TILE_ID_BORDER_RIGHT_DOWN},
        {"TILE_ID_BORDER_RIGHT_DOWN_LEFT",        TILEID::TILE_ID_BORDER_RIGHT_DOWN_LEFT},
        {"TILE_ID_BORDER_DOWN_LEFT",              TILEID::TILE_ID_BORDER_DOWN_LEFT},
        {"TILE_ID_BORDER_UP_DOWN_LEFT",           TILEID::TILE_ID_BORDER_UP_DOWN_LEFT},
        {"TILE_ID_BORDER_UP_LEFT",                TILEID::TILE_ID_BORDER_UP_LEFT},
        {"TILE_ID_BORDER_UP_RIGHT_LEFT",          TILEID::TILE_ID_BORDER_UP_RIGHT_LEFT},
        {"TILE_ID_BORDER_RIGHT_UP",               TILEID::TILE_ID_BORDER_RIGHT_UP},
        {"TILE_ID_BORDER_UP_RIGHT_DOWN",          TILEID::TILE_ID_BORDER_UP_RIGHT_DOWN},
    };


    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        return it->second;
    }
    TraceLog(LOG_INFO, "tile ID not found ");
    return TILEID::TILE_ID_NONE;
}

