#include "../core/gamedefs.h"


ItemInstanceData GenerateItem(ItemID item_id, int uid, std::string container_id) {
    
    ItemInstanceData new_instance;

    new_instance.instance_id = uid;
    new_instance.item_id = item_id;
    new_instance.item_name = "error locating item data";
    new_instance.sprite_id = ITEM_ID_NONE;
    new_instance.rarity = RARITY_COMMON;

    new_instance.item_name = g_item_data[item_id].item_name;
    new_instance.type = g_item_data[item_id].type;
    new_instance.value = g_item_data[item_id].value;
    new_instance.container_id = container_id;

    new_instance.spell_id = SPELL_ID_NONE;
    new_instance.spell_data = {};
    
    new_instance.weapon_id = ITEM_ID_NONE;
    new_instance.weapon_data = {};

    new_instance.armor_id = ITEM_ID_NONE;
    new_instance.armor_data = {};
    new_instance.food_id = ITEM_ID_NONE;
    new_instance.food_data = {};
    new_instance.sprite_id = item_id;
    new_instance.icon_id = item_id;
    new_instance.level = 0;

    new_instance.mod_slots = 0;


    if(new_instance.type == TYPE_WEAPON ) {
        GenerateWeapon(new_instance, 0, false);
    }
    if(new_instance.type >= TYPE_HEAD_ARMOR and new_instance.type <= TYPE_HAND_ARMOR ) {
        GenerateArmor(new_instance, 0, false);
    }
    if(new_instance.type == TYPE_FOOD ) {
        GenerateFood(new_instance, 0, false);   
    }

    //TraceLog(LOG_INFO, "----------item type  %i-----------\n", new_instance.type);
    //TraceLog(LOG_INFO, "---------------------\n");

    return new_instance;
}

ItemInstanceData GenerateRandomItem(ItemID item_id, int uid, std::string container_id, int loot_level) {

    
    ItemInstanceData new_instance;

    new_instance.instance_id = uid;
    new_instance.item_id = item_id;
    new_instance.item_name = "error locating item data";
    new_instance.sprite_id = ITEM_ID_NONE;
    new_instance.rarity = RARITY_COMMON;

    new_instance.item_name = g_item_data[item_id].item_name;
    new_instance.type = g_item_data[item_id].type;
    new_instance.value = g_item_data[item_id].value;
    new_instance.container_id = container_id;
    
    //new_instance.item_mods = {};
    //new_instance.char_mods = {};

    new_instance.spell_id = SPELL_ID_NONE;
    new_instance.spell_data = {};
    
    new_instance.weapon_id = ITEM_ID_NONE;
    new_instance.weapon_data = {};

    new_instance.armor_id = ITEM_ID_NONE;
    new_instance.armor_data = {};
    new_instance.food_id = ITEM_ID_NONE;
    new_instance.food_data = {};
    new_instance.sprite_id = item_id;
    new_instance.icon_id = item_id;
    new_instance.level = 0;

    new_instance.mod_slots = 0;

        //add chara mods here


    if(new_instance.type == TYPE_WEAPON ) {
        GenerateWeapon(new_instance, loot_level, true);
    }
    if(new_instance.type >= TYPE_HEAD_ARMOR and new_instance.type <= TYPE_HAND_ARMOR ) {
        GenerateArmor(new_instance, loot_level, true);
    }
    if(new_instance.type == TYPE_FOOD ) {
        GenerateFood(new_instance, loot_level, true);
            //GenerateRandomWeapon(new_instance, loot_level);
    }


    //TraceLog(LOG_INFO, "----------rarity  %i-----------\n", new_instance.rarity);
    //TraceLog(LOG_INFO, "---------random item------------\n");
    return new_instance;
}

