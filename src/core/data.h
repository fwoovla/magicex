#pragma once

//#include <raylib.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include "json.hpp"

#include "gamedefs.h"

using json = nlohmann::json;



inline std::string unit_names[10] = {
    "blerfy",
    "toyf",
    "margolot",
    "dr farts",
    "muffin man",
    "doof",
    "scar",
    "diffny",
    "bammabi",
    "skort mc dort"
};


struct ClassData {
    int health;
    int exp;
    float base_speed;
    int sprite_sheet_id;
    int portrait_id;
    std::string name;
    std::string class_name;
};

extern std::unordered_map<int, ClassData> g_class_data;

/* struct PlayerData {
    int health;
    int exp;
    int sprite_sheet_id;
    Texture2D sprite_sheet;
};
 */

extern ClassData g_player_data;

extern ClassData g_save_data;


struct GameData {
    bool is_new_player = true;
    bool save_available = false;
};

extern GameData g_game_data;


struct TileData {
    int id;
    //std::string id;
    Vector2 pos;
};

struct TileLayerData {
    std::string layer_name;
    std::vector<TileData> tiles;
};

struct MapData {
    int tile_size;
    float inv_tile_size;
    int map_width;
    int map_height;
    std::vector<TileLayerData> layers;
};

extern std::vector<MapData> g_map_data;

inline void LoadGameData() {
    TraceLog(LOG_INFO, "LOADING GAME DATA....data.json");

    std::ifstream cfile("assets/data.json");
    if (!cfile.is_open()) {
        TraceLog(LOG_INFO, "CANNOT OPEN FILE");
        return;
    }
    
    json cj;
    cfile>>cj;
    TraceLog(LOG_INFO, "==========LOADING GAME DATA FROM FILE================");
    TraceLog(LOG_INFO, "==========loading class data================");

    for(int i = 0; i < cj["base_class"].size(); i++) {
        int health = cj["base_class"][i]["health"];
        int exp = 0;
        float base_speed = cj["base_class"][i]["base_speed"];
        int sprite_sheet_id = cj["base_class"][i]["sprite_sheet_id"];
        int portrait_id = cj["base_class"][i]["portrait_id"];
        std::string class_name = cj["base_class"][i]["class_name"];

        ClassData this_class = {
            .health = health,
            .exp = exp,
            .base_speed = base_speed,
            .sprite_sheet_id = sprite_sheet_id,
            .portrait_id = portrait_id,
            .name = "not assigned",
            .class_name = class_name
        };

        g_class_data[i] = this_class;
    }
    cfile.close();
 
    TraceLog(LOG_INFO, "==========end class data================");
    TraceLog(LOG_INFO, "==========check save data================");

    std::string save_path = "assets/save.json";
    if(std::filesystem::exists(save_path)) {
        g_game_data.save_available = true;
        TraceLog(LOG_INFO, "SAVE FILE FOUND");
    }

    TraceLog(LOG_INFO, "==========end save data================");
    TraceLog(LOG_INFO, "==========load map data================");

    std::string map_path = "assets/maps/map.json";
    if(std::filesystem::exists(map_path)) {
        TraceLog(LOG_INFO, "MAP FILE FOUND");

        
        std::ifstream mfile(map_path);
        if (!mfile.is_open()) {
            TraceLog(LOG_INFO, "filed to open map file");
            return;
        }
        TraceLog(LOG_INFO, "map file opened");
        json mj;
        //TraceLog(LOG_INFO, "file 1 check");
        mfile>>mj;

        //TraceLog(LOG_INFO, "file 2  check");
        int tile_size = mj["tileSize"];
        //TraceLog(LOG_INFO, "tile size check");
        int map_width = mj["mapWidth"];
        //TraceLog(LOG_INFO, "map width check");
        int map_height = mj["mapHeight"];
        //TraceLog(LOG_INFO, "map height check %i", map_height);

        std::vector<TileLayerData> these_layers;


        for(int l = 0; l < mj["layers"].size(); l++) {

            TileLayerData this_layer;
            this_layer.layer_name = mj["layers"][l]["name"];
            TraceLog(LOG_INFO, "layer name check");

            for (int t = 0; t < mj["layers"][l]["tiles"].size(); t++) {
                std::string id = mj["layers"][l]["tiles"][t]["id"];
                TileData this_tile = {
                    .id = std::stoi(id.c_str()),
                    .pos = { mj["layers"][l]["tiles"][t]["x"], mj["layers"][l]["tiles"][t]["y"] }
                };
                TraceLog(LOG_INFO, "tile check %0.0f %0.0f    %i", this_tile.pos.x, this_tile.pos.y, this_tile.id);

                this_layer.tiles.push_back(this_tile);
            }

            these_layers.push_back(this_layer);
            TraceLog(LOG_INFO, "layer loaded %s", this_layer.layer_name.c_str());

        }

        MapData this_map = {
            .tile_size = tile_size,
            .inv_tile_size = 1.0f/(float)tile_size,
            .map_width = map_width,
            .map_height = map_height,
            .layers = these_layers
        };

        g_map_data.push_back(this_map);

        mfile.close();
    }

    TraceLog(LOG_INFO, "==========end map data================ maps loaded %i", g_map_data.size());
    TraceLog(LOG_INFO, "==========DATA LOADED================");
}

inline void SaveGame() {
    std::string save_path = "assets/save.json";
    std::ofstream file(save_path);
    if (!file.is_open()) {
        TraceLog(LOG_INFO, "filed to open save file");
        return;
    }

    json j;

    j["health"] = g_player_data.health;
    j["exp"] = g_player_data.exp;
    j["base_speed"] = g_player_data.base_speed;
    j["sprite_sheet_id"] = g_player_data.sprite_sheet_id;
    j["portrait_id"] = g_player_data.portrait_id;
    j["name"] = g_player_data.name;
    j["class_name"] = g_player_data.class_name;


    file<<j.dump(4);

    file.close();

}

inline void LoadGame() {
    TraceLog(LOG_INFO, "LOADING SAVED DATA....save.json");

    std::string save_path = "assets/save.json";
    std::ifstream file(save_path);
    if (!file.is_open()) {
        TraceLog(LOG_INFO, "CANNOT OPEN FILE");
        return;
    }
    
    json j;
    file>>j;

    g_player_data.health = j["health"];
    g_player_data.exp = j["exp"];
    g_player_data.base_speed = j["base_speed"];
    g_player_data.sprite_sheet_id = j["sprite_sheet_id"];
    g_player_data.portrait_id = j["portrait_id"];
    g_player_data.name = j["name"];
    g_player_data.class_name = j["class_name"];

    file.close();

}