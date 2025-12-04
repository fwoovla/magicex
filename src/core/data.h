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
#include "sprite.h"

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


struct ItemInstanceData {
    std::string container_id;
    ItemID item_id;
    int instance_id;
    int value;
    ItemType type;
    std::string item_name;
    SpellID spell_id;
    int clip_size;
    int ammo_count;
    float cooldown;
};

extern std::unordered_map<int, ItemData> g_item_data;

extern std::unordered_map<int, ItemInstanceData> g_item_instances;

struct WeaponData {
    std::string weapon_name;
    ItemID weapon_id;
    float cooldown;
    SpellID spell_id;
    int clip_size;
    int ammo_count;
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


struct MushroomZoneData {
    int max_mushrooms;
    Vector2 position_i;
    Vector2 size;
};

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
    std::string iid;
    int level_index;
    int loot_table_id;
    int sprite_id;
    Vector2 position_i;
    Vector2 position_f;
    Vector2 size;
    std::vector<int> item_list;
};

extern std::unordered_map<std::string, ContainerData> g_persistant_containers;

struct LevelPrecalcData {
    int collision_layer_index;
    int foreground_layer_index;
    int map_index;
    int map_width;
    int tile_size;
    float inv_tile_size;
};

struct LevelData {
    bool is_shelter;
    Vector2 spawn_position;
    LevelPrecalcData precalc;

    std::vector<MushroomZoneData> mushroom_zones;
    std::vector<LevelTransitionData> level_transitions;
    std::vector<ContainerData> container_data;
    std::vector<BaseArea*> game_areas;
    std::vector<BaseEntity*> entity_list;
    std::vector<Polygon> collision_polys;
    std::vector<EnvironmentalEntity *> environment_entities;
    std::vector<BaseEntity *> draw_list;

};

extern std::vector<std::vector<int>> g_loot_tables;

void LoadGameData();

void SaveGame(LevelData &level_data);

void LoadGame();


void ClearLevelData(LevelData &level_data);

void LoadLevelData(LevelData &level_data);


void PrecalculateTileCollisionData(LevelData &level_data);

void PrecalculateShadowData(LevelData &level_data);

void InstanceItemList(std::vector<int> &source_list, std::vector<int> &dest_list, std::string container_id);

void GenerateContainerItemList(int lti, std::vector<int> &list);


SpellID StrToSpellId(const std::string& s);

ItemID StrToItemId(const std::string& s);

EnvironmentSpriteID StrToEnviroSpriteId(const std::string& s);

void from_json(const json &j, ItemInstanceData &i);

void from_json(const json &j, ContainerData &i);

void YSortEntities(LevelData & _level_data);