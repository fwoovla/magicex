#pragma once
#include "gamedefs.h"


enum TILEID {
    TILE_ID_NONE = -1,
    TILE_ID_GRASS_1,
    TILE_ID_MAX,
};



struct WorldGenTile {
    int tile_id;
    Vector2 position;
    Vector2 source;
};

struct WorldGenTileSet {
    int c_wid;
    int c_hei;
    int uid;
    std::string rel_path;
    int px_wid;
    int px_hei;
    int tile_grid_size;
    std::vector <LDTKEnumTag> tile_tags;
    std::unordered_map <int, Vector2> tile_lookup;

};

struct WorldGenFieldInstance {
    std::string identifier;
    std::string value_s;
    Vector2 value_v;
    int value_i;
    std::vector<int> i_list;
};

struct WorldGenEntityInstance {
    std::string identifier;
    std::string iid;
    int64_t width;
    int64_t height;
    int64_t def_uid;
    std::vector<int64_t> px;
    std::vector<WorldGenFieldInstance> field_instances;
};

struct WorldGenLayer {
    int tileset_id;
    std::string identifier;
    std::string type;
    int64_t c_wid;
    int64_t c_hei;
    int64_t grid_size;
    std::vector <WorldGenTile> grid_tiles;
    std::vector<WorldGenEntityInstance> entity_instances;
};

struct WordGenMap {
    Vector2 map_size;
    std::vector <WorldGenLayer> layers;
};

extern std::vector<WorldGenTileSet> g_worldgen_tilesets;

void GenerateWorldGenTilesets(std::string _path);

void GenerateMap(LDTKLevel &new_level, int tileset_id);