void GenerateWeapon(ItemInstanceData &instance, int loot_level, bool random) {
    instance.weapon_id = (ItemID)(instance.item_id - ITEM_ID_DAGGER);
    instance.weapon_data = g_weapon_data[instance.weapon_id];
    instance.weapon_data.current_power = instance.weapon_data.max_power;
    instance.mod_slots = instance.weapon_data.mod_slots;

    //TraceLog(LOG_INFO, "making new weapon %s  %i  wid %i", instance.item_name.c_str(), instance.item_id, instance.weapon_id);

    instance.spell_id = instance.weapon_data.spell_id;
    if(instance.spell_id != SPELL_ID_NONE) {
        instance.spell_id = instance.spell_id;
        instance.spell_data = g_spell_data[instance.spell_id];
    }

    //TraceLog(LOG_INFO, "-item %i", instance.item_id );
    //TraceLog(LOG_INFO, "-sprite_id %i", instance.sprite_id );
    //TraceLog(LOG_INFO, "-cooldown %0.3f", instance.weapon_data.cooldown );
    //TraceLog(LOG_INFO, "-damage %i", instance.weapon_data.damage );
    //TraceLog(LOG_INFO, "-recoil %f", instance.weapon_data.recoil );
    //TraceLog(LOG_INFO, "-knockback %f", instance.weapon_data.knockback );
    //TraceLog(LOG_INFO, "-max power %f", instance.weapon_data.max_power );
    //TraceLog(LOG_INFO, "-spell id %i", instance.spell_id );
    //TraceLog(LOG_INFO, "-mod slots %i", instance.mod_slots );
    //TraceLog(LOG_INFO, "-shots %i", instance.weapon_data.shots );

}



void GenerateArmor(ItemInstanceData &instance, int loot_level, bool random) {
    //TraceLog(LOG_INFO, "making new armor %s  %i  aid %i", instance.item_name.c_str(), instance.item_id, instance.armor_id);

    instance.armor_id = (ItemID)(instance.item_id - ITEM_ID_HELMET);
    instance.armor_data = g_armor_data[instance.armor_id];
    instance.sprite_id = g_armor_data[instance.armor_id].armor_id;
    instance.icon_id = g_armor_data[instance.armor_id].armor_id;
    instance.mod_slots = g_armor_data[instance.armor_id].mod_slots;
    instance.mod_slots = instance.armor_data.mod_slots;

    //TraceLog(LOG_INFO, "-icon_id %i", instance.icon_id );
    //TraceLog(LOG_INFO, "-sprite_id %i", instance.sprite_id );
    //TraceLog(LOG_INFO, "-defence %i", instance.armor_data.defence );
    //TraceLog(LOG_INFO, "-magic_defence %0.3f", instance.armor_data.magic_defence );
    //TraceLog(LOG_INFO, "-mod slots %i", instance.mod_slots );

}


void GenerateFood(ItemInstanceData &instance, int loot_level, bool random) {

    //TraceLog(LOG_INFO, "making new food %s", g_food_data[instance.item_id - ITEM_ID_APPLE].food_name.c_str());
    instance.food_id =  (ItemID)(instance.item_id - ITEM_ID_APPLE);
    instance.food_data = g_food_data[instance.food_id];

    ItemModID mod_id = (ItemModID)GetRandomValue(ITEMMOD_NUTRITIOUS, ITEMMOD_MOLDY);

    instance.item_name  = g_food_mod_data[mod_id - ITEMMOD_NUTRITIOUS].mod_name + " " + instance.item_name;
    instance.food_data.saturation += g_food_mod_data[mod_id - ITEMMOD_NUTRITIOUS].saturation;

    //TraceLog(LOG_INFO, "-----food sat: %0.2f-----", instance.food_data.saturation);
}


void GenerateScroll(ItemInstanceData &instance, ItemModID mod_id, std::string container_id) {

    //TraceLog(LOG_INFO, "-generating scroll with mod  + %i", mod_id);
}


void GenerateCharm(ItemInstanceData &instance) {

}


void AddSpellToItem(ItemInstanceData &instance, SpellID spell_id) {
    instance.spell_id = spell_id;
    instance.spell_data = g_spell_data[instance.spell_id];
    instance.item_name += " of " + instance.spell_data.spell_name;
    instance.weapon_data.max_power = 10;
    instance.weapon_data.current_power = instance.weapon_data.max_power;

    //TraceLog(LOG_INFO, "-name  + %s   pps %0.2f", instance.item_name.c_str(), instance.weapon_data.pps);


    //TraceLog(LOG_INFO, "-icon_id %i", instance.icon_id );
    //TraceLog(LOG_INFO, "-sprite_id %i", instance.sprite_id );
    //TraceLog(LOG_INFO, "-cooldown %0.3f", instance.weapon_data.cooldown );
    //TraceLog(LOG_INFO, "-damage %i", instance.weapon_data.damage );
}

void AddModToItem(ItemInstanceData &instance, ItemModID mod_id) {

}