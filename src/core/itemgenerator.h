#pragma once

#include "gamedefs.h"
#include "loottables.h"
#include "data.h"


ItemInstanceData GenerateItem(ItemID item_id, int uid, std::string container_id);

ItemInstanceData GenerateRandomItem(ItemID item_id, int uid, std::string container_id, int loot_level);

void GenerateWeapon(ItemInstanceData &instance, int loot_level, bool random);

void GenerateArmor(ItemInstanceData &instance, int loot_level, bool random);

void GenerateFood(ItemInstanceData &instance, int loot_level, bool random);

void GenerateCaster(ItemInstanceData &instance, int loot_level, bool random);

void BuildItemSprite(ItemInstanceData &instance);

//void AddSpellToItem(ItemInstanceData &instance, SpellID spell_id);

void AddModToItem(ItemInstanceData &instance, ItemModID mod_id);

