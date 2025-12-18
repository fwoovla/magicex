#pragma once

#include "gamedefs.h"
#include "data.h"


ItemInstanceData GenerateItem(ItemID item_id, int uid, std::string container_id);

ItemInstanceData GenerateRandomItem(ItemID item_id, int uid, std::string container_id, int loot_level);

void GenerateScroll(ItemInstanceData &instance, SpellID spell_id, std::string container_id);

void GenerateWeapon(ItemInstanceData &instance, int loot_level, bool random);

void GenerateArmor(ItemInstanceData &instance, int loot_level, bool random);

void GenerateFood(ItemInstanceData &instance, int loot_level, bool random);



