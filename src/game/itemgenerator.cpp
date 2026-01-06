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

    TraceLog(LOG_INFO, "----------item type  %i-----------\n", new_instance.type);
    TraceLog(LOG_INFO, "---------------------\n");

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

    if(new_instance.type == TYPE_CHARM ) {
        GenerateCharm(new_instance);

    }

    if(new_instance.type == TYPE_SCROLL ) {
        int mod_id;
        if(new_instance.item_id == ITEM_ID_SCROLL) {
            mod_id =  g_equipment_mod_data[GetRandomValue(ITEMMOD_ARMORSTART, ITEMMOD_ARMOREND - 1)].mod_id;
        }
        else {
            mod_id =  ITEM_ID_RESIST_SCROLL - new_instance.item_id;
        }

        GenerateScroll(new_instance, (ItemModID)mod_id, "");
    }
    
    TraceLog(LOG_INFO, "----------rarity  %i-----------\n", new_instance.rarity);
    TraceLog(LOG_INFO, "---------------------\n");
    return new_instance;
}

void GenerateWeapon(ItemInstanceData &instance, int loot_level, bool random) {
    TraceLog(LOG_INFO, "making new weapon %s  %i", instance.item_name.c_str(), instance.item_id);
    instance.weapon_id = (ItemID)(instance.item_id - ITEM_ID_DAGGER);
    instance.weapon_data = g_weapon_data[instance.weapon_id];

    if(random) {
        instance.weapon_data.max_power = GetRandomValue(10,20);
        instance.weapon_data.current_power = instance.weapon_data.max_power;
    }
    else {
        instance.weapon_data.max_power = 10;
        instance.weapon_data.current_power = instance.weapon_data.max_power;
    }

    TraceLog(LOG_INFO, "-item %i", instance.item_id );
    TraceLog(LOG_INFO, "-sprite_id %i", instance.sprite_id );
    TraceLog(LOG_INFO, "-cooldown %0.3f", instance.weapon_data.cooldown );
    TraceLog(LOG_INFO, "-damage %i", instance.weapon_data.damage );
    TraceLog(LOG_INFO, "-recoil %f", instance.weapon_data.recoil );
    TraceLog(LOG_INFO, "-knockback %f", instance.weapon_data.knockback );
    TraceLog(LOG_INFO, "-max power %f", instance.weapon_data.max_power );


    if(instance.item_id == ITEM_ID_WAND and random) {
        instance.spell_id = (SpellID)GetRandomValue(SPELL_ID_MAGICMISSLE_WAND, SPELL_ID_POISON_WAND);

        instance.item_name += " of " + g_spell_data[instance.spell_id].spell_name;
        instance.spell_data = g_spell_data[instance.spell_id];

        TraceLog(LOG_INFO, "-spell selected  + %i", instance.spell_id);
        TraceLog(LOG_INFO, "-name  + %s   pps %0.2f", g_spell_data[instance.spell_id].spell_name.c_str(), g_spell_data[instance.spell_id].pps);

        if(instance.spell_id == SPELL_ID_MAGICMISSLE_WAND) {
            instance.sprite_id = ITEM_ID_MAGICMISSLE_WAND;
            instance.icon_id = ITEM_ID_MAGICMISSLE_WAND;
        }

        if(instance.spell_id == SPELL_ID_FIREBALL_WAND) {
            instance.sprite_id = ITEM_ID_FIREBALL_WAND;
            instance.icon_id = ITEM_ID_FIREBALL_WAND;
        }
        
        if(instance.spell_id == SPELL_ID_LIGHTNING_WAND) {
            instance.sprite_id = ITEM_ID_LIGHTNING_WAND;
            instance.icon_id = ITEM_ID_LIGHTNING_WAND;
        }
        
        if(instance.spell_id == SPELL_ID_POISON_WAND) {
            instance.sprite_id = ITEM_ID_POISON_WAND;
            instance.icon_id = ITEM_ID_POISON_WAND;
        }


    }

    if(instance.item_id == ITEM_ID_STAFF and random) {

        instance.spell_id = (SpellID)GetRandomValue(SPELL_ID_MAGICMISSLE_STAFF, SPELL_ID_POISON_STAFF);
        
        instance.item_name += " of " + g_spell_data[instance.spell_id].spell_name;
        instance.spell_data = g_spell_data[instance.spell_id];
        
        TraceLog(LOG_INFO, "-spell selected  + %i", instance.spell_id);
        TraceLog(LOG_INFO, "-name  + %s   pps %0.2f", g_spell_data[instance.spell_id].spell_name.c_str(), g_spell_data[instance.spell_id].pps);

        if(instance.spell_id == SPELL_ID_MAGICMISSLE_STAFF) {
            instance.sprite_id = ITEM_ID_MAGICMISSLE_STAFF;
            instance.icon_id = ITEM_ID_MAGICMISSLE_STAFF;
        }

        if(instance.spell_id == SPELL_ID_FIREBALL_STAFF) {
            instance.sprite_id = ITEM_ID_FIREBALL_STAFF;
            instance.icon_id = ITEM_ID_FIREBALL_STAFF;
        }
        
        if(instance.spell_id == SPELL_ID_LIGHTNING_STAFF) {
            instance.sprite_id = ITEM_ID_LIGHTNING_STAFF;
            instance.icon_id = ITEM_ID_LIGHTNING_STAFF;
        }
        
        if(instance.spell_id == SPELL_ID_POISON_STAFF) {
            instance.sprite_id = ITEM_ID_POISON_STAFF;
            instance.icon_id = ITEM_ID_POISON_STAFF;
        }

    }

    TraceLog(LOG_INFO, "-icon_id %i", instance.icon_id );
    TraceLog(LOG_INFO, "-sprite_id %i", instance.sprite_id );
    TraceLog(LOG_INFO, "-cooldown %0.3f", instance.weapon_data.cooldown );
    TraceLog(LOG_INFO, "-damage %i", instance.weapon_data.damage );

}



