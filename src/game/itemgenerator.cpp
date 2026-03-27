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

    //new_instance.spell_id = SPELL_ID_NONE;
    
    new_instance.weapon_id = ITEM_ID_NONE;
    new_instance.weapon_data = {};
    //new_instance.weapon_data.caster_data = {};

    new_instance.armor_id = ITEM_ID_NONE;
    new_instance.armor_data = {};
    new_instance.food_id = ITEM_ID_NONE;
    new_instance.food_data = {};
    new_instance.sprite_id = item_id;
    //new_instance.icon_id = item_id;
    new_instance.icon_texture = g_icon_sprites[item_id];
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

    
    new_instance.weapon_id = ITEM_ID_NONE;
    new_instance.weapon_data = {};
    //new_instance.weapon_data.caster_data = {};

    new_instance.armor_id = ITEM_ID_NONE;
    new_instance.armor_data = {};
    new_instance.food_id = ITEM_ID_NONE;
    new_instance.food_data = {};
    new_instance.sprite_id = item_id;
    new_instance.icon_texture = g_icon_sprites[item_id];
    //new_instance.icon_id = item_id;
    new_instance.level = loot_level;

    new_instance.mod_slots = 0;

        //add chara mods here

    

    if(new_instance.type == TYPE_WEAPON ) {
        GenerateWeapon(new_instance, loot_level, true);
    }
    else if(new_instance.type >= TYPE_HEAD_ARMOR and new_instance.type <= TYPE_HAND_ARMOR ) {
        GenerateArmor(new_instance, loot_level, true);
    }
    else if(new_instance.type == TYPE_FOOD ) {
        GenerateFood(new_instance, loot_level, true);
            //GenerateRandomWeapon(new_instance, loot_level);
    }
    else {
        BuildItemSprite(new_instance);
    }


    //TraceLog(LOG_INFO, "----------rarity  %i-----------\n", new_instance.rarity);
    //TraceLog(LOG_INFO, "---------random item------------\n");
    GenerateItemName(new_instance);
    return new_instance;
}

void GenerateWeapon(ItemInstanceData &instance, int loot_level, bool random) {
    instance.weapon_id = (ItemID)(instance.item_id - ITEM_ID_DAGGER);
    instance.weapon_data = g_weapon_data[instance.weapon_id];
    instance.weapon_data.max_power = 0;
    instance.weapon_data.current_power = 0;
    instance.weapon_data.pps = 0;
    
    
    if(instance.weapon_data.weapon_type == WEAPON_TYPE_CASTER) {
        GenerateCaster(instance, loot_level, true);
    }

    TraceLog(LOG_INFO, "making new weapon %s  %i   weapon_id= %i ", instance.item_name.c_str(), instance.item_id, instance.weapon_id);
     
    TraceLog(LOG_INFO, "-----max power %0.2f ", instance.weapon_data.max_power);
    TraceLog(LOG_INFO, "-----current_power %0.2f ", instance.weapon_data.current_power);
    TraceLog(LOG_INFO, "-----cooldown %0.2f ", instance.weapon_data.cooldown);
    TraceLog(LOG_INFO, "-----knockback %0.2f ", instance.weapon_data.knockback);
    TraceLog(LOG_INFO, "-----accuracy %0.02f ", instance.weapon_data.accuracy);
    TraceLog(LOG_INFO, "-----weapon type %i ", instance.weapon_data.weapon_type); 
    //TraceLog(LOG_INFO, "-----# of spells %i ", instance.weapon_data.wand_data.spells.size());

    
}



