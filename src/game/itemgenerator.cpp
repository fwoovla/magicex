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
    new_instance.spell_data = nullptr;
    new_instance.sprite_id = item_id;
    new_instance.icon_id = item_id;
    new_instance.level = 0;
    new_instance.max_power = 0;
    new_instance.current_power = 0;
    new_instance.saturation = 0;
    new_instance.defence = 0;
    new_instance.magic_defence = 0;


    if(new_instance.type == TYPE_WEAPON ) {
        GenerateWeapon(new_instance, 0, false);
    }
    if(new_instance.type >= TYPE_HEAD_ARMOR and new_instance.type <= TYPE_HAND_ARMOR ) {
        GenerateArmor(new_instance, 0, false);
    }
    if(new_instance.type == TYPE_FOOD ) {
        GenerateFood(new_instance, 0, false);   
    }

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
    new_instance.spell_id = SPELL_ID_NONE;
    new_instance.spell_data = nullptr;
    new_instance.sprite_id = item_id;
    new_instance.icon_id = item_id;
    new_instance.level = 0;
    new_instance.max_power = 0;
    new_instance.current_power = 0;
    new_instance.saturation = 0.0f;
    new_instance.defence = 0;
    new_instance.magic_defence = 0;


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
    
    TraceLog(LOG_INFO, "----------rarity  %i-----------\n", new_instance.rarity);
    TraceLog(LOG_INFO, "---------------------\n");
    return new_instance;
}

