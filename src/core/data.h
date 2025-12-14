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

enum ItemRarity {
    RARITY_COMMON = 50,
    RARITY_UNCOMMON = 30,
    RARITY_RARE = 10,
    RARITY_VERYRARE = 5,
    RARITY_ULTRARARE = 1,
};


enum ItemType {
    TYPE_WEAPON,
    TYPE_ARMOR,
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
extern std::unordered_map<int, Color>g_rarity_colors;

struct FoodModData {
    ItemModID mod_id;
    std::string mod_name;
    float saturation;
    int rarity;
};

extern std::unordered_map<int, FoodModData> g_food_mod_data;

struct ArmorModData {
    ItemModID mod_id;
    std::string mod_name;
    int defence;
    int rarity;
};

extern std::unordered_map<int, ArmorModData> g_armor_mod_data;


struct WeaponModData {
    ItemModID mod_id;
    std::string mod_name;

    float cooldown;
    int max_power;
    int damage;
    int rarity;
};

extern std::unordered_map<int, WeaponModData> g_weapon_mod_data;


struct FoodData {
    ItemID food_id;
    std::string food_name;
    SpellID spell_id;
    float saturation;
    int rarity;
};

extern std::unordered_map<int, FoodData> g_food_data;


struct CharacterEffectData {
    CharEffectID effect_id;
    std::string effect_name;
    float durration;
    int rarity;
};

extern std::unordered_map<int, CharacterEffectData> g_char_effect_data;

struct CharacterModData {
    CharModID mod_id;
    std::string mod_name;
    int health;
    float speed;
    float durration;
    int rarity;
};

extern std::unordered_map<int, CharacterModData> g_char_mod_data;

struct ItemData {
    ItemID id;
    int value;
    ItemType type;
    std::string item_name;
};

extern std::unordered_map<int, ItemData> g_item_data;

struct ItemInstanceData {
    std::vector<ItemModID> modifications;
    std::vector<CharModID> char_mods;
    std::string container_id;
    ItemID item_id;
    int instance_id;
    int value;
    ItemType type;
    std::string item_name;
    SpellID spell_id;
    float cooldown;
    int damage;
    int sprite_id;
    int icon_id;
    int level;
    int defence;
    int magic_defence;
    float saturation;
    int rarity;
    float max_power;
    float current_power;
};

extern std::unordered_map<int, ItemInstanceData> g_item_instances;

struct WeaponData {
    std::string weapon_name;
    ItemID weapon_id;
    float cooldown;
    SpellID spell_id;
    int max_power;
    int ammo_count;
    int damage;
};

extern std::unordered_map<int, WeaponData> g_weapon_data;

struct ArmorData {
    std::string armor_name;
    ItemID armor_id;
    SpellID spell_id;
    int defence;
    int magic_defence;
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
    ItemID plan_id;
    std::string plan_name;
    std::vector<int> ingredients;
    ModuleID module_id;
};

extern std::unordered_map<int, PlanData> g_plan_data;




struct CharacterData {
    int creature_id = -1;
    int health = 0;
    int exp = 0;
    float base_speed = 0;
    float current_speed = 0;
    int defence = 0;
    int magic_defence = 0;
    float max_power = 0;
    float current_power = 0;
    float saturation = 0;

    int sprite_sheet_id = -1;
    int portrait_id = -1;

    std::string name = "";
    std::string class_name = "";
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

extern std::unordered_map<int, CharacterData> g_class_data;
extern std::unordered_map<int, CharacterData> g_creature_data;

//extern PlayerData g_player_data;

extern CharacterData g_save_data;


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
    std::vector<BaseEntity*> spell_list;
    std::vector<Polygon> collision_polys;
    std::vector<EnvironmentalEntity *> environment_entities;
    std::vector<BaseEntity *> draw_list;
    //std::unordered_map <std::string, std::unique_ptr< SubScene> > sub_scene_instances; 

};

extern std::unordered_map<ItemType, std::vector<ItemID>> g_loot_tables;

void LoadGameData();

void SaveGame(LevelData &level_data);

int LoadGame();


void ClearLevelData(LevelData &level_data);

void ClearSubLevelData();

void LoadLevelData(LevelData &level_data);


void PrecalculateTileCollisionData(LevelData &level_data);

void PrecalculateShadowData(LevelData &level_data);

void InstanceItemList(std::vector<int> &source_list, std::vector<int> &dest_list, std::string container_id);

void InstanceRandomItemsFromList(std::vector<int> &source_list, std::vector<int> &dest_list, std::string container_id, int loot_level);

void InstanceCharacterItem(ItemID item_id, int character_uid);





CharEffectID StrToCharEffectId(const std::string& s);

CharModID StrToCharModId(const std::string& s);

ItemModID StrToItemModId(const std::string& s);

ItemRarity StrToItemRarity(const std::string& s);

ItemType StrToItemType(const std::string& s);

SpellID StrToSpellId(const std::string& s);

CreatureID StrToCreatureId(const std::string& s);

ItemID StrToItemId(const std::string& s);

PlanID StrToPlanId(const std::string& s);

ModuleID StrToModuleId(const std::string& s);

std::string ModuleIdToStr(const int id);

RecipieID StrToRecipieId(const std::string& s);

EnvironmentSpriteID StrToEnviroSpriteId(const std::string& s);

void from_json(const json &j, ItemInstanceData &i);

void from_json(const json &j, ContainerData &i);

void YSortEntities(LevelData & _level_data);





