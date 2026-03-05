#include "../core/gamedefs.h"
#include "../core/layergen.h"
#include "../core/pathgen.h"
#include "../core/scenerygen.h"
#include <algorithm>
#include <random>

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
                std::string identifier = j["defs"]["tilesets"][i]["identifier"];

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
                    this_tileset.uid = uid;

                    EctractTileData(j["defs"]["tilesets"][i], this_tileset);

                    g_worldgen_tilesets.push_back(this_tileset);

                }
            }
        }
    }

    for(int set = 0; set< g_worldgen_tilesets.size(); set++) {
        TraceLog(LOG_INFO, "set # %i", set);
        TraceLog(LOG_INFO, "-----uid %i", g_worldgen_tilesets[set].uid);
        TraceLog(LOG_INFO, "-----tile tag size %i", g_worldgen_tilesets[set].tile_tags.size());
        for(int tag = 0; tag < g_worldgen_tilesets[set].tile_tags.size(); tag++) {
            //TraceLog(LOG_INFO, "-------tag # %i", tag);
            for(int id = 0; id < g_worldgen_tilesets[set].tile_tags[tag].tile_ids.size(); id++) {
                //TraceLog(LOG_INFO, "------------tile # %i", g_worldgen_tilesets[set].tile_tags[tag].tile_ids[id]);
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

                        if (this_layer.identifier == "TerrainTiles") {
                            BuildTerrainTileSet(j["levels"][level]["layerInstances"][layer]["gridTiles"], *this_tileset);
                        }
                        else if(this_layer.identifier == "StructureTiles") {
                            BuildStructureTileSet(j["levels"][level]["layerInstances"][layer]["gridTiles"], *this_tileset);
                        }
                        else if(this_layer.identifier == "ForrestHouses") {
                            json *bounds_layer = nullptr;

                            for(int l = 0; l < j["levels"][level]["layerInstances"].size(); l++) {
                                if( j["levels"][level]["layerInstances"][l]["__identifier"] == "ForrestHouseBounds") {
                                    bounds_layer = &j["levels"][level]["layerInstances"][l];
                                    break;
                                }
                            }

                            if(bounds_layer) {
                                std::vector<LDTKEntityInstance> structure_bounding_entities;
                                ExtractStructureBounds(*bounds_layer, structure_bounding_entities);
                                BuildPremadeStructures(j["levels"][level]["layerInstances"][layer]["gridTiles"], *this_tileset, structure_bounding_entities, this_tileset->structure_lookup);
                            }
                        }
                        else if(this_layer.identifier == "LayerDecco") {
                            json *bounds_layer = nullptr;

                            for(int l = 0; l < j["levels"][level]["layerInstances"].size(); l++) {
                                if( j["levels"][level]["layerInstances"][l]["__identifier"] == "LayerDeccoBounds") {
                                    bounds_layer = &j["levels"][level]["layerInstances"][l];
                                    break;
                                }
                            }

                            if(bounds_layer) {
                                std::vector<LDTKEntityInstance> decco_bounding_entities;
                                ExtractStructureBounds(*bounds_layer, decco_bounding_entities);
                                BuildPremadeStructures(j["levels"][level]["layerInstances"][layer]["gridTiles"], *this_tileset, decco_bounding_entities, this_tileset->layer_decco_lookup);
                                //BuildDeccoStructures(j["levels"][level]["layerInstances"][layer]["gridTiles"], *this_tileset, decco_bounding_entities);
                            }
                        }
                        else if(this_layer.identifier == "EntityDecco") {
                            json *bounds_layer = nullptr;

                            for(int l = 0; l < j["levels"][level]["layerInstances"].size(); l++) {
                                if( j["levels"][level]["layerInstances"][l]["__identifier"] == "EntityDeccoBounds") {
                                    bounds_layer = &j["levels"][level]["layerInstances"][l];
                                    break;
                                }
                            }

                            if(bounds_layer) {
                                std::vector<LDTKEntityInstance> entity_bounding_entities;
                                ExtractStructureBounds(*bounds_layer, entity_bounding_entities);
                                BuildPremadeStructures(j["levels"][level]["layerInstances"][layer]["gridTiles"], *this_tileset, entity_bounding_entities, this_tileset->entity_decco_lookup);
                                //BuildDeccoStructures(j["levels"][level]["layerInstances"][layer]["gridTiles"], *this_tileset, decco_bounding_entities);
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
    }
}

void GenerateMap(LDTKLevel &new_level, std::string map_name) {

    int tileset_terrain_uid = -1;
    int tileset_structure_uid = -1;

    WorldGenData *wg_data = nullptr;

    for(WorldGenData &data : g_worldgen_data) {
        if(data.map_name == map_name) {

            wg_data = &data;
        }
    }

    if(wg_data == nullptr) {
        TraceLog(LOG_INFO, "WORLD GEN DATA NOT FOUND FOR MAP: %s", map_name.c_str());
        return;
    }

    WorldGenTileSet *this_tileset = nullptr;

    for(int ts = 0; ts < g_worldgen_tilesets.size(); ts++) {
        TraceLog(LOG_INFO, "looking for tileset %s  %s", wg_data->terrrain_set_name.c_str(), g_worldgen_tilesets[ts].identifier.c_str());
        if(g_worldgen_tilesets[ts].identifier == wg_data->terrrain_set_name) {
            this_tileset = &g_worldgen_tilesets[ts];
        }
    }
    
    if(this_tileset == nullptr) {
        TraceLog(LOG_INFO, "could not find tileset %s  %s", map_name.c_str(), wg_data->terrrain_set_name.c_str());
        return;
        
    }
    
    this_tileset->wg_data = *wg_data;

    new_level.identifier = map_name;
    new_level.is_worldgen = true;
    new_level.px_wid = wg_data->map_width * this_tileset->tile_grid_size;
    new_level.px_hei = wg_data->map_height * this_tileset->tile_grid_size;

    


    this_tileset->map_size = {(float)this_tileset->wg_data.map_width, (float)this_tileset->wg_data.map_height};

    this_tileset->sorted_tiles.grass_tiles.clear();
    this_tileset->sorted_tiles.dirt_tiles.clear();
    this_tileset->sorted_tiles.path_tiles.clear();
    this_tileset->sorted_tiles.border_tiles.clear();

    this_tileset->collision_grid.clear();
    this_tileset->collision_grid.resize((int)(this_tileset->map_size.x + 1) * (int)(this_tileset->map_size.y + 1), 0);
    this_tileset->lower_zone_grid.clear();
    this_tileset->lower_zone_grid.resize((int)(this_tileset->map_size.x + 1) * (int)(this_tileset->map_size.y + 1), ZONE_NONE);
    this_tileset->upper_zone_grid.clear();
    this_tileset->upper_zone_grid.resize((int)(this_tileset->map_size.x + 1) * (int)(this_tileset->map_size.y + 1), ZONE_NONE);
    this_tileset->spawn_zone_grid.clear();
    this_tileset->spawn_zone_grid.resize((int)(this_tileset->map_size.x + 1) * (int)(this_tileset->map_size.y + 1), ZONE_NONE);


    this_tileset->collision_layer_index = -1;

    
    for(auto &tile : this_tileset->tile_lookup) {
        TILEID tile_id = (TILEID)tile.first;

        if(tile_id > TILE_ID_GRASS_START and tile_id < TILE_ID_GRASS_END) {
            this_tileset->sorted_tiles.grass_tiles.push_back(tile_id);
        }
        if(tile_id > TILE_ID_BORDER_START and tile_id < TILE_ID_BORDER_END) {
            this_tileset->sorted_tiles.border_tiles.push_back(tile_id);
        }
        if(tile_id > TILE_ID_PATH_START and tile_id < TILE_ID_PATH_END) {
            this_tileset->sorted_tiles.path_tiles.push_back(tile_id);
        }
        if(tile_id > TILE_ID_DIRT_START and tile_id < TILE_ID_DIRT_END) {
            this_tileset->sorted_tiles.dirt_tiles.push_back(tile_id);
        }
        if(tile_id > TILE_ID_FENCE_START and tile_id < TILE_ID_FENCE_END) {
            this_tileset->sorted_tiles.fence_tiles.push_back(tile_id);
        }
        if(tile_id > TILE_ID_ROAD_START and tile_id < TILE_ID_ROAD_END) {
            this_tileset->sorted_tiles.road_tiles.push_back(tile_id);
        }
    }



    TraceLog(LOG_INFO, "-------------------------------------------");
    TraceLog(LOG_INFO, "-------TILESET #%s READY TO BUILD MAP------", map_name.c_str());
    TraceLog(LOG_INFO, "-------------------------------------------");

    this_tileset->wg_plan = GenerateWorldGenPlan(this_tileset->wg_data);
    g_debug_plan = this_tileset->wg_plan;

    GenerateEntitiesLayer(new_level, *this_tileset);
    GenerateZones(new_level, *this_tileset);
    ShapeHills(new_level, *this_tileset);

    //ConnectStructuresWithPaths(*this_tileset);

    BuildRoads(*this_tileset);

    //AddMushroomZones(new_level, *this_tileset);

    GenerateStructuresLayer(new_level, *this_tileset,  g_worldgen_tilesets[1]);
    GenerateUpperTerrainLayer(new_level, *this_tileset);
    GenerateDeccoLayer(new_level, *this_tileset,  g_worldgen_tilesets[1]);
    GenerateLowerTerrainLayer(new_level, *this_tileset);

    //PopulateDeccoEntities(new_level, *this_tileset,  g_worldgen_tilesets[1]);

    PopulateGrass(new_level, *this_tileset);
    PopulateTrees(new_level, *this_tileset);
    PlaceEntities(new_level, *this_tileset);
    PlaceCreatureEntities(new_level, *this_tileset);
    
    GenerateCollisionLayer(new_level, *this_tileset);

    GenerateDebugVisuals(new_level, *this_tileset);
}




void GenerateZones(LDTKLevel &level, WorldGenTileSet &_tileset) {

    //baseline grass on lower
    for(int y = 0; y < _tileset.map_size.y; y++) {
        for(int x = 0; x < _tileset.map_size.x; x++) {
            int index = y * _tileset.map_size.x + x;
            _tileset.lower_zone_grid[index] = ZONE_NONE;

            if(x < 4 or y < 4 or x > _tileset.map_size.x-5 or y > _tileset.map_size.y-5) {              
                _tileset.upper_zone_grid[index] = ZONE_BORDER;
                 _tileset.collision_grid[index] = 1;
            }
        }
    }
    GenerateTerrainZones(level, _tileset);

}




TILEID GetAutoTile(std::vector<TILEID> &tile_list, WorldGenTileSet &_tileset, std::vector<MAPZONE> &zone_grid, MAPZONE target_zone, int grid_index) {
    //TraceLog(LOG_INFO, " do auto tile for  %i ", target_zone);
    
    //TraceLog(LOG_INFO, "target zone %i  | zone grid size  %i  | tile list size %i", target_zone, zone_grid.size(), tile_list.size());
    TILEID id = TILE_ID_NONE;
    bool tile_found = false; 

    int x = grid_index%(int)_tileset.map_size.x;
    int y = grid_index/(int)_tileset.map_size.x;

    bool this_tile_marked_sides[4] = {false, false, false, false};

    if(x < 0 or y < 0 or x > _tileset.map_size.x-1 or y > _tileset.map_size.y-1) {
        return id;
    }

    if(zone_grid[grid_index] == target_zone) {
        int index_right = y * _tileset.map_size.x + (x+1);
        if(x+1 >= _tileset.map_size.x) {
            index_right = y * _tileset.map_size.x + x;
        }
        int index_left = y * _tileset.map_size.x + (x-1);
        if(x-1 < 0) {
            index_left = y * _tileset.map_size.x + x;
        }
        int index_down = (y+1) * _tileset.map_size.x + x;
        if(y+1 >= _tileset.map_size.y) {
            index_down = y * _tileset.map_size.x + x;
        }
        int index_up = (y-1) * _tileset.map_size.x + x;
        if(y-1 < 0) {
            index_up = y * _tileset.map_size.x + x;
        }
        
        
        if (zone_grid[index_up] == target_zone) {
            this_tile_marked_sides[TILESIDE_UP] = true;
        }
        if (zone_grid[index_right] == target_zone) {
            this_tile_marked_sides[TILESIDE_RIGHT] = true;
        }
        if (zone_grid[index_down] == target_zone) {
            this_tile_marked_sides[TILESIDE_DOWN] = true;
        }
        if (zone_grid[index_left] == target_zone) {
            this_tile_marked_sides[TILESIDE_LEFT] = true;
        }

        for(TILEID &tile : tile_list) {
            
            bool up_ok = false;
            bool right_ok = false;
            bool down_ok = false;
            bool left_ok = false;
            
            
            if(_tileset.tile_lookup[tile].marked_sides[TILESIDE_UP] == true and this_tile_marked_sides[TILESIDE_UP] == true) {
                up_ok = true;
            }
            if(_tileset.tile_lookup[tile].marked_sides[TILESIDE_UP] == false and this_tile_marked_sides[TILESIDE_UP] == false) {
                up_ok = true;
            }
            

            if(_tileset.tile_lookup[tile].marked_sides[TILESIDE_RIGHT] == true and this_tile_marked_sides[TILESIDE_RIGHT] == true) {
                right_ok = true;
            }
            if(_tileset.tile_lookup[tile].marked_sides[TILESIDE_RIGHT] == false and this_tile_marked_sides[TILESIDE_RIGHT] == false) {
                right_ok = true;
            }
            

            if(_tileset.tile_lookup[tile].marked_sides[TILESIDE_DOWN] == true and this_tile_marked_sides[TILESIDE_DOWN] == true) {
                down_ok = true;
            }
            if(_tileset.tile_lookup[tile].marked_sides[TILESIDE_DOWN] == false and this_tile_marked_sides[TILESIDE_DOWN] == false) {
                down_ok = true;
            }
            

            if(_tileset.tile_lookup[tile].marked_sides[TILESIDE_LEFT] == true and this_tile_marked_sides[TILESIDE_LEFT] == true) {
                left_ok = true;
            }
            if(_tileset.tile_lookup[tile].marked_sides[TILESIDE_LEFT] == false and this_tile_marked_sides[TILESIDE_LEFT] == false) {
                left_ok = true;
            }
            

            if(up_ok and right_ok and down_ok and left_ok) {
                id =  (TILEID)_tileset.tile_lookup[tile].tile_id;
                tile_found = true;
                return id;
            }
        }
    }

    if(!tile_found) {
        TraceLog(LOG_INFO, "................ u=%i  r=%i   d=%i   l=%i.....................(ZONE %i) zone auto TILE NOT found  ----default id %i   (%i, %i)", 
            this_tile_marked_sides[TILESIDE_UP],
            this_tile_marked_sides[TILESIDE_RIGHT],
            this_tile_marked_sides[TILESIDE_DOWN],
            this_tile_marked_sides[TILESIDE_LEFT],
            target_zone, 
            id, 
            x, 
            y
        );
    }
    return id;
}
    
              
void EctractTileData(json &tj, WorldGenTileSet &this_tileset){

    this_tileset.identifier = tj["identifier"];
    this_tileset.c_wid = tj["__cWid"];
    this_tileset.c_hei = tj["__cHei"];
    this_tileset.rel_path = tj["relPath"];
    this_tileset.px_wid = tj["pxWid"];
    this_tileset.px_hei = tj["pxHei"];
    this_tileset.tile_grid_size = tj["tileGridSize"];
    
    
    for(int tag = 0; tag < tj["enumTags"].size(); tag++) {
        LDTKEnumTag new_tag;
        new_tag.value_string = tj["enumTags"][tag]["enumValueId"];
        TraceLog(LOG_INFO, "====enum tag %s", new_tag.value_string.c_str());

        for(int tid = 0; tid <  tj["enumTags"][tag]["tileIds"].size(); tid++) {
            int id = tj["enumTags"][tag]["tileIds"][tid];
            new_tag.tile_ids.push_back(id);
            //TraceLog(LOG_INFO, "====tile id %i", id);
            
        }
        this_tileset.tile_tags.push_back(new_tag);
        //TraceLog(LOG_INFO, "====tiles tags loaded_____________ %i\n", this_tileset.tile_tags.size());
        
    }
} 


void BuildTerrainTileSet(json &grid_tiles, WorldGenTileSet &this_tileset){
    TraceLog(LOG_INFO, "building terrain tileset");
    for( int tile = 0; tile < grid_tiles.size(); tile++) {
        Vector2 atlas_pos;
        atlas_pos.x = grid_tiles[tile]["src"][0];
        atlas_pos.y = grid_tiles[tile]["src"][1];
        int tile_id = grid_tiles[tile]["t"];

        for(int tile_tag = 0; tile_tag < this_tileset.tile_tags.size(); tile_tag++) {
            if(this_tileset.tile_tags[tile_tag].tile_ids.size() > 0) {
                if(tile_id == this_tileset.tile_tags[tile_tag].tile_ids[0]) {
                    int alatered_id = StrToTileId(this_tileset.tile_tags[tile_tag].value_string);
                    if(alatered_id != TILE_ID_NONE) {

                        WorldGenAutoTile new_tile;
                        new_tile.atlas_position = atlas_pos;
                        new_tile.position = {0,0};
                        new_tile.tile_id = alatered_id;
                        new_tile.marked_sides.fill(false);
                        new_tile.has_collision = false;

                        if(new_tile.tile_id == TILE_ID_BORDER) {
                            new_tile.has_collision = true;
                        }
                        //BuildAutoTile(new_tile);
                        
                        new_tile.marked_sides = TileIdGetAutotile((TILEID)new_tile.tile_id);
                        this_tileset.tile_lookup[(TILEID)alatered_id] = new_tile;
                         TraceLog(LOG_INFO, "tile found id: %i  u%i r%i d%i l%i\n", 
                            alatered_id, 
                            new_tile.marked_sides[0],
                            new_tile.marked_sides[1],
                            new_tile.marked_sides[2],
                            new_tile.marked_sides[3]
                            );
                    }
                }
            }
        }
    }
} 
    

void BuildStructureTileSet(json &grid_tiles, WorldGenTileSet &this_tileset){
    TraceLog(LOG_INFO, "building structuire tileset");
    for( int tile = 0; tile < grid_tiles.size(); tile++) {
        Vector2 atlas_pos;
        atlas_pos.x = grid_tiles[tile]["src"][0];
        atlas_pos.y = grid_tiles[tile]["src"][1];
        int tile_id = grid_tiles[tile]["t"];

        //TraceLog(LOG_INFO, "tile t = %i", tile_id);

        WorldGenAutoTile new_tile;
        new_tile.atlas_position = atlas_pos;
        new_tile.position = {0,0};
        new_tile.tile_id = tile_id;
        new_tile.marked_sides.fill(false);
        new_tile.has_collision = true;

        for(int tile_tag = 0; tile_tag < this_tileset.tile_tags.size(); tile_tag++) {
            if(this_tileset.tile_tags[tile_tag].value_string == "SHADOW") {
                for(int i = 0; i < this_tileset.tile_tags[tile_tag].tile_ids.size(); i++) {
                    if(new_tile.tile_id == this_tileset.tile_tags[tile_tag].tile_ids[i]) {
                        new_tile.has_collision = false;
                        //TraceLog(LOG_INFO, "shadow tile");
                    }
                }
            }
        }

        this_tileset.tile_lookup[tile_id] = new_tile;
    }
}


void ExtractStructureBounds(json &bounds, std::vector<LDTKEntityInstance> &bounding_entities) {
    TraceLog(LOG_INFO, "extracting structure bounds");
    for (int entity = 0; entity < bounds["entityInstances"].size(); entity++) {
        if(bounds["entityInstances"][entity]["__identifier"] == "StructureBounds") {

            LDTKEntityInstance new_entity;
            new_entity.identifier = bounds["entityInstances"][entity]["__identifier"];
            TraceLog(LOG_INFO, "++++++------------NEW ENTITY FOUND %s", new_entity.identifier.c_str());
            new_entity.iid = bounds["entityInstances"][entity]["iid"];
            new_entity.px.push_back(bounds["entityInstances"][entity]["px"][0]);
            new_entity.px.push_back(bounds["entityInstances"][entity]["px"][1]);
            new_entity.width = bounds["entityInstances"][entity]["width"];
            new_entity.height = bounds["entityInstances"][entity]["height"];

            LDTKFieldInstance new_field;

            new_field.identifier = bounds["entityInstances"][entity]["fieldInstances"][0]["__identifier"];
            new_field.value_s = bounds["entityInstances"][entity]["fieldInstances"][0]["__value"];

            new_entity.field_instances.push_back(new_field);
            
            bounding_entities.push_back(new_entity);        
        }
    }
}

void BuildPremadeStructures(json &grid_tiles, WorldGenTileSet &this_tileset, std::vector<LDTKEntityInstance> structure_bounding_entities, std::unordered_map<std::string, WorldGenStructureData> &structure_lookup){

    TraceLog(LOG_INFO, "building premade structuires  # of tiles in grid  %i", grid_tiles.size());

    //std::vector<Rectangle> bounding_rects;

    for(int r = 0; r < structure_bounding_entities.size(); r++) {
        Rectangle new_rect;

        new_rect.x = structure_bounding_entities[r].px[0];
        new_rect.y = structure_bounding_entities[r].px[1];
        new_rect.width = structure_bounding_entities[r].width;
        new_rect.height = structure_bounding_entities[r].height;

        std::string structure_id_string = structure_bounding_entities[r].field_instances[0].value_s;

        TraceLog(LOG_INFO, "%s structure rect at x:%0.0f y:%0.0f w:%0.0f h:%0.0f", structure_id_string.c_str() , new_rect.x, new_rect.y, new_rect.width, new_rect.height);

        WorldGenStructureData new_structure;
        new_structure.position = {0,0};
        new_structure.structure_size = {new_rect.width/this_tileset.tile_grid_size, new_rect.height/this_tileset.tile_grid_size};

        for( int tile = 0; tile < grid_tiles.size(); tile++) {
            int tile_x = grid_tiles[tile]["px"][0];
            int tile_y = grid_tiles[tile]["px"][1];

            if( (tile_x >= (int)new_rect.x and tile_x < (int)(new_rect.x + new_rect.width))
                    and (tile_y >= (int)new_rect.y and tile_y < (int)(new_rect.y + new_rect.height)) ) {
                   
                Vector2 atlas_pos;
                atlas_pos.x = grid_tiles[tile]["src"][0];
                atlas_pos.y = grid_tiles[tile]["src"][1];
                        
                WorldGenAutoTile new_tile;
                new_tile.position = {(float)tile_x - new_rect.x, (float)tile_y - new_rect.y};
                new_tile.atlas_position = atlas_pos;
                new_tile.tile_id = grid_tiles[tile]["t"];
                new_tile.marked_sides.fill(false);
                new_tile.has_collision = this_tileset.tile_lookup[(TILEID)new_tile.tile_id].has_collision;

                new_structure.structure_grid_tiles.push_back(new_tile);
                //TraceLog(LOG_INFO, "new tile %i    at x:%0.0f y:%0.0f", new_tile.tile_id, new_tile.position.x, new_tile.position.y);
            }
        }
        structure_lookup[structure_id_string] = new_structure;
    }

    for(auto &structure : this_tileset.structure_lookup) {
        TraceLog(LOG_INFO, "structure loaded  %s", structure.first.c_str());
        TraceLog(LOG_INFO, "structure tiles  %i", structure.second.structure_grid_tiles.size());
        for(WorldGenAutoTile &tile : structure.second.structure_grid_tiles) {
            //TraceLog(LOG_INFO, "------tile id %i relitive position x %0.0f y %0.0f", tile.tile_id, tile.position.x, tile.position.y);
        }
    }    
}

void BuildStructureParts(json &grid_tiles, WorldGenTileSet &this_tileset, std::vector<LDTKEntityInstance> structure_bounding_entities){

std::vector<Rectangle> bounding_rects;

    for(int r = 0; r < structure_bounding_entities.size(); r++) {
        Rectangle new_rect;

        new_rect.x = structure_bounding_entities[r].px[0];
        new_rect.y = structure_bounding_entities[r].px[1];
        new_rect.width = structure_bounding_entities[r].width;
        new_rect.height = structure_bounding_entities[r].height;

        std::string structure_id_string = structure_bounding_entities[r].field_instances[0].value_s;

        TraceLog(LOG_INFO, "%s structure rect at x:%0.0f y:%0.0f w:%0.0f h:%0.0f", structure_id_string.c_str() , new_rect.x, new_rect.y, new_rect.width, new_rect.height);

        WorldGenStructureData new_part;
        new_part.position = {0,0};
        new_part.structure_size = {new_rect.width/this_tileset.tile_grid_size, new_rect.height/this_tileset.tile_grid_size};

        for( int tile = 0; tile < grid_tiles.size(); tile++) {
            int tile_x = grid_tiles[tile]["px"][0];
            int tile_y = grid_tiles[tile]["px"][1];

            if( (tile_x >= (int)new_rect.x and tile_x <= (int)(new_rect.x + new_rect.width))
                    and (tile_y >= (int)new_rect.y and tile_y <= (int)(new_rect.y + new_rect.height)) ) {
                   
                Vector2 atlas_pos;
                atlas_pos.x = grid_tiles[tile]["src"][0];
                atlas_pos.y = grid_tiles[tile]["src"][1];
                        
                WorldGenAutoTile new_tile;
                new_tile.position = {(float)tile_x - new_rect.x, (float)tile_y - new_rect.y};
                new_tile.atlas_position = atlas_pos;
                new_tile.tile_id = grid_tiles[tile]["t"];
                new_tile.marked_sides.fill(false);
                new_tile.has_collision = this_tileset.tile_lookup[(TILEID)new_tile.tile_id].has_collision;

                new_part.structure_grid_tiles.push_back(new_tile);
                //TraceLog(LOG_INFO, "new tile %i    at x:%0.0f y:%0.0f", new_tile.tile_id, new_tile.position.x, new_tile.position.y);
            }
        }
        this_tileset.structure_lookup[structure_id_string] = new_part;
    }

    for(auto &structure : this_tileset.structure_lookup) {
        TraceLog(LOG_INFO, "structure loaded  %s", structure.first.c_str());
        TraceLog(LOG_INFO, "structure tiles  %i", structure.second.structure_grid_tiles.size());
        for(WorldGenAutoTile &tile : structure.second.structure_grid_tiles) {
            //TraceLog(LOG_INFO, "------tile id %i relitive position x %0.0f y %0.0f", tile.tile_id, tile.position.x, tile.position.y);
        }
    }    
}

void BuildDeccoStructures(json &grid_tiles, WorldGenTileSet &this_tileset, std::vector<LDTKEntityInstance> decco_bounding_entities, std::unordered_map<std::string, WorldGenStructureData> &lookup){

    std::vector<Rectangle> bounding_rects;

    for(int r = 0; r < decco_bounding_entities.size(); r++) {
        Rectangle new_rect;

        new_rect.x = decco_bounding_entities[r].px[0];
        new_rect.y = decco_bounding_entities[r].px[1];
        new_rect.width = decco_bounding_entities[r].width;
        new_rect.height = decco_bounding_entities[r].height;

        std::string structure_id_string = decco_bounding_entities[r].field_instances[0].value_s;

        TraceLog(LOG_INFO, "%s decco rect at x:%0.0f y:%0.0f w:%0.0f h:%0.0f", structure_id_string.c_str() , new_rect.x, new_rect.y, new_rect.width, new_rect.height);

        WorldGenStructureData new_structure;
        new_structure.position = {0,0};
        new_structure.structure_size = {new_rect.width/this_tileset.tile_grid_size, new_rect.height/this_tileset.tile_grid_size};

        for( int tile = 0; tile < grid_tiles.size(); tile++) {
            int tile_x = grid_tiles[tile]["px"][0];
            int tile_y = grid_tiles[tile]["px"][1];

            if( (tile_x >= (int)new_rect.x and tile_x <= (int)(new_rect.x + new_rect.width))
                    and (tile_y >= (int)new_rect.y and tile_y <= (int)(new_rect.y + new_rect.height)) ) {
                   
                Vector2 atlas_pos;
                atlas_pos.x = grid_tiles[tile]["src"][0];
                atlas_pos.y = grid_tiles[tile]["src"][1];
                        
                WorldGenAutoTile new_tile;
                new_tile.position = {(float)tile_x - new_rect.x, (float)tile_y - new_rect.y};
                new_tile.atlas_position = atlas_pos;
                new_tile.tile_id = grid_tiles[tile]["t"];
                new_tile.marked_sides.fill(false);
                new_tile.has_collision = this_tileset.tile_lookup[(TILEID)new_tile.tile_id].has_collision;

                new_structure.structure_grid_tiles.push_back(new_tile);
                //TraceLog(LOG_INFO, "new tile %i    at x:%0.0f y:%0.0f", new_tile.tile_id, new_tile.position.x, new_tile.position.y);
            }
        }
        lookup[structure_id_string] = new_structure;
    }

    for(auto &structure : this_tileset.layer_decco_lookup) {
        TraceLog(LOG_INFO, "decco loaded  %s", structure.first.c_str());
        TraceLog(LOG_INFO, "decco tiles  %i", structure.second.structure_grid_tiles.size());
        for(WorldGenAutoTile &tile : structure.second.structure_grid_tiles) {
            //TraceLog(LOG_INFO, "------tile id %i relitive position x %0.0f y %0.0f", tile.tile_id, tile.position.x, tile.position.y);
        }
    }    
}


void CreateSpawnPatches(WorldGenTileSet &_tileset, int patch_size) {
/* 
    int x_patches = (_tileset.map_size.x/patch_size);
    int y_patches = (_tileset.map_size.y/patch_size);

    TraceLog(LOG_INFO, "------ x patches %i   y patches %i  ", x_patches, y_patches);
    //int num_structure_patches = x_patches + y_patches;

    int x_patch_size = patch_size;
    int y_patch_size = patch_size;

    for(int y_patch = 1; y_patch < y_patches - 1; y_patch++ ) {
        for(int x_patch = 1; x_patch < x_patches - 1; x_patch++) {
            int x_pos = (x_patch * x_patch_size);
            int y_pos = (y_patch * y_patch_size);

            Rectangle new_patch;
            new_patch.x = x_pos;
            new_patch.y = y_pos;
            new_patch.width = patch_size;
            new_patch.height = patch_size;


            if(new_patch.x < 0) {new_patch.x = 0;}
            if(new_patch.x > _tileset.map_size.x-1) { new_patch.x = _tileset.map_size.x-1;}
            if(new_patch.x + patch_size > _tileset.map_size.x) {
                int diff = (new_patch.x + patch_size) - _tileset.map_size.x;
                new_patch.width -= diff;
            }

            if(new_patch.y < 0) {new_patch.y = 0;}
            if(new_patch.y + patch_size > _tileset.map_size.y-1) { new_patch.y = _tileset.map_size.y-1;}
            if(new_patch.y + patch_size > _tileset.map_size.y) {
                int diff = (new_patch.y + patch_size) - _tileset.map_size.y;
                new_patch.height -= diff;
            }

            _tileset.wg_plan.spawn_patches.push_back(new_patch);

            /* TraceLog(LOG_INFO, "------ patch  x %0.0f   y %0.0f    w %0.0f    h %0.0f",
                new_patch.x,
                new_patch.y,
                new_patch.width,
                new_patch.height);
        }
    } 
    */
}


Rectangle GetAvailableSpawnPatch(WorldGenTileSet &_tileset) {

    std::vector<int> index_choices;

    for(int i = 0; i < _tileset.wg_plan.spawn_patches.size(); i++ ) {
        bool found = false;
        for(int patch : _tileset.wg_plan.used_spawn_patches) {
            if(i == patch) {
                found = true;
            }
        }

        if(!found) {index_choices.push_back(i);}
    }

    if(index_choices.size() < 1) {
        Rectangle empty_rect;
        return empty_rect;
    }
    
    int choice = index_choices[GetRandomValue(0, index_choices.size()-1)];

    _tileset.wg_plan.used_spawn_patches.push_back(choice);

    return _tileset.wg_plan.spawn_patches[choice];

}



TILEID GetFenceTileTop(WorldGenTileSet &_tileset, std::vector<MAPZONE> &zone_grid, int tile_x, int tile_y) {

    TILEID id = TILE_ID_NONE;

    int index = tile_y * _tileset.map_size.x + tile_x;

    int index_up = (tile_y-1) * _tileset.map_size.x + tile_x;

    int index_right = tile_y * _tileset.map_size.x + (tile_x+1);

    int index_down = (tile_y+1) * _tileset.map_size.x + tile_x;

    int index_left = tile_y * _tileset.map_size.x + (tile_x-1);

    bool has_up = false;
    bool has_right = false;
    bool has_down = false;
    bool has_left = false;


    if(zone_grid[index_right] == ZONE_FENCE/*  or zone_grid[index_right] == ZONE_FENCE_BOTTOM */) {has_right = true;}

    if(zone_grid[index_left] == ZONE_FENCE/*  or zone_grid[index_left] == ZONE_FENCE_BOTTOM */) {has_left = true;}

    if(zone_grid[index_up] == ZONE_FENCE or zone_grid[index_up] == ZONE_FENCE_BOTTOM) {has_up = true;}

    if(zone_grid[index_down] == ZONE_FENCE or zone_grid[index_down] == ZONE_FENCE_BOTTOM) {has_down = true;}


    if(!has_up and !has_right and has_down and has_left) {id = TILE_ID_FENCE_TOP_RIGHT_CORNER; zone_grid[index_down] = ZONE_FENCE_BOTTOM;}

    else if(!has_up and has_right and has_down and !has_left) {id = TILE_ID_FENCE_TOP_LEFT_CORNER; zone_grid[index_down] = ZONE_FENCE_BOTTOM;}

    else if(has_up and has_right and !has_down and !has_left) {id = TILE_ID_FENCE_BOTTOM_LEFT_CORNER; zone_grid[index_down] = ZONE_FENCE_BOTTOM;}

    else if(has_up and !has_right and !has_down and has_left) {id = TILE_ID_FENCE_BOTTOM_RIGHT_CORNER; zone_grid[index_down] = ZONE_FENCE_BOTTOM;}

    else if(has_up and !has_right and has_down and !has_left) {id = TILE_ID_FENCE_MID_VERTICAL;}

    else if(!has_up and has_right and !has_down and has_left) {id = TILE_ID_FENCE_MID_HORIZANTAL; zone_grid[index_down] = ZONE_FENCE_BOTTOM;}

    else if(has_up and !has_right and !has_down and !has_left) {id = TILE_ID_FENCE_END_DOWN; zone_grid[index_down] = ZONE_FENCE_BOTTOM;}

    else if(!has_up and !has_right and has_down and !has_left) {id = TILE_ID_FENCE_END_UP;}

    else if(!has_up and has_right and !has_down and !has_left) {id = TILE_ID_FENCE_END_LEFT; zone_grid[index_down] = ZONE_FENCE_BOTTOM;}

    else if(!has_up and !has_right and !has_down and has_left) {id = TILE_ID_FENCE_END_RIGHT; zone_grid[index_down] = ZONE_FENCE_BOTTOM;}

    //TraceLog(LOG_INFO, "------fence tile  %i", id);
    return id;

}



TILEID GetFenceTileBottom(WorldGenTileSet &_tileset, std::vector<MAPZONE> &zone_grid, int tile_x, int tile_y) {

    TILEID id = TILE_ID_NONE;

    int index = tile_y * _tileset.map_size.x + tile_x;

    int top_index = (tile_y-1) * _tileset.map_size.x + tile_x;

    int top_index_up = (tile_y-2) * _tileset.map_size.x + tile_x;

    int top_index_right = (tile_y-1) * _tileset.map_size.x + (tile_x+1);

    int top_index_down = (tile_y) * _tileset.map_size.x + tile_x;

    int index_down = (tile_y+1) * _tileset.map_size.x + tile_x;

    int top_index_left = (tile_y-1) * _tileset.map_size.x + (tile_x-1);

    bool has_up = false;
    bool has_right = false;
    bool has_down = false;
    bool has_left = false;

    if(zone_grid[top_index_right] == ZONE_FENCE) {has_right = true;}
    if(zone_grid[top_index_left] == ZONE_FENCE) {has_left = true;}
    if(zone_grid[top_index_up] == ZONE_FENCE) {has_up = true;}
    if(zone_grid[index_down] == ZONE_FENCE) {has_down = true;}
    


    if(has_up and has_right and !has_down and !has_left) {id = TILE_ID_FENCE_BOTTOM_LEFT_CORNER_LOWER;}

    else if(has_up and !has_right and !has_down and has_left) {id = TILE_ID_FENCE_BOTTOM_RIGHT_CORNER_LOWER;}

    else if(!has_up and !has_right and has_down and has_left) {id = TILE_ID_FENCE_TOP_RIGHT_CORNER_LOWER;}

    else if(!has_up and has_right and has_down and !has_left) {id = TILE_ID_FENCE_TOP_LEFT_CORNER_LOWER;}


    else if(!has_up and has_right and !has_down and has_left) {id = TILE_ID_FENCE_MID_HORIZANTAL_LOWER;}


    else if(has_up and !has_right and !has_down and !has_left) {id = TILE_ID_FENCE_END_DOWN_LOWER;}

    else if(!has_up and has_right and !has_down and !has_left) {id = TILE_ID_FENCE_END_LEFT_LOWER;}

    else if(!has_up and !has_right and !has_down and has_left) {id = TILE_ID_FENCE_END_RIGHT_LOWER;}

    //TraceLog(LOG_INFO, "------fence tile bottom  %i\n", id);
    return id;

}


TILEID StrToTileId(const std::string& s) {

    static const std::unordered_map<std::string, TILEID> lookup_table = {
        {"TILE_ID_GRASS_1",                     TILEID::TILE_ID_GRASS_1},
        {"TILE_ID_GRASS_2",                     TILEID::TILE_ID_GRASS_2},
        {"TILE_ID_GRASS_3",                     TILEID::TILE_ID_GRASS_3},
        {"TILE_ID_GRASS_4",                     TILEID::TILE_ID_GRASS_4},
        {"TILE_ID_BORDER",                      TILEID::TILE_ID_BORDER},

        {"TILE_ID_PATH_MID",                    TILEID::TILE_ID_PATH_MID},
        {"TILE_ID_PATH_END_LEFT",               TILEID::TILE_ID_PATH_END_LEFT},
        {"TILE_ID_PATH_END_RIGHT",              TILEID::TILE_ID_PATH_END_RIGHT},
        {"TILE_ID_PATH_END_UP",                 TILEID::TILE_ID_PATH_END_UP},
        {"TILE_ID_PATH_END_DOWN",               TILEID::TILE_ID_PATH_END_DOWN},
        {"TILE_ID_PATH_SINGLE",                 TILEID::TILE_ID_PATH_SINGLE},
        {"TILE_ID_PATH_UP_RIGHT",               TILEID::TILE_ID_PATH_UP_RIGHT},
        {"TILE_ID_PATH_UP_DOWN",                TILEID::TILE_ID_PATH_UP_DOWN},
        {"TILE_ID_PATH_UP_LEFT",                TILEID::TILE_ID_PATH_UP_LEFT},
        {"TILE_ID_PATH_UP_RIGHT_DOWN",          TILEID::TILE_ID_PATH_UP_RIGHT_DOWN},
        {"TILE_ID_PATH_UP_DOWN_LEFT",           TILEID::TILE_ID_PATH_UP_DOWN_LEFT},
        {"TILE_ID_PATH_RIGHT_LEFT",             TILEID::TILE_ID_PATH_RIGHT_LEFT},
        {"TILE_ID_PATH_UP_RIGHT_LEFT",          TILEID::TILE_ID_PATH_UP_RIGHT_LEFT},
        {"TILE_ID_PATH_RIGHT_DOWN_LEFT",        TILEID::TILE_ID_PATH_RIGHT_DOWN_LEFT},
        {"TILE_ID_PATH_DOWN_LEFT",              TILEID::TILE_ID_PATH_DOWN_LEFT},
        {"TILE_ID_PATH_RIGHT_DOWN",             TILEID::TILE_ID_PATH_RIGHT_DOWN},

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

        {"TILE_ID_BORDER_SINGLE",               TILEID::TILE_ID_BORDER_SINGLE},
        {"TILE_ID_BORDER_MID",                  TILEID::TILE_ID_BORDER_MID},
        {"TILE_ID_BORDER_RIGHT_DOWN",           TILEID::TILE_ID_BORDER_RIGHT_DOWN},
        {"TILE_ID_BORDER_RIGHT_DOWN_LEFT",      TILEID::TILE_ID_BORDER_RIGHT_DOWN_LEFT},
        {"TILE_ID_BORDER_DOWN_LEFT",            TILEID::TILE_ID_BORDER_DOWN_LEFT},
        {"TILE_ID_BORDER_UP_DOWN_LEFT",         TILEID::TILE_ID_BORDER_UP_DOWN_LEFT},
        {"TILE_ID_BORDER_UP_LEFT",              TILEID::TILE_ID_BORDER_UP_LEFT},
        {"TILE_ID_BORDER_UP_RIGHT_LEFT",        TILEID::TILE_ID_BORDER_UP_RIGHT_LEFT},
        {"TILE_ID_BORDER_RIGHT_UP",             TILEID::TILE_ID_BORDER_RIGHT_UP},
        {"TILE_ID_BORDER_UP_RIGHT_DOWN",        TILEID::TILE_ID_BORDER_UP_RIGHT_DOWN}, 

        {"TILE_ID_FENCE_TOP_LEFT_CORNER",       TILEID::TILE_ID_FENCE_TOP_LEFT_CORNER},
        {"TILE_ID_FENCE_TOP_RIGHT_CORNER",      TILEID::TILE_ID_FENCE_TOP_RIGHT_CORNER},
        {"TILE_ID_FENCE_BOTTOM_LEFT_CORNER",    TILEID::TILE_ID_FENCE_BOTTOM_LEFT_CORNER},
        {"TILE_ID_FENCE_BOTTOM_RIGHT_CORNER",   TILEID::TILE_ID_FENCE_BOTTOM_RIGHT_CORNER},
        {"TILE_ID_FENCE_END_UP",                TILEID::TILE_ID_FENCE_END_UP},
        {"TILE_ID_FENCE_END_RIGHT",             TILEID::TILE_ID_FENCE_END_RIGHT},
        {"TILE_ID_FENCE_END_DOWN",              TILEID::TILE_ID_FENCE_END_DOWN},
        {"TILE_ID_FENCE_END_LEFT",              TILEID::TILE_ID_FENCE_END_LEFT},
        {"TILE_ID_FENCE_MID_VERTICAL",          TILEID::TILE_ID_FENCE_MID_VERTICAL},
        {"TILE_ID_FENCE_MID_HORIZANTAL",        TILEID::TILE_ID_FENCE_MID_HORIZANTAL},

        {"TILE_ID_FENCE_TOP_LEFT_CORNER_LOWER",          TILEID::TILE_ID_FENCE_TOP_LEFT_CORNER_LOWER},
        {"TILE_ID_FENCE_TOP_RIGHT_CORNER_LOWER",          TILEID::TILE_ID_FENCE_TOP_RIGHT_CORNER_LOWER},
        {"TILE_ID_FENCE_BOTTOM_LEFT_CORNER_LOWER",          TILEID::TILE_ID_FENCE_BOTTOM_LEFT_CORNER_LOWER},
        {"TILE_ID_FENCE_BOTTOM_RIGHT_CORNER_LOWER",          TILEID::TILE_ID_FENCE_BOTTOM_RIGHT_CORNER_LOWER},
        {"TILE_ID_FENCE_END_RIGHT_LOWER",          TILEID::TILE_ID_FENCE_END_RIGHT_LOWER},
        {"TILE_ID_FENCE_END_DOWN_LOWER",          TILEID::TILE_ID_FENCE_END_DOWN_LOWER},
        {"TILE_ID_FENCE_END_LEFT_LOWER",          TILEID::TILE_ID_FENCE_END_LEFT_LOWER},
        {"TILE_ID_FENCE_MID_HORIZANTAL_LOWER",          TILEID::TILE_ID_FENCE_MID_HORIZANTAL_LOWER},

        {"TILE_ID_ROAD_UP_RIGHT_LEFT",          TILEID::TILE_ID_ROAD_UP_RIGHT_LEFT},
        {"TILE_ID_ROAD_UP_RIGHT_DOWN",          TILEID::TILE_ID_ROAD_UP_RIGHT_DOWN},
        {"TILE_ID_ROAD_UP_DOWN_LEFT",           TILEID::TILE_ID_ROAD_UP_DOWN_LEFT},
        {"TILE_ID_ROAD_RIGHT_DOWN_LEFT",       TILEID::TILE_ID_ROAD_RIGHT_DOWN_LEFT},
        {"TILE_ID_ROAD_CENTER",                 TILEID::TILE_ID_ROAD_CENTER},
        {"TILE_ID_DEBUG_1",                 TILEID::TILE_ID_DEBUG_1},


    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        return it->second;
    }
    TraceLog(LOG_INFO, "the tile ID is not found ---- %s", s.c_str());
    return TILEID::TILE_ID_NONE;
}



BIOME_TYPE StrToBiomeId(const std::string& s) {

    static const std::unordered_map<std::string, BIOME_TYPE> lookup_table = {
        {"BIOME_PLAINS",                     BIOME_TYPE::BIOME_PLAINS},
        {"BIOME_FORREST",                     BIOME_TYPE::BIOME_FORREST},
        {"BIOME_HILLS",                     BIOME_TYPE::BIOME_HILLS},
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        return it->second;
    }
    TraceLog(LOG_INFO, "the BIOME_TYPE ID is not found ---- %s", s.c_str());
    return BIOME_TYPE::BIOME_NONE;
}


std::array<bool,4> TileIdGetAutotile(TILEID tile_id) {
    
    std::unordered_map<TILEID, std::array<bool,4>> side_map = {

        {TILE_ID_DIRT_SINGLE,               {false, false, false, false}},
        {TILE_ID_DIRT_MID,                  {true, true, true, true}},
        {TILE_ID_DIRT_RIGHT_DOWN,           {false, true, true, false}},
        {TILE_ID_DIRT_RIGHT_DOWN_LEFT,      {false, true, true, true}},
        {TILE_ID_DIRT_DOWN_LEFT,            {false, false, true, true}},
        {TILE_ID_DIRT_UP_DOWN_LEFT,         {true, false, true, true}},
        {TILE_ID_DIRT_UP_LEFT,              {true, false, false, true}},
        {TILE_ID_DIRT_UP_RIGHT_LEFT,        {true, true, false, true}},
        {TILE_ID_DIRT_RIGHT_UP,             {true, true, false, false}},
        {TILE_ID_DIRT_UP_RIGHT_DOWN,        {true, true, true, false}},

        {TILE_ID_BORDER_SINGLE,             {false, false, false, false}},
        {TILE_ID_BORDER_MID,                {true, true, true, true}},
        {TILE_ID_BORDER_RIGHT_DOWN,         {false, true, true, false}},
        {TILE_ID_BORDER_RIGHT_DOWN_LEFT,    {false, true, true, true}},
        {TILE_ID_BORDER_DOWN_LEFT,          {false, false, true, true}},
        {TILE_ID_BORDER_UP_DOWN_LEFT,       {true, false, true, true}},
        {TILE_ID_BORDER_UP_LEFT,            {true, false, false, true}},
        {TILE_ID_BORDER_UP_RIGHT_LEFT,      {true, true, false, true}},
        {TILE_ID_BORDER_RIGHT_UP,           {true, true, false, false}},
        {TILE_ID_BORDER_UP_RIGHT_DOWN,      {true, true, true, false}},

        {TILE_ID_BORDER_END_LEFT,           {false, false, false, true}},
        {TILE_ID_BORDER_END_RIGHT,          {false, true, false, false}},
        {TILE_ID_BORDER_END_UP,             {true, false, false, false}},
        {TILE_ID_BORDER_END_DOWN,           {false, false, true, false}},

        {TILE_ID_BORDER_UP_DOWN,            {true, false, true, false}},
        {TILE_ID_BORDER_RIGHT_LEFT,         {false, true, false, true}},


        {TILE_ID_PATH_SINGLE,               {false, false, false, false}},
        {TILE_ID_PATH_MID,                  {true, true, true, true}},
        {TILE_ID_PATH_END_UP,               {false, false, true, false}},
        {TILE_ID_PATH_END_RIGHT,            {false, false, false, true}},
        {TILE_ID_PATH_END_DOWN,             {true, false, false, false}},
        {TILE_ID_PATH_END_LEFT,             {false, true, false, false}},
        {TILE_ID_PATH_UP_RIGHT,             {true, true, false, false}},
        {TILE_ID_PATH_UP_DOWN,              {true, false, true, false}},
        {TILE_ID_PATH_UP_LEFT,              {true, false, false, true}},
        {TILE_ID_PATH_UP_RIGHT_DOWN,        {true, true, true, false}},
        {TILE_ID_PATH_UP_DOWN_LEFT,         {true, false, true, true}},
        {TILE_ID_PATH_RIGHT_LEFT,           {false, true, false, true}},
        {TILE_ID_PATH_UP_RIGHT_LEFT,        {true, true, false, true}},
        {TILE_ID_PATH_RIGHT_DOWN_LEFT,      {false, true, true, true}},
        {TILE_ID_PATH_DOWN_LEFT,            {false, false, true, true}},
        {TILE_ID_PATH_RIGHT_DOWN,           {false, true, true, false}},

        {TILE_ID_ROAD_UP_RIGHT_DOWN,        {true, true, true, false}},
        {TILE_ID_ROAD_UP_DOWN_LEFT,         {true, false, true, true}},
        {TILE_ID_ROAD_RIGHT_DOWN_LEFT,      {false, true, true, true}},
        {TILE_ID_ROAD_UP_RIGHT_LEFT,        {true, true, false, true}},
        {TILE_ID_ROAD_CENTER,               {true, true, true, true}},

    };

    if (auto it = side_map.find(tile_id); it != side_map.end()) {
        return it->second;
    }
    TraceLog(LOG_INFO, "autotile ID not found %i", tile_id);
    return {false, false, false, false};
}



void LoadMapGenData(json &j) {

    for(int map = 0; map < j.size(); map++) {
        WorldGenData new_data;
        new_data.map_name = j[map]["map_name"];
        new_data.map_width = j[map]["map_w"];
        new_data.map_height = j[map]["map_h"];   
        new_data.tree_coverage = j[map]["tree_coverage"];   
        new_data.grass_coverage = j[map]["grass_coverage"];        
        new_data.terrrain_set_name = j[map]["tileset_terrain_name"];
        new_data.structure_set_name = j[map]["tileset_structure_name"];
        new_data.has_shelter = j[map]["has_shelter"];


        for(int r = 0; r < j[map]["biomes"].size(); r++) {

            WorldGenBiomeData new_biome;
            new_biome.type = StrToBiomeId(j[map]["biomes"][r]["type"]);
            new_biome.coverage = j[map]["biomes"][r]["coverage"];
            new_data.biome_data.push_back(new_biome);
        }

        for(int l = 0; l < j[map]["structures"].size(); l++) {
            WorldGenStructure new_structure;
            new_structure.id = j[map]["structures"][l]["id"];
            new_structure.biome = StrToBiomeId(j[map]["structures"][l]["prefer_region"]);
            new_structure.count_min = j[map]["structures"][l]["count"][0];
            new_structure.count_max = j[map]["structures"][l]["count"][1];
            new_structure.min_distance = j[map]["structures"][l]["min_distance"];
            new_structure.size.x = j[map]["structures"][l]["size"][0];
            new_structure.size.y = j[map]["structures"][l]["size"][1];
            new_data.structure_data.push_back(new_structure);
        }

        for(WorldGenStructure &structure : new_data.structure_data) {
            structure.profile = BuildStructureProfile(structure);
        }


        for(WorldGenStructure &structure : new_data.structure_data) {
            TraceLog(LOG_INFO, "-----------------structure data: %s  ", structure.id.c_str());
            TraceLog(LOG_INFO, "-------------------------- wants_road: %0.2f  ", structure.profile.wants_road);
            TraceLog(LOG_INFO, "-------------------------- wants_center: %0.2f  ", structure.profile.wants_center);
            TraceLog(LOG_INFO, "-------------------------- wants_edge: %0.2f  ", structure.profile.wants_edge);
            TraceLog(LOG_INFO, "-------------------------- wants_forrest: %0.2f  ", structure.profile.wants_forrest);
        }


        WorldGenTerrain new_terrain;

        new_terrain.hill_patches = j[map]["terrain"]["hill_patches"];
        new_terrain.hill_size = j[map]["terrain"]["hill_size"];
        new_terrain.dirt_patches = j[map]["terrain"]["dirt_patches"];
        new_terrain.dirt_size = j[map]["terrain"]["dirt_size"];

        new_data.terrain = new_terrain;


        for(int s = 0; s < j[map]["exits"].size(); s++) {
            std::string d_string = j[map]["exits"][s];
            new_data.exit_dest_strings.push_back(d_string);
        }
        g_worldgen_data.push_back(new_data);
        TraceLog(LOG_INFO, "-----------------wg data map: %s  ", new_data.map_name.c_str());
    }

}


//  PLAN

WorldGenPlan GenerateWorldGenPlan(WorldGenData &wg_data) {
    WorldGenPlan new_plan;

    new_plan.map_size = {(float)wg_data.map_width, (float)wg_data.map_height};

    PlanGeography(wg_data, new_plan);
    PlanRegions(wg_data, new_plan);
    PlanRoads(wg_data, new_plan);

    int infl_cell_size = 10;

    CreateInfluenceGrid(new_plan, infl_cell_size);
    
    PlanStructures(wg_data, new_plan);

    return new_plan;
}


void PlanGeography(WorldGenData &wg_data, WorldGenPlan &wg_plan) {

    wg_plan.exit_dest_strings = wg_data.exit_dest_strings;

    int fourty_percent_x = (int)(wg_plan.map_size.x * 0.4f);
    int fourty_percent_y = (int)(wg_plan.map_size.y * 0.4f);

    int max_exits = wg_plan.exit_dest_strings.size();
    
    wg_plan.road_midpoint.x = GetRandomValue(fourty_percent_x, wg_plan.map_size.x - fourty_percent_x);
    wg_plan.road_midpoint.y = GetRandomValue(fourty_percent_y, wg_plan.map_size.y - fourty_percent_y);

    std::vector<Vector2> exit_positions;
 
    Vector2 up = {wg_plan.road_midpoint.x, 1};
    Vector2 right = { wg_plan.map_size.x - 1, wg_plan.road_midpoint.y};
    Vector2 down = {wg_plan.road_midpoint.x, wg_plan.map_size.y - 1};
    Vector2 left = {1, wg_plan.road_midpoint.y};

    exit_positions.push_back(up);
    exit_positions.push_back(right);
    exit_positions.push_back(down);
    exit_positions.push_back(left);

    for(int i = 0; i < exit_positions.size(); i++) {
        int j = GetRandomValue(0, i);
        std::swap(exit_positions[i], exit_positions[j]);
    }
    for(int exit = 0; exit < wg_plan.exit_dest_strings.size(); exit++) {
        
        Vector2 exit_pos = exit_positions[exit];

        wg_plan.exit_positions.push_back(exit_pos);
    }




    for(int y = 0; y <= wg_plan.map_size.y; y++) {
        for(int x = 0; x <= wg_plan.map_size.x; x++) {
            if(x < 1 or y < 1 or x > wg_plan.map_size.x-1 or y > wg_plan.map_size.y-1) {              
                wg_plan.edge_positions.push_back( Vector2{(float)x,(float)y} );
                //TraceLog(LOG_INFO, "edge pos------  (%i, %i) ", x, y);
            }
        }
    }



}



void PlanRegions(WorldGenData &wg_data, WorldGenPlan &wg_plan) {

    int patch_size = 25;
    int x_patches = (wg_plan.map_size.x/patch_size);
    int y_patches = (wg_plan.map_size.y/patch_size);

    std::vector<Rectangle> region_rects;

    TraceLog(LOG_INFO, "region patches------ x patches %i   y patches %i  ", x_patches, y_patches);

    for(int y_patch = 0; y_patch < y_patches; y_patch++ ) {
        for(int x_patch = 0; x_patch < x_patches; x_patch++) {
            int x_pos = (x_patch * patch_size);
            int y_pos = (y_patch * patch_size);

            Rectangle new_rect;
            new_rect.x = x_pos;
            new_rect.y = y_pos;
            new_rect.width = patch_size;
            new_rect.height = patch_size;

            if(new_rect.x < 0) {new_rect.x = 0;}
            if(new_rect.x > wg_plan.map_size.x) { new_rect.x = wg_plan.map_size.x;}
            if(new_rect.x + patch_size > wg_plan.map_size.x) {
                int diff = (new_rect.x + patch_size) - wg_plan.map_size.x;
                new_rect.width -= diff;
            }

            if(new_rect.y < 0) {new_rect.y = 0;}
            if(new_rect.y + patch_size > wg_plan.map_size.y) { new_rect.y = wg_plan.map_size.y;}
            if(new_rect.y + patch_size > wg_plan.map_size.y) {
                int diff = (new_rect.y + patch_size) - wg_plan.map_size.y;
                new_rect.height -= diff;
            }


            region_rects.push_back(new_rect);
        }
    }

    for(Rectangle r : region_rects) {
        WorldGenBiome biome;
        biome.type = ChooseBiome(wg_data);// (BIOME_TYPE)GetRandomValue(BIOME_PLAINS, BIOME_FORREST); // forest/plains/hills
        //TraceLog(LOG_INFO, "-----------------biome id: %i  ", biome.type);
        biome.rect = r;

        wg_plan.biomes.push_back(biome);
    }
}


void PlanStructures(WorldGenData &wg_data, WorldGenPlan &wg_plan) {



    for(WorldGenStructure & structure : wg_data.structure_data) {

        int count = GetRandomValue(structure.count_min, structure.count_max);
        StructureProfile &p = structure.profile;
        Vector2 structure_size = Vector2Add(structure.size, (Vector2){(float)structure.min_distance, (float)structure.min_distance});


        TraceLog(LOG_INFO, "placing structure: %s ", structure.id.c_str());

        for(int i = 0; i < count; i++) {
            std::vector<std::pair<InfluenceCell*, float>> candidates;


            for(InfluenceCell &cell : wg_plan.influence_grid) {

                if(!cell.reserved) {

                    float min_dist = (float)structure.min_distance * 16.0f;
                    
                    if(IsFarEnough(cell.rect, wg_plan.poi_patches, min_dist)) {

                        float score = 
                        (cell.center  * p.wants_center) + 
                        (cell.road  * p.wants_road) +
                        (cell.forrest  * p.wants_forrest) +
                        (cell.edge * p.wants_edge);

                        if (score > 0.001f) {

                            Vector2 center = {cell.rect.x + (cell.rect.width/2), cell.rect.y + (cell.rect.height/2)};

                            float structure_half_w = structure_size.x/2;
                            float structure_half_h = structure_size.y/2;

                            Rectangle  footprint;
                            footprint.x = center.x - structure_half_w;
                            footprint.y = center.y - structure_half_h;
                            footprint.width = structure_size.x;
                            footprint.height =structure_size.y;

                            if(IsRectFree(footprint, wg_plan.reserved_rects)) {
                                candidates.push_back({ &cell, score });
                                //wg_plan.reserved_rects.push_back(footprint);
                            }

                        }

                    }
                }
            }

            if(!candidates.empty()) {

                TraceLog(LOG_INFO, "choosing from %i candidate cells ", candidates.size());
                            
                std::sort(candidates.begin(), candidates.end(), [](const auto& a, const auto& b){
                    return a.second > b.second;
                });

                int top_range = std::min(5, (int)candidates.size());
                int choice = GetRandomValue(0, top_range - 1);

                InfluenceCell* chosen = candidates[choice].first;
                chosen->reserved = true;
               
                float cell_half_w = chosen->rect.width/2;
                float cell_half_h = chosen->rect.height/2;

                float structure_half_w = structure_size.x/2;
                float structure_half_h = structure_size.y/2;

                Vector2 center = {chosen->rect.x + cell_half_w, chosen->rect.y + cell_half_h};

                Rectangle  footprint;
                footprint.x = center.x - structure_half_w;
                footprint.y = center.y - structure_half_h;
                footprint.width = structure_size.x;
                footprint.height =structure_size.y;


                StructurePatch patch;
                patch.rect = footprint;
                patch.id = structure.id;

                // Store influence context (useful later for AI or loot logic)
                patch.road_proximity    = chosen->road;
                patch.shelter_proximity = chosen->shelter;
                patch.forrest_proximity = chosen->forrest;
                patch.center_proximity  = chosen->center;
                patch.edge_proximity    = chosen->edge;

                wg_plan.poi_patches.push_back(patch);

                wg_plan.reserved_rects.push_back(patch.rect);

                TraceLog(LOG_INFO, "Planned structure %s at %.0f %.0f",
                    structure.id.c_str(),
                    center.x,
                    center.y);
 
            }
            else {
                TraceLog(LOG_INFO, "no available candadate cells");
            }

        }
    }



    if(wg_data.has_shelter) {
        Vector2 shelter_position = wg_plan.road_midpoint;


        TraceLog(LOG_INFO, "++++++------------shelter position planned at %0.0f  %0.0f", shelter_position.x, shelter_position.y);
        wg_plan.structure_positions.push_back(shelter_position);
    }
}



void PlanRoads(WorldGenData &wg_data, WorldGenPlan &wg_plan) {

    for(int position_index = 0; position_index < wg_plan.exit_positions.size(); position_index++) {
        Vector2 start_position = wg_plan.road_midpoint;

        Rectangle new_rect;
        new_rect.x = start_position.x;
        new_rect.y = start_position.y;

        if(wg_plan.exit_positions[position_index].y == wg_plan.road_midpoint.y) {
            if(wg_plan.exit_positions[position_index].x < wg_plan.road_midpoint.x) {
                new_rect.x = 0;
                new_rect.width = start_position.x;
                new_rect.height = 2;
            }
            else {
                new_rect.width = wg_plan.map_size.x -  start_position.x;
                new_rect.height = 2;
            }
        }
        else {
            if(wg_plan.exit_positions[position_index].y < wg_plan.road_midpoint.y) {
                new_rect.y = 0;
                new_rect.height = start_position.y;
                new_rect.width = 2;
            }
            else {
                new_rect.height = wg_plan.map_size.y -  start_position.y;
                new_rect.width = 2; 
            }
        }

        wg_plan.reserved_rects.push_back(new_rect);


        for(int y = new_rect.y; y < new_rect.y + new_rect.height; y++){
            for(int x = new_rect.x; x < new_rect.x + new_rect.width; x++) {
                wg_plan.road_positions.push_back( (Vector2){(float)x, (float)y} );
                //TraceLog(LOG_INFO, "road pos------  (%i, %i) ", x, y);
            }
        }

    }
}


BIOME_TYPE ChooseBiome(WorldGenData &wg_data) {

    int total = 0;

    for(WorldGenBiomeData &biome_data : wg_data.biome_data) {
        //TraceLog(LOG_INFO, "-----------------biome id: %i  ", biome_data.type);
        total += biome_data.coverage * 100;
    }

        int roll = GetRandomValue(0, total - 1);
        int acc = 0;

    for(WorldGenBiomeData &biome_data : wg_data.biome_data) {
        acc += biome_data.coverage * 100;

        if(roll < acc) {
            return biome_data.type;
        }

    }
    return BIOME_NONE;
}


void SplitRegion(std::vector<Rectangle>& regions)
{

    int min_size = 10;

    int idx = GetRandomValue(0, regions.size()-1);
    Rectangle r = regions[idx];

    while(r.width < min_size*2 or r.height < min_size*2) {
        idx = GetRandomValue(0, regions.size()-1);
        r = regions[idx];
    }

    bool vertical = GetRandomValue(0,1);

    Rectangle a, b;

    if(vertical)
    {
        float cut = r.width * (GetRandomValue(35, 65) * 0.01f);

        a = { r.x, r.y, cut, r.height };
        b = { r.x + cut, r.y, r.width - cut, r.height };
    }
    else
    {
        float cut = r.height * (GetRandomValue(35, 65) * 0.01f);

        a = { r.x, r.y, r.width, cut };
        b = { r.x, r.y + cut, r.width, r.height - cut };
    }

    regions.erase(regions.begin() + idx);
    regions.push_back(a);
    regions.push_back(b);
}

WorldGenBiome *GetBiome(Vector2 position, WorldGenTileSet &_tileset) {


    for(WorldGenBiome &biome : _tileset.wg_plan.biomes) {
        if( (position.x > biome.rect.x and position.x < biome.rect.x + biome.rect.width ) and 
            (position.y > biome.rect.y and position.y < biome.rect.y + biome.rect.height)  ) {

                return &biome;
        }
    }

    return nullptr;
}


void CreatePoiPatches(WorldGenPlan &wg_plan, int patch_size) {

    int x_patches = (wg_plan.map_size.x/patch_size);
    int y_patches = (wg_plan.map_size.y/patch_size);
    
    TraceLog(LOG_INFO, "Generating POI Patches------ x patches %i   y patches %i  ", x_patches, y_patches);
    //int num_structure_patches = x_patches + y_patches;
    
    /* 

    for(int y_patch = 1; y_patch < y_patches - 1; y_patch++ ) {
        for(int x_patch = 1; x_patch < x_patches - 1; x_patch++) {
            int x_pos = (x_patch * x_patch_size);
            int y_pos = (y_patch * y_patch_size);

            Rectangle new_patch;
            new_patch.x = x_pos;
            new_patch.y = y_pos;
            new_patch.width = patch_size;
            new_patch.height = patch_size;

            if(new_patch.x < 0) {new_patch.x = 0;}
            if(new_patch.x > wg_plan.map_size.x-1) { new_patch.x = wg_plan.map_size.x-1;}
            if(new_patch.x + patch_size > wg_plan.map_size.x) {
                int diff = (new_patch.x + patch_size) - wg_plan.map_size.x;
                new_patch.width -= diff;
            }

            if(new_patch.y < 0) {new_patch.y = 0;}
            if(new_patch.y + patch_size > wg_plan.map_size.y-1) { new_patch.y = wg_plan.map_size.y-1;}
            if(new_patch.y + patch_size > wg_plan.map_size.y) {
                int diff = (new_patch.y + patch_size) - wg_plan.map_size.y;
                new_patch.height -= diff;
            }


            wg_plan.poi_patches.push_back(new_patch);

             TraceLog(LOG_INFO, "------ patch  x %0.0f   y %0.0f    w %0.0f    h %0.0f",
                new_patch.x,
                new_patch.y,
                new_patch.width,
                new_patch.height); 
        }
    } */
}


Rectangle GetAvailablePoiPatch(WorldGenTileSet &_tileset) {

/* 
    std::vector<int> index_choices;

    for(int i = 0; i < _tileset.wg_plan.poi_patches.size(); i++ ) {
        bool found = false;
        for(int patch : _tileset.wg_plan.used_poi_patches) {
            if(i == patch) {
                found = true;s
            }
        }

        if(!found) {index_choices.push_back(i);}
    }


    if(index_choices.size() < 1) {
        Rectangle empty_rect;
        return empty_rect;
    }
    
    int choice = index_choices[GetRandomValue(0, index_choices.size()-1)];

    _tileset.wg_plan.used_poi_patches.push_back(choice);

    return _tileset.wg_plan.poi_patches[choice];
    */
   Rectangle rect;
   return rect;

}



void PaintInfluence(std::vector<InfluenceCell>& grid, int w, int h, int start_x, int start_y, float strength, float decay, InfluenceType type) {
    std::queue<std::pair<int,int>> q;

    auto idx = [&](int x,int y){ return y*w+x; };

    GetInfluenceChannel(grid[idx(start_x,start_y)], type) = strength;
    q.push({start_x,start_y});

    while(!q.empty())
    {
        auto [x,y] = q.front();
        q.pop();

        float current =
            GetInfluenceChannel(grid[idx(x,y)], type);

        float next = current - decay;
        if(next <= 0) continue;

        const int dirs[8][2] =
        {
            {1,0},{-1,0},{0,1},{0,-1},
            {1,1},{1,-1},{-1,1},{-1,-1}
        };

        for(auto& d : dirs)
        {
            int nx = x + d[0];
            int ny = y + d[1];

            //if(nx<0||ny<0||nx>w||ny>h)
            if (nx < 0 || ny < 0 || nx >= w || ny >= h)
                continue;

            float& neighbor =
                GetInfluenceChannel(grid[idx(nx,ny)], type);

            if(neighbor >= next)
                continue;

            neighbor = next;
            q.push({nx,ny});
        }
    }
}



float& GetInfluenceChannel(InfluenceCell& c, InfluenceType t)
{
    switch(t)
    {
        case INF_ROAD:       return c.road;
        case INF_SHELTER:    return c.shelter;
        case INF_CENTER:    return c.center;
        case INF_EDGE:    return c.edge;
        default:             return c.forrest;
    }
}


void CreateInfluenceGrid(WorldGenPlan &wg_plan, int patch_size) {


    int x_patches = (wg_plan.map_size.x/patch_size);
    int y_patches = (wg_plan.map_size.y/patch_size);
    
    TraceLog(LOG_INFO, "Generating Influence Grid------  %i x  %i  ", x_patches, y_patches);

    for(int y_patch = 0; y_patch < y_patches; y_patch++ ) {
        for(int x_patch = 0; x_patch < x_patches; x_patch++) {
            InfluenceCell new_patch;
            new_patch.rect = {
                .x = (float)x_patch * patch_size,
                .y = (float)y_patch * patch_size,
                .width = (float)patch_size,
                .height = (float)patch_size
            };

            wg_plan.influence_grid.push_back(new_patch);

        }
    }



    int grid_w = wg_plan.map_size.x/patch_size;
    int grid_h = wg_plan.map_size.y/patch_size;

    for (Vector2 r_pos : wg_plan.road_positions) {
        int x = (int)r_pos.x;
        int y = (int)r_pos.y;
        int gx = (int)(x / patch_size);
        int gy = (int)(y / patch_size);
        PaintInfluence(wg_plan.influence_grid, grid_w, grid_h, gx, gy, 1.0f, 0.2f, INF_ROAD);
    }


    for (Vector2 pos : wg_plan.edge_positions) {
        int x = (int)pos.x;
        int y = (int)pos.y;
        int gx = (int)(x / patch_size);
        int gy = (int)(y / patch_size);
        PaintInfluence(wg_plan.influence_grid, grid_w, grid_h, gx, gy, 1.0f, 0.1f, INF_EDGE);
    }


    int c_gx = (int)(wg_plan.road_midpoint.x / patch_size);
    int c_gy = (int)(wg_plan.road_midpoint.y / patch_size);

    PaintInfluence(wg_plan.influence_grid, grid_w, grid_h, c_gx, c_gy, 1.0f, 0.1f, INF_CENTER);


    //CreatePoiPatches(new_plan, 10);

    int i = 0;
    for(InfluenceCell &cell : wg_plan.influence_grid) {
        
        int x = i%(int)(wg_plan.map_size.x/patch_size);
        int y = i/(int)(wg_plan.map_size.y/patch_size);
/*         TraceLog(LOG_INFO, "infl cell: #%i    (%i,%i)  r:%0.2f  c:%0.2f  e:%0.2f s:%0.2f f:%0.2f    rect %0.2f %0.2f %0.2f %0.2f ", i, x, y, 
            cell.road,
            cell.center,
            cell.edge,
            cell.shelter,
            cell.forrest,
            cell.rect.x,
            cell.rect.y,
            cell.rect.width,
            cell.rect.height            
        );  */
        i++;
    }

}


StructureProfile BuildStructureProfile(WorldGenStructure &structure) {
    StructureProfile p;

/*     switch(structure.biome)
    {
        case BIOME_PLAINS:
            p.wants_road   = 0.6f;
            p.wants_center = 0.5f;
            break;

        case BIOME_FORREST:
            p.wants_forrest = 1.0f;
            p.wants_road    = 0.2f;
            break;

        case BIOME_HILLS:
            p.wants_edge = 0.6f;
            break;
    }
 */
    // shelters tend to cluster slightly
    if(structure.id == "house"){
        p.wants_road = 0.5f;
        p.wants_edge = 0.0f;
        p.wants_center = 0.5f;
    }
    if(structure.id == "spawn"){
        p.wants_road = 0.5f;
        p.wants_edge = 0.0f;
        p.wants_center = -0.5f;
    }
    if(structure.id == "ruins"){
        p.wants_road = -0.5f;
        p.wants_edge = 0.5f;
        p.wants_center = 0.5f;
    }

    return p;
}




bool IsFarEnough(Rectangle& rect, std::vector<StructurePatch>& placed, float min_dist) {
    float min_dist_sq = min_dist * min_dist;

    for (StructurePatch& p : placed)
    {
        if (RectCenterDistSq(rect, p.rect) < min_dist_sq)
            return false;
    }

    return true;
}


bool IsRectFree(Rectangle& candidate, std::vector<Rectangle>& reserved) {

    for (const Rectangle& r : reserved)
    {
        if (CheckCollisionRecs(candidate, r))
            return false;
    }
    return true;
}







//unused

/* //stamp rectangles of dirt
void GenerateDirtZonesRect(LDTKLevel &level, WorldGenTileSet &_tileset) {

    std::vector<Rectangle> dirt_rects;

    int num_dirt_patches = (level.px_wid/_tileset.tile_grid_size);

    for(int patch = 0; patch < num_dirt_patches; patch++ ) {
        Rectangle new_rect;
        new_rect.x = (float)GetRandomValue(1, _tileset.map_size.x-5);
        new_rect.y = (float)GetRandomValue(1, _tileset.map_size.y-5);
        new_rect.width = (float)GetRandomValue(2, 10);
        new_rect.height = (float)GetRandomValue(2, 10);

        dirt_rects.push_back(new_rect);
    }

    Rectangle spawn_rect;
    spawn_rect.x = (float)(level.px_wid/_tileset.tile_grid_size)/2 - 7;
    spawn_rect.y = (float)(level.px_hei/_tileset.tile_grid_size)/2 - 7;
    spawn_rect.width = 15;
    spawn_rect.height = 15;

    dirt_rects.push_back(spawn_rect);

    for(Rectangle &patch : dirt_rects) {
        for(int y = (int)patch.y; y < (int)patch.height + (int)patch.y; y++) {
            for(int x = (int)patch.x; x < (int)patch.width + (int)patch.x; x++) {
                int index = y * (int)_tileset.map_size.x + x;
                if(index < _tileset.lower_zone_grid.size()-1) {
                    _tileset.lower_zone_grid[index] = ZONE_DIRT;
                }
            }
        }
    }
}


//uses rect as a brush
void GenerateDirtZonesBrush(LDTKLevel &level, WorldGenTileSet &_tileset, int brush_size) {

    std::vector<Vector2> dirt_positions;
    std::vector<Vector2> patch_positions;

    int x_patches = _tileset.map_size.x/10;
    int y_patches = _tileset.map_size.y/10;
    int num_structure_patches = x_patches + y_patches;

    int x_patch_size = (level.px_wid/_tileset.tile_grid_size)/x_patches;
    int y_patch_size = (level.px_hei/_tileset.tile_grid_size)/y_patches;

    for(int y_patch = 0; y_patch <= y_patches; y_patch++ ) {
        for(int x_patch = 0; x_patch <= x_patches; x_patch++) {
            int x_pos = 10 + (x_patch * x_patch_size);
            int y_pos = 10 + (y_patch * y_patch_size);
            patch_positions.push_back(Vector2{(float)x_pos, (float)y_pos});
        }
    }


    for(int i = 0; i < _tileset.wg_data.terrain.dirt_patches; i++ ) {
        Vector2 selection = patch_positions[GetRandomValue(0, patch_positions.size() -1)];
        dirt_positions.push_back(selection);
    }

    patch_positions.push_back(Vector2{_tileset.map_size.x/2, _tileset.map_size.y/2});


    Rectangle brush;
    brush.x = 0;
    brush.y = 0;
    brush.width = brush_size;
    brush.height = brush_size;

    int max_loops = 150;

    for(Vector2 position : dirt_positions) {
        brush.x = position.x;
        brush.y = position.y;
        bool running = true;
        int loops = 0;

        //TraceLog(LOG_INFO, "brush start pos %0.0f %0.0f", brush.x, brush.y);
        while(running) {
            
            for(int y = (int)brush.y; y < (int)brush.height + (int)brush.y; y++) {
                for(int x = (int)brush.x; x < (int)brush.width + (int)brush.x; x++) {
                    int index = y * (int)_tileset.map_size.x + x;
                    if(index < _tileset.lower_zone_grid.size()-1) {
                        _tileset.lower_zone_grid[index] = ZONE_DIRT;
                    }
                }
            }

            brush.x += GetRandomValue(-1, 1);

            if(brush.x < 1) {
                brush.x = 1;
                //running = false;
            }
            if(brush.x + brush.width >= _tileset.map_size.x-2) {
                brush.x -= ((brush.x + brush.width) - _tileset.map_size.x-3);
                //running = false;
            }
            
            brush.y += GetRandomValue(-1, 1);

            if(brush.y < 1) {
                brush.y = 1;
                //running = false;
            }
            if(brush.y + brush.height >= _tileset.map_size.y-2) {
                brush.y -= ((brush.y + brush.height) - _tileset.map_size.y-3);
                //running = false;
            }
            loops++;
            if(loops > max_loops) {
                running = false;
            }
        }
    }
}

void GenerateTreeZones(LDTKLevel &level, WorldGenTileSet &_tileset) {
/////
//// I dont think this does anything
//// the final algorythm does not use this unformation.
/////
    std::vector<Rectangle> tree_rects;

    int num_tree_patches = (level.px_wid/_tileset.tile_grid_size) / 5;
    for(int patch = 0; patch < num_tree_patches; patch++ ) {
        Rectangle new_rect;
        new_rect.x = (float)GetRandomValue(15, _tileset.map_size.x-15);
        new_rect.y = (float)GetRandomValue(15, _tileset.map_size.y-15);
        new_rect.width = (float)GetRandomValue(2, 5);
        new_rect.height = (float)GetRandomValue(2, 5);

        tree_rects.push_back(new_rect);
    }

    for(Rectangle &patch : tree_rects) {
        for(int y = (int)patch.y; y < (int)patch.height + (int)patch.y; y++) {
            for(int x = (int)patch.x; x < (int)patch.width + (int)patch.x; x++) {
                int index = y * (int)_tileset.map_size.x + x;
                if(index < _tileset.upper_zone_grid.size()-1) {
                    if(GetRandomValue(0, 100) >70) {
                        _tileset.upper_zone_grid[index] = ZONE_TREE;
                    }
                }
            }
        }
    }
}

void GenerateRuinsZones(LDTKLevel &level, WorldGenTileSet &_tileset) {

    TraceLog(LOG_INFO, "GENERATING RUINS ZONES ");
    /* std::vector<Rectangle> shroom_rects; 

    int num_ruins_patches = 5;

    for(int patch = 0; patch < num_ruins_patches; patch++ ) {
        Rectangle new_rect = GetAvailableSpawnPatch(_tileset);

        _tileset.wg_plan.ruins_rects.push_back(new_rect);

        //_tileset.layer_decco_positions.push_back({new_rect.x + (new_rect.width/2), new_rect.y + (new_rect.height/2)});

        WorldGenDeccoEntityData new_decco_entity;
        new_decco_entity.decco_name = "DeccoEntity_1";
        new_decco_entity.position = {new_rect.x + (new_rect.width/2), new_rect.y + (new_rect.height/2)};

        _tileset.entity_decco_data.push_back(new_decco_entity);

        WorldGenDeccoEntityData new_layer_entity;
        new_layer_entity.decco_name = "LayerDecco_1";
        new_layer_entity.position = {new_rect.x + (new_rect.width/2), new_rect.y + (new_rect.height/2)};

        _tileset.entity_decco_data.push_back(new_layer_entity);
    }


    for(Rectangle &patch : _tileset.wg_plan.ruins_rects) {
        for(int y = (int)patch.y; y < (int)patch.height + (int)patch.y; y++) {
            for(int x = (int)patch.x; x < (int)patch.width + (int)patch.x; x++) {
                int index = y * (int)_tileset.map_size.x + x;

                //TraceLog(LOG_INFO, "shroom  pos %i %i", x, y);
                if(_tileset.upper_zone_grid[index] != ZONE_STRUCTURE) {
                    _tileset.upper_zone_grid[index] = ZONE_RUINS;
                    _tileset.lower_zone_grid[index] = ZONE_DIRT;
                    
                    if(x == (patch.width/2) + patch.x and  y == (patch.height/2) + patch.y ) {
                        _tileset.spawn_zone_grid[index] = ZONE_CREATURE;
                    }
                }

            }
        }
    }
}


void GenerateCreatureZones(LDTKLevel &level, WorldGenTileSet &_tileset) {

    TraceLog(LOG_INFO, "GENERATING CREATURE ZONES ");
    int num_creatures = 1;

    std::vector<Rectangle> patches;

    for(int creature_index = 0; creature_index < num_creatures; creature_index++ ) {
        patches.push_back(GetAvailableSpawnPatch(_tileset));
    }

    for(Rectangle patch : patches) {
        for(int y = (int)patch.y; y < (int)patch.height + (int)patch.y; y++) {
            for(int x = (int)patch.x; x < (int)patch.width + (int)patch.x; x++) {
                int index = y * (int)_tileset.map_size.x + x;
                if(_tileset.upper_zone_grid[index] != ZONE_BORDER and _tileset.upper_zone_grid[index] != ZONE_STRUCTURE) {
                    if(GetRandomValue(0, 100) > 90) {
                        _tileset.spawn_zone_grid[index] = ZONE_CREATURE;
                    }
                }
            }
        }
    }
}




void GenerateStartingShelter(LDTKLevel &level, WorldGenTileSet &_tileset) {


    Rectangle shelter_rect = GetAvailablePoiPatch(_tileset);

    Vector2 rect_center = {shelter_rect.x + (shelter_rect.width/2), shelter_rect.y + (shelter_rect.height/2)};

    shelter_rect.x = rect_center.x - 5;
    shelter_rect.y = rect_center.y - 5;
    shelter_rect.width = 10;
    shelter_rect.height = 10;

    //should be at the first index
    _tileset.wg_plan.structure_positions.push_back( rect_center );



    for(int y = 0; y < shelter_rect.height; y++) {
        for(int x = 0; x < shelter_rect.width; x++) {

            Vector2 position;
            position.x = x + shelter_rect.x;
            position.y = y + shelter_rect.y;

            if(position.x < 0){position.x = 0;}
            if(position.x > _tileset.map_size.x-1) {position.x = _tileset.map_size.x-1;}

            if(position.y < 0){position.y = 0;}
            if(position.y > _tileset.map_size.y-1) {position.y = _tileset.map_size.y-1;}


            int index = position.y * _tileset.map_size.x + position.x;

            _tileset.upper_zone_grid[index] = ZONE_STRUCTURE;
            _tileset.lower_zone_grid[index] = ZONE_DIRT;

            //TraceLog(LOG_INFO, "structure  pos %0.0f %0.0f", position.x, position.y);
            
        }
    }
    /* Rectangle fr = shelter_rect; //fence rect
    fr.x -=1;
    fr.y -=1;
    fr.width +=2;
    fr.height +=2;

    if(fr.x < 1) {fr.x = 1;};
    if(fr.y < 1) {fr.y = 1;};
    if(fr.y + fr.height > _tileset.map_size.y - 1) {fr.height = fr.height - (fr.y + fr.height - _tileset.map_size.y - 1); }
    if(fr.x + fr.width > _tileset.map_size.x - 1) {fr.width = fr.width - (fr.x + fr.width - _tileset.map_size.x - 1); };
    if(fr.y + fr.height > _tileset.map_size.y - 1) {fr.height = fr.height - (fr.y + fr.height - _tileset.map_size.y - 1); }

    for(int y = (int)fr.y; y < (int)fr.height + (int)fr.y; y++) {
        for(int x = (int)fr.x; x < (int)fr.width + (int)fr.x; x++) {
            if(x == (int)fr.x or y == (int)fr.y or x == (int)fr.width - 1 + (int)fr.x or y == (int)fr.height - 1 + (int)fr.y) {
                int index = y * (int)_tileset.map_size.x + x;
                _tileset.upper_zone_grid[index] = ZONE_FENCE;
                //_tileset.lower_zone_grid[index] = ZONE_GRASS;
                _tileset.collision_grid[index] = 1;
            }
        }
    } 



}


void GenerateVillageZone(LDTKLevel &level, WorldGenTileSet &_tileset) {


    Rectangle new_rect = GetAvailablePoiPatch(_tileset);
                

    //clamp
    if(new_rect.x + new_rect.width > _tileset.map_size.x - 5) {new_rect.width = new_rect.width - (new_rect.x + new_rect.width - _tileset.map_size.x); };
    if(new_rect.y + new_rect.height > _tileset.map_size.y - 5) {new_rect.height = new_rect.height - (new_rect.y + new_rect.height - _tileset.map_size.y); }
    
    Vector2 rect_center = {new_rect.x + (new_rect.width/2), new_rect.y + (new_rect.height/2)};

    _tileset.wg_plan.structure_positions.push_back( rect_center );


    for(int y = 0; y < new_rect.height; y++) {
        for(int x = 0; x < new_rect.width; x++) {

            Vector2 position;
            position.x = x + new_rect.x;
            position.y = y + new_rect.y;

            if(position.x < 0){position.x = 0;}
            if(position.x > _tileset.map_size.x-1) {position.x = _tileset.map_size.x-1;}

            if(position.y < 0){position.y = 0;}
            if(position.y > _tileset.map_size.y-1) {position.y = _tileset.map_size.y-1;}


            int index = position.y * _tileset.map_size.x + position.x;

            if(_tileset.upper_zone_grid[index] != ZONE_BORDER) {
                _tileset.upper_zone_grid[index] = ZONE_STRUCTURE;
                //_tileset.lower_zone_grid[index] = ZONE_DIRT;
            }

        }
    }

    Rectangle fr = new_rect; //fence rect
    fr.x -=1;
    fr.y -=1;
    fr.width +=2;
    fr.height +=2;


        //clamp
    if(fr.x < 1) {fr.x = 1;};
    if(fr.y < 1) {fr.y = 1;};
    if(fr.y + fr.height > _tileset.map_size.y - 1) {fr.height = fr.height - (fr.y + fr.height - _tileset.map_size.y - 1); }
    if(fr.x + fr.width > _tileset.map_size.x - 1) {fr.width = fr.width - (fr.x + fr.width - _tileset.map_size.x - 1); };
    if(fr.y + fr.height > _tileset.map_size.y - 1) {fr.height = fr.height - (fr.y + fr.height - _tileset.map_size.y - 1); }
    

    for(int y = (int)fr.y; y < (int)fr.height + (int)fr.y; y++) {
        for(int x = (int)fr.x; x < (int)fr.width + (int)fr.x; x++) {


            if(x == (int)fr.x or y == (int)fr.y or x == (int)fr.width - 1 + (int)fr.x or y == (int)fr.height - 1 + (int)fr.y) {
                int index = y * (int)_tileset.map_size.x + x;
                _tileset.upper_zone_grid[index] = ZONE_FENCE;
                //_tileset.lower_zone_grid[index] = ZONE_GRASS;
                _tileset.collision_grid[index] = 1;
            }
        }
    }


}


void GenerateStructureZone(LDTKLevel &level, WorldGenTileSet &_tileset) {


    Rectangle structure_rect = GetAvailableSpawnPatch(_tileset);

    Vector2 rect_center = {structure_rect.x + (structure_rect.width/2), structure_rect.y + (structure_rect.height/2)};


    structure_rect.x = rect_center.x - 5;
    structure_rect.y = rect_center.y - 5;
    structure_rect.width = 10;
    structure_rect.height = 10;
    //should be at the first index
    _tileset.wg_plan.structure_positions.push_back( rect_center );


    for(int y = 0; y < structure_rect.height; y++) {
        for(int x = 0; x < structure_rect.width; x++) {

            Vector2 position;
            position.x = x + structure_rect.x;
            position.y = y + structure_rect.y;

            if(position.x < 0){position.x = 0;}
            if(position.x > _tileset.map_size.x-1) {position.x = _tileset.map_size.x-1;}

            if(position.y < 0){position.y = 0;}
            if(position.y > _tileset.map_size.y-1) {position.y = _tileset.map_size.y-1;}


            int index = position.y * _tileset.map_size.x + position.x;

            _tileset.upper_zone_grid[index] = ZONE_STRUCTURE;
            _tileset.lower_zone_grid[index] = ZONE_DIRT;

            //TraceLog(LOG_INFO, "structure  pos %0.0f %0.0f", position.x, position.y);
            
        }
    }
    /* Rectangle fr = shelter_rect; //fence rect
    fr.x -=1;
    fr.y -=1;
    fr.width +=2;
    fr.height +=2;

    if(fr.x < 1) {fr.x = 1;};
    if(fr.y < 1) {fr.y = 1;};
    if(fr.y + fr.height > _tileset.map_size.y - 1) {fr.height = fr.height - (fr.y + fr.height - _tileset.map_size.y - 1); }
    if(fr.x + fr.width > _tileset.map_size.x - 1) {fr.width = fr.width - (fr.x + fr.width - _tileset.map_size.x - 1); };
    if(fr.y + fr.height > _tileset.map_size.y - 1) {fr.height = fr.height - (fr.y + fr.height - _tileset.map_size.y - 1); }

    for(int y = (int)fr.y; y < (int)fr.height + (int)fr.y; y++) {
        for(int x = (int)fr.x; x < (int)fr.width + (int)fr.x; x++) {
            if(x == (int)fr.x or y == (int)fr.y or x == (int)fr.width - 1 + (int)fr.x or y == (int)fr.height - 1 + (int)fr.y) {
                int index = y * (int)_tileset.map_size.x + x;
                _tileset.upper_zone_grid[index] = ZONE_FENCE;
                //_tileset.lower_zone_grid[index] = ZONE_GRASS;
                _tileset.collision_grid[index] = 1;
            }
        }
    } 
}


void GenerateStructureZones(LDTKLevel &level, WorldGenTileSet &_tileset) {

   /*  std::vector<Rectangle> structure_rects;
    

    int x_patches = _tileset.max_structures/2;
    int y_patches = _tileset.max_structures/2;
    int num_structure_patches = x_patches + y_patches;

    int x_patch_size = (level.px_wid/_tileset.tile_grid_size)/x_patches;
    int y_patch_size = (level.px_hei/_tileset.tile_grid_size)/y_patches;

    for(int y_patch = 0; y_patch < y_patches; y_patch++ ) {
        for(int x_patch = 0; x_patch < x_patches; x_patch++) {

            if(y_patch != 1 or  x_patch != 1) {

                
                int x_pos = x_patch * x_patch_size;
                int y_pos = y_patch * y_patch_size;
                
                Rectangle new_rect;
                
                new_rect.x = (float)GetRandomValue(x_pos, x_pos + x_patch_size);
                if(new_rect.x < 2) {new_rect.x = 2; }

                new_rect.y = (float)GetRandomValue(y_pos, y_pos + y_patch_size);
                if(new_rect.y < 2) {new_rect.y = 2; }

                new_rect.width = 10 + GetRandomValue(5, 10);
                if(new_rect.x + new_rect.width > _tileset.map_size.x - 5) {new_rect.width = new_rect.width - (new_rect.x + new_rect.width - _tileset.map_size.x); }

                new_rect.height = 10 + GetRandomValue(5, 10);
                if(new_rect.y + new_rect.height > _tileset.map_size.y - 5) {new_rect.height = new_rect.height - (new_rect.y + new_rect.height - _tileset.map_size.y); }

                structure_rects.push_back(new_rect);
            }
        }
    }

    //spawn position
    _tileset.structure_positions.push_back({ (float)(level.px_wid/_tileset.tile_grid_size)/2, (float)(level.px_hei/_tileset.tile_grid_size)/2});


    for(Rectangle &patch : structure_rects) {
        for(int y = (int)patch.y; y < (int)patch.height + (int)patch.y; y++) {
            for(int x = (int)patch.x; x < (int)patch.width + (int)patch.x; x++) {
                int index = y * (int)_tileset.map_size.x + x;

                if(index < _tileset.upper_zone_grid.size()-1) {
                    if(_tileset.upper_zone_grid[index] != ZONE_BORDER) {
                        _tileset.upper_zone_grid[index] = ZONE_STRUCTURE;
                        _tileset.lower_zone_grid[index] = ZONE_DIRT;
                    }
                    if(x == (int)(patch.width/2) + (int)patch.x and y == (int)(patch.height/2) + (int)patch.y) {
                        _tileset.structure_positions.push_back({(float)x,(float)y});
                    }
                }
            }
        }
        //make fence
        Rectangle fr = patch; //fence rect
        fr.x -=1;
        fr.y -=1;
        fr.width +=2;
        fr.height +=2;

        for(int y = (int)fr.y; y < (int)fr.height + (int)fr.y; y++) {
            for(int x = (int)fr.x; x < (int)fr.width + (int)fr.x; x++) {
                if(x == (int)fr.x or y == (int)fr.y or x == (int)fr.width - 1 + (int)fr.x or y == (int)fr.height - 1 + (int)fr.y) {
                    int index = y * (int)_tileset.map_size.x + x;
                    _tileset.upper_zone_grid[index] = ZONE_FENCE;
                    _tileset.lower_zone_grid[index] = ZONE_DIRT;
                    _tileset.collision_grid[index] = 1;
                }
            }
        }
    } 

}


void GenerateHillZonesRect(LDTKLevel &level, WorldGenTileSet &_tileset) {
    std::vector<Rectangle> hill_rects;
    int num_hill_patches = 5;

    for(int patch = 0; patch < num_hill_patches; patch++ ) {
        Rectangle new_rect;
        new_rect.x = (float)GetRandomValue(1, _tileset.map_size.x-5);
        new_rect.y = (float)GetRandomValue(1, _tileset.map_size.y-5);
        new_rect.width = GetRandomValue(6, 25);
        new_rect.height = GetRandomValue(6, 25);

        hill_rects.push_back(new_rect);
    }

    for(Rectangle &patch : hill_rects) {
        for(int y = (int)patch.y; y < (int)patch.height + (int)patch.y; y++) {
            for(int x = (int)patch.x; x < (int)patch.width + (int)patch.x; x++) {
                int index = y * (int)_tileset.map_size.x + x;
                if(index < _tileset.upper_zone_grid.size()-1) {
                    if(_tileset.upper_zone_grid[index] != ZONE_STRUCTURE) {
                        _tileset.upper_zone_grid[index] = ZONE_BORDER;
                        _tileset.collision_grid[index] = 1;
                    }
                }
            }
        }
    }
}


void GenerateExits(LDTKLevel &level, WorldGenTileSet &_tileset) {

    int fourty_percent_x = (int)(_tileset.map_size.x * 0.4f);
    int fourty_percent_y = (int)(_tileset.map_size.y * 0.4f);

    int max_exits = _tileset.wg_plan.exit_dest_strings.size();
    
    //Vector2 mid_point;
    _tileset.wg_plan.road_midpoint.x = GetRandomValue(fourty_percent_x, _tileset.map_size.x - fourty_percent_x);
    _tileset.wg_plan.road_midpoint.y = GetRandomValue(fourty_percent_y, _tileset.map_size.y - fourty_percent_y);


    std::vector<Vector2> exit_positions;
 
    Vector2 up = {_tileset.wg_plan.road_midpoint.x, 1};
    Vector2 right = { _tileset.map_size.x - 1, _tileset.wg_plan.road_midpoint.y};
    Vector2 down = {_tileset.wg_plan.road_midpoint.x, _tileset.map_size.y - 1};
    Vector2 left = {1, _tileset.wg_plan.road_midpoint.y};

    exit_positions.push_back(up);
    exit_positions.push_back(right);
    exit_positions.push_back(down);
    exit_positions.push_back(left);

    for(int i = 0; i < exit_positions.size(); i++) {
        int j = GetRandomValue(0, i);
        std::swap(exit_positions[i], exit_positions[j]);
    }


    //std::shuffle(exit_positions.begin(), exit_positions.end(), GetRandomValue(0, 10000));


    for(int exit = 0; exit < _tileset.wg_plan.exit_dest_strings.size(); exit++) {
        //int dir = GetRandomValue(0, exit_positions.size() - 1);
        
        Vector2 exit_pos = exit_positions[exit];

         if(dir == 0) {exit_pos = {_tileset.road_midpoint.x, 1};}
        if(dir == 1) {exit_pos = {_tileset.map_size.x - 1, _tileset.road_midpoint.y};}
        if(dir == 2) {exit_pos = {_tileset.road_midpoint.x, _tileset.map_size.y - 1};}
        if(dir == 3) {exit_pos = {1, _tileset.road_midpoint.y};} 

        _tileset.wg_plan.exit_positions.push_back(exit_pos);
    }
}



void GenerateHillZonesBrush(LDTKLevel &level, WorldGenTileSet &_tileset, int brush_size) {

    std::vector<Vector2> hill_positions;
    std::vector<Vector2> patch_positions;


    int x_patches = _tileset.map_size.x/50;
    int y_patches = _tileset.map_size.y/50;

    int x_patch_size = (level.px_wid/_tileset.tile_grid_size)/x_patches;
    int y_patch_size = (level.px_hei/_tileset.tile_grid_size)/y_patches;


    for(int y_patch = 0; y_patch <= y_patches; y_patch++ ) {
        for(int x_patch = 0; x_patch <= x_patches; x_patch++) {
            int x_pos = 10 + (x_patch * x_patch_size);
            int y_pos = 10 + (y_patch * y_patch_size);
            patch_positions.push_back(Vector2{(float)x_pos + GetRandomValue(-5, 5), (float)y_pos + GetRandomValue(-5, 5)});
        }
    }


    for(int i = 0; i < _tileset.wg_data.terrain.hill_patches; i++ ) {
        Vector2 selection = patch_positions[GetRandomValue(0, patch_positions.size() -1)];
        hill_positions.push_back(selection);
    }


    Rectangle brush;
    brush.x = 0;
    brush.y = 0;
    brush.width = brush_size;
    brush.height = brush_size;

    int max_loops = 150;

    for(Vector2 position : hill_positions) {
        brush.x = position.x;
        brush.y = position.y;
        bool running = true;
        int loops = 0;

        //TraceLog(LOG_INFO, "brush start pos %0.0f %0.0f", brush.x, brush.y);
        while(running) {
            
            for(int y = (int)brush.y; y < (int)brush.height + (int)brush.y; y++) {
                for(int x = (int)brush.x; x < (int)brush.width + (int)brush.x; x++) {

                    int index = y * (int)_tileset.map_size.x + x;
                    if(index < _tileset.upper_zone_grid.size()-1) {
                        if(_tileset.upper_zone_grid[index] != ZONE_STRUCTURE and _tileset.upper_zone_grid[index] != ZONE_RUINS) {
                            _tileset.upper_zone_grid[index] = ZONE_BORDER;
                        }
                    }
                }
            }

            brush.x += GetRandomValue(-1, 1);

            if(brush.x < 1) {
                brush.x = 1;
                //running = false;
            }
            if(brush.x + brush.width >= _tileset.map_size.x-2) {
                brush.x -= ((brush.x + brush.width) - _tileset.map_size.x-3);
                //running = false;
            }
            
            brush.y += GetRandomValue(-1, 1);

            if(brush.y < 1) {
                brush.y = 1;
                //running = false;
            }
            if(brush.y + brush.height >= _tileset.map_size.y-2) {
                brush.y -= ((brush.y + brush.height) - _tileset.map_size.y-3);
                //running = false;
            }
            loops++;
            if(loops > max_loops) {
                running = false;
            }
        }
    }
}


void FillWithHillZone(LDTKLevel &level, WorldGenTileSet &_tileset) {

    for(int y = 0; y < _tileset.map_size.y; y++) {
        for(int x = 0; x < _tileset.map_size.x; x++) {
            int index = y * _tileset.map_size.x + x;
            _tileset.upper_zone_grid[index] = ZONE_BORDER;
        }
    }

}

void CarveEmptySpace(LDTKLevel &level, WorldGenTileSet &_tileset) {

    std::vector<Vector2> start_positions;

    int brush_size = 15;
    int x_patches = _tileset.map_size.x/20;
    int y_patches = _tileset.map_size.y/20;
    int num_structure_patches = x_patches + y_patches;

    int x_patch_size = (level.px_wid/_tileset.tile_grid_size)/x_patches;
    int y_patch_size = (level.px_hei/_tileset.tile_grid_size)/y_patches;

    for(int y_patch = 1; y_patch < y_patches - 1; y_patch++ ) {
        for(int x_patch = 1; x_patch < x_patches - 1; x_patch++) {
            int x_pos = 10 + (x_patch * x_patch_size);
            int y_pos = 10 + (y_patch * y_patch_size);
            start_positions.push_back(Vector2{(float)x_pos + GetRandomValue(-5, 5), (float)y_pos + GetRandomValue(-5, 5)});
        }
    }


    Rectangle brush;
    brush.x = 0;
    brush.y = 0;
    brush.width = brush_size;
    brush.height = brush_size;

    int max_loops = 100;

    for(Vector2 position : start_positions) {
        brush.x = position.x;
        brush.y = position.y;
        bool running = true;
        int loops = 0;

        //TraceLog(LOG_INFO, "brush start pos %0.0f %0.0f", brush.x, brush.y);
        while(running) {
            
            for(int y = (int)brush.y; y < (int)brush.height + (int)brush.y; y++) {
                for(int x = (int)brush.x; x < (int)brush.width + (int)brush.x; x++) {

                    if(x > 0 and x < _tileset.map_size.x-1 and y > 0 and y < _tileset.map_size.y-1){

                        int index = y * (int)_tileset.map_size.x + x;
                        _tileset.upper_zone_grid[index] = ZONE_NONE;
                        
                        //if(index < _tileset.upper_zone_grid.size()-1) {  
                        //}
                    }
                }
            }

            brush.x += GetRandomValue(-1, 1);

            if(brush.x < 1) {
                brush.x = 1;
                //running = false;
            }
            if(brush.x + brush.width >= _tileset.map_size.x-2) {
                brush.x -= ((brush.x + brush.width) - _tileset.map_size.x-3);
                //running = false;
            }
            
            brush.y += GetRandomValue(-1, 1);

            if(brush.y < 1) {
                brush.y = 1;
                //running = false;
            }
            if(brush.y + brush.height >= _tileset.map_size.y-2) {
                brush.y -= ((brush.y + brush.height) - _tileset.map_size.y-3);
                //running = false;
            }
            loops++;
            if(loops > max_loops) {
                running = false;
            }
        }
    }

}
 */