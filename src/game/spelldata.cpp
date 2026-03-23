#include "../core/gamedefs.h"



void LoadWandData(json &j) {
    TraceLog(LOG_INFO, "loading wands %i", j["wood_types"].size());

    g_wand_woods.resize(j["wood_types"].size());
    
    
    for(int type = 0; type < j["wood_types"].size(); type++) {
        TraceLog(LOG_INFO, "new_wood ");
        WandWood new_wood;
        new_wood.name = j["wood_types"][type]["name"];
        new_wood.bonus_points = j["wood_types"][type]["bonus_points"];
        new_wood.type = StrToWandWoodType(j["wood_types"][type]["wood_type"]);
        
        g_wand_woods[new_wood.type] = new_wood;
    }


    g_wand_profiles.resize(j["wand_profile"].size());
    
    for(int profile = 0; profile < j["wand_profile"].size(); profile++) {
        TraceLog(LOG_INFO, "new effect ");
        WandProfile new_profile;
        new_profile.name = j["wand_profile"][profile]["name"];

        new_profile.charge_bonus = j["wand_profile"][profile]["charge_bonus"];
        new_profile.damage_bonus = j["wand_profile"][profile]["damage_bonus"];
        new_profile.radius_bonus = j["wand_profile"][profile]["radius_bonus"];
        new_profile.duration_bonus = j["wand_profile"][profile]["duration_bonus"];

        g_wand_profiles.push_back(new_profile);
    }
}

void LoadSpellData(json &j) {

    TraceLog(LOG_INFO, "LOADING SPELL GEN RULES ");
    SpellRules new_rules;
    
    new_rules.stats.resize(SPELLSTAT_COUNT);

    for(int stat = 0; stat < j["stats"].size(); stat++) {

        SpellStat new_stat;
        
        SPELL_STAT stat_id =  StrToSpellStatId(j["stats"][stat]["stat_id"]);

        new_stat.stat_id = stat_id;
        new_stat.base = j["stats"][stat]["base"];
        new_stat.cost = j["stats"][stat]["cost"];
        new_stat.step = j["stats"][stat]["step"];
        new_stat.scale = j["stats"][stat]["scale"];
        new_stat.limit.min = j["stats"][stat]["min"];
        new_stat.limit.max = j["stats"][stat]["max"];
        
        new_rules.stats[stat_id] = new_stat;
    }


    new_rules.effects.resize(SPELL_EFFECT_COUNT);
    for(int effect = 0; effect < j["spell_effects"].size(); effect++) {
        SpellEffectData new_effect;
        new_effect.id = StrToSpellEffectId(j["spell_effects"][effect]["id"]);
        new_effect.color.r = j["spell_effects"][effect]["color"][0];
        new_effect.color.g = j["spell_effects"][effect]["color"][1];
        new_effect.color.b = j["spell_effects"][effect]["color"][2];
        new_rules.effects[new_effect.id] = new_effect;
    }

    for(int level = 0; level < j["spell_levels"].size(); level++) {
        new_rules.level_points.push_back(j["spell_levels"][level]["points"]);
    }


    TraceLog(LOG_INFO, "SPELL GEN RULES LOADED");

    g_spell_rules = new_rules;

}



SPELL_EFFECT StrToSpellEffectId(const std::string& s) {

    static const std::unordered_map<std::string, SPELL_EFFECT> lookup_table = {
        {"SPELL_EFFECT_FORCE",              SPELL_EFFECT::SPELL_EFFECT_FORCE},
        {"SPELL_EFFECT_FIRE",               SPELL_EFFECT::SPELL_EFFECT_FIRE},
        {"SPELL_EFFECT_LIGHTNING",          SPELL_EFFECT::SPELL_EFFECT_LIGHTNING},
        {"SPELL_EFFECT_POISON",             SPELL_EFFECT::SPELL_EFFECT_POISON},
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        return it->second;
    }
    TraceLog(LOG_INFO, "spell effect ID not found ");
    return SPELL_EFFECT::SPELL_EFFECT_NONE;
}


SPELL_STAT StrToSpellStatId(const std::string& s) {

    static const std::unordered_map<std::string, SPELL_STAT> lookup_table = {
        {"SPELLSTAT_CHARGE",          SPELL_STAT::SPELLSTAT_CHARGE},
        {"SPELLSTAT_DAMAGE",          SPELL_STAT::SPELLSTAT_DAMAGE},
        {"SPELLSTAT_RADIUS",          SPELL_STAT::SPELLSTAT_RADIUS},
        {"SPELLSTAT_DURATION",        SPELL_STAT::SPELLSTAT_DURATION},
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        return it->second;
    }
    TraceLog(LOG_INFO, "SPELL_STAT ID not found ");
    return SPELL_STAT::SPELLSTAT_CHARGE;
}

WANDWOOD_TYPE StrToWandWoodType(const std::string &s) {
    static const std::unordered_map<std::string, WANDWOOD_TYPE> lookup_table = {
        {"WANDWOOD_FRESH",                        WANDWOOD_TYPE::WANDWOOD_FRESH},
        {"WANDWOOD_AGED",                        WANDWOOD_TYPE::WANDWOOD_AGED},

    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        return it->second;
    }
    TraceLog(LOG_INFO, "spell WANDWOOD_TYPE ID not found ");
    return WANDWOOD_TYPE::WANDWOOD_FRESH;
}


int GetUsedSpellPoints(const SpellBuildData &build_data) {
    int used = 0;

    used += build_data.stat_points[SPELLSTAT_CHARGE];
    used += build_data.stat_points[SPELLSTAT_DAMAGE];
    used += build_data.stat_points[SPELLSTAT_RADIUS];
    used += build_data.stat_points[SPELLSTAT_DURATION];

    if (build_data.is_lingering) used += 2;
    if (build_data.is_exploding) used += 2;

    return used;
}

int GetRemainingSpellPoints(const SpellBuildData &build_data) {
    return build_data.total_points - GetUsedSpellPoints(build_data);
}

bool CanAddSpellPoint(const SpellBuildData &build_data, int cost) {
    return GetRemainingSpellPoints(build_data) >= cost;
}