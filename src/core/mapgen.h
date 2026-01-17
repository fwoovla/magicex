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

    TILE_ID_DIRT_INNER_UPPER_RIGHT,
    TILE_ID_DIRT_INNER_UPPER_LEFT,
    TILE_ID_DIRT_INNER_LOWER_RIGHT,
    TILE_ID_DIRT_INNER_LOWER_LEFT,
    TILE_ID_DIRT_END,

    TILE_ID_MAX,
};

enum AUTOTILE_TYPE {
    AUTOTILE_PATH,
    AUTOTILE_FENCE
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

struct WorldGenAutoTile {
    int tile_id;
    Vector2 position;
    Vector2 atlas_position;
    bool marked_sides[8];

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

struct WorldGenTileSet {
    int c_wid;
    int c_hei;
    int uid;
    std::string rel_path;
    int px_wid;
    int px_hei;
    int tile_grid_size;
    int num_paths;
    std::vector <LDTKEnumTag> tile_tags;
    std::unordered_map <TILEID, WorldGenAutoTile> tile_lookup;
    SortedTiles sorted_tiles;
    std::vector<PathWorm> path_worms;
    //std::vector<WorldGenAutoTile> path_auto_tiles;

};



extern std::vector<WorldGenTileSet> g_worldgen_tilesets;

void GenerateWorldGenTilesets(std::string _path);

void GenerateMap(LDTKLevel &new_level, int tileset_id, Vector2 _map_size);

void GenerateLowerTerrainLayer(LDTKLevel &_level, WorldGenTileSet &_tileset, Vector2 _map_size);


void GenerateMapPaths(LDTKLayerInstance &_layer, WorldGenTileSet &_tileset, Vector2 _map_size);

void GenerateDirtPatch(LDTKLayerInstance &_layer, WorldGenTileSet &_tileset, Vector2 _map_size);

void GenerateUpperTerrainLayer(LDTKLevel &level, WorldGenTileSet &_tileset, Vector2 _map_size);

void GenerateCollisionLayer(LDTKLevel &level, WorldGenTileSet &_tileset, Vector2 _map_size);

void GenerateEntitiesLayer(LDTKLevel &level, WorldGenTileSet &_tileset, Vector2 _map_size);

void GenerateStructuresLayer(LDTKLevel &level, WorldGenTileSet &_tileset, Vector2 _map_size);

void GenerateEnvironmentLayer(LDTKLevel &level, WorldGenTileSet &_tileset, Vector2 _map_size);


TILEID StrToTileId(const std::string& s);