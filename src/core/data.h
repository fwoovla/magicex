#pragma once

//#include <raylib.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include "json.hpp"

#include "gamedefs.h"
#include "resources.h"

#include "utils.h"
#include "spells.h"
#include "areas.h"
#include "spells.h"
#include "baseentity.h"
#include "entities.h"

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

enum ItemType {
    TYPE_WEAPON,
    TYPE_HEAD_ARMOR,
    TYPE_BODY_ARMOR,
    TYPE_LEG_ARMOR,
    TYPE_FEET_ARMOR,
    TYPE_HAND_ARMOR,
    TYPE_CONSUMEABLE,
    TYPE_RESOURCE,
    TYPE_ALL,
};

struct ItemData {
    ItemID id;
    int value;
    ItemType type;
    std::string item_name;
    SpellID spell_id;
};

extern std::unordered_map<int, ItemData> g_item_data;

struct WeaponData {
    std::string weapon_name;
    ItemID weapon_id;
    float cooldown;
    SpellID spell_id;
};

extern std::unordered_map<int, WeaponData> g_weapon_data;


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

    std::vector<int> primary;
    std::vector<int> secondary;
    std::vector<int> head;
    std::vector<int> body;
    std::vector<int> legs;
    std::vector<int> feet;
    std::vector<int> hands;
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

struct ContainerData {
    std::string identifier;
    int loot_table_id;
    int sprite_id;
    Vector2 position_i;
    Vector2 position_f;
    Vector2 size;
    std::vector<int> item_list;
};

struct LevelData {
    bool is_shelter;
    Vector2 spawn_position;
    std::vector<LevelTransitionData> level_transitions;
    std::vector<ContainerData> container_data;
    std::vector<BaseArea*> game_areas;
    std::vector<BaseEntity*> entity_list;
    std::vector<BaseEntity*> spell_list;
};

extern std::vector<std::vector<ItemID>> g_loot_tables;

void LoadGameData();

void SaveGame();

void LoadGame();


void ClearLevelData(LevelData &level_data);

void LoadLevelData(LevelData &level_data);


void GenerateContainerItemList(int lti, std::vector<int> &list);


SpellID StrToSpellId(const std::string& s);


//SpellID StrToWeaponID(const std::string& s);


ItemID StrToItemId(const std::string& s);
