#pragma once
#include "gamedefs.h"
#include <queue>


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

    TILE_ID_FENCE_START,

    TILE_ID_FENCE_TOP_LEFT_CORNER,
    TILE_ID_FENCE_TOP_RIGHT_CORNER,
    TILE_ID_FENCE_BOTTOM_LEFT_CORNER,
    TILE_ID_FENCE_BOTTOM_RIGHT_CORNER,
    TILE_ID_FENCE_END_UP,
    TILE_ID_FENCE_END_RIGHT,
    TILE_ID_FENCE_END_DOWN,
    TILE_ID_FENCE_END_LEFT,
    TILE_ID_FENCE_MID_VERTICAL,
    TILE_ID_FENCE_MID_HORIZANTAL,

    TILE_ID_FENCE_TOP_LEFT_CORNER_LOWER,
    TILE_ID_FENCE_TOP_RIGHT_CORNER_LOWER,
    TILE_ID_FENCE_BOTTOM_LEFT_CORNER_LOWER,
    TILE_ID_FENCE_BOTTOM_RIGHT_CORNER_LOWER,
    //TILE_ID_FENCE_END_UP_LOWER,
    TILE_ID_FENCE_END_RIGHT_LOWER,
    TILE_ID_FENCE_END_DOWN_LOWER,
    TILE_ID_FENCE_END_LEFT_LOWER,
    TILE_ID_FENCE_MID_HORIZANTAL_LOWER,
    TILE_ID_FENCE_END,

    TILE_ID_ROAD_START,

    TILE_ID_ROAD_UP_RIGHT_DOWN,
    TILE_ID_ROAD_UP_DOWN_LEFT,
    TILE_ID_ROAD_RIGHT_DOWN_LEFT,
    TILE_ID_ROAD_UP_RIGHT_LEFT,
    TILE_ID_ROAD_CENTER,
    
    TILE_ID_ROAD_END,

    TILE_ID_DEBUG_1,

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
    ZONE_FORREST,
    ZONE_DIRT,
    ZONE_PATH,
    ZONE_PATH_SIDE,
    ZONE_TREE,
    ZONE_STRUCTURE,
    ZONE_STRUCTURE_SIDE,
    ZONE_BORDER,
    ZONE_FENCE,
    ZONE_FENCE_BOTTOM,
    ZONE_MUSHROOM,
    ZONE_RUINS,
    ZONE_DECCO_ENTITY,
    ZONE_CREATURE,
    ZONE_ROAD,
    Zone_ROAD_SIDE,
};


enum BIOME_TYPE {
    BIOME_NONE = -1,
    BIOME_PLAINS,
    BIOME_FORREST,
    BIOME_HILLS,
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
    std::vector<TILEID> fence_tiles;
    std::vector<TILEID> road_tiles;
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

struct WorldGenDeccoEntityData {

    std::string decco_name;
    Vector2 position;

};

struct WorldGenBiomeData {
    BIOME_TYPE type;
    float coverage;
};

struct StructureProfile {
    float wants_road   = 0.0f;
    float wants_center = 0.0f;
    float wants_edge   = 0.0f;
    float wants_forrest = 0.0f;
};

struct WorldGenStructure {
    StructureProfile profile;
    Vector2 size;
    std::string id;
    BIOME_TYPE biome;
    int count_min;
    int count_max;
    int min_distance;
};


struct WorldGenTerrain {
    int hill_patches;
    int hill_size;
    int dirt_patches;
    int dirt_size;
};


struct WorldGenData {
    
    std::string map_name;
    int map_width;
    int map_height;

    float tree_coverage;
    float grass_coverage;

    bool has_shelter;

    std::string terrrain_set_name;
    std::string structure_set_name;

    WorldGenTerrain terrain;

    std::vector<std::string> exit_dest_strings;
    std::vector< WorldGenBiomeData > biome_data;
    std::vector< WorldGenStructure> structure_data;
};

extern std::vector<WorldGenData> g_worldgen_data;



struct WorldGenBiome {
    BIOME_TYPE type;
    Rectangle rect;
};


enum InfluenceType {
    INF_ROAD,
    INF_SHELTER,
    INF_WILDERNESS,
    INF_CENTER,
    INF_EDGE
};


struct InfluenceCell {
    Rectangle rect;
    bool reserverd = false;
    float road = 0.0f;
    float shelter = 0.0f;
    float forrest = 0.0f;
    float center = 0.0f;
    float edge = 0.0f;
};


struct StructurePatch {
    Rectangle rect;
    std::string id;
    float road_proximity;
    float shelter_proximity;
    float forrest_proximity;
    float center_proximity;
    float edge_proximity;
};


struct WorldGenPlan {

    Vector2 map_size;
    Vector2 road_midpoint;
    std::vector< std::vector<Vector2> > paths;
    std::vector<Vector2> structure_positions;
    std::vector<Vector2> exit_positions;
    std::vector<std::string> exit_dest_strings;
    std::vector<Rectangle> ruins_rects;
    std::vector<WorldGenBiome> biomes;
    std::vector<StructurePatch> poi_patches;
    std::vector<Rectangle> spawn_patches;
    std::vector<int> used_spawn_patches;
    std::vector<Rectangle> reserved_rects;

    std::vector<Vector2> road_positions;
    std::vector<Vector2> edge_positions;
    std::vector<InfluenceCell> influence_grid;



};

struct WorldGenTileSet {
    WorldGenPlan wg_plan;
    WorldGenData wg_data;
    std::string identifier;
    int c_wid;
    int c_hei;
    int uid;
    std::string rel_path;
    int px_wid;
    int px_hei;
    int tile_grid_size;
    //int num_paths;
    Vector2 map_size;

