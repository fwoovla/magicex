#pragma once

#include "gamedefs.h"
#include "data.h"


ItemInstanceData GenerateItem(ItemID item_id, int uid, std::string container_id);

ItemInstanceData GenerateRandomItem(ItemID item_id, int uid, std::string container_id, int loot_level);


void GenerateRandomWeapon(ItemInstanceData &instance, int loot_level);