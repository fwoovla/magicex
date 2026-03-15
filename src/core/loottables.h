#pragma once

#include "gamedefs.h"
//#include "loottables.h"


struct ItemInstanceData;
enum ItemID;



enum LootRarity {
    LOOTRARITY_COMMON,
    LOOTRARITY_UNCOMMON,
    LOOTRARITY_RARE,
};

enum LootTableID {
    LT_NONE = -1,
    LT_COMMON_CHEST,
    LT_SCAVENGER,
    LT_ANIMAL,
    LT_STARTINGCHEST
};

enum LootPoolID {
    POOL_NONE = -1,
    POOL_WEAPONS,
    POOL_FOOD,
    POOL_RESOURCE,
    POOL_ARMOR,
    POOL_STARTINGCHEST
};


struct LootEntry {

    ItemID item_id;
    float weight;
    LootRarity rarity;

};

struct LootPool {
    LootPoolID pool_id;
    int min_rolls;
    int max_rolls;
    std::vector<LootEntry> entries;

};


struct LootTable {

    LootTableID table_id;
    std::vector<LootPoolID> pool_ids;

};


extern std::unordered_map<LootTableID, LootTable> g_creature_loot_tables;
extern std::unordered_map<LootPoolID, LootPool> g_creature_loot_pools;

void LoadLootTables(json &j);

void LoadLootPools(json &j);

void LoadStatLimits(json &j);

void LoadStatGenData(json &j);

ItemID RollWeighted(std::vector<LootEntry>& entries, int max_rarity);

std::vector<int> GenerateItemsFromLootTable(LootTableID table_id, std::string container_id, int level);


STAT_ID StrToStatId(const std::string& s);

LootTableID StrToLootTableId(const std::string& s);

LootPoolID StrToLootPoolId(const std::string& s);

LootRarity StrToLootRarity(const std::string& s);