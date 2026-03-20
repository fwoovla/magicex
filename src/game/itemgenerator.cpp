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
    TraceLog(LOG_INFO, "-----# of spells %i ", instance.weapon_data.wand_data.spells.size());

    
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
    new_wand.spells.push_back(GenerateSpell(instance));
    new_wand.spells.push_back(GenerateSpell(instance));
    new_wand.spells.push_back(GenerateSpell(instance));

    instance.weapon_data.wand_data = new_wand;

    instance.weapon_data.max_power = 100;
    instance.weapon_data.current_power = 100;
    instance.weapon_data.pps = 10;
    instance.weapon_data.cooldown = new_wand.spells[new_wand.active_spell_index].chargetime;

    instance.sprite_ids.push_back(instance.item_id);
    BuildItemSprite(instance);



/*     int base = GetRandomValue(0, g_casterbase_data.size()-1);
    int igniter = GetRandomValue(0, g_igniter_data.size()-1);
    int coupler = GetRandomValue(0, g_coupler_data.size()-1);
    int rod = GetRandomValue(0, g_rod_data.size()-1);

    ITEM_TIER base_tier = GetTier(loot_level);
    ITEM_TIER igniter_tier = GetTier(loot_level);
    ITEM_TIER coupler_tier = GetTier(loot_level);
    ITEM_TIER rod_tier = GetTier(loot_level);

    TraceLog(LOG_INFO, "-----type %i ", instance.weapon_data.caster_data.coupler.type);
    TraceLog(LOG_INFO, "-----base %i tier %i ", base, base_tier);
    TraceLog(LOG_INFO, "-----igniter %i tier %i ", igniter, igniter_tier);
    TraceLog(LOG_INFO, "-----coupler %i tier  %i ", coupler, coupler_tier);
    TraceLog(LOG_INFO, "-----rod %i tier  %i ", rod, rod_tier);

    instance.weapon_data.caster_data.base = GenerateBase(g_casterbase_data[base], base_tier);
    instance.sprite_ids.push_back(instance.weapon_data.caster_data.base.id);

    instance.weapon_data.caster_data.igniter = GenerateIgniter(g_igniter_data[igniter], igniter_tier);
    instance.sprite_ids.push_back(instance.weapon_data.caster_data.igniter.id);

    instance.weapon_data.caster_data.coupler = GenerateCoupler(g_coupler_data[coupler], coupler_tier);
    instance.sprite_ids.push_back(instance.weapon_data.caster_data.coupler.id);

    instance.weapon_data.caster_data.rod = GenerateRod(g_rod_data[rod], rod_tier);
    instance.sprite_ids.push_back(instance.weapon_data.caster_data.rod.id);

    //int points = g_tier_tables[TYPE_WEAPON][tier];
    
    instance.weapon_data.max_power = instance.weapon_data.caster_data.igniter.max_power;
    instance.weapon_data.current_power = instance.weapon_data.max_power;
    instance.weapon_data.pps = instance.weapon_data.caster_data.coupler.pps;
    instance.weapon_data.cooldown = instance.weapon_data.caster_data.coupler.cooldown;

    BuildItemSprite(instance);
    //TraceLog(LOG_INFO, "caster creater  " );
    //TraceLog(LOG_INFO, "+++--tier %i ", tier);


    */

    TraceLog(LOG_INFO, "-------chargetime %0.2f ", instance.weapon_data.wand_data.spells[0].chargetime);

}

