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


inline void LoadGameData() {
    TraceLog(LOG_INFO, "LOADING GAME DATA....data.json");

    std::ifstream file("assets/data.json");
    if (!file.is_open()) {
        TraceLog(LOG_INFO, "CANNOT OPEN FILE");
        return;
    }
    
    json j;
    file>>j;
    TraceLog(LOG_INFO, "==========LOADING GAME DATA FROM FILE================");


    TraceLog(LOG_INFO, "==========loading class data================");

    for(int i = 0; i < j["base_class"].size(); i++) {
        int health = j["base_class"][i]["health"];
        int exp = 0;
        float base_speed = j["base_class"][i]["base_speed"];
        int sprite_sheet_id = j["base_class"][i]["sprite_sheet_id"];
        int portrait_id = j["base_class"][i]["portrait_id"];
        std::string class_name = j["base_class"][i]["class_name"];

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
    file.close();
 
    TraceLog(LOG_INFO, "==========end class data================");


    TraceLog(LOG_INFO, "==========check save data================");

    std::string save_path = "assets/save.json";
    if(std::filesystem::exists(save_path)) {
        g_game_data.save_available = true;
        TraceLog(LOG_INFO, "SAVE FILE FOUND");

/*         std::ifstream file(save_path);
        if (!file.is_open()) {
            TraceLog(LOG_INFO, "CANNOT OPEN FILE");
            //return;
        } */

    }

    TraceLog(LOG_INFO, "==========end save data================");


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