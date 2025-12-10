#include "../core/gamedefs.h"


ItemInstanceData GenerateItem(ItemID item_id, int uid, std::string container_id) {
    
    ItemInstanceData new_instance;

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
        new_instance.sprite_id = item_id;
        new_instance.icon_id = item_id;
        new_instance.level = 1;

    }
    return new_instance;
}

ItemInstanceData GenerateRandomItem(ItemID item_id, int uid, std::string container_id, int loot_level) {

    
    ItemInstanceData new_instance;

    new_instance.instance_id = uid;
    new_instance.item_id = item_id;

    auto item_it = g_item_data.find(item_id);
    if(item_it != g_item_data.end()) {
        new_instance.item_name = item_it->second.item_name;
        TraceLog(LOG_INFO, "Generating new Item %s", item_it->second.item_name.c_str());
        new_instance.type = item_it->second.type;
        new_instance.value = item_it->second.value;
        new_instance.clip_size = 0;
        new_instance.ammo_count = 0;
        new_instance.container_id = container_id;
        new_instance.spell_id = SPELL_ID_NONE;
        new_instance.sprite_id = item_id;
        new_instance.icon_id = item_id;
        new_instance.defence = 0;
        new_instance.magic_defence = 0;

        if(new_instance.type == TYPE_WEAPON ) {
            GenerateRandomWeapon(new_instance, loot_level);
        }
        if(new_instance.type >= TYPE_HEAD_ARMOR and new_instance.type <= TYPE_HAND_ARMOR ) {
            GenerateRandomArmor(new_instance, loot_level);
        }
        if(new_instance.type == TYPE_FOOD ) {
            GenerateRandomFood(new_instance, loot_level);
            
            //GenerateRandomWeapon(new_instance, loot_level);
        }
    }
    TraceLog(LOG_INFO, "---------------------\n");
    return new_instance;
}

void GenerateRandomWeapon(ItemInstanceData &instance, int loot_level) {
    TraceLog(LOG_INFO, "making new weapon %i", loot_level);

    auto w_itter = g_weapon_data.find(instance.item_id);
    if(w_itter != g_weapon_data.end()) {
        instance.clip_size = w_itter->second.clip_size;
        TraceLog(LOG_INFO, "-clipsize %i", instance.clip_size );
        instance.cooldown = w_itter->second.cooldown;
        TraceLog(LOG_INFO, "-cooldown %0.3f", instance.cooldown );
        instance.damage =  w_itter->second.damage;
        TraceLog(LOG_INFO, "-damage %i", instance.damage );
        instance.sprite_id = w_itter->second.weapon_id;
        instance.icon_id = w_itter->second.weapon_id;
        TraceLog(LOG_INFO, "-sprite_id %i", instance.sprite_id );
        TraceLog(LOG_INFO, "-icon_id %i", instance.icon_id );
    }

    if(loot_level >= 1) {
        ItemModID mod_id = (ItemModID)GetRandomValue(ITEMMOD_WEPONSTART, ITEMMOD_WEPONEND);

        auto m_itter = g_weapon_mod_data.find(mod_id);
        if(m_itter != g_weapon_mod_data.end()) {
            if(m_itter->second.cooldown != -10000) { instance.cooldown += m_itter->second.cooldown; }
            if(m_itter->second.clip_size != -10000) { instance.clip_size += m_itter->second.clip_size; }
            if(m_itter->second.damage != -10000) { instance.damage += m_itter->second.damage; }

            instance.modifications.push_back(mod_id);
            TraceLog(LOG_INFO, "-----weapon mod selected  + %i------", instance.modifications[instance.modifications.size() - 1]);
        }
    }

    if(instance.item_id == ITEM_ID_WAND or instance.item_id == ITEM_ID_STAFF) {

        instance.spell_id = (SpellID)GetRandomValue(SPELL_ID_MAGICMISSLE, SPELL_ID_LIGHTNING);
        TraceLog(LOG_INFO, "-spell selected  + %i", instance.spell_id);

        auto s_itter = g_spell_data.find((int)instance.spell_id);
        if(s_itter != g_spell_data.end()) {
            instance.item_name += " of " + s_itter->second.spell_name;
            TraceLog(LOG_INFO, "-name  + %s", s_itter->second.spell_name.c_str());

            if(instance.spell_id == SPELL_ID_MAGICMISSLE) {
                if(instance.item_id == ITEM_ID_WAND) {
                    instance.sprite_id = ITEM_ID_MAGICMISSLE_WAND;
                    instance.icon_id = ITEM_ID_MAGICMISSLE_WAND;
                }
                if(instance.item_id == ITEM_ID_STAFF) {
                    instance.sprite_id = ITEM_ID_MAGICMISSLE_STAFF;
                    instance.icon_id = ITEM_ID_MAGICMISSLE_WAND;
                }
            }
            
            if(instance.spell_id == SPELL_ID_FIREBALL) {
                if(instance.item_id == ITEM_ID_WAND) {
                    instance.sprite_id = ITEM_ID_FIREBALL_WAND;
                    instance.icon_id = ITEM_ID_FIREBALL_WAND;
                }
                if(instance.item_id == ITEM_ID_STAFF) {
                    instance.sprite_id = ITEM_ID_FIREBALL_STAFF;
                    instance.icon_id = ITEM_ID_FIREBALL_STAFF;
                }
            }
            
            if(instance.spell_id == SPELL_ID_LIGHTNING) {
                if(instance.item_id == ITEM_ID_WAND) {
                    instance.sprite_id = ITEM_ID_LIGHTNING_WAND;
                    instance.icon_id = ITEM_ID_LIGHTNING_WAND;
                }
                if(instance.item_id == ITEM_ID_STAFF) {
                    instance.sprite_id = ITEM_ID_LIGHTNING_STAFF;
                    instance.icon_id = ITEM_ID_LIGHTNING_STAFF;
                }
            }
        }
    }
    TraceLog(LOG_INFO, "-sprite_id %i", instance.sprite_id );
    TraceLog(LOG_INFO, "-icon_id %i", instance.icon_id );
}



