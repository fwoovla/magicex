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

void GenerateMap(LDTKLevel &new_level, int tileset_uid, Vector2 _map_size, std::string map_name) {

    WorldGenTileSet *this_tileset = nullptr;

    for(int ts = 0; ts < g_worldgen_tilesets.size(); ts++) {
        if(g_worldgen_tilesets[ts].uid == tileset_uid) {
            this_tileset = &g_worldgen_tilesets[ts];
        }
    }

    if(this_tileset == nullptr) {
        TraceLog(LOG_INFO, "could not find tileset %i", tileset_uid);
        return;
    
    }

    new_level.identifier = map_name;
    new_level.is_worldgen = true;
    new_level.px_wid = _map_size.x * this_tileset->tile_grid_size;
    new_level.px_hei = _map_size.y * this_tileset->tile_grid_size;


    this_tileset->map_size = _map_size;

    this_tileset->max_grass = 5 * new_level.px_wid + 5 * new_level.px_hei;
    this_tileset->max_trees = 0.40 * new_level.px_wid + 0.40 * new_level.px_hei;

    this_tileset->max_structures = (_map_size.x/50) + (_map_size.y/100);

    this_tileset->max_hills = 1 + (_map_size.x/2);

    this_tileset->sorted_tiles.grass_tiles.clear();
    this_tileset->sorted_tiles.dirt_tiles.clear();
    this_tileset->sorted_tiles.path_tiles.clear();
    this_tileset->sorted_tiles.border_tiles.clear();

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
    }

    this_tileset->paths.clear();

    this_tileset->structure_positions.clear();

    this_tileset->collision_grid.clear();
    this_tileset->collision_grid.resize((int)(this_tileset->map_size.x + 1) * (int)(this_tileset->map_size.y + 1), 0);
    this_tileset->lower_zone_grid.clear();
    this_tileset->lower_zone_grid.resize((int)(this_tileset->map_size.x + 1) * (int)(this_tileset->map_size.y + 1), ZONE_NONE);
    this_tileset->upper_zone_grid.clear();
    this_tileset->upper_zone_grid.resize((int)(this_tileset->map_size.x + 1) * (int)(this_tileset->map_size.y + 1), ZONE_NONE);


    TraceLog(LOG_INFO, "-------------max_grass %i", this_tileset->max_grass);
    TraceLog(LOG_INFO, "-------------max_trees %i", this_tileset->max_trees);
    TraceLog(LOG_INFO, "-------------max_hills %i", this_tileset->max_hills);
    TraceLog(LOG_INFO, "-------------max_structures %i", this_tileset->max_structures);
    TraceLog(LOG_INFO, "-------TILESET #%i READY TO BUILD MAP------", tileset_uid);

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
    //transition areas                      X
    //lootables                             -
    //mobs                                  -
    //fix sub maps                          X
    //fix paths                             -
    //fences around structures              X

    GenerateZones(new_level, *this_tileset);

    GenerateEntitiesLayer(new_level, *this_tileset);

    ConnectStructuresWithPaths(*this_tileset);

    GenerateStructuresLayer(new_level, *this_tileset,  g_worldgen_tilesets[1]);

    GenerateUpperTerrainLayer(new_level, *this_tileset);

    GenerateLowerTerrainLayer(new_level, *this_tileset);

    GenerateCollisionLayer(new_level, *this_tileset);

    PopulateGrass(new_level, *this_tileset);

    PopulateTrees(new_level, *this_tileset);

    PlaceEntities(new_level, *this_tileset);

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

    //GenerateDirtZonesRect(level, _tileset);
    GenerateDirtZonesBrush(level, _tileset, 3);
    GenerateTreeZones(level, _tileset);
    GenerateStructureZones(level, _tileset);
    //GenerateHillZonesRect(level, _tileset);
    GenerateHillZonesBrush(level, _tileset, 4);
}






