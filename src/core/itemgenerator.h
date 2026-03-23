#pragma once

#include "gamedefs.h"
#include "loottables.h"
#include "data.h"

struct StatGenData
{
    float cost;
    float step;
};


extern std::vector<Limit> g_stat_limits;
//extern std::vector<StatGenData> g_statgen_data;


ItemInstanceData GenerateItem(ItemID item_id, int uid, std::string container_id);

ItemInstanceData GenerateRandomItem(ItemID item_id, int uid, std::string container_id, int loot_level);

void GenerateWeapon(ItemInstanceData &instance, int loot_level, bool random);

void GenerateArmor(ItemInstanceData &instance, int loot_level, bool random);

void GenerateFood(ItemInstanceData &instance, int loot_level, bool random);

void GenerateCaster(ItemInstanceData &instance, int loot_level, bool random);

void BuildItemSprite(ItemInstanceData &instance);

void GenerateItemName(ItemInstanceData &instance);

SpellData GenerateSpell(ItemInstanceData &instance);

//void AddSpellToItem(ItemInstanceData &instance, SpellID spell_id);

/* CasterBaseData GenerateBase(CasterBaseData &base, ITEM_TIER tier);

IgniterData GenerateIgniter(IgniterData &base, ITEM_TIER tier);

CouplerData GenerateCoupler(CouplerData &base, ITEM_TIER tier);

RodData GenerateRod(RodData &base, ITEM_TIER tier); */

void SpendStatPoints(float &stat, STAT_ID id, int &points);

//void ApplyTierBonusPercent(float &stat, STAT_ID id, float bonus);

//void ApplyTierBonusPoints(float &stat, STAT_ID id, float bonus);


void ClampStat(float &stat, STAT_ID id);

ITEM_TIER GetTier(int loot_level);
