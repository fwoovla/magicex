#pragma once
#include "gamedefs.h"


enum TILEID {
    TILE_ID_NONE = -1,
    TILE_ID_GRASS_START,
    TILE_ID_GRASS_1,
    TILE_ID_GRASS_2,
    TILE_ID_GRASS_3,
    TILE_ID_GRASS_4,
    TILE_ID_GRASS_END,
    TILE_ID_BORDER,

    TILE_ID_PATH_START,
    TILE_ID_PATH_MID,
    TILE_ID_PATH_END_LEFT,
    TILE_ID_PATH_END_RIGHT,
    TILE_ID_PATH_END_UP,
    TILE_ID_PATH_END_DOWN,
    TILE_ID_PATH_SINGLE,
    TILE_ID_PATH_UP_RIGHT,
    TILE_ID_PATH_UP_DOWN,
    TILE_ID_PATH_UP_LEFT,
    TILE_ID_PATH_UP_RIGHT_DOWN,
    TILE_ID_PATH_UP_DOWN_LEFT,
    TILE_ID_PATH_RIGHT_LEFT,
    TILE_ID_PATH_UP_RIGHT_LEFT,
    TILE_ID_PATH_RIGHT_DOWN_LEFT,
    TILE_ID_PATH_DOWN_LEFT,
    TILE_ID_PATH_RIGHT_DOWN,
    TILE_ID_PATH_END,

    TILE_ID_DIRT_START,
    TILE_ID_DIRT_SINGLE,
    TILE_ID_DIRT_MID,
    TILE_ID_DIRT_RIGHT_DOWN,
    TILE_ID_DIRT_RIGHT_DOWN_LEFT,
    TILE_ID_DIRT_DOWN_LEFT,
    TILE_ID_DIRT_UP_DOWN_LEFT,
    TILE_ID_DIRT_UP_LEFT,
    TILE_ID_DIRT_UP_RIGHT_LEFT,
    TILE_ID_DIRT_RIGHT_UP,
    TILE_ID_DIRT_UP_RIGHT_DOWN,
    TILE_ID_DIRT_END,

    TILE_ID_BORDER_START,
    TILE_ID_BORDER_SINGLE,
    TILE_ID_BORDER_MID,
    TILE_ID_BORDER_RIGHT_DOWN,
    TILE_ID_BORDER_RIGHT_DOWN_LEFT,
    TILE_ID_BORDER_DOWN_LEFT,
    TILE_ID_BORDER_UP_DOWN_LEFT,
    TILE_ID_BORDER_UP_LEFT,
    TILE_ID_BORDER_UP_RIGHT_LEFT,
    TILE_ID_BORDER_RIGHT_UP,
    TILE_ID_BORDER_UP_RIGHT_DOWN,

    TILE_ID_BORDER_END_LEFT,
    TILE_ID_BORDER_END_RIGHT,
    TILE_ID_BORDER_END_UP,
    TILE_ID_BORDER_END_DOWN,

    TILE_ID_BORDER_UP_DOWN,
    TILE_ID_BORDER_RIGHT_LEFT,

    TILE_ID_BORDER_END,


    TILE_ID_MAX,
};

enum AUTOTILE_TYPE {
    AUTOTILE_TERRAIN,
    AUTOTILE_SOLID,
    AUTOTILE_SHADOW,
};

enum TILESIDE {
    TILESIDE_UP,
    TILESIDE_RIGHT,
    TILESIDE_DOWN,
    TILESIDE_LEFT,
    TILESIDE_UPPER_RIGHT,
    TILESIDE_UPPER_LEFT,
    TILESIDE_LOWER_RIGHT,
    TILESIDE_LOWER_LEFT,
};

enum MAPZONE {
    ZONE_NONE = -1,
    ZONE_GRASS,
    ZONE_DIRT,
    ZONE_PATH,
    ZONE_PATH_SIDE,
    ZONE_TREE,
    ZONE_STRUCTURE,
    ZONE_BORDER,
};


struct WorldGenAutoTile {
    int tile_id;
    Vector2 position;
    Vector2 atlas_position;
    std::array<bool,4> marked_sides;
    bool has_collision;

};


struct SortedTiles {
    std::vector<TILEID> grass_tiles;
    std::vector<TILEID> border_tiles;
    std::vector<TILEID> path_tiles;
    std::vector<TILEID> dirt_tiles;
};


struct PathWorm {
    Vector2 position;
    Vector2 locked_dir;
    Vector2 last_locked_dir;
};

struct WorldGenStructureData {
    std::vector<WorldGenAutoTile> structure_grid_tiles;
    Vector2 position;
    Vector2 structure_size;
    int uid;

};

struct WorldGenTileSet {
    int c_wid;
    int c_hei;
    int uid;
    std::string rel_path;
    int px_wid;
    int px_hei;
    int tile_grid_size;
    int num_paths;
    Vector2 map_size;
    std::vector <LDTKEnumTag> tile_tags;
    std::unordered_map <TILEID, WorldGenAutoTile> tile_lookup;
    SortedTiles sorted_tiles;
    std::vector<PathWorm> path_worms;
    std::vector<MAPZONE> lower_zone_grid;
    std::vector<MAPZONE> upper_zone_grid;
    std::vector< std::vector<Vector2> > paths;
    std::vector<Vector2> structure_positions;
    std::unordered_map<std::string, WorldGenStructureData> structure_lookup;
    std::vector<int> house_transition_tiles;
    std::vector<int> shadow_tiles;
};



extern std::vector<WorldGenTileSet> g_worldgen_tilesets;


//tileset

void GenerateWorldGenTilesets(std::string _path);

void EctractTileData(json &tj, WorldGenTileSet &this_tileset);

void BuildTerrainTileSet(json &grid_tiles, WorldGenTileSet &this_tileset);
void BuildStructureTileSet(json &grid_tiles, WorldGenTileSet &this_tileset);
void BuildPremadeStructures(json &grid_tiles, WorldGenTileSet &this_tileset, std::vector<LDTKEntityInstance> structure_bounding_entities);
void ExtractStructureBounds(json &bounds, std::vector<LDTKEntityInstance> &bounding_entities);

//worldgen
void GenerateMap(LDTKLevel &new_level, int tileset_id, Vector2 _map_size);

void GenerateZones(LDTKLevel &level, WorldGenTileSet &_tileset);

void GenerateDirtZones(LDTKLevel &level, WorldGenTileSet &_tileset);
void GenerateTreeZones(LDTKLevel &level, WorldGenTileSet &_tileset);
void GenerateStructureZones(LDTKLevel &level, WorldGenTileSet &_tileset);
void GenerateHillZones(LDTKLevel &level, WorldGenTileSet &_tileset);


//utils

TILEID GetAutoTile(std::vector<TILEID> &tile_list, WorldGenTileSet &_tileset, std::vector<MAPZONE> &zone_grid, MAPZONE target_zone, int grid_index);

TILEID StrToTileId(const std::string& s);

std::array<bool,4> TileIdGetAutotile(TILEID tile_id);