#include "../core/gamedefs.h"







void LoadLootTables(json &j) {

    for(int table = 0; table < j.size(); table++) {
        LootTable new_table;

        new_table.table_id = StrToLootTableId(j[table]["table_id"]);

        //TraceLog(LOG_INFO, "++++++ LOOT TABLE Data  Loading  id: %i  ", new_table.table_id);

        for(int pool = 0; pool < j[table]["pools"].size(); pool++) {
            LootPoolID id = StrToLootPoolId(j[table]["pools"][pool]);
            new_table.pool_ids.push_back(id);
            //TraceLog(LOG_INFO, "        pool id: %i  ", id);
        }

        g_creature_loot_tables[new_table.table_id] = new_table;
        
    }


}


void LoadLootPools(json &j) {

    for(int pool = 0; pool < j.size(); pool++) {
        LootPool new_pool;

        new_pool.pool_id = StrToLootPoolId(j[pool]["pool_id"]);
        new_pool.min_rolls = j[pool]["rolls_min"];
        new_pool.max_rolls = j[pool]["rolls_max"];

        //TraceLog(LOG_INFO, "----- POOL Data  Loading  id: %i  ", new_pool.pool_id);

        for( int entry = 0; entry < j[pool]["entries"].size(); entry++) {
            LootEntry new_entry;

            new_entry.item_id = StrToItemId(j[pool]["entries"][entry]["item_id"]);
            new_entry.weight = j[pool]["entries"][entry]["weight"];
            new_entry.rarity = StrToLootRarity(j[pool]["entries"][entry]["min_rarity"]);

            new_pool.entries.push_back(new_entry);
            //TraceLog(LOG_INFO, "LOOT POOL ENTRY Data  Loaded  id: %i  ", new_entry.item_id);

        }
        g_creature_loot_pools[new_pool.pool_id] = new_pool;
        
    }

}


void LoadStatLimits(json &j) {

    g_stat_limits.resize(STAT_COUNT, {0,0});

    g_stat_limits[STAT_DAMAGE] = {
        j["damage_min"],
        j["damage_max"],
    };

    g_stat_limits[STAT_CHARGE] = {
        j["cooldown_min"],
        j["cooldown_max"],
    };

    g_stat_limits[STAT_PPS] = {
        j["pps_min"],
        j["pps_max"],
    };

    g_stat_limits[STAT_MAX_POWER] = {
        j["max_power_min"],
        j["max_power_max"],
    };

    g_stat_limits[STAT_RECOIL] = {
        j["recoil_min"],
        j["recoil_max"],
    };

    g_stat_limits[STAT_KNOCKBACK] = {
        j["knockback_min"],
        j["knockback_max"],
    };

    g_stat_limits[STAT_ACCURACY] = {
        j["accuracy_min"],
        j["accuracy_max"],
    };
    
}


void LoadStatGenData(json &j) {
/*     g_statgen_data.resize(STAT_COUNT);

    g_statgen_data[STAT_DAMAGE] = {
        j["damage"]["cost"],
        j["damage"]["step"]
    };

    g_statgen_data[STAT_PPS] = {
        j["pps"]["cost"],
        j["pps"]["step"]
    };

    g_statgen_data[STAT_COOLDOWN] = {
        j["cooldown"]["cost"],
        j["cooldown"]["step"]
    };

    g_statgen_data[STAT_KNOCKBACK] = {
        j["knockback"]["cost"],
        j["knockback"]["step"]
    };

    g_statgen_data[STAT_RECOIL] = {
        j["recoil"]["cost"],
        j["recoil"]["step"]
    };

    g_statgen_data[STAT_ACCURACY] = {
        j["accuracy"]["cost"],
        j["accuracy"]["step"]
    };

    g_statgen_data[STAT_MAX_POWER] = {
        j["max_power"]["cost"],
        j["max_power"]["step"]
    }; */
}


