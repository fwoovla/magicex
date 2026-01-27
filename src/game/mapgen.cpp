#include "../core/gamedefs.h"
#include "../core/layergen.h"
#include "../core/pathgen.h"
#include "../core/scenerygen.h"

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
            TraceLog(LOG_INFO, "-------tag # %i", tag);
            for(int id = 0; id < g_worldgen_tilesets[set].tile_tags[tag].tile_ids.size(); id++) {
                TraceLog(LOG_INFO, "------------tile # %i", g_worldgen_tilesets[set].tile_tags[tag].tile_ids[id]);
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
                        else if(this_layer.identifier == "PremadeStructures") {
                            json *bounds_layer = nullptr;

                            for(int l = 0; l < j["levels"][level]["layerInstances"].size(); l++) {
                                if( j["levels"][level]["layerInstances"][l]["__identifier"] == "PremadeStructureBounds") {
                                    bounds_layer = &j["levels"][level]["layerInstances"][l];
                                    break;
                                }
                            }

                            if(bounds_layer) {
                                std::vector<LDTKEntityInstance> structure_bounding_entities;
                                ExtractStructureBounds(*bounds_layer, structure_bounding_entities);
                                BuildPremadeStructures(j["levels"][level]["layerInstances"][layer]["gridTiles"], *this_tileset, structure_bounding_entities);
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

void GenerateMap(LDTKLevel &new_level, int tileset_uid, Vector2 _map_size) {

    WorldGenTileSet *this_tilset = nullptr;

    for(int ts = 0; ts < g_worldgen_tilesets.size(); ts++) {
        if(g_worldgen_tilesets[ts].uid == tileset_uid) {
            this_tilset = &g_worldgen_tilesets[ts];
        }
    }

    if(this_tilset == nullptr) {
        TraceLog(LOG_INFO, "could not find tileset %i", tileset_uid);
        return;
    
    }

    this_tilset->map_size = _map_size;

    this_tilset->sorted_tiles.grass_tiles.clear();
    this_tilset->sorted_tiles.dirt_tiles.clear();
    this_tilset->sorted_tiles.path_tiles.clear();
    this_tilset->sorted_tiles.border_tiles.clear();

    for(auto &tile : this_tilset->tile_lookup) {
        TILEID tile_id = (TILEID)tile.first;

        if(tile_id > TILE_ID_GRASS_START and tile_id < TILE_ID_GRASS_END) {
            this_tilset->sorted_tiles.grass_tiles.push_back(tile_id);
        }
        if(tile_id > TILE_ID_BORDER_START and tile_id < TILE_ID_BORDER_END) {
            this_tilset->sorted_tiles.border_tiles.push_back(tile_id);
        }
        if(tile_id > TILE_ID_PATH_START and tile_id < TILE_ID_PATH_END) {
            this_tilset->sorted_tiles.path_tiles.push_back(tile_id);
        }
        if(tile_id > TILE_ID_DIRT_START and tile_id < TILE_ID_DIRT_END) {
            this_tilset->sorted_tiles.dirt_tiles.push_back(tile_id);
        }
    }

    this_tilset->paths.clear();

    this_tilset->collision_grid.clear();
    this_tilset->collision_grid.resize((int)(this_tilset->map_size.x + 1) * (int)(this_tilset->map_size.y + 1), 0);
    this_tilset->lower_zone_grid.clear();
    this_tilset->lower_zone_grid.resize((int)(this_tilset->map_size.x + 1) * (int)(this_tilset->map_size.y + 1), ZONE_NONE);
    this_tilset->upper_zone_grid.clear();
    this_tilset->upper_zone_grid.resize((int)(this_tilset->map_size.x + 1) * (int)(this_tilset->map_size.y + 1), ZONE_NONE);

    new_level.identifier = "WORLD GEN LEVEL";
    new_level.px_wid = this_tilset->map_size.x * this_tilset->tile_grid_size;
    new_level.px_hei = this_tilset->map_size.y * this_tilset->tile_grid_size;


    //grass layer                           X
    //border                                X
    //structure areas                       X
    //structure positions                   X
    //structure tiles                       X
    //dirt patches                          x
    //paths connect structure areas         X
    //obsticles                             X
    //trees                                 X
    //grass                                 X
    //spawn point                           X
    //transition areas                      1/2
    //lootables                             -
    //mobs                                  -
    //fix sub maps                          X
    //fix paths                             -

    GenerateZones(new_level, *this_tilset);

    GenerateEntitiesLayer(new_level, *this_tilset);

    ConnectStructuresWithPaths(*this_tilset);

    GenerateStructuresLayer(new_level, *this_tilset,  g_worldgen_tilesets[1]);

    GenerateUpperTerrainLayer(new_level, *this_tilset);

    GenerateLowerTerrainLayer(new_level, *this_tilset);

    GenerateCollisionLayer(new_level, *this_tilset);

    PopulateGrass(new_level, *this_tilset);

    PopulateTrees(new_level, *this_tilset);

    PlaceEntities(new_level, *this_tilset);

}




void GenerateZones(LDTKLevel &level, WorldGenTileSet &_tileset) {

    //baseline grass on lower
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

    GenerateDirtZones(level, _tileset);
    GenerateTreeZones(level, _tileset);
    GenerateStructureZones(level, _tileset);
    GenerateHillZones(level, _tileset);
    //GenerateDirtZones(level, _tileset);

}







void GenerateDirtZones(LDTKLevel &level, WorldGenTileSet &_tileset) {

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


void GenerateTreeZones(LDTKLevel &level, WorldGenTileSet &_tileset) {

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


void GenerateStructureZones(LDTKLevel &level, WorldGenTileSet &_tileset) {

    std::vector<Rectangle> structure_rects;
    

    int x_patches = 3;
    int y_patches = 3;
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
                new_rect.y = (float)GetRandomValue(y_pos, y_pos + y_patch_size);
                new_rect.width = 10 + GetRandomValue(5, 10);
                new_rect.height = 10 + GetRandomValue(5, 10);
                
                structure_rects.push_back(new_rect);
            }
        }
           
    }

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
    }

}


void GenerateHillZones(LDTKLevel &level, WorldGenTileSet &_tileset) {
    std::vector<Rectangle> hill_rects;
    int num_hill_patches = 1 + ((level.px_wid/_tileset.tile_grid_size)/5);

    for(int patch = 0; patch < num_hill_patches; patch++ ) {
        Rectangle new_rect;
        new_rect.x = (float)GetRandomValue(1, _tileset.map_size.x-5);
        new_rect.y = (float)GetRandomValue(1, _tileset.map_size.y-5);
        new_rect.width = GetRandomValue(2, 20);
        new_rect.height = GetRandomValue(2, 20);

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
        TraceLog(LOG_INFO, "=====u%i=r%i=d%i=l%i=======================(ZONE %i) zone auto TILE NOT found  ----default id %i   (%i, %i)", 
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
            TraceLog(LOG_INFO, "====tile id %i", id);
            
        }
        this_tileset.tile_tags.push_back(new_tag);
        TraceLog(LOG_INFO, "====tiles tags loaded_____________ %i\n", this_tileset.tile_tags.size());
        
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

        TraceLog(LOG_INFO, "tile t = %i", tile_id);

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

void BuildPremadeStructures(json &grid_tiles, WorldGenTileSet &this_tileset, std::vector<LDTKEntityInstance> structure_bounding_entities){

    TraceLog(LOG_INFO, "building premade structuires  # of tiles in grid  %i", grid_tiles.size());

    std::vector<Rectangle> bounding_rects;

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
                TraceLog(LOG_INFO, "new tile %i    at x:%0.0f y:%0.0f", new_tile.tile_id, new_tile.position.x, new_tile.position.y);
            }
        }
        this_tileset.structure_lookup[structure_id_string] = new_structure;
    }

    for(auto &structure : this_tileset.structure_lookup) {
        TraceLog(LOG_INFO, "structure loaded  %s", structure.first.c_str());
        TraceLog(LOG_INFO, "structure tiles  %i", structure.second.structure_grid_tiles.size());
        for(WorldGenAutoTile &tile : structure.second.structure_grid_tiles) {
            TraceLog(LOG_INFO, "------tile id %i relitive position x %0.0f y %0.0f", tile.tile_id, tile.position.x, tile.position.y);
        }
    }    
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

/*         {"TILE_ID_BORDER_END_LEFT",          TILEID::TILE_ID_BORDER_END_LEFT},
        {"TILE_ID_BORDER_END_RIGHT",          TILEID::TILE_ID_BORDER_END_RIGHT},
        {"TILE_ID_BORDER_END_UP",          TILEID::TILE_ID_BORDER_END_UP},
        {"TILE_ID_BORDER_END_DOWN",          TILEID::TILE_ID_BORDER_END_DOWN},
        {"TILE_ID_BORDER_UP_DOWN",          TILEID::TILE_ID_BORDER_UP_DOWN},
        {"TILE_ID_BORDER_RIGHT_LEFT",          TILEID::TILE_ID_BORDER_RIGHT_LEFT}, */
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        return it->second;
    }
    //TraceLog(LOG_INFO, "the tile ID is not found ---- %s", s.c_str());
    return TILEID::TILE_ID_NONE;
}




