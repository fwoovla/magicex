#pragma once
#include "gamedefs.h"



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
    //Texture2D texture;

};

extern std::vector<WorldGenTileSet> g_worldgen_tilesets;

void GenerateWorldGenTilesets(std::string _path);

void GenerateMap(LDTKLevel &new_level);