//returns a list of instance ids
std::vector<int> GenerateItemsFromLootTable(LootTableID table_id, std::string container_id, int level) {
    
    std::vector<int> item_list;
    
    LootTable &table = g_creature_loot_tables[table_id];
    
    TraceLog(LOG_INFO, "-----------------generating loot from table: %i  #pools %i loot level %i", table_id, table.pool_ids.size(), level);

    for(LootPoolID &pool_id : table.pool_ids) {
        TraceLog(LOG_INFO, "-----------------pool id: %i  ", pool_id);
        LootPool& pool = g_creature_loot_pools[pool_id];

        int rolls = GetRandomValue(pool.min_rolls, pool.max_rolls);

        for(int r = 0; r < rolls; r++) {
            TraceLog(LOG_INFO, "------------------------roll: %i  ", r);
            ItemID entry = RollWeighted(pool.entries, 2);

            int instance_id = GetRandomValue(10000, 10000000);
            ItemInstanceData new_inst = GenerateRandomItem(entry, instance_id, container_id, level);
            g_item_instances[instance_id] = new_inst;
    
            item_list.push_back(instance_id);
            TraceLog(LOG_INFO, "-----------------loot item generated  id: %i  ", instance_id);
        }
    }

    return item_list;
}




ItemID RollWeighted(std::vector<LootEntry>& entries, int max_rarity) {

    int total = 0;

    for(const auto& e : entries) {
        if(e.rarity <= max_rarity) {
            total += e.weight;
        }
    }

    int roll = GetRandomValue(0, total - 1);

    int acc = 0;
    for(const auto& e : entries) {
        if(e.rarity <= max_rarity) {
            acc += e.weight;
            if(roll < acc) {
                return e.item_id;
            }
        }
    }

    return ITEM_ID_NONE;
}



STAT_ID StrToStatId(const std::string& s) {

    static const std::unordered_map<std::string, STAT_ID> lookup_table = {

        {"STAT_DAMAGE",     STAT_ID::STAT_DAMAGE},
        {"STAT_PPS",        STAT_ID::STAT_PPS},
        {"STAT_CHARGE",   STAT_ID::STAT_CHARGE},
        {"STAT_KNOCKBACK",  STAT_ID::STAT_KNOCKBACK},
        {"STAT_RECOIL",     STAT_ID::STAT_RECOIL},
        {"STAT_ACCURACY",   STAT_ID::STAT_ACCURACY},
       

    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        return it->second;
        
    }
    TraceLog(LOG_INFO, "STAT_ID  not found ");
    return STAT_ID::STAT_DAMAGE;

}


LootTableID StrToLootTableId(const std::string& s) {

    static const std::unordered_map<std::string, LootTableID> lookup_table = {

        {"LT_ANIMAL",        LootTableID::LT_ANIMAL},
        {"LT_COMMON_CHEST",        LootTableID::LT_COMMON_CHEST},
        {"LT_SCAVENGER",        LootTableID::LT_SCAVENGER},
        {"LT_STARTINGCHEST",        LootTableID::LT_STARTINGCHEST},

    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        return it->second;
    }
    return LootTableID::LT_NONE;
}


LootPoolID StrToLootPoolId(const std::string& s) {

    static const std::unordered_map<std::string, LootPoolID> lookup_table = {

        {"POOL_WEAPONS",         LootPoolID::POOL_WEAPONS},
        {"POOL_FOOD",             LootPoolID::POOL_FOOD},
        {"POOL_RESOURCE",             LootPoolID::POOL_RESOURCE},
        {"POOL_ARMOR",             LootPoolID::POOL_ARMOR},
        {"POOL_STARTINGCHEST",             LootPoolID::POOL_STARTINGCHEST},

    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        return it->second;
    }
    return LootPoolID::POOL_NONE;
}

LootRarity StrToLootRarity(const std::string& s) {

    static const std::unordered_map<std::string, LootRarity> lookup_table = {

        {"LOOTRARITY_COMMON",         LootRarity::LOOTRARITY_COMMON},
        {"LOOTRARITY_UNCOMMON",             LootRarity::LOOTRARITY_UNCOMMON},
        {"LOOTRARITY_RARE",             LootRarity::LOOTRARITY_RARE},

    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        return it->second;
    }
    return LootRarity::LOOTRARITY_COMMON;
}