void GenerateWeapon(ItemInstanceData &instance, int loot_level, bool random) {
    TraceLog(LOG_INFO, "making new weapon %i", loot_level);

    
    instance.cooldown = g_weapon_data[instance.item_id - ITEM_ID_DAGGER].cooldown;
    instance.damage =  g_weapon_data[instance.item_id - ITEM_ID_DAGGER].damage;
    instance.sprite_id = g_weapon_data[instance.item_id - ITEM_ID_DAGGER].weapon_id;
    instance.icon_id = g_weapon_data[instance.item_id - ITEM_ID_DAGGER].weapon_id;
    instance.max_power = g_weapon_data[instance.item_id - ITEM_ID_DAGGER].max_power;
    instance.current_power = g_weapon_data[instance.item_id - ITEM_ID_DAGGER].max_power;
    TraceLog(LOG_INFO, "-icon_id %i", instance.icon_id );
    TraceLog(LOG_INFO, "-sprite_id %i", instance.sprite_id );
    TraceLog(LOG_INFO, "-cooldown %0.3f", instance.cooldown );
    TraceLog(LOG_INFO, "-damage %i", instance.damage );


    if(loot_level >= 1 and random) {

        std::vector<int> wep_mod_list;
        for(auto mod : g_weapon_mod_data) {
            for(int i = 0; i < mod.rarity; i++) {
                wep_mod_list.push_back(mod.mod_id);
            }
        }
        TraceLog(LOG_INFO, "-----weapon mod list size  + %i------", wep_mod_list.size());
        ItemModID mod_id = (ItemModID) wep_mod_list[ GetRandomValue(0, wep_mod_list.size() - 1)];
        
        if(g_weapon_mod_data[mod_id - ITEMMOD_SWIFTNESS1].cooldown > -100) { instance.cooldown += g_weapon_mod_data[mod_id - ITEMMOD_SWIFTNESS1].cooldown; }
        if(g_weapon_mod_data[mod_id - ITEMMOD_SWIFTNESS1].max_power != -1000) { instance.max_power += g_weapon_mod_data[mod_id - ITEMMOD_SWIFTNESS1].max_power; }
        if(g_weapon_mod_data[mod_id - ITEMMOD_SWIFTNESS1].rarity < instance.rarity){instance.rarity = g_weapon_mod_data[mod_id - ITEMMOD_SWIFTNESS1].rarity;}
        if(g_weapon_mod_data[mod_id - ITEMMOD_SWIFTNESS1].damage != -1000) { instance.damage += g_weapon_mod_data[mod_id - ITEMMOD_SWIFTNESS1].damage; }
        TraceLog(LOG_INFO, "-----weapon mod   + %i------", mod_id - ITEMMOD_SWIFTNESS1);
    }

    if(loot_level >= 1 and random) {
        std::vector<int> char_mod_list;
        for(auto mod : g_char_mod_data) {
            for(int i = 0; i < mod.rarity; i++) {
                char_mod_list.push_back(mod.mod_id);
            }
        }

        bool can_add = false;
        if(GetRandomValue(0, 100) <= loot_level*10 and random) {
            can_add = true;
        }
        if(can_add) {

            CharModID mod_id = (CharModID)char_mod_list[ GetRandomValue(0, char_mod_list.size() - 1)];


            instance.char_mods.push_back(g_char_mod_data[mod_id]);
            if(g_char_mod_data[mod_id].rarity < instance.rarity){instance.rarity = g_char_mod_data[mod_id].rarity;}
            TraceLog(LOG_INFO, "-----character mod selected  + %s------", instance.char_mods[instance.char_mods.size() - 1].mod_name.c_str());
        }
    }

    if(  (instance.item_id == ITEM_ID_WAND or instance.item_id == ITEM_ID_STAFF) and random) {

        instance.spell_id = (SpellID)GetRandomValue(SPELL_ID_MAGICMISSLE, SPELL_ID_LIGHTNING);
        TraceLog(LOG_INFO, "-spell selected  + %i", instance.spell_id);

        instance.item_name += " of " + g_spell_data[instance.spell_id].spell_name;
        //instance.spell_data = &g_spell_data[instance.spell_id];

        TraceLog(LOG_INFO, "-name  + %s   pps %0.2f", g_spell_data[instance.spell_id].spell_name.c_str(), g_spell_data[instance.spell_id].pps);

        if(instance.spell_id == SPELL_ID_MAGICMISSLE) {
            if(instance.item_id == ITEM_ID_WAND) {
                instance.sprite_id = ITEM_ID_MAGICMISSLE_WAND;
                instance.icon_id = ITEM_ID_MAGICMISSLE_WAND;
            }
            if(instance.item_id == ITEM_ID_STAFF) {
                instance.sprite_id = ITEM_ID_MAGICMISSLE_STAFF;
                instance.icon_id = ITEM_ID_MAGICMISSLE_STAFF;
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

    TraceLog(LOG_INFO, "-icon_id %i", instance.icon_id );
    TraceLog(LOG_INFO, "-sprite_id %i", instance.sprite_id );
    TraceLog(LOG_INFO, "-cooldown %0.3f", instance.cooldown );
    TraceLog(LOG_INFO, "-damage %i", instance.damage );

}



void GenerateArmor(ItemInstanceData &instance, int loot_level, bool random) {
    TraceLog(LOG_INFO, "making new armor %i", loot_level);

    instance.defence = g_armor_data[instance.item_id - ITEM_ID_HELMET].defence;
    TraceLog(LOG_INFO, "-defence %i", instance.defence );
    instance.magic_defence = g_armor_data[instance.item_id - ITEM_ID_HELMET].magic_defence;
    TraceLog(LOG_INFO, "-magic_defence %0.3f", instance.magic_defence );

    instance.sprite_id = g_armor_data[instance.item_id - ITEM_ID_HELMET].armor_id;
    instance.icon_id = g_armor_data[instance.item_id - ITEM_ID_HELMET].armor_id;
    TraceLog(LOG_INFO, "-sprite_id %i", instance.sprite_id );
    TraceLog(LOG_INFO, "-icon_id %i", instance.icon_id );

    
    if(loot_level >= 1) {
        std::vector<int> armor_mod_list;
        for(auto mod : g_armor_mod_data) {
            for(int i = 0; i < mod.rarity; i++) {
                armor_mod_list.push_back(mod.mod_id);
            }
        }

        TraceLog(LOG_INFO, "-----armor mod list size  + %i------", armor_mod_list.size());

        ItemModID mod_id = (ItemModID) armor_mod_list[ GetRandomValue(0, armor_mod_list.size() - 1)];

        if(g_armor_mod_data[mod_id - ITEMMOD_TOUGHNESS1].defence != -1000) { instance.defence += g_armor_mod_data[mod_id - ITEMMOD_TOUGHNESS1].defence; }
        if(g_armor_mod_data[mod_id - ITEMMOD_TOUGHNESS1].rarity < instance.rarity){instance.rarity = g_armor_mod_data[mod_id - ITEMMOD_TOUGHNESS1].rarity;}
        TraceLog(LOG_INFO, "-----armor mod selected  + %s -----", g_armor_mod_data[mod_id - ITEMMOD_TOUGHNESS1].mod_name.c_str());

        bool can_add = false;
        if(GetRandomValue(0, 100) <= loot_level*10) {
            can_add = true;
        }

        if(can_add) {
            std::vector<int> char_mod_list;
            for(auto mod : g_char_mod_data) {
                for(int i = 0; i < mod.rarity; i++) {
                    char_mod_list.push_back(mod.mod_id);
                }
            }

            CharModID mod_id = (CharModID)char_mod_list[ GetRandomValue(0, char_mod_list.size() - 1)];


            instance.char_mods.push_back(g_char_mod_data[mod_id]);

            if(g_char_mod_data[mod_id].rarity < instance.rarity){instance.rarity = g_char_mod_data[mod_id].rarity;}
            TraceLog(LOG_INFO, "-----character mod selected  + %s------", instance.char_mods[instance.char_mods.size() - 1].mod_name.c_str());

        }
   }
}



void GenerateFood(ItemInstanceData &instance, int loot_level, bool random) {

    TraceLog(LOG_INFO, "making new food %s", g_food_data[instance.item_id - ITEM_ID_APPLE].food_name.c_str());
    instance.saturation =  g_food_data[instance.item_id - ITEM_ID_APPLE].saturation;


    ItemModID mod_id = (ItemModID)GetRandomValue(ITEMMOD_NUTRITIOUS, ITEMMOD_MOLDY);

    instance.item_name  = g_food_mod_data[mod_id - ITEMMOD_NUTRITIOUS].mod_name + " " + instance.item_name;
    instance.saturation += g_food_mod_data[mod_id - ITEMMOD_NUTRITIOUS].saturation;

    TraceLog(LOG_INFO, "-----food sat: %0.2f-----", instance.saturation);


}




void GenerateScroll(ItemInstanceData &instance, SpellID spell_id, std::string container_id) {

    TraceLog(LOG_INFO, "-generating scroll with spell  + %i", spell_id);


    instance.spell_id = spell_id;
    instance.item_name = g_spell_data[instance.spell_id].spell_name +  " " + instance.item_name;
    instance.max_power = GetRandomValue(5, 15);
    instance.current_power = instance.max_power;

    if(instance.spell_id == SPELL_ID_MAGICMISSLE) {
        instance.sprite_id = ITEM_ID_MAGICMMISSLE_SCROLL;
        instance.icon_id = ITEM_ID_MAGICMMISSLE_SCROLL;
    }

    if(instance.spell_id == SPELL_ID_FIREBALL) {
        instance.sprite_id = ITEM_ID_FIREBALL_SCROLL;
        instance.icon_id = ITEM_ID_FIREBALL_SCROLL;
    }

    if(instance.spell_id == SPELL_ID_LIGHTNING) {
        instance.sprite_id = ITEM_ID_LIGHTNING_SCROLL;
        instance.icon_id = ITEM_ID_LIGHTNING_SCROLL;
    }


}