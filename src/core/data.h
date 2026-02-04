#pragma once

//#include <raylib.h>
#include <vector>
#include <span>
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
    TYPE_CHARM,
    TYPE_ALL,
    TYPE_MAX
};

extern std::vector<Color> g_item_type_colors;

extern std::vector<Color> g_rarity_colors;

struct FoodModData {
    ItemModID mod_id;
    std::string mod_name;
    float saturation;
    int rarity;
};

extern std::vector<FoodModData> g_food_mod_data;


/* struct ItemModData {
    ItemModID mod_id;
    std::string mod_name;

    float saturation;
    int defence;
    int magic_defence;
    float cooldown;
    int max_power;
    int damage;
    int rarity;
};

extern std::vector<ItemModData> g_equipment_mod_data;
 */

struct FoodData {
    ItemID food_id;
    std::string food_name;
    SpellID spell_id;
    float saturation;
    //int rarity;
};

extern std::vector<FoodData> g_food_data;


struct ItemData {
    ItemID id;
    int value;
    ItemType type;
    std::string item_name;
};

extern std::vector<ItemData> g_item_data;



struct WeaponData {
    std::string weapon_name;
    ItemID weapon_id;
    float cooldown;
    SpellID spell_id;
    float max_power;
    float current_power;
    int damage;
    float recoil;
    float knockback;
    int mod_slots;
    float pps;
    int shots;
    float accuracy;
    int spread;
};

extern std::vector<WeaponData> g_weapon_data;

struct ArmorData {
    std::string armor_name;
    ItemID armor_id;
    SpellID spell_id;
    int defence;
    int magic_defence;
    int mod_slots;
};

extern std::vector<ArmorData> g_armor_data;


/* struct CharmData {
    std::string charm_name;
    ItemID charm_id;
    ItemModID mod_id;
    ItemType use_type;

};

extern std::vector<CharmData> g_charm_data;
 */
struct RecipieData {
    std::string recipie_name;
    RecipieID recipie_id;
    std::vector<int> ingredients;
    ItemID produces;
};
extern std::vector<RecipieData> g_recipie_data;


struct PlanData {
    ItemID plan_id;
    std::string plan_name;
    std::vector<int> ingredients;
    ModuleID module_id;
};

extern std::vector<PlanData> g_plan_data;



struct ItemInstanceData {
    SpellID spell_id;
    SpellData spell_data;
    ItemID weapon_id;
    WeaponData weapon_data;
    ItemID armor_id;
    ArmorData armor_data;
    ItemID food_id;
    FoodData food_data;
    
    ItemID item_id;
    ItemType type;
    std::string item_name;
    std::string container_id;
    int instance_id;
    int value;
    int sprite_id;
    int icon_id;
    int level;
    int rarity;
    int mod_slots;

};

extern std::unordered_map<int, ItemInstanceData> g_item_instances;

struct AiData {
    AIID ai_id;
    std::string ai_name;
    int hostility;
};

extern std::vector<AiData> g_ai_data;

struct CharacterData {


    std::vector<int> inventory;
    std::vector<int> hotbar;

    std::vector<int> primary;
    std::vector<int> secondary;
    std::vector<int> head;
    std::vector<int> body;
    std::vector<int> legs;
    std::vector<int> feet;
    std::vector<int> hands;

    int creature_id = -1;
    int health = 0;
    int max_health;
    int exp = 0;
    float base_speed = 0;
    float current_speed = 0;
    int defence = 0;
    int magic_defence = 0;
    float max_power = 0;
    float current_power = 0;
    float saturation = 0.0f;
    float max_saturation = 0.0f;
    float max_stamina = 0.0f;
    float current_stamina = 0.0f;
    AiData ai_data;
    bool is_npc = false;

    int sprite_sheet_id = -1;
    int portrait_id = -1;
    Vector2 spawn_position;

    std::string name = "";
    std::string class_name = "";

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
extern std::unordered_map<std::string, ContainerData> g_sub_temp_containers;

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
    std::vector<CharacterData> creature_data;
    std::vector<Polygon> collision_polys;

    std::vector<BaseEntity *> draw_list;
    
    std::vector<std::unique_ptr<BaseEntity>> entity_list;
    std::vector<std::unique_ptr<BaseEntity>> spell_list;
    std::vector<std::unique_ptr<BaseEntity>> environment_entities;
    std::vector<std::unique_ptr<BaseEntity>> ui_entities;
    std::vector<std::unique_ptr<BaseArea>> game_areas;
};


struct SubSceneState {
    std::vector<ContainerData> container_data;
};




extern std::unordered_map<ItemType, std::vector<ItemID>> g_loot_tables;
extern std::unordered_map<CreatureID, std::vector<ItemID>> g_creature_loot_tables;


void LoadGameData();

void SaveGame(LevelData &level_data);

int LoadGame();

void SaveSubSceneState(LevelData &level_data, SubSceneState & sub_state);

void LoadSubSceneState(SubSceneState & sub_state, LevelData &level_data);

void ClearLevelData(LevelData &level_data);

//void ClearSubLevelData();

void LoadLevelData(LevelData &level_data);


void PrecalculateTileCollisionData(LevelData &level_data);

void PrecalculateShadowData(LevelData &level_data);

void InstanceItemList(std::vector<int> &source_list, std::vector<int> &dest_list, std::string container_id);

void InstanceRandomItemsFromList(std::vector<int> &source_list, std::vector<int> &dest_list, std::string container_id, int loot_level);

ItemInstanceData* InstanceCharacterItem(ItemID item_id, int character_uid);

ItemInstanceData* InstanceRandomCharacterItem(ItemID item_id, int character_uid, int _level);


CharEffectID StrToCharEffectId(const std::string& s);

CharModID StrToCharModId(const std::string& s);

ItemModID StrToItemModId(const std::string& s);

ItemRarity StrToItemRarity(const std::string& s);

ItemType StrToItemType(const std::string& s);

SpellID StrToSpellId(const std::string& s);

AIID StrToAiId(const std::string& s);

CreatureID StrToCreatureId(const std::string& s);

ItemID StrToItemId(const std::string& s);

SpriteSheetID StrToSpriteId(const std::string& s);

PlanID StrToPlanId(const std::string& s);

ModuleID StrToModuleId(const std::string& s);

std::string ModuleIdToStr(const int id);

std::string ItemTypeToStr(const int id);

RecipieID StrToRecipieId(const std::string& s);

EnvironmentSpriteID StrToEnviroSpriteId(const std::string& s);

std::string EnvironmentalIdToStr(const int id);

void from_json(const json &j, ItemInstanceData &i);

void from_json(const json &j, ContainerData &i);

void YSortEntities(LevelData & _level_data);