    int tilesheet_uid;
    int collision_layer_index = -1;

    SortedTiles sorted_tiles;
    std::vector <LDTKEnumTag> tile_tags;
    std::unordered_map <int, WorldGenAutoTile> tile_lookup;
    std::vector<MAPZONE> lower_zone_grid;
    std::vector<MAPZONE> upper_zone_grid;
    std::vector<MAPZONE> spawn_zone_grid;
    std::vector<BIOME_TYPE> biome_grid;
    std::vector<int> collision_grid;

    std::vector<WorldGenDeccoEntityData> layer_decco_data;
    std::vector<WorldGenDeccoEntityData> entity_decco_data;
    std::unordered_map<std::string, WorldGenStructureData> structure_lookup;
    std::unordered_map<std::string, WorldGenStructureData> layer_decco_lookup;
    std::unordered_map<std::string, WorldGenStructureData> entity_decco_lookup;
};

extern std::vector<WorldGenTileSet> g_worldgen_tilesets;


//tileset

void GenerateWorldGenTilesets(std::string _path);

void EctractTileData(json &tj, WorldGenTileSet &this_tileset);

void BuildTerrainTileSet(json &grid_tiles, WorldGenTileSet &this_tileset);
void BuildStructureTileSet(json &grid_tiles, WorldGenTileSet &this_tileset);
void BuildPremadeStructures(json &grid_tiles, WorldGenTileSet &this_tileset, std::vector<LDTKEntityInstance> structure_bounding_entities, std::unordered_map<std::string, WorldGenStructureData> &structure_lookup);
void ExtractStructureBounds(json &bounds, std::vector<LDTKEntityInstance> &bounding_entities);

//worldgen
void GenerateMap(LDTKLevel &new_level, std::string map_name);

void GenerateZones(LDTKLevel &level, WorldGenTileSet &_tileset);

//utils


void CreateSpawnPatches(WorldGenTileSet &_tileset, int patch_size);
Rectangle GetAvailableSpawnPatch(WorldGenTileSet &_tileset);


TILEID GetAutoTile(std::vector<TILEID> &tile_list, WorldGenTileSet &_tileset, std::vector<MAPZONE> &zone_grid, MAPZONE target_zone, int grid_index);

TILEID GetFenceTileTop(WorldGenTileSet &_tileset, std::vector<MAPZONE> &zone_grid, int tile_x, int tile_y);

TILEID GetFenceTileBottom(WorldGenTileSet &_tileset, std::vector<MAPZONE> &zone_grid, int tile_x, int tile_y);

TILEID StrToTileId(const std::string& s);



std::array<bool,4> TileIdGetAutotile(TILEID tile_id);

void LoadMapGenData(json &j);


WorldGenPlan GenerateWorldGenPlan(WorldGenData &wg_data);

void PlanGeography(WorldGenData &wg_data, WorldGenPlan &wg_plan);

void PlanRegions(WorldGenData &wg_data, WorldGenPlan &wg_plan);

void PlanStructures(WorldGenData &wg_data, WorldGenPlan &wg_plan);

void PlanRoads(WorldGenData &wg_data, WorldGenPlan &wg_plan);

BIOME_TYPE ChooseBiome(WorldGenData &wg_data);

void SplitRegion(std::vector<Rectangle>& regions);

WorldGenBiome* GetBiome(Vector2 position, WorldGenTileSet &_tileset);

void CreatePoiPatches(WorldGenPlan &wg_plan, int patch_size);

Rectangle GetAvailablePoiPatch(WorldGenTileSet &_tileset);


float& GetInfluenceChannel(InfluenceCell& c, InfluenceType t);

void PaintInfluence(std::vector<InfluenceCell>& grid, int w, int h, int start_x, int start_y, float strength, float decay, InfluenceType type);

void CreateInfluenceGrid(WorldGenPlan &wg_plan, int patch_size);

StructureProfile BuildStructureProfile(WorldGenStructure &structure);


//unused
//void CreatePoiPatches(WorldGenTileSet &_tileset, int patch_size);
//void GenerateDirtZonesRect(LDTKLevel &level, WorldGenTileSet &_tileset);
//void GenerateDirtZonesBrush(LDTKLevel &level, WorldGenTileSet &_tileset, int brush_size);
//void GenerateTreeZones(LDTKLevel &level, WorldGenTileSet &_tileset);
//void GenerateStructureZones(LDTKLevel &level, WorldGenTileSet &_tileset);
//void GenerateStartingShelter(LDTKLevel &level, WorldGenTileSet &_tileset);
//void GenerateVillageZone(LDTKLevel &level, WorldGenTileSet &_tileset);
//void GenerateStructureZone(LDTKLevel &level, WorldGenTileSet &_tileset);
//void GenerateHillZonesRect(LDTKLevel &level, WorldGenTileSet &_tileset);
//void GenerateHillZonesBrush(LDTKLevel &level, WorldGenTileSet &_tileset, int brush_size);
//void GenerateRuinsZones(LDTKLevel &level, WorldGenTileSet &_tileset);
//void GenerateExits(LDTKLevel &level, WorldGenTileSet &_tileset);
//void GenerateCreatureZones(LDTKLevel &level, WorldGenTileSet &_tileset);
//void FillWithHillZone(LDTKLevel &level, WorldGenTileSet &_tileset);
//void CarveEmptySpace(LDTKLevel &level, WorldGenTileSet &_tileset);