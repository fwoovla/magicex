#pragma once
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <fstream>

#include "json.hpp"


#define MAX_TILE_SHEETS 10

using json = nlohmann::json;

struct LDTKLayer {
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

struct LDTKCachedPixelData {
    std::string opaque_tiles;
    std::string average_colors;
};

struct LDTKTileset {
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
    LDTKCachedPixelData cached_pixel_data;
};

struct LDTKDefs {
    std::vector<LDTKLayer> layers;
    std::vector<nlohmann::json> entities;
    std::vector<LDTKTileset> tilesets;
    std::vector<nlohmann::json> enums;
    std::vector<nlohmann::json> external_enums;
    std::vector<nlohmann::json> level_fields;
};

struct LDTKHeader {
    std::string file_type;
    std::string app;
    std::string doc;
    std::string schema;
    std::string app_author;
    std::string app_version;
    std::string url;
};

struct LDTKGridTile {
    std::vector<int64_t> px;
    std::vector<int64_t> src;
    int64_t f;
    int64_t t;
    std::vector<int64_t> d;
    int64_t a;
};

struct LDTKLayerInstance {
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
    std::vector<LDTKGridTile> grid_tiles;
    std::vector<nlohmann::json> entity_instances;
};

struct LDTKLevel {
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
    std::vector<LDTKLayerInstance> layer_instances;
    std::vector<nlohmann::json> neighbours;
};

struct LDTKMaps {
    LDTKHeader header;
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
    LDTKDefs defs;
    std::vector<LDTKLevel> levels;
    std::vector<nlohmann::json> worlds;
    std::string dummy_world_iid;
    std::vector<LDTKTileset> tilesets;

};





struct TileSheetData {
    int uid;
    Texture2D texture;
};


struct GameData {
    //bool is_new_player = true;
    bool save_available = false;
    bool paused = false;
    SCENE_ID current_scene_id;
    int current_map_index;
    int shelter_map_index;
};

extern GameData g_game_data;


extern LDTKMaps g_ldtk_maps;

extern std::unordered_map<int, TileSheetData> g_ldtk_tilesheets;
extern std::vector<LDTKLevel> g_ldtk_levels;







inline void LDTKLoadTileSets (json &mj);
inline void LDTKLoadMaps (json &mj);





inline int load_ldtk_maps() {

    TraceLog(LOG_INFO, "++++++++++++++++++++++++++++++++ LOADING LDTK DATA....  (*.ldtk) ++++++++++++++++++++++++++\n");

    std::ifstream mfile("assets/maps/ldtk/test.ldtk");
    if (!mfile.is_open()) {
        TraceLog(LOG_INFO, "-CANNOT OPEN FILE");
        return 0;
    }
    
    json mj;
    mfile>>mj;
    TraceLog(LOG_INFO, "+FOUND MAP FILE");

    LDTKLoadTileSets(mj);

    LDTKLoadMaps(mj);

    mfile.close();

    return 1;
}



inline void LDTKLoadTileSets(json &mj) {
    if(mj.contains("defs")) {
        TraceLog(LOG_INFO, "++DEFS FOUND");
        if(mj["defs"].contains("tilesets")) {
            TraceLog(LOG_INFO, "+++TILESETS FOUND");
            int num_tilesets = 0;
            for(int i = 0; i < mj["defs"]["tilesets"].size(); i++) {
                num_tilesets++;

                std::filesystem::path pathObj(mj["defs"]["tilesets"][i]["relPath"]);
                std::string texture_path = pathObj.filename().string();


                if(std::filesystem::exists( "assets/maps/ldtk/" + texture_path)) {
                    TileSheetData this_sheet = {
                        .uid = mj["defs"]["tilesets"][i]["uid"],
                        .texture = LoadTexture(("assets/maps/ldtk/" + texture_path).c_str())
                    };

                    g_ldtk_tilesheets[mj["defs"]["tilesets"][i]["uid"]] = this_sheet;
                    
                    TraceLog(LOG_INFO, "==========TILESETS TEXTURE LOADED %s", texture_path.c_str());
                }

                LDTKTileset this_tileset = {
                    .c_wid = mj["defs"]["tilesets"][i]["__cWid"],
                    .uid = mj["defs"]["tilesets"][i]["uid"],
                    .rel_path = mj["defs"]["tilesets"][i]["relPath"],
                    .px_wid = mj["defs"]["tilesets"][i]["pxWid"],
                    .px_hei = mj["defs"]["tilesets"][i]["pxHei"],
                    .tile_grid_size = mj["defs"]["tilesets"][i]["tileGridSize"]  
                };
                
                g_ldtk_maps.tilesets.push_back(this_tileset);

            }
            TraceLog(LOG_INFO, "==========TILESETS LOADED %i", g_ldtk_maps.tilesets.size());
        }
    }
}

inline void LDTKLoadMaps (json &mj) {


    g_ldtk_maps.default_grid_size = mj["defaultGridSize"];

    if(mj.contains("levels")) {
        TraceLog(LOG_INFO, "(%i) LEVELS FOUND", mj["levels"].size());
        for(int level = 0; level < mj["levels"].size(); level++) {

            if(mj["levels"][level].contains("fieldInstances")) {
                for (int field = 0; field < mj["levels"][level]["fieldInstances"].size(); field++) {
                    if(mj["levels"][level]["fieldInstances"][field]["__identifier"] == "is_shelter") {
                        if(mj["levels"][level]["fieldInstances"][field]["__value"] == true) {
                            g_game_data.shelter_map_index = level;
                            TraceLog(LOG_INFO, "SHELTER LEVEL FOUND %i", level);
                        }
                    }
                }
            }
            else {
                TraceLog(LOG_INFO, "(%i) is not SHELTER LEVEL", level);
            }
            
            LDTKLevel this_level;
            
            this_level.identifier = mj["levels"][level]["identifier"];
            this_level.bg_color = mj["levels"][level]["__bgColor"];
            this_level.world_x = mj["levels"][level]["worldX"];
            this_level.world_y = mj["levels"][level]["worldY"];
            this_level.uid = mj["levels"][level]["uid"];
            this_level.px_wid = mj["levels"][level]["pxWid"];
            this_level.px_hei = mj["levels"][level]["pxHei"];
            
            for(int layer = 0; layer < mj["levels"][level]["layerInstances"].size(); layer++) {

                LDTKLayerInstance this_layer;

                this_layer.grid_size = mj["levels"][level]["layerInstances"][layer]["__gridSize"];
                this_layer.tileset_def_uid = mj["levels"][level]["layerInstances"][layer]["__tilesetDefUid"];
                this_layer.c_hei = mj["levels"][level]["layerInstances"][layer]["__cHei"];
                this_layer.c_wid = mj["levels"][level]["layerInstances"][layer]["__cWid"];

                int size = this_layer.c_wid * this_layer.c_hei;
                for (int i = 0; i < size; i++) {
                    LDTKGridTile empty_tile;
                    empty_tile.t = -1;
                    this_layer.grid_tiles.push_back(empty_tile );
                }
                

                for( int tile = 0; tile < mj["levels"][level]["layerInstances"][layer]["gridTiles"].size(); tile++) {
                    LDTKGridTile this_tile;
                    this_tile.px.push_back(mj["levels"][level]["layerInstances"][layer]["gridTiles"][tile]["px"][0]);
                    this_tile.px.push_back(mj["levels"][level]["layerInstances"][layer]["gridTiles"][tile]["px"][1]);
                    this_tile.src.push_back(mj["levels"][level]["layerInstances"][layer]["gridTiles"][tile]["src"][0]);
                    this_tile.src.push_back(mj["levels"][level]["layerInstances"][layer]["gridTiles"][tile]["src"][1]);
                    this_tile.f = mj["levels"][level]["layerInstances"][layer]["gridTiles"][tile]["f"];
                    this_tile.a = mj["levels"][level]["layerInstances"][layer]["gridTiles"][tile]["a"];
                    this_tile.t = mj["levels"][level]["layerInstances"][layer]["gridTiles"][tile]["t"];
                    this_tile.d.push_back(mj["levels"][level]["layerInstances"][layer]["gridTiles"][tile]["d"][0]);

                    int index = (this_tile.px[1] / this_layer.grid_size) * this_layer.c_wid + (this_tile.px[0] / this_layer.grid_size);
                    this_layer.grid_tiles[index ] = this_tile;
                }

                TraceLog(LOG_INFO, "++++++TILES ADDED %i", this_layer.grid_tiles.size());
                this_level.layer_instances.push_back(this_layer);
                TraceLog(LOG_INFO, "++++++LAYERS FOUND %i/%i", this_level.layer_instances.size(), mj["levels"][level]["layerInstances"].size());
            }

            g_ldtk_maps.levels.push_back(this_level);
            TraceLog(LOG_INFO, "++++++LEVEL ADDED %i", g_ldtk_maps.levels.size());
        }
    }
}


inline void  LDTKDrawMap(Vector2 focus_position) {

    int tile_size = g_ldtk_maps.default_grid_size ;
    float inv_tile_size = 1.0f/(float)tile_size;
    
    int x_offset = ((g_resolution.x * 0.5f) / g_camera.zoom) * inv_tile_size;
    //x_offset--;
    int y_offset = ((g_resolution.y * 0.5f) / g_camera.zoom) * inv_tile_size;
    //y_offset--;

    float x_cam_offset = (g_resolution.x * 0.5f) / g_camera.zoom;
    float y_cam_offset = (g_resolution.y * 0.5f) / g_camera.zoom;

    //Vector2 center = {focus_position.x * inv_tile_size, focus_position.y * inv_tile_size};
    Vector2 center = Vector2Add(g_camera.target, {x_cam_offset, y_cam_offset} ) * inv_tile_size;

    //Vector2 center = {x_cam_offset * inv_tile_size, y_cam_offset * inv_tile_size};
    //TraceLog(LOG_INFO, "offset x:%i  y:%i \n", x_offset, y_offset); 
    TraceLog(LOG_INFO, "center %0.2f  %0.2f \n", center.x, center.y); 



    LDTKLevel *this_level = &g_ldtk_maps.levels[g_game_data.current_map_index];

    int64_t map_width = this_level->px_wid * inv_tile_size;
    int64_t map_height = this_level->px_hei * inv_tile_size;
    
    int x_min = center.x - (x_offset + 1);
    if(x_min < 0) {
        x_min = 0;
    }
        
    int x_max = center.x + (x_offset + 1);
    if(x_max > map_width) {
        x_max = map_width;
    } 
            
    int y_min = center.y - (y_offset + 1);
    if(y_min < 0) {
        y_min = 0;
    }
                
    int y_max = center.y + (y_offset + 1);
    if(y_max > map_height) {
        y_max = map_height;
    } 
                    
    //TraceLog(LOG_INFO, "x max: %i  y max %i \n", x_max, y_max);           

    int tiles_drawn = 0;

    //invert layers for drawing

    int tilesheet_id = 0;

    for (int l = this_level->layer_instances.size() - 1; l >= 0; l--) {
        tilesheet_id = this_level->layer_instances[l].tileset_def_uid;
        //TraceLog(LOG_INFO, "tileset id %i  ", tilesheet_id);

        int index = 0;   
        for (int y = y_min; y < y_max; y++) {
            for (int x = x_min; x < x_max; x++) {

                index = (y) * map_width + (x);
 

                 if(index >= this_level->layer_instances[l].grid_tiles.size()) {
                    break;
                }

                LDTKGridTile *this_tile = &this_level->layer_instances[l].grid_tiles[index];
                int tile_id = this_tile->t;
                //TraceLog(LOG_INFO, "index %i tile id %i  ", index, tile_id);
                    
                if(tile_id != -1) {
                    Vector2 atlas_pos = {(float)this_tile->src[0], (float)this_tile->src[1]};

                    Vector2 tile_pos = {(float)this_tile->px[0], (float)this_tile->px[1]};

                    Color color = WHITE;


                    DrawTexturePro(
                        g_ldtk_tilesheets[tilesheet_id].texture,
                        {atlas_pos.x, atlas_pos.y, (float)tile_size, (float)tile_size},
                        {(float)tile_pos.x, (float)tile_pos.y,(float)tile_size, (float )tile_size},
                        {0,0},
                        0.0,
                        color
                    ); 

                }
                tiles_drawn++;
            }
            
        }
    }
    //TraceLog(LOG_INFO, "ldtk draw");
}

inline void  LDTKDrawMapFree() {

}
