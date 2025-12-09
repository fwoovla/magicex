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



enum ItemRarity {
    RARITY_COMMON = 50,
    RARITY_UNCOMMON = 30,
    RARITY_RARE = 10,
    RARITY_VERYRARE = 5,
    RARITY_ULTRARARE = 1,
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
    TYPE_PLAN,
    TYPE_SCROLL,
    TYPE_FOOD,
    TYPE_ALL,
};

extern Color g_item_type_colors[TYPE_ALL];

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
    int damage;
    int sprite_id;
    int icon_id;
    int level;
    float defence;
    float magic_defence;
    //Color item_color;
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
    int damage;
};

extern std::unordered_map<int, WeaponData> g_weapon_data;

struct ArmorData {
    std::string armor_name;
    ItemID armor_id;
    SpellID spell_id;
    float defence;
    float magic_defence;
};

extern std::unordered_map<int, ArmorData> g_armor_data;

struct RecipieData {
    std::string recipie_name;
    RecipieID recipie_id;
    std::vector<int> ingredients;
    ItemID produces;
};
extern std::unordered_map<int, RecipieData> g_recipie_data;


struct PlanData {
    PlanID plan_id;
    std::string plan_name;
    std::vector<int> ingredients;
    ModuleID module_id;
};

extern std::unordered_map<int, PlanData> g_plan_data;



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
    std::string uid;
    Vector2 position_i;
    Vector2 position_f;
    Vector2 return_position;
    Vector2 size;
};

struct ModuleEntityData {
    std::string identifier;
    std::string iid;
    int level_index;
    int module_id;
    Vector2 position_i;
    Vector2 position_f;
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
    int loot_level;
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
    std::vector<ModuleEntityData> module_data;
    std::vector<BaseArea*> game_areas;
    std::vector<BaseEntity*> entity_list;
    std::vector<Polygon> collision_polys;
    std::vector<EnvironmentalEntity *> environment_entities;
    std::vector<BaseEntity *> draw_list;
    //std::unordered_map <std::string, std::unique_ptr< SubScene> > sub_scene_instances; 

};

extern std::vector<std::vector<int>> g_loot_tables;

void LoadGameData();

void SaveGame(LevelData &level_data);

void LoadGame();


void ClearLevelData(LevelData &level_data);

void ClearSubLevelData();

void LoadLevelData(LevelData &level_data);


void PrecalculateTileCollisionData(LevelData &level_data);

void PrecalculateShadowData(LevelData &level_data);

void InstanceItemList(std::vector<int> &source_list, std::vector<int> &dest_list, std::string container_id);

void InstanceRandomItemsFromList(std::vector<int> &source_list, std::vector<int> &dest_list, std::string container_id, int loot_level);

void InstancePlayerItem(ItemID item_id);

void GenerateContainerItemList(int lti, std::vector<int> &list);




//Color StrToItemTypeColor(const std::string& s);

ItemRarity StrToItemRarity(const std::string& s);

ItemType StrToItemType(const std::string& s);

SpellID StrToSpellId(const std::string& s);

ItemID StrToItemId(const std::string& s);

PlanID StrToPlanId(const std::string& s);

ModuleID StrToModuleId(const std::string& s);

RecipieID StrToRecipieId(const std::string& s);

EnvironmentSpriteID StrToEnviroSpriteId(const std::string& s);

void from_json(const json &j, ItemInstanceData &i);

void from_json(const json &j, ContainerData &i);

void YSortEntities(LevelData & _level_data);