void GenerateRandomArmor(ItemInstanceData &instance, int loot_level) {
    TraceLog(LOG_INFO, "making new armor %i", loot_level);

    auto a_itter = g_armor_data.find(instance.item_id);
    if(a_itter != g_armor_data.end()) {
        instance.defence = a_itter->second.defence;
        TraceLog(LOG_INFO, "-defence %i", instance.defence );
        instance.magic_defence = a_itter->second.magic_defence;
        TraceLog(LOG_INFO, "-magic_defence %0.3f", instance.cooldown );

        instance.sprite_id = a_itter->second.armor_id;
        instance.icon_id = a_itter->second.armor_id;
        TraceLog(LOG_INFO, "-sprite_id %i", instance.sprite_id );
        TraceLog(LOG_INFO, "-icon_id %i", instance.icon_id );


        if(loot_level >= 1) {
        ItemModID mod_id = (ItemModID)GetRandomValue(ITEMMOD_ARMORSTART, ITEMMOD_ARMOREND);

        auto m_itter = g_armor_mod_data.find(mod_id);
        if(m_itter != g_armor_mod_data.end()) {
            if(m_itter->second.defence != -10000) { instance.defence += m_itter->second.defence; }

            instance.modifications.push_back(mod_id);
            TraceLog(LOG_INFO, "-----armor mod selected  + %i  defence %i-----", instance.modifications[instance.modifications.size() - 1], instance.defence);
            }
        }
    }
}


void GenerateRandomFood(ItemInstanceData &instance, int loot_level) {

    TraceLog(LOG_INFO, "making new food %i", loot_level);
    auto f_itter = g_food_data.find(instance.item_id);

    if(f_itter != g_food_data.end()) {
        instance.saturation =  f_itter->second.saturation;
    }

    ItemModID mod_id = (ItemModID)GetRandomValue(ITEMMOD_FOODSTART, ITEMMOD_FOODEND);
    TraceLog(LOG_INFO, "selected mod %i", mod_id);

    auto m_itter = g_food_mod_data.find(mod_id);
    if(m_itter != g_food_mod_data.end()) {
        
        if(m_itter->second.saturation != -10000) { instance.saturation += m_itter->second.saturation; }

        instance.modifications.push_back(mod_id);
        TraceLog(LOG_INFO, "-----food mod selected  + %i  saturation %0.2f-----", instance.modifications[instance.modifications.size() - 1], instance.saturation);
        }

    }