//stamp rectangles of dirt
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


    //Vector2 start_position = {10,10};

    std::vector<Vector2> start_positions;

    int x_patches = _tileset.map_size.x/50;
    int y_patches = _tileset.map_size.y/50;
    int num_structure_patches = x_patches + y_patches;

    int x_patch_size = (level.px_wid/_tileset.tile_grid_size)/x_patches;
    int y_patch_size = (level.px_hei/_tileset.tile_grid_size)/y_patches;

    for(int y_patch = 0; y_patch < y_patches; y_patch++ ) {
        for(int x_patch = 0; x_patch < x_patches; x_patch++) {
            int x_pos = 10 + (x_patch * x_patch_size);
            int y_pos = 10 + (y_patch * y_patch_size);
            start_positions.push_back(Vector2{(float)x_pos, (float)y_pos});
        }
    }
    start_positions.push_back(Vector2{_tileset.map_size.x/2, _tileset.map_size.y/2});


    Rectangle brush;
    brush.x = 0;
    brush.y = 0;
    brush.width = brush_size;
    brush.height = brush_size;

    int max_loops = 250;

    for(Vector2 position :start_positions) {
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

            brush.x += GetRandomValue(-3, 3);

            if(brush.x < 1) {
                brush.x = 1;
                //running = false;
            }
            if(brush.x + brush.width >= _tileset.map_size.x-2) {
                brush.x -= ((brush.x + brush.width) - _tileset.map_size.x-3);
                //running = false;
            }
            
            brush.y += GetRandomValue(-3, 3);

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
    int num_hill_patches = _tileset.max_hills;

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



void GenerateHillZonesBrush(LDTKLevel &level, WorldGenTileSet &_tileset, int brush_size) {

    std::vector<Vector2> start_positions;

    int x_patches = _tileset.map_size.x/30;
    int y_patches = _tileset.map_size.y/30;
    int num_structure_patches = x_patches + y_patches;

    int x_patch_size = (level.px_wid/_tileset.tile_grid_size)/x_patches;
    int y_patch_size = (level.px_hei/_tileset.tile_grid_size)/y_patches;

    for(int y_patch = 0; y_patch < y_patches; y_patch++ ) {
        for(int x_patch = 0; x_patch < x_patches; x_patch++) {
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

    int max_loops = 200;

    for(Vector2 position :start_positions) {
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
                        if(_tileset.upper_zone_grid[index] != ZONE_STRUCTURE) {
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
        //TraceLog(LOG_INFO, "====enum tag %s", new_tag.value_string.c_str());

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
/*                         TraceLog(LOG_INFO, "tile found id: %i  u%i r%i d%i l%i\n", 
                            alatered_id, 
                            new_tile.marked_sides[0],
                            new_tile.marked_sides[1],
                            new_tile.marked_sides[2],
                            new_tile.marked_sides[3]
                            ); */
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
                //TraceLog(LOG_INFO, "new tile %i    at x:%0.0f y:%0.0f", new_tile.tile_id, new_tile.position.x, new_tile.position.y);
            }
        }
        this_tileset.structure_lookup[structure_id_string] = new_structure;
    }

    for(auto &structure : this_tileset.structure_lookup) {
        TraceLog(LOG_INFO, "structure loaded  %s", structure.first.c_str());
        TraceLog(LOG_INFO, "structure tiles  %i", structure.second.structure_grid_tiles.size());
        for(WorldGenAutoTile &tile : structure.second.structure_grid_tiles) {
            //TraceLog(LOG_INFO, "------tile id %i relitive position x %0.0f y %0.0f", tile.tile_id, tile.position.x, tile.position.y);
        }
    }    
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

        {"TILE_ID_FENCE_TOP_LEFT_CORNER",          TILEID::TILE_ID_FENCE_TOP_LEFT_CORNER},
        {"TILE_ID_FENCE_TOP_RIGHT_CORNER",          TILEID::TILE_ID_FENCE_TOP_RIGHT_CORNER},
        {"TILE_ID_FENCE_BOTTOM_LEFT_CORNER",          TILEID::TILE_ID_FENCE_BOTTOM_LEFT_CORNER},
        {"TILE_ID_FENCE_BOTTOM_RIGHT_CORNER",          TILEID::TILE_ID_FENCE_BOTTOM_RIGHT_CORNER},
        {"TILE_ID_FENCE_END_UP",          TILEID::TILE_ID_FENCE_END_UP},
        {"TILE_ID_FENCE_END_RIGHT",          TILEID::TILE_ID_FENCE_END_RIGHT},
        {"TILE_ID_FENCE_END_DOWN",          TILEID::TILE_ID_FENCE_END_DOWN},
        {"TILE_ID_FENCE_END_LEFT",          TILEID::TILE_ID_FENCE_END_LEFT},
        {"TILE_ID_FENCE_MID_VERTICAL",          TILEID::TILE_ID_FENCE_MID_VERTICAL},
        {"TILE_ID_FENCE_MID_HORIZANTAL",          TILEID::TILE_ID_FENCE_MID_HORIZANTAL},

        {"TILE_ID_FENCE_TOP_LEFT_CORNER_LOWER",          TILEID::TILE_ID_FENCE_TOP_LEFT_CORNER_LOWER},
        {"TILE_ID_FENCE_TOP_RIGHT_CORNER_LOWER",          TILEID::TILE_ID_FENCE_TOP_RIGHT_CORNER_LOWER},
        {"TILE_ID_FENCE_BOTTOM_LEFT_CORNER_LOWER",          TILEID::TILE_ID_FENCE_BOTTOM_LEFT_CORNER_LOWER},
        {"TILE_ID_FENCE_BOTTOM_RIGHT_CORNER_LOWER",          TILEID::TILE_ID_FENCE_BOTTOM_RIGHT_CORNER_LOWER},
        {"TILE_ID_FENCE_END_RIGHT_LOWER",          TILEID::TILE_ID_FENCE_END_RIGHT_LOWER},
        {"TILE_ID_FENCE_END_DOWN_LOWER",          TILEID::TILE_ID_FENCE_END_DOWN_LOWER},
        {"TILE_ID_FENCE_END_LEFT_LOWER",          TILEID::TILE_ID_FENCE_END_LEFT_LOWER},
        {"TILE_ID_FENCE_MID_HORIZANTAL_LOWER",          TILEID::TILE_ID_FENCE_MID_HORIZANTAL_LOWER},

    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        return it->second;
    }
    TraceLog(LOG_INFO, "the tile ID is not found ---- %s", s.c_str());
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