void GenerateArmor(ItemInstanceData &instance, int loot_level, bool random) {
    TraceLog(LOG_INFO, "making new armor %i", loot_level);

    instance.armor_id = (ItemID)(instance.item_id - ITEM_ID_HELMET);
    instance.armor_data = g_armor_data[instance.armor_id];
    instance.sprite_id = g_armor_data[instance.item_id - ITEM_ID_HELMET].armor_id;
    instance.icon_id = g_armor_data[instance.item_id - ITEM_ID_HELMET].armor_id;
    instance.mod_slots = g_armor_data[instance.item_id - ITEM_ID_HELMET].mod_slots;

    TraceLog(LOG_INFO, "-icon_id %i", instance.icon_id );
    TraceLog(LOG_INFO, "-sprite_id %i", instance.sprite_id );
    TraceLog(LOG_INFO, "-defence %i", instance.armor_data.defence );
    TraceLog(LOG_INFO, "-magic_defence %0.3f", instance.armor_data.magic_defence );


    
/*     if(random) {

        std::vector<int> armor_mod_list;
        for(auto mod : g_equipment_mod_data) {
            if(mod.mod_id > ITEMMOD_ARMORSTART and mod.mod_id < ITEMMOD_ARMOREND) {
                for(int i = 0; i < mod.rarity; i++) {
                    armor_mod_list.push_back(mod.mod_id);
                }
            }
        }

        TraceLog(LOG_INFO, "-----armor mod list size  + %i------", armor_mod_list.size());

        ItemModID mod_id = (ItemModID) armor_mod_list[ GetRandomValue(0, armor_mod_list.size() - 1)];
        instance.item_mods.push_back(g_equipment_mod_data[mod_id]);

        if(g_equipment_mod_data[mod_id].defence != -1000) { instance.defence += g_equipment_mod_data[mod_id].defence; }
        if(g_equipment_mod_data[mod_id].magic_defence != -1000) { instance.magic_defence += g_equipment_mod_data[mod_id].magic_defence; }
        if(g_equipment_mod_data[mod_id].rarity < instance.rarity){instance.rarity = g_equipment_mod_data[mod_id].rarity;}
        TraceLog(LOG_INFO, "-----armor mod selected  + %s -----", g_equipment_mod_data[mod_id].mod_name.c_str());
        
   } */
}



void GenerateFood(ItemInstanceData &instance, int loot_level, bool random) {

    TraceLog(LOG_INFO, "making new food %s", g_food_data[instance.item_id - ITEM_ID_APPLE].food_name.c_str());
    instance.food_id =  (ItemID)(instance.item_id - ITEM_ID_APPLE);
    instance.food_data = g_food_data[instance.food_id];

    ItemModID mod_id = (ItemModID)GetRandomValue(ITEMMOD_NUTRITIOUS, ITEMMOD_MOLDY);

    instance.item_name  = g_food_mod_data[mod_id - ITEMMOD_NUTRITIOUS].mod_name + " " + instance.item_name;
    instance.food_data.saturation += g_food_mod_data[mod_id - ITEMMOD_NUTRITIOUS].saturation;

    //TraceLog(LOG_INFO, "-----food sat: %0.2f-----", instance.food_data.saturation);


}


