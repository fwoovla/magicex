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


struct PlayerData {
    int health;
    int exp;
    float base_speed;
    int sprite_sheet_id;
    int portrait_id;
    std::string name;
    std::string class_name;

};

extern std::unordered_map<int, PlayerData> g_class_data;

extern PlayerData g_player_data;

extern PlayerData g_save_data;



inline void LoadGameData() {
    TraceLog(LOG_INFO, "LOADING GAME DATA....data.json");

    std::ifstream cfile("assets/data.json");
    if (!cfile.is_open()) {
        TraceLog(LOG_INFO, "CANNOT OPEN DATA FILE");
        return;
    }
    
    json cj;
    cfile>>cj;
    //TraceLog(LOG_INFO, "==========LOADING GAME DATA FROM FILE================");
    //TraceLog(LOG_INFO, "==========loading class data================");

    for(int i = 0; i < cj["base_class"].size(); i++) {
        int health = cj["base_class"][i]["health"];
        int exp = 0;
        float base_speed = cj["base_class"][i]["base_speed"];
        int sprite_sheet_id = cj["base_class"][i]["sprite_sheet_id"];
        int portrait_id = cj["base_class"][i]["portrait_id"];
        std::string class_name = cj["base_class"][i]["class_name"];

        PlayerData this_class = {
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
 
    //TraceLog(LOG_INFO, "==========end class data================");
    //TraceLog(LOG_INFO, "==========check save data================");

    std::string save_path = "assets/save.json";
    if(std::filesystem::exists(save_path)) {
        g_game_data.save_available = true;
        TraceLog(LOG_INFO, "SAVE FILE FOUND");
    }

    //TraceLog(LOG_INFO, "==========end save data================");
    //TraceLog(LOG_INFO, "==========load map data================");

/*     std::string map_dir = "assets/maps";
    
    int num_maps = load_json_maps(map_dir);
    TraceLog(LOG_INFO, "----------------------------------------- maps found %i", num_maps);


    TraceLog(LOG_INFO, "==========end map data================ maps loaded %i", g_maps_data.size()); */

    TraceLog(LOG_INFO, "==========LOADING LDTK MAPS================");

    std::string ldtk_map_dir = "assets/maps/ldtk";
    int num_maps = load_ldtk_maps();

    TraceLog(LOG_INFO, "==========END LOADING LDTK MAPS================  loaded %i maps", num_maps);

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


