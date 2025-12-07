#include "../core/gamedefs.h"


ItemInstanceData GenerateItem(ItemID item_id, int uid, std::string container_id) {
    
    ItemInstanceData new_instance;

    //int uid = GetRandomValue(1000, 1000000000);
    new_instance.instance_id = uid;
    new_instance.item_id = item_id;

    auto item_it = g_item_data.find(item_id);
    if(item_it != g_item_data.end()) {
        new_instance.item_name = item_it->second.item_name;
        new_instance.type = item_it->second.type;
        new_instance.value = item_it->second.value;
        new_instance.clip_size = 0;
        new_instance.ammo_count = 0;
        new_instance.container_id = container_id;

        //TraceLog(LOG_INFO, "item uid %i container id %s", uid, new_instance.container_id.c_str());
    }
    return new_instance;
}

ItemInstanceData GenerateRandomItem(ItemID item_id, int uid, std::string container_id, int loot_level) {
    
    ItemInstanceData new_instance;

    //int uid = GetRandomValue(1000, 1000000000);
    new_instance.instance_id = uid;
    new_instance.item_id = item_id;

    auto item_it = g_item_data.find(item_id);
    if(item_it != g_item_data.end()) {
        new_instance.item_name = item_it->second.item_name;
        new_instance.type = item_it->second.type;
        new_instance.value = item_it->second.value;
        new_instance.clip_size = 0;
        new_instance.ammo_count = 0;
        new_instance.container_id = container_id;
        new_instance.spell_id = SPELL_ID_NONE;

        if(new_instance.type == TYPE_WEAPON ) {
            GenerateRandomWeapon(new_instance, loot_level);
        }
        //TraceLog(LOG_INFO, "item uid %i container id %s", uid, new_instance.container_id.c_str());
    }
    return new_instance;
}

void GenerateRandomWeapon(ItemInstanceData &instance, int loot_level) {
    TraceLog(LOG_INFO, "making new weapon" );

    auto w_itter = g_weapon_data.find(instance.item_id);
    if(w_itter != g_weapon_data.end()) {
        instance.clip_size = w_itter->second.clip_size;
        TraceLog(LOG_INFO, "-clipsize %i", instance.clip_size );
        instance.cooldown = w_itter->second.cooldown;
        TraceLog(LOG_INFO, "-cooldown %0.3f", instance.cooldown );
        instance.damage =  w_itter->second.damage;
    }

    if(instance.item_id == ITEM_ID_WAND or instance.item_id == ITEM_ID_STAFF) {
        instance.spell_id = (SpellID)GetRandomValue(SPELL_ID_MAGICMISSLE_1, SPELL_ID_LIGHTNING_1 + ( 3 * loot_level) );
        auto s_itter = g_spell_data.find(instance.spell_id);
        if(s_itter != g_spell_data.end()) {
            instance.item_name += " of " + s_itter->second.spell_name;
            TraceLog(LOG_INFO, "-name  + %s", s_itter->second.spell_name.c_str());
        }
    }
}