void GenerateScroll(ItemInstanceData &instance, ItemModID mod_id, std::string container_id) {

    TraceLog(LOG_INFO, "-generating scroll with mod  + %i", mod_id);
}


void GenerateCharm(ItemInstanceData &instance) {

    ItemID charm_id = (ItemID)0;
    
    if(instance.item_id == ITEM_ID_CHARM) {  //blank charm

        charm_id = g_charm_data[GetRandomValue(0, g_charm_data.size() - 1)].charm_id;
        TraceLog(LOG_INFO, "-blank charm charm id %i", charm_id);
        
    }
    else {  //has charm id
        charm_id = instance.item_id;
    }
    
    int charm_index = charm_id - ITEM_ID_SWIFTNESS_CHARM_1;

    instance.item_id = charm_id;
    instance.sprite_id = instance.item_id;
    instance.icon_id = instance.item_id;
    instance.item_name = g_charm_data[charm_index].charm_name;
    
    TraceLog(LOG_INFO, "-generating charm with id %i", charm_id);
    TraceLog(LOG_INFO, "-generating charm %s with charm id:   %i   mod id: %i", instance.item_name.c_str(), g_charm_data[charm_index].charm_id, g_charm_data[charm_index].mod_id);


}


void AddSpellToItem(ItemInstanceData &instance, SpellID spell_id) {
    instance.spell_id = spell_id;
    instance.spell_data = g_spell_data[instance.spell_id];
    instance.item_name += " of " + instance.spell_data.spell_name;
    instance.weapon_data.max_power = 10;
    instance.weapon_data.current_power = instance.weapon_data.max_power;

    TraceLog(LOG_INFO, "-name  + %s   pps %0.2f", instance.item_name.c_str(), g_spell_data[instance.spell_id].pps);

    if(instance.spell_id == SPELL_ID_MAGICMISSLE_WAND) {
        instance.sprite_id = ITEM_ID_MAGICMISSLE_WAND;
        instance.icon_id = ITEM_ID_MAGICMISSLE_WAND;
    }

    if(instance.spell_id == SPELL_ID_FIREBALL_WAND) {
        instance.sprite_id = ITEM_ID_FIREBALL_WAND;
        instance.icon_id = ITEM_ID_FIREBALL_WAND;
    }
        
    if(instance.spell_id == SPELL_ID_LIGHTNING_WAND) {
        instance.sprite_id = ITEM_ID_LIGHTNING_WAND;
        instance.icon_id = ITEM_ID_LIGHTNING_WAND;
    }
        
    if(instance.spell_id == SPELL_ID_POISON_WAND) {
        instance.sprite_id = ITEM_ID_POISON_WAND;
        instance.icon_id = ITEM_ID_POISON_WAND;
    }


    if(instance.spell_id == SPELL_ID_MAGICMISSLE_STAFF) {
        instance.sprite_id = ITEM_ID_MAGICMISSLE_STAFF;
        instance.icon_id = ITEM_ID_MAGICMISSLE_STAFF;
    }

    if(instance.spell_id == SPELL_ID_FIREBALL_STAFF) {
        instance.sprite_id = ITEM_ID_FIREBALL_STAFF;
        instance.icon_id = ITEM_ID_FIREBALL_STAFF;
    }
        
    if(instance.spell_id == SPELL_ID_LIGHTNING_STAFF) {
        instance.sprite_id = ITEM_ID_LIGHTNING_STAFF;
        instance.icon_id = ITEM_ID_LIGHTNING_STAFF;
    }
        
    if(instance.spell_id == SPELL_ID_POISON_STAFF) {
        instance.sprite_id = ITEM_ID_POISON_STAFF;
        instance.icon_id = ITEM_ID_POISON_STAFF;
    }


    TraceLog(LOG_INFO, "-icon_id %i", instance.icon_id );
    TraceLog(LOG_INFO, "-sprite_id %i", instance.sprite_id );
    TraceLog(LOG_INFO, "-cooldown %0.3f", instance.weapon_data.cooldown );
    TraceLog(LOG_INFO, "-damage %i", instance.weapon_data.damage );
}

void AddModToItem(ItemInstanceData &instance, ItemModID mod_id) {

}