/* 

        {
            "item_name": "MAGICMISSLE_WAND 1",
            "item_id":"ITEM_ID_MAGICMISSLE_WAND_1",
            "value":0,
            "item_type":0
        },
        {
            "item_name": "MAGICMISSLE_WAND 2",
            "item_id":"ITEM_ID_MAGICMISSLE_WAND_2",
            "value":0,
            "item_type":0
        },
        {
            "item_name": "MAGICMISSLE_WAND 3",
            "item_id":"ITEM_ID_MAGICMISSLE_WAND_3",
            "value":0,
            "item_type":0
        },
        {
            "item_name": "FIREBALL_WAND 1",
            "item_id":"ITEM_ID_FIREBALL_WAND_1",
            "value":0,
            "item_type":0
        },
        {
            "item_name": "FIREBALL_WAND 2",
            "item_id":"ITEM_ID_FIREBALL_WAND_2",
            "value":0,
            "item_type":0
        },
        {
            "item_name": "FIREBALL_WAND 3",
            "item_id":"ITEM_ID_FIREBALL_WAND_3",
            "value":0,
            "item_type":0
        },
        {
            "item_name": "LIGHTNING_WAND 1",
            "item_id":"ITEM_ID_LIGHTNING_WAND_1",
            "value":0,
            "item_type":0
        },
        {
            "item_name": "LIGHTNING_WAND 2",
            "item_id":"ITEM_ID_LIGHTNING_WAND_2",
            "value":0,
            "item_type":0
        },
        {
            "item_name": "LIGHTNING_WAND 3",
            "item_id":"ITEM_ID_LIGHTNING_WAND_3",
            "value":0,
            "item_type":0
        },


        {
            "item_name": "MAGICMISSLE_STAFF 1",
            "item_id":"ITEM_ID_MAGICMISSLE_STAFF_1",
            "value":0,
            "item_type":0
        },
        {
            "item_name": "MAGICMISSLE_STAFF 2",
            "item_id":"ITEM_ID_MAGICMISSLE_STAFF_2",
            "value":0,
            "item_type":0
        },
        {
            "item_name": "MAGICMISSLE_STAFF 3",
            "item_id":"ITEM_ID_MAGICMISSLE_STAFF_3",
            "value":0,
            "item_type":0
        },
        {
            "item_name": "FIREBALL_STAFF 1",
            "item_id":"ITEM_ID_FIREBALL_STAFF_1",
            "value":0,
            "item_type":0
        },
        {
            "item_name": "FIREBALL_STAFF 2",
            "item_id":"ITEM_ID_FIREBALL_STAFF_2",
            "value":0,
            "item_type":0
        },
        {
            "item_name": "FIREBALL_STAFF 3",
            "item_id":"ITEM_ID_FIREBALL_STAFF_3",
            "value":0,
            "item_type":0
        },
        {
            "item_name": "LIGHTNING_STAFF 1",
            "item_id":"ITEM_ID_LIGHTNING_STAFF_1",
            "value":0,
            "item_type":0
        },
        {
            "item_name": "LIGHTNING_STAFF 2",
            "item_id":"ITEM_ID_LIGHTNING_STAFF_2",
            "value":0,
            "item_type":0
        },
        {
            "item_name": "LIGHTNING_STAFF 3",
            "item_id":"ITEM_ID_LIGHTNING_STAFF_3",
            "value":0,
            "item_type":0
        },




        {
            "weapon_id": "ITEM_ID_MAGICMISSLE_WAND_1",
            "weapon_name": "MAGICMISSLE WAND 1",
            "cooldown": 0.4,
            "spell_id":"SPELL_ID_MAGICMISSLE_1",
            "clip_size":10
        },
        {
            "weapon_id": "ITEM_ID_MAGICMISSLE_WAND_2",
            "weapon_name": "MAGICMISSLE WAND 2",
            "cooldown": 0.4,
            "spell_id":"SPELL_ID_MAGICMISSLE_2",
            "clip_size":10
        },
        {
            "weapon_id": "ITEM_ID_MAGICMISSLE_WAND_3",
            "weapon_name": "MAGICMISSLE WAND 3",
            "cooldown": 0.4,
            "spell_id":"SPELL_ID_MAGICMISSLE_3",
            "clip_size":10
        },

        {
            "weapon_id": "ITEM_ID_FIREBALL_WAND_1",
            "weapon_name": "FIREBALL WAND 1",
            "cooldown": 0.4,
            "spell_id":"SPELL_ID_FIREBALL_1",
            "clip_size":10
        },
        {
            "weapon_id": "ITEM_ID_FIREBALL_WAND_2",
            "weapon_name": "FIREBALL WAND 2",
            "cooldown": 0.4,
            "spell_id":"SPELL_ID_FIREBALL_2",
            "clip_size":10
        },
        {
            "weapon_id": "ITEM_ID_FIREBALL_WAND_3",
            "weapon_name": "FIREBALL WAND 3",
            "cooldown": 0.4,
            "spell_id":"SPELL_ID_FIREBALL_3",
            "clip_size":10
        },

        
        {
            "weapon_id": "ITEM_ID_LIGHTNING_WAND_1",
            "weapon_name": "LIGHTNING WAND 1",
            "cooldown": 0.4,
            "spell_id":"SPELL_ID_LIGHTNING_1",
            "clip_size":10
        },
        {
            "weapon_id": "ITEM_ID_LIGHTNING_WAND_2",
            "weapon_name": "LIGHTNING WAND 2",
            "cooldown": 0.4,
            "spell_id":"SPELL_ID_LIGHTNING_2",
            "clip_size":10
        },
        {
            "weapon_id": "ITEM_ID_LIGHTNING_WAND_3",
            "weapon_name": "LIGHTNING WAND 3",
            "cooldown": 0.4,
            "spell_id":"SPELL_ID_LIGHTNING_3",
            "clip_size":10
        },



        {
            "weapon_id": "ITEM_ID_MAGICMISSLE_STAFF_1",
            "weapon_name": "MAGICMISSLE STAFF 1",
            "cooldown": 0.1,
            "spell_id":"SPELL_ID_MAGICMISSLE_1",
            "clip_size":10
        },
        {
            "weapon_id": "ITEM_ID_MAGICMISSLE_STAFF_2",
            "weapon_name": "MAGICMISSLE STAFF 2",
            "cooldown": 0.4,
            "spell_id":"SPELL_ID_MAGICMISSLE_2",
            "clip_size":10
        },
        {
            "weapon_id": "ITEM_ID_MAGICMISSLE_STAFF_3",
            "weapon_name": "MAGICMISSLE STAFF 3",
            "cooldown": 0.4,
            "spell_id":"SPELL_ID_MAGICMISSLE_3",
            "clip_size":10
        },

        {
            "weapon_id": "ITEM_ID_FIREBALL_STAFF_1",
            "weapon_name": "FIREBALL STAFF 1",
            "cooldown": 0.4,
            "spell_id":"SPELL_ID_FIREBALL_1",
            "clip_size":10
        },
        {
            "weapon_id": "ITEM_ID_FIREBALL_STAFF_2",
            "weapon_name": "FIREBALL STAFF 2",
            "cooldown": 0.4,
            "spell_id":"SPELL_ID_FIREBALL_2",
            "clip_size":10
        },
        {
            "weapon_id": "ITEM_ID_FIREBALL_STAFF_3",
            "weapon_name": "FIREBALL STAFF 3",
            "cooldown": 0.4,
            "spell_id":"SPELL_ID_FIREBALL_3",
            "clip_size":10
        },

        {
            "weapon_id": "ITEM_ID_LIGHTNING_STAFF_1",
            "weapon_name": "LIGHTNING STAFF 1",
            "cooldown": 0.4,
            "spell_id":"SPELL_ID_LIGHTNING_1",
            "clip_size":10
        },
        {
            "weapon_id": "ITEM_ID_LIGHTNING_STAFF_2",
            "weapon_name": "LIGHTNING STAFF 2",
            "cooldown": 0.4,
            "spell_id":"SPELL_ID_LIGHTNING_2",
            "clip_size":10
        },
        {
            "weapon_id": "ITEM_ID_LIGHTNING_STAFF_3",
            "weapon_name": "LIGHTNING STAFF 3",
            "cooldown": 0.01,
            "spell_id":"SPELL_ID_LIGHTNING_3",
            "clip_size":10
        } */