SpellData GenerateSpell(ItemInstanceData &instance) {

    SpellData new_spell;

    new_spell.chargetime = g_spell_rules.chargetime_base;
    TraceLog(LOG_INFO, "-------chargetime %0.2f ", g_spell_rules.chargetime_base);
    new_spell.damage = g_spell_rules.damage_base;
    new_spell.radius = g_spell_rules.radius_base;
    new_spell.durration = g_spell_rules.duration_base;

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








//===========================================================
/* 

CasterBaseData GenerateBase(CasterBaseData &base, ITEM_TIER tier) {

    
    CasterBaseData new_base = base;
    new_base.tier = tier;

    int max = g_tier_tables[TYPE_WEAPON][tier];
    int min = (int)(g_tier_tables[TYPE_WEAPON][tier]/2);

    float points = GetRandomValue(min, max);
    float bonus_percent = points/100.0f;

    TraceLog(LOG_INFO, "generating caster base %s -----tier %i   bonus_percent %0.02f", base.name.c_str(), tier, bonus_percent);

    //ApplyTierBonusPercent(new_base.accuracy, STAT_ACCURACY, bonus_percent);

     

    ApplyTierBonusPoints(new_base.accuracy, STAT_ACCURACY, points);

    ClampStat(new_base.accuracy, STAT_ACCURACY);

    return new_base;
}


IgniterData GenerateIgniter(IgniterData &base, ITEM_TIER tier) {

    IgniterData new_igniter = base;
    new_igniter.tier = tier;
    int max = g_tier_tables[TYPE_WEAPON][tier];
    int min = (int)(g_tier_tables[TYPE_WEAPON][tier]/2);

    float points = GetRandomValue(min, max);
    float bonus_percent = points/100.0f;

    TraceLog(LOG_INFO, "generating igniter %s -----tier %i   bonus_percent %0.02f", base.name.c_str(), tier, bonus_percent);

    //ApplyTierBonusPercent(new_igniter.recoil, STAT_RECOIL, bonus_percent);
    //ApplyTierBonusPercent(new_igniter.max_power, STAT_MAX_POWER, bonus_percent);

    ApplyTierBonusPoints(new_igniter.recoil, STAT_RECOIL, points);
    ApplyTierBonusPoints(new_igniter.max_power, STAT_MAX_POWER, points);

    ClampStat(new_igniter.recoil, STAT_RECOIL);
    ClampStat(new_igniter.max_power, STAT_MAX_POWER);

    return new_igniter;
}

CouplerData GenerateCoupler(CouplerData &base, ITEM_TIER tier) {

    CouplerData new_coupler = base;
    new_coupler.tier = tier;
    int max = g_tier_tables[TYPE_WEAPON][tier];
    int min = (int)(g_tier_tables[TYPE_WEAPON][tier]/2);

    float points = GetRandomValue(min, max);
    float bonus_percent = points/100.0f;

    TraceLog(LOG_INFO, "generating coupler %s -----tier %i   bonus_percent %0.02f", base.name.c_str(), tier, bonus_percent);


    //ApplyTierBonusPercent(new_coupler.cooldown, STAT_COOLDOWN, bonus_percent);
    //ApplyTierBonusPercent(new_coupler.damage, STAT_DAMAGE, bonus_percent);
    //ApplyTierBonusPercent(new_coupler.pps, STAT_PPS, bonus_percent);


    ApplyTierBonusPoints(new_coupler.cooldown, STAT_COOLDOWN, points);
    ApplyTierBonusPoints(new_coupler.damage, STAT_DAMAGE, points);
    ApplyTierBonusPoints(new_coupler.pps, STAT_PPS, points);

    ClampStat(new_coupler.cooldown, STAT_COOLDOWN);
    ClampStat(new_coupler.damage, STAT_DAMAGE);
    ClampStat(new_coupler.pps, STAT_PPS);

    return new_coupler;

}

RodData GenerateRod(RodData &base, ITEM_TIER tier) {

    RodData new_rod = base;
    new_rod.tier = tier;
    int max = g_tier_tables[TYPE_WEAPON][tier];
    int min = (int)(g_tier_tables[TYPE_WEAPON][tier]/2);

    float points = GetRandomValue(min, max);
    float bonus_percent = points/100.0f;

    TraceLog(LOG_INFO, "generating rod %s -----tier %i   bonus_percent %0.02f", base.name.c_str(), tier, bonus_percent);

    //ApplyTierBonusPercent(new_rod.knockback, STAT_KNOCKBACK, bonus_percent);

    ApplyTierBonusPoints(new_rod.knockback, STAT_KNOCKBACK, points);

    ClampStat(new_rod.knockback, STAT_KNOCKBACK);

    return new_rod;
}


void ApplyTierBonusPoints(float &stat, STAT_ID id, float points) {

    auto &gen = g_statgen_data[id];

    float delta = points * gen.step;
    float new_value = stat + delta;
    stat = new_value;
    TraceLog(LOG_INFO, "%i  stat---new  value %0.02f  bonus points:%0.02f  delta: %0.02f", id, new_value, points, delta);  
}

void ApplyTierBonusPercent(float &stat, STAT_ID id, float bonus) {
    //auto &gen = g_statgen_data[id];
    //auto &lim = g_stat_limits[id];

    float delta;
    if(id == STAT_COOLDOWN || id == STAT_RECOIL || id == STAT_PPS){
        delta =  stat * -bonus;
    }
    else {
        delta =  stat * bonus;
    }
    
    float new_value = stat + delta;
    
    stat = new_value;
    //points -= gen.cost;
    TraceLog(LOG_INFO, "%i  stat---new  value %0.02f  tier bonus:%0.02f", id, new_value, bonus);
}
 */

void ClampStat(float &stat, STAT_ID id) {
    auto &lim = g_stat_limits[id];

    if(stat < lim.min)
        stat = lim.min;

    if(stat > lim.max)
        stat = lim.max;
}

void SpendStatPoints(float &stat, STAT_ID id, int &points) {
    auto &gen = g_statgen_data[id];
    auto &lim = g_stat_limits[id];

    if(points < gen.cost) {
        points -= gen.cost;
        return;
    }

    float new_value = stat + gen.step;
    
    points -= gen.cost;
    
    if(new_value < lim.min || new_value > lim.max) {
        return;
    }
    
    stat = new_value;
    //points -= gen.cost;
    TraceLog(LOG_INFO, "---new  stat value %0.02f  points left:%i", new_value, points);
}



void GenerateItemName(ItemInstanceData &instance) {

    std::string new_name = "";

    if(instance.weapon_data.weapon_type == WEAPON_TYPE_CASTER) {
        int best_tier = 0;
        //if(instance.weapon_data.caster_data.base.tier > best_tier) best_tier = instance.weapon_data.caster_data.base.tier;
        //if(instance.weapon_data.caster_data.igniter.tier > best_tier) best_tier = instance.weapon_data.caster_data.igniter.tier;
        //if(instance.weapon_data.caster_data.coupler.tier > best_tier) best_tier = instance.weapon_data.caster_data.coupler.tier;
        //if(instance.weapon_data.caster_data.rod.tier > best_tier) best_tier = instance.weapon_data.caster_data.rod.tier;

/*         std::string tier_str = std::to_string(best_tier);
        new_name += "Tier " + tier_str + " ";

        std::string type_str = SpellTypeToStr(instance.weapon_data.caster_data.coupler.type);
        new_name += type_str + " ";

        std::string effect_str = SpellEffectToStr(instance.weapon_data.caster_data.igniter.effect);
        new_name += effect_str + " Caster";
         */
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