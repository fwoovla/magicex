#pragma once
#include <raylib.h>
#include <string>
#include <fstream>

#include "json.hpp"


#define MAX_TILE_SHEETS 10

using json = nlohmann::json;

struct Layer {
    std::string type;
    std::string identifier;
    std::string layer_type;
    int64_t uid;
    nlohmann::json doc;
    nlohmann::json ui_color;
    int64_t grid_size;
    int64_t guide_grid_wid;
    int64_t guide_grid_hei;
    int64_t display_opacity;
    int64_t inactive_opacity;
    bool hide_in_list;
    bool hide_fields_when_inactive;
    bool can_select_when_inactive;
    bool render_in_world_view;
    int64_t px_offset_x;
    int64_t px_offset_y;
    int64_t parallax_factor_x;
    int64_t parallax_factor_y;
    bool parallax_scaling;
    std::vector<nlohmann::json> required_tags;
    std::vector<nlohmann::json> excluded_tags;
    nlohmann::json auto_tiles_killed_by_other_layer_uid;
    std::vector<nlohmann::json> ui_filter_tags;
    bool use_async_render;
    std::vector<nlohmann::json> int_grid_values;
    std::vector<nlohmann::json> int_grid_values_groups;
    std::vector<nlohmann::json> auto_rule_groups;
    nlohmann::json auto_source_layer_def_uid;
    int64_t tileset_def_uid;
    int64_t tile_pivot_x;
    int64_t tile_pivot_y;
    nlohmann::json biome_field_uid;
};

struct CachedPixelData {
    std::string opaque_tiles;
    std::string average_colors;
};

struct Tileset {
    int64_t c_wid;
    int64_t c_hei;
    std::string identifier;
    int64_t uid;
    std::string rel_path;
    nlohmann::json embed_atlas;
    int64_t px_wid;
    int64_t px_hei;
    int64_t tile_grid_size;
    int64_t spacing;
    int64_t padding;
    std::vector<nlohmann::json> tags;
    nlohmann::json tags_source_enum_uid;
    std::vector<nlohmann::json> enum_tags;
    std::vector<nlohmann::json> custom_data;
    std::vector<nlohmann::json> saved_selections;
    CachedPixelData cached_pixel_data;
};

struct Defs {
    std::vector<Layer> layers;
    std::vector<nlohmann::json> entities;
    std::vector<Tileset> tilesets;
    std::vector<nlohmann::json> enums;
    std::vector<nlohmann::json> external_enums;
    std::vector<nlohmann::json> level_fields;
};

struct Header {
    std::string file_type;
    std::string app;
    std::string doc;
    std::string schema;
    std::string app_author;
    std::string app_version;
    std::string url;
};

struct GridTile {
    std::vector<int64_t> px;
    std::vector<int64_t> src;
    int64_t f;
    int64_t t;
    std::vector<int64_t> d;
    int64_t a;
};

struct LayerInstance {
    std::string identifier;
    std::string type;
    int64_t c_wid;
    int64_t c_hei;
    int64_t grid_size;
    int64_t opacity;
    int64_t px_total_offset_x;
    int64_t px_total_offset_y;
    int64_t tileset_def_uid;
    std::string tileset_rel_path;
    std::string iid;
    int64_t level_id;
    int64_t layer_def_uid;
    int64_t px_offset_x;
    int64_t px_offset_y;
    bool visible;
    std::vector<nlohmann::json> optional_rules;
    std::vector<nlohmann::json> int_grid_csv;
    std::vector<nlohmann::json> auto_layer_tiles;
    int64_t seed;
    nlohmann::json override_tileset_uid;
    std::vector<GridTile> grid_tiles;
    std::vector<nlohmann::json> entity_instances;
};

struct Level {
    std::string identifier;
    std::string iid;
    int64_t uid;
    int64_t world_x;
    int64_t world_y;
    int64_t world_depth;
    int64_t px_wid;
    int64_t px_hei;
    std::string bg_color;
    nlohmann::json level_bg_color;
    bool use_auto_identifier;
    nlohmann::json bg_rel_path;
    nlohmann::json level_bg_pos;
    double bg_pivot_x;
    double bg_pivot_y;
    std::string smart_color;
    nlohmann::json bg_pos;
    nlohmann::json external_rel_path;
    std::vector<nlohmann::json> field_instances;
    std::vector<LayerInstance> layer_instances;
    std::vector<nlohmann::json> neighbours;
};

struct Welcome {
    Header header;
    std::string iid;
    std::string json_version;
    int64_t app_build_id;
    int64_t next_uid;
    std::string identifier_style;
    std::vector<nlohmann::json> toc;
    std::string world_layout;
    int64_t world_grid_width;
    int64_t world_grid_height;
    int64_t default_level_width;
    int64_t default_level_height;
    int64_t default_pivot_x;
    int64_t default_pivot_y;
    int64_t default_grid_size;
    int64_t default_entity_width;
    int64_t default_entity_height;
    std::string bg_color;
    std::string default_level_bg_color;
    bool minify_json;
    bool external_levels;
    bool export_tiled;
    bool simplified_export;
    std::string image_export_mode;
    bool export_level_bg;
    nlohmann::json png_file_pattern;
    bool backup_on_save;
    int64_t backup_limit;
    nlohmann::json backup_rel_path;
    std::string level_name_pattern;
    nlohmann::json tutorial_desc;
    std::vector<nlohmann::json> custom_commands;
    std::vector<nlohmann::json> flags;
    Defs defs;
    std::vector<Level> levels;
    std::vector<nlohmann::json> worlds;
    std::string dummy_world_iid;
};








struct LDTK_MapData {
    std::vector<Tileset> tilesets;
};

extern std::vector<LDTK_MapData> g_ldtk_maps_data;

extern Texture2D g_ldtk_tilesheets[MAX_TILE_SHEETS];

inline int load_ldtk_tiles(std::vector<Vector2> &tilesheet) {

    return 0;
}




inline int load_ldtk_maps() {

    TraceLog(LOG_INFO, "LOADING GAME DATA....map.ldtk");

    std::ifstream mfile("assets/maps/ldtk/test.ldtk");
    if (!mfile.is_open()) {
        TraceLog(LOG_INFO, "CANNOT OPEN FILE");
        return 0;
    }
    
    json mj;
    mfile>>mj;
    TraceLog(LOG_INFO, "==========FOUND MAP FILE================");

    if(mj.contains("defs")) {
        TraceLog(LOG_INFO, "==========DEFS FOUND");
        if(mj["defs"].contains("tilesets")) {
            TraceLog(LOG_INFO, "==========TILESETS FOUND");
            int num_tilesets = 0;
            for(int i = 0; i < mj["defs"]["tilesets"].size(); i++) {
                num_tilesets++;

                std::filesystem::path pathObj(mj["defs"]["tilesets"][i]["relPath"]);
                std::string texture_path = pathObj.filename().string();

                if(std::filesystem::exists( "assets/maps/ldtk/" + texture_path)) {
                    g_ldtk_tilesheets[i] = LoadTexture(("assets/maps/ldtk/" + texture_path).c_str());
                    TraceLog(LOG_INFO, "==========TILESETS TEXTURE LOADED %s", texture_path.c_str());
                }

                Tileset this_tileset = {
                    .c_wid = mj["defs"]["tilesets"][i]["__cWid"]
                };

                TraceLog(LOG_INFO, "==========================TILESET c_wid %i", this_tileset.c_wid);
            }
            TraceLog(LOG_INFO, "==========TILESETS LOADED %i", num_tilesets);
        }
    }



    mfile.close();
    return 1;
}