std::array<bool,4> TileIdGetAutotile(TILEID tile_id) {
    
    std::unordered_map<TILEID, std::array<bool,4>> side_map = {

        {TILE_ID_DIRT_SINGLE,          {false, false, false, false}},
        {TILE_ID_DIRT_MID,          {true, true, true, true}},
        {TILE_ID_DIRT_RIGHT_DOWN,          {false, true, true, false}},
        {TILE_ID_DIRT_RIGHT_DOWN_LEFT,          {false, true, true, true}},
        {TILE_ID_DIRT_DOWN_LEFT,          {false, false, true, true}},
        {TILE_ID_DIRT_UP_DOWN_LEFT,          {true, false, true, true}},
        {TILE_ID_DIRT_UP_LEFT,          {true, false, false, true}},
        {TILE_ID_DIRT_UP_RIGHT_LEFT,          {true, true, false, true}},
        {TILE_ID_DIRT_RIGHT_UP,          {true, true, false, false}},
        {TILE_ID_DIRT_UP_RIGHT_DOWN,          {true, true, true, false}},

        {TILE_ID_BORDER_SINGLE,          {false, false, false, false}},
        {TILE_ID_BORDER_MID,          {true, true, true, true}},
        {TILE_ID_BORDER_RIGHT_DOWN,          {false, true, true, false}},
        {TILE_ID_BORDER_RIGHT_DOWN_LEFT,          {false, true, true, true}},
        {TILE_ID_BORDER_DOWN_LEFT,          {false, false, true, true}},
        {TILE_ID_BORDER_UP_DOWN_LEFT,          {true, false, true, true}},
        {TILE_ID_BORDER_UP_LEFT,          {true, false, false, true}},
        {TILE_ID_BORDER_UP_RIGHT_LEFT,          {true, true, false, true}},
        {TILE_ID_BORDER_RIGHT_UP,          {true, true, false, false}},
        {TILE_ID_BORDER_UP_RIGHT_DOWN,          {true, true, true, false}},

        {TILE_ID_BORDER_END_LEFT,          {false, false, false, true}},
        {TILE_ID_BORDER_END_RIGHT,          {false, true, false, false}},
        {TILE_ID_BORDER_END_UP,          {true, false, false, false}},
        {TILE_ID_BORDER_END_DOWN,          {false, false, true, false}},

        {TILE_ID_BORDER_UP_DOWN,          {true, false, true, false}},
        {TILE_ID_BORDER_RIGHT_LEFT,          {false, true, false, true}},


        {TILE_ID_PATH_SINGLE,          {false, false, false, false}},
        {TILE_ID_PATH_MID,          {true, true, true, true}},
        {TILE_ID_PATH_END_UP,          {false, false, true, false}},
        {TILE_ID_PATH_END_RIGHT,          {false, false, false, true}},
        {TILE_ID_PATH_END_DOWN,          {true, false, false, false}},
        {TILE_ID_PATH_END_LEFT,          {false, true, false, false}},
        {TILE_ID_PATH_UP_RIGHT,          {true, true, false, false}},
        {TILE_ID_PATH_UP_DOWN,          {true, false, true, false}},
        {TILE_ID_PATH_UP_LEFT,          {true, false, false, true}},
        {TILE_ID_PATH_UP_RIGHT_DOWN,          {true, true, true, false}},
        {TILE_ID_PATH_UP_DOWN_LEFT,          {true, false, true, true}},
        {TILE_ID_PATH_RIGHT_LEFT,          {false, true, false, true}},
        {TILE_ID_PATH_UP_RIGHT_LEFT,          {true, true, false, true}},
        {TILE_ID_PATH_RIGHT_DOWN_LEFT,          {false, true, true, true}},
        {TILE_ID_PATH_DOWN_LEFT,          {false, false, true, true}},
        {TILE_ID_PATH_RIGHT_DOWN,          {false, true, true, false}},
    };

    if (auto it = side_map.find(tile_id); it != side_map.end()) {
        return it->second;
    }
    //TraceLog(LOG_INFO, "autotile ID not found ");
    return {false, false, false, false};
}