void GenerateArmor(ItemInstanceData &instance, int loot_level, bool random) {
    //TraceLog(LOG_INFO, "making new armor %s  %i  aid %i", instance.item_name.c_str(), instance.item_id, instance.armor_id);

    instance.armor_id = (ItemID)(instance.item_id - ITEM_ID_HELMET);
    instance.armor_data = g_armor_data[instance.armor_id];
    instance.sprite_id = g_armor_data[instance.armor_id].armor_id;
    //instance.icon_id = g_armor_data[instance.armor_id].armor_id;
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


void GenerateCaster(ItemInstanceData &instance, int loot_level, bool random) {

    //TraceLog(LOG_INFO, "making new caster weapon %s  %i   weapon_id= %i ", instance.item_name.c_str(), instance.item_id, instance.weapon_id);

    TraceLog(LOG_INFO, "wand creater ");

    WandData new_wand;
    new_wand.slot_count = 1;
    new_wand.profile = g_wand_profiles[GetRandomValue(0, g_wand_profiles.size()-1)];
    new_wand.spell = GenerateSpell(instance);


    instance.weapon_data.wand_data = new_wand;

    instance.weapon_data.max_power = 100;
    instance.weapon_data.current_power = 100;
    instance.weapon_data.pps = 10;
    instance.weapon_data.cooldown = new_wand.spell.chargetime;

    instance.sprite_ids.push_back(instance.item_id);
    BuildItemSprite(instance);



    TraceLog(LOG_INFO, "-------chargetime %0.2f ", instance.weapon_data.wand_data.spell.chargetime);

}

SpellData GenerateSpell(ItemInstanceData &instance) {

    SpellData new_spell;

    new_spell.chargetime = g_spell_rules.stats[SPELLSTAT_CHARGE].base;
    TraceLog(LOG_INFO, "-------chargetime %0.2f ", g_spell_rules.stats[SPELLSTAT_CHARGE].base);
    new_spell.damage = g_spell_rules.stats[SPELLSTAT_DAMAGE].base;
    new_spell.radius = g_spell_rules.stats[SPELLSTAT_RADIUS].base;
    new_spell.durration = g_spell_rules.stats[SPELLSTAT_DURATION].base;

    new_spell.delivery_type = (SPELL_DELIVERY)GetRandomValue(0, DELIVERY_PROJECTILE);//DELIVERY_PROJECTILE;
    new_spell.effect_type = SPELL_EFFECT_NONE;
    new_spell.speed = 300.0f;
    new_spell.is_exploding = false;
    new_spell.is_exploding = false;
    new_spell.tick_damage = 0.0f;
    new_spell.tick_rate = 0.0f;

    return new_spell;

}

void BuildItemSprite(ItemInstanceData &instance) {
    TraceLog(LOG_INFO, "            -S----building item sprite and icon sprite----- #ids %i", instance.sprite_ids.size());

    RenderTexture2D icon_rt = LoadRenderTexture(16,16);

    BeginTextureMode(icon_rt);
        ClearBackground(BLANK);
        if(instance.sprite_ids.size() > 0) {
            for(int id : instance.sprite_ids)
                DrawTexture(g_icon_part_sprites[id],0,0,WHITE);
        }
        else {
            DrawTexture(g_icon_sprites[instance.sprite_id],0,0,WHITE);
        }
    EndTextureMode();

    Image icon_img = LoadImageFromTexture(icon_rt.texture);
    ImageFlipVertical(&icon_img);
    instance.icon_texture = LoadTextureFromImage(icon_img);

    UnloadImage(icon_img);
    UnloadRenderTexture(icon_rt);

//================================
//================================
    RenderTexture2D item_rt = LoadRenderTexture(16,16);

    BeginTextureMode(item_rt);
        ClearBackground(BLANK);
        if(instance.sprite_ids.size() > 0) {
            for(int id : instance.sprite_ids)
                DrawTexture(g_item_part_sprites[id],0,0,WHITE);
        }
        else {
            DrawTexture(g_item_sprites[instance.sprite_id],0,0,WHITE);
        }
    EndTextureMode();

    Image item_img = LoadImageFromTexture(item_rt.texture);
    ImageFlipVertical(&item_img);
    instance.item_texture = LoadTextureFromImage(item_img);

    UnloadImage(item_img);
    UnloadRenderTexture(item_rt);

    //instance.icon_generated = true;
}




void ClampStat(float &stat, STAT_ID id) {
    auto &lim = g_stat_limits[id];

    if(stat < lim.min)
        stat = lim.min;

    if(stat > lim.max)
        stat = lim.max;
}

void SpendStatPoints(float &stat, STAT_ID id, int &points) {

}



void GenerateItemName(ItemInstanceData &instance) {

    std::string new_name = "";

    if(instance.weapon_data.weapon_type == WEAPON_TYPE_CASTER) {
        int best_tier = 0;

        new_name = "non specified wand";
    }

    instance.item_name = new_name;
}



ITEM_TIER GetTier(int loot_level)
{
    int max_tier = loot_level;

    if(max_tier >= TIER_MAX)
        max_tier = TIER_MAX - 1;


    int roll = GetRandomValue(0, 99);

    TraceLog(LOG_INFO, "max: %i roll: %i", max_tier, roll);

    if(roll < 20 || max_tier == TIER_1)
        return TIER_1;

    if(roll < 50 || max_tier == TIER_2)
        return TIER_2;

    if(roll < 70 || max_tier == TIER_3)
        return TIER_3;

    if(roll < 90 || max_tier == TIER_4)
        return TIER_4;

    return TIER_5;
}