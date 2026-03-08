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
    new_instance.level = loot_level;

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


    //power
    if(instance.weapon_data.max_power > 0) {

        float base_power = instance.weapon_data.max_power + (loot_level * 5);
        int twenty_percent = (int)(base_power * 0.2f);
        int ten_percent = (int)(base_power * 0.1f);
        int adjustment = GetRandomValue(-twenty_percent, ten_percent);
        float adjusted_power = base_power -  adjustment;
        instance.weapon_data.max_power = adjusted_power;
        instance.weapon_data.current_power = instance.weapon_data.max_power;
        
        float base_pps = instance.weapon_data.pps;
        float adjusted_pps = base_pps - (loot_level);
        instance.weapon_data.pps = adjusted_pps;
    }

    //damage (if mele)
    int base_damage = instance.weapon_data.damage;
    int adjusted_damage = base_damage * loot_level;
    instance.weapon_data.damage = adjusted_damage;

    //recoil
    if(instance.weapon_data.recoil > 0) {
        int base_recoil = instance.weapon_data.recoil - loot_level;
        int recoil_adjustment = GetRandomValue(0, loot_level );
        instance.weapon_data.recoil = base_recoil - recoil_adjustment;
    }

    //accuracy
    float base_accuracy = instance.weapon_data.accuracy;
    float _a = GetRandomValue(0, loot_level * 50) * 0.001f;
    instance.weapon_data.accuracy = base_accuracy + _a;
    if(instance.weapon_data.accuracy > 1.0f) {instance.weapon_data.accuracy = 1.0f;}

    instance.mod_slots = GetRandomValue(0, (loot_level));

    instance.spell_id = instance.weapon_data.spell_id;
    
    if(instance.spell_id != SPELL_ID_NONE) {

        //instance.spell_id = instance.spell_id;
        instance.spell_data = g_spell_data[instance.spell_id];
        
        instance.spell_data.damage = instance.spell_data.damage * loot_level;
        
        instance.item_name += " " + std::to_string(loot_level);
        
    }
    //TraceLog(LOG_INFO, "making new weapon %s  %i  wid %i", instance.item_name.c_str(), instance.item_id, instance.weapon_id);

}



void GenerateArmor(ItemInstanceData &instance, int loot_level, bool random) {
    //TraceLog(LOG_INFO, "making new armor %s  %i  aid %i", instance.item_name.c_str(), instance.item_id, instance.armor_id);

    instance.armor_id = (ItemID)(instance.item_id - ITEM_ID_HELMET);
    instance.armor_data = g_armor_data[instance.armor_id];
    instance.sprite_id = g_armor_data[instance.armor_id].armor_id;
    instance.icon_id = g_armor_data[instance.armor_id].armor_id;
    instance.mod_slots = g_armor_data[instance.armor_id].mod_slots;
    instance.mod_slots = instance.armor_data.mod_slots;

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