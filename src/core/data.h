#pragma once

//#include <raylib.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include "json.hpp"

#include "gamedefs.h"
#include "utils.h"
#include "areas.h"

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

struct ItemData {
    int id;
    int sprite_id;
    std::string item_name;
};

extern std::unordered_map<int, ItemData> g_item_data;

struct PlayerData {
    int health;
    int exp;
    float base_speed;
    int sprite_sheet_id;
    int portrait_id;
    std::string name;
    std::string class_name;
    std::vector<int> inventory;
    std::vector<int> hotbar;

};

extern std::unordered_map<int, PlayerData> g_class_data;

extern PlayerData g_player_data;

extern PlayerData g_save_data;


struct LevelTransitionData {
    std::string identifier;
    std::string dest_string;
    Vector2 position_i;
    Vector2 position_f;
    Vector2 return_position;
    Vector2 size;
};

struct LevelData {
    bool is_shelter;
    Vector2 spawn_position;
    std::vector<LevelTransitionData> level_transitions;
    std::vector<BaseArea*> game_areas;
};

extern LevelData g_level_data;


void LoadGameData();

void SaveGame();

void LoadGame();


void ClearLevelData();

void LoadLevelData();


