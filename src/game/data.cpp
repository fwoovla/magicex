#include "../core/gamedefs.h"
#include "../core/utils.h"

static const std::string data_save_path = "saves/save.json";
static const std::string worldgen_save_path = "saves/worldgen_save.json";


void LoadGameData() {
    TraceLog(LOG_INFO, "LOADING GAME DATA....data.json");


    g_item_type_colors.resize(TYPE_MAX);
    g_item_type_colors[TYPE_WEAPON] = WEAPONCOLOR;
    g_item_type_colors[TYPE_HEAD_ARMOR] = ARMORCOLOR;
    g_item_type_colors[TYPE_BODY_ARMOR] = ARMORCOLOR;
    g_item_type_colors[TYPE_LEG_ARMOR] = ARMORCOLOR;
    g_item_type_colors[TYPE_FEET_ARMOR] = ARMORCOLOR;
    g_item_type_colors[TYPE_HAND_ARMOR] = ARMORCOLOR;
    g_item_type_colors[TYPE_RESOURCE] = RESOURCECOLOR;
    g_item_type_colors[TYPE_PLAN] = PLANCOLOR;
    g_item_type_colors[TYPE_SCROLL] = SCROLLCOLOR;
    g_item_type_colors[TYPE_FOOD] = FOODCOLOR;
    g_item_type_colors[TYPE_CHARM] = CHARMCOLOR;

    g_item_type_colors[TYPE_ALL] = DEFAULTITEMCOLOR;
    g_item_type_colors[TYPE_CONSUMEABLE] = DEFAULTITEMCOLOR;


    g_spell_effect_colors.resize(SPELL_EFFECT_COUNT);
    g_spell_effect_colors[SPELL_EFFECT_NONE] = BASESPELLCOLOR;
    g_spell_effect_colors[SPELL_EFFECT_FORCE] = FORCECOLOR;
    g_spell_effect_colors[SPELL_EFFECT_FIRE] = FIRECOLOR;
    g_spell_effect_colors[SPELL_EFFECT_LIGHTNING] = LIGHTNINGCOLOR;
    g_spell_effect_colors[SPELL_EFFECT_POISON]= POISONCOLOR;


    g_rarity_colors.resize(10);
    g_rarity_colors[0] = COMMONCOLOR;
    g_rarity_colors[1] = UNCOMMONCOLOR;
    g_rarity_colors[2] = RARECOLOR;
    g_rarity_colors[3] = VERYRARECOLOR;
    g_rarity_colors[4] = ULTRARARECOLOR;
    g_rarity_colors[5] = ULTRARARECOLOR;
    g_rarity_colors[6] = ULTRARARECOLOR;
    g_rarity_colors[7] = ULTRARARECOLOR;
    g_rarity_colors[8] = ULTRARARECOLOR;
    g_rarity_colors[9] = ULTRARARECOLOR;


    g_font = LoadFontEx("assets/FFatF.ttf", 128, nullptr, 0);
    SetTextureFilter(g_font.texture, TEXTURE_FILTER_POINT);

    std::ifstream cfile("assets/data.json");
    if (!cfile.is_open()) {
        TraceLog(LOG_INFO, "CANNOT OPEN DATA FILE");
        return;
    }
    
    json cj;
    cfile>>cj;

    for(int i = 0; i < cj["base_class"].size(); i++) {
        int max_health = cj["base_class"][i]["max_health"];
        int health = max_health;
        //int exp = 0;
        float base_speed = cj["base_class"][i]["base_speed"];
        float current_speed = base_speed;
        int sprite_sheet_id = StrToSpriteId(cj["base_class"][i]["sprite_sheet_id"]);
        int portrait_id = cj["base_class"][i]["portrait_id"];
        std::string class_name = cj["base_class"][i]["class_name"];

        float max_stamina = cj["base_class"][i]["max_stamina"];
        
        std::vector<int> inv;
        inv.push_back(-1);
        
        std::vector<int> hot;
        hot.push_back(-1);

        std::vector<int> p;
        p.push_back(-1);

        std::vector<int> s;
        s.push_back(-1);

        std::vector<int> hd;
        hd.push_back(-1);

        std::vector<int> bd;
        bd.push_back(-1);

        std::vector<int> lg;
        lg.push_back(-1);

        std::vector<int> ft;
        ft.push_back(-1);

        std::vector<int> hs;
        hs.push_back(-1);


        CharacterData this_class = {
        
            .inventory = inv,
            .hotbar = hot,

            .primary = p,
            .secondary = s,
            .head = hd,
            .body = bd,
            .legs = lg,
            .feet = ft,
            .hands = hs,


            .creature_id = -1,
            .health = health,
            .max_health = max_health,
            .exp = 0,
            .base_speed = base_speed,
            .current_speed = current_speed,
            .defence = 0,
            .magic_defence = 0,
            .max_power = 0,
            .current_power = 0,
            .saturation = 10,
            .max_saturation = 10,
            .max_stamina = max_stamina,
            .current_stamina = max_stamina,
            .ai_data = AiData(),
            .sprite_sheet_id = sprite_sheet_id,
            .portrait_id = portrait_id,
            .name = "not assigned",
            .class_name = class_name,
        };

        g_class_data[i] = this_class;
    }

//------------------item data
    g_item_data.resize(ITEM_ID_MAX);
    TraceLog(LOG_INFO, "item data size  %i  %i", g_item_data.size(), cj["item_data"].size());

    for(int i = 0; i < cj["item_data"].size(); i++) {

        std::string id_s = cj["item_data"][i]["item_id"];
        ItemID id = StrToItemId(id_s);

        int value = cj["item_data"][i]["value"];
        std::string name = cj["item_data"][i]["item_name"];


        ItemType type = StrToItemType( cj["item_data"][i]["item_type"] );

        ItemData new_item = {
            .id = id,
            .value = value,
            .type = type,
            .item_name = name            
        };

        TraceLog(LOG_INFO, "Item Data Loaded  id: %i  %s", id, name.c_str());

        g_item_data[(int)id] = new_item;

    }


//---------------------caster data
    g_casterbase_data.resize(cj["caster_data"].size());
    TraceLog(LOG_INFO, "caster_data size %i  ", cj["caster_data"].size());
    TraceLog(LOG_INFO, "g_casterbase_data size %i  ", g_casterbase_data.size());

    for(int i = 0; i < cj["caster_data"].size(); i++) {
        CasterBaseData new_base;
        new_base.name = cj["caster_data"][i]["part_name"];
        new_base.id = StrToItemId(cj["caster_data"][i]["part_id"]);
        new_base.accuracy = cj["caster_data"][i]["accuracy"];

        TraceLog(LOG_INFO, "caster_base_data Loaded  id: %i  %s", new_base.id, new_base.name.c_str());
        g_casterbase_data[(int)new_base.id - ITEM_ID_CASTERBASE] = new_base;
    }
    TraceLog(LOG_INFO, "caster_base_data size %i  ", g_casterbase_data.size());

//---------------------caster data
    g_igniter_data.resize(cj["igniter_data"].size());
    for(int i = 0; i < cj["igniter_data"].size(); i++) {
        IgniterData new_igniter;
        new_igniter.name = cj["igniter_data"][i]["part_name"];
        new_igniter.id = StrToItemId(cj["igniter_data"][i]["part_id"]);
        new_igniter.effect = StrToSpellEffectId( cj["igniter_data"][i]["spell_effect"]);
        new_igniter.max_power = cj["igniter_data"][i]["max_power"];
        new_igniter.recoil = cj["igniter_data"][i]["recoil"];

        g_igniter_data[(int)new_igniter.id - ITEM_ID_IGNITER_1] = new_igniter;
        TraceLog(LOG_INFO, "igniter_data Loaded  id: %i  %s", new_igniter.id, new_igniter.name.c_str());
    }
    TraceLog(LOG_INFO, "g_igniter_data size %i  ", g_igniter_data.size());


//---------------------caster data
    g_coupler_data.resize(cj["coupler_data"].size());
    for(int i = 0; i < cj["coupler_data"].size(); i++) {
        CouplerData new_coupler;
        new_coupler.name = cj["coupler_data"][i]["part_name"];
        new_coupler.id = StrToItemId(cj["coupler_data"][i]["part_id"]);
        new_coupler.type = StrToSpellTypeId(cj["coupler_data"][i]["spell_type"]);
        new_coupler.pps = cj["coupler_data"][i]["pps"];
        new_coupler.radius = cj["coupler_data"][i]["radius"];
        new_coupler.damage = cj["coupler_data"][i]["damage"];
        new_coupler.cooldown = cj["coupler_data"][i]["cooldown"];
        new_coupler.speed = cj["coupler_data"][i]["speed"];

        g_coupler_data[(int)new_coupler.id - ITEM_ID_COUPLER_1] = new_coupler;
        TraceLog(LOG_INFO, "coupler_data Loaded  id: %i  %s  type: %i", new_coupler.id, new_coupler.name.c_str(), new_coupler.type);
    }
    //TraceLog(LOG_INFO, "g_coupler_data size %i  ", g_coupler_data.size());


//---------------------caster data
    g_rod_data.resize(cj["rod_data"].size());

    for(int i = 0; i < cj["rod_data"].size(); i++) {
        RodData new_rod;
        new_rod.name = cj["rod_data"][i]["part_name"];
        new_rod.id = StrToItemId( cj["rod_data"][i]["part_id"] );
        new_rod.knockback = cj["rod_data"][i]["knockback"];
        new_rod.damage = cj["rod_data"][i]["damage"];

        g_rod_data[(int)new_rod.id - ITEM_ID_ROD_1] = new_rod;
        TraceLog(LOG_INFO, "rod_data Loaded  id: %i  %s", new_rod.id, new_rod.name.c_str());

    }
    //TraceLog(LOG_INFO, "g_rod_data size %i  ", g_rod_data.size());


//---------------------spell data
 /*    g_spell_data.resize(cj["spell_data"].size());

    for(int i = 0; i < cj["spell_data"].size(); i++) {
        SpellData new_spell;

        std::string sp_id = cj["spell_data"][i]["spell_id"];
        SpellID spell_id = StrToSpellId(sp_id);

        std::string name = cj["spell_data"][i]["spell_name"];

        float lifetime = cj["spell_data"][i]["lifetime"];
        float damage = cj["spell_data"][i]["damage"];
        int speed = cj["spell_data"][i]["speed"];
        float radius = cj["spell_data"][i]["radius"];
        float knockback = cj["spell_data"][i]["knockback"];
        SPELL_EFFECT spell_effect = StrToSpellEffectId(cj["spell_data"][i]["spell_effect"]);

        new_spell.lifetime = lifetime;
        new_spell.damage = damage;
        new_spell.spell_id = spell_id;
        new_spell.radius = radius;
        new_spell.speed = speed;
        new_spell.spell_name = name;
        new_spell.knockback = knockback;
        new_spell.spell_effect = spell_effect;
        
        g_spell_data[spell_id] = new_spell;
        TraceLog(LOG_INFO, "Spell Data Loaded  id: %i  %s", spell_id, sp_id.c_str());
    } */

//------------------------------------armor data
    g_armor_data.resize(cj["armor_data"].size());

    for(int i = 0; i < cj["armor_data"].size(); i++) {

        std::string w_id_s = cj["armor_data"][i]["armor_id"];
        ItemID w_id = StrToItemId(w_id_s);

        std::string name = cj["armor_data"][i]["armor_name"];

        int defence = cj["armor_data"][i]["defence"];

        int magic_defence = cj["armor_data"][i]["magic_defence"];

        int mod_slots = cj["armor_data"][i]["mod_slots"];

        ArmorData new_armor = {
            .armor_name= name,
            .armor_id = w_id,
            .defence = defence,
            .magic_defence = magic_defence,
            .mod_slots = mod_slots    
        };

        TraceLog(LOG_INFO, "Armor Data Loaded  id: %i  %s %i", w_id, name.c_str(), mod_slots);
        g_armor_data[(int)w_id - ITEM_ID_HELMET] = new_armor;
    }


//------------------------------------weapon data
    g_weapon_data.resize(cj["weapon_data"].size());

    for(int i = 0; i < cj["weapon_data"].size(); i++) {

        std::string w_id_s = cj["weapon_data"][i]["weapon_id"];
        ItemID w_id = StrToItemId(w_id_s);

        std::string name = cj["weapon_data"][i]["weapon_name"];
        float cooldown = cj["weapon_data"][i]["cooldown"];

        WEAPON_TYPE weapon_type = StrToWeaponTypeId(cj["weapon_data"][i]["weapon_type"]);

        int damage = cj["weapon_data"][i]["damage"];

        float recoil = cj["weapon_data"][i]["recoil"];
        float knockback = cj["weapon_data"][i]["knockback"];

        int mod_slots = cj["weapon_data"][i]["mod_slots"];
        
        


        WeaponData new_weapon = {
            .weapon_name = name,
            .weapon_id = w_id,
            .weapon_type = weapon_type,
            .cooldown = cooldown,
            .damage = damage,
            .recoil = recoil,
            .knockback = knockback,
            .mod_slots = mod_slots,
        };

        TraceLog(LOG_INFO, "Weapon Data Loaded  id: %i  %s  type: %i", w_id, name.c_str(), weapon_type);
        g_weapon_data[(int)w_id - ITEM_ID_DAGGER] = new_weapon;
    }



//------------------------------------food data
    int food_size = cj["food_data"].size();

    g_food_data.resize(food_size);
    TraceLog(LOG_INFO, "FOOD Data size  %i  %i", g_food_data.size(), food_size);

    for(int i = 0; i < cj["food_data"].size(); i++) {

        std::string f_id_s = cj["food_data"][i]["food_id"];
        ItemID f_id = StrToItemId(f_id_s);

        std::string name = cj["food_data"][i]["food_name"];

        float saturation = cj["food_data"][i]["saturation"];

        FoodData new_food = {
            .food_id = f_id,
            .food_name = name,
            .saturation = saturation

        };

        TraceLog(LOG_INFO, "FOOD Data Loaded  id: %i  %s sat %0.2f", (int)f_id - ITEM_ID_APPLE, name.c_str(), saturation);
        g_food_data[(int)f_id - ITEM_ID_APPLE] = new_food;
    }


//------------------------------------plan data
    g_plan_data.resize(cj["plan_data"].size());

    for(int i = 0; i < cj["plan_data"].size(); i++) {

        std::string p_id_s = cj["plan_data"][i]["plan_id"];
        ItemID p_id = StrToItemId(p_id_s);

        std::string name = cj["plan_data"][i]["plan_name"];

        std::vector<int> recipie_list;

        for(int r = 0; r < cj["plan_data"][i]["required_items"].size(); r++) {
            int _id = StrToItemId( cj["plan_data"][i]["required_items"][r] );
            TraceLog(LOG_INFO, "required item  %i", _id);
            recipie_list.push_back(_id);
        }

        ModuleID m_id = StrToModuleId( cj["plan_data"][i]["module_id"]);



        PlanData new_plan = {
            .plan_id = p_id,
            .plan_name = name,
            .ingredients = recipie_list,
            .module_id = m_id,
        };

        TraceLog(LOG_INFO, "Plan Data Loaded  id: %i  %s  %i", p_id, name.c_str(), recipie_list.size());
        g_plan_data[(int)p_id - ITEM_ID_STOVE_PLAN] = new_plan;
    }

//------------------------------------module data
    g_module_data.resize(cj["module_data"].size());

    for(int i = 0; i < cj["module_data"].size(); i++) {

        std::string m_id_s = cj["module_data"][i]["module_id"];
        ModuleID m_id = StrToModuleId(m_id_s);

        std::string name = cj["module_data"][i]["module_name"];

        std::vector<int> recipie_list;

        for(int r = 0; r < cj["module_data"][i]["recipies"].size(); r++) {
            int _id = StrToRecipieId( cj["module_data"][i]["recipies"][r] );
            recipie_list.push_back(_id);
        }

        std::vector<int> plan_list;

        for(int r = 0; r < cj["module_data"][i]["accepted_plans"].size(); r++) {
            int _id = StrToItemId( cj["module_data"][i]["accepted_plans"][r] );
            plan_list.push_back(_id);
            TraceLog(LOG_INFO, "accepted plan id: %i", _id);
        }

        ModuleData new_module = {
            .module_id = m_id,
            .module_name = name,
            .recipies = recipie_list,
            .accepted_plans = plan_list
        };

        TraceLog(LOG_INFO, "Module Data Loaded  id: %i  %s  %i %i", m_id, name.c_str(), recipie_list.size(),plan_list.size());
        g_module_data[(int)m_id] = new_module;
    }


//------------------------------------recipie data
    g_recipie_data.resize(cj["recipie_data"].size());

    for(int i = 0; i < cj["recipie_data"].size(); i++) {

        std::string m_id_s = cj["recipie_data"][i]["recipie_id"];
        RecipieID m_id = StrToRecipieId(m_id_s);

        std::string name = cj["recipie_data"][i]["recipie_name"];

        std::vector<int> ingredient_list;

        for(int r = 0; r < cj["recipie_data"][i]["ingredients"].size(); r++) {
            ItemID _id = StrToItemId( cj["recipie_data"][i]["ingredients"][r] );
            ingredient_list.push_back(_id);
        }

        ItemID produces = StrToItemId(cj["recipie_data"][i]["produces"]);

        RecipieData new_recipie = {
            .recipie_name = name,      
            .recipie_id = m_id,
            .ingredients = ingredient_list,
            .produces = produces
        };

        TraceLog(LOG_INFO, "recipie_data  Loaded  id: %i  %s  produces %i", m_id, name.c_str(), produces);
        g_recipie_data[(int)m_id] = new_recipie;
    }


    //--------------------food mods
    g_food_mod_data.resize(cj["food_modifiers"].size());

    for(int i = 0; i < cj["food_modifiers"].size(); i++) {

        FoodModData new_mod;

        new_mod.mod_id = StrToItemModId( cj["food_modifiers"][i]["mod_id"] );
        new_mod.mod_name = cj["food_modifiers"][i]["mod_name"];
        new_mod.saturation = cj["food_modifiers"][i]["saturation"];
        new_mod.rarity = StrToItemRarity( cj["food_modifiers"][i]["rarity"]);
                
        TraceLog(LOG_INFO, "FOOD Mod Data  Loaded  id: %i  %s", new_mod.mod_id, new_mod.mod_name.c_str());
        g_food_mod_data[new_mod.mod_id - ITEMMOD_NUTRITIOUS] = new_mod;
    }

    //--------------------ai data
    g_ai_data.resize(cj["ai_data"].size());
    for(int i = 0; i < cj["ai_data"].size(); i++) {

        AiData new_ai;

        new_ai.ai_id = StrToAiId( cj["ai_data"][i]["ai_id"] );
        new_ai.ai_name = cj["ai_data"][i]["ai_name"];
        new_ai.hostility = StrToAiId( cj["ai_data"][i]["hostility"] );
                
        TraceLog(LOG_INFO, "AI Data  Loaded  id: %i  %s", new_ai.ai_id, new_ai.ai_name.c_str());
        g_ai_data[new_ai.ai_id] = new_ai;
    }



    for(int i = 0; i < cj["creature_data"].size(); i++) {
        int creature_id = StrToCreatureId( cj["creature_data"][i]["creature_id"] );
        int max_health = cj["creature_data"][i]["max_health"];
        int health = max_health;
        int exp = 0;
        float base_speed = cj["creature_data"][i]["base_speed"];
        float current_speed = base_speed;
        int sprite_sheet_id = StrToSpriteId(cj["creature_data"][i]["sprite_sheet_id"]);
        int portrait_id = -1;
        std::string name = cj["creature_data"][i]["creature_name"];

        float max_stamina = cj["creature_data"][i]["max_stamina"];

        AIID ai_id = StrToAiId( cj["creature_data"][i]["ai_id"]);


        LootTableID loot_table_id = StrToLootTableId( cj["creature_data"][i]["loot_table_id"]);;

 
        bool is_npc = cj["creature_data"][i]["is_npc"];

        DIALOGUE_TOPIC dialogue_id = TOPIC_NONE;

        if(is_npc) {
            dialogue_id = StrToDialogueId(cj["creature_data"][i]["dialogue_id"]);
        }
        
        TraceLog(LOG_INFO, "creature : %i", creature_id);
        std::vector<int> inv;
        inv.push_back(-1);
        
        std::vector<int> hot;
        hot.push_back(-1);

        std::vector<int> p;
        p.push_back(-1);

        std::vector<int> s;
        s.push_back(-1);

        std::vector<int> hd;
        hd.push_back(-1);

        std::vector<int> bd;
        bd.push_back(-1);

        std::vector<int> lg;
        lg.push_back(-1);

        std::vector<int> ft;
        ft.push_back(-1);

        std::vector<int> hs;
        hs.push_back(-1);

        std::vector<int> ti;
        ti.push_back(-1);


        CharacterData this_creature = {
            
            
            .inventory = inv,
            .hotbar = hot,

            .primary = p,
            .secondary = s,
            .head = hd,
            .body = bd,
            .legs = lg,
            .feet = ft,
            .hands = hs,

            .trade_items = ti,

            .creature_id = creature_id,
            .health = health,
            .max_health = max_health,
            .exp = 0,
            .base_speed = base_speed,
            .current_speed = current_speed,
            .defence = 0,
            .magic_defence = 0,
            .max_power = 0,
            .current_power = 0,
            .saturation = 10,
            .max_saturation = 10,
            .max_stamina = max_stamina,
            .current_stamina = max_stamina,
            .ai_data = g_ai_data[ai_id],
            .is_npc = is_npc,
            .sprite_sheet_id = sprite_sheet_id,
            .portrait_id = portrait_id,
            .name = name,
            .class_name = name,
            .dialogue_id = dialogue_id,
            .loot_table_id = loot_table_id
        };

        g_creature_data[this_creature.creature_id] = this_creature;
        TraceLog(LOG_INFO, "CREATURE Data  Loaded  id: %i  %s", this_creature.creature_id, this_creature.name.c_str());
    }


        //--------------------creature loot tables

    LoadLootTables(cj["loot_tables"]);

    LoadLootPools(cj["loot_pools"]);



    LoadMapGenData(cj["map_gen"]);




    cfile.close();
 


    //TraceLog(LOG_INFO, "==========check save data================");

    if(std::filesystem::exists(data_save_path)) {
        g_game_data.save_available = true;
        TraceLog(LOG_INFO, "SAVE FILE FOUND");
    }
}


void SaveGame(LevelData &level_data) {

    TraceLog(LOG_INFO, "----------------------SAVING GAME------------------------" );
    
    std::ofstream file(data_save_path);
    if (!file.is_open()) {
        TraceLog(LOG_INFO, "filed to open save file");
        return;
    }

    json j;
    int uid = g_current_player->uid;
    j["health"] = g_active_creature_data[uid].health;
    j["max_health"] = g_active_creature_data[uid].max_health;
    j["exp"] = g_active_creature_data[uid].exp;
    j["max_power"] = g_active_creature_data[uid].max_power;
    j["current_power"] = g_active_creature_data[uid].current_power;
    j["defence"] = g_active_creature_data[uid].defence;
    j["magic_defence"] = g_active_creature_data[uid].magic_defence;
    j["base_speed"] = g_active_creature_data[uid].base_speed;
    j["saturation"] = g_active_creature_data[uid].saturation;
    j["sprite_sheet_id"] = g_active_creature_data[uid].sprite_sheet_id;
    j["portrait_id"] = g_active_creature_data[uid].portrait_id;
    j["name"] = g_active_creature_data[uid].name;
    j["class_name"] = g_active_creature_data[uid].class_name;
    j["max_saturation"] = g_active_creature_data[uid].max_saturation;
    j["max_stamina"] = g_active_creature_data[uid].max_stamina;

    //j["inventory"] = {};
    for(int i = 0; i < g_active_creature_data[uid].inventory.size(); i++) {
        j["inventory"][i] = g_active_creature_data[uid].inventory[i];
    }
    for(int i = 0; i < g_active_creature_data[uid].hotbar.size(); i++) {
        j["hotbar"][i] = g_active_creature_data[uid].hotbar[i];
    }

    for(int i = 0; i < g_active_creature_data[uid].primary.size(); i++) {
        j["primary"][i] = g_active_creature_data[uid].primary[i];
    }
    for(int i = 0; i < g_active_creature_data[uid].secondary.size(); i++) {
        j["secondary"][i] = g_active_creature_data[uid].secondary[i];
    }
    for(int i = 0; i < g_active_creature_data[uid].head.size(); i++) {
        j["head"][i] = g_active_creature_data[uid].head[i];
    }
    for(int i = 0; i < g_active_creature_data[uid].body.size(); i++) {
        j["body"][i] = g_active_creature_data[uid].body[i];
    }
    for(int i = 0; i < g_active_creature_data[uid].legs.size(); i++) {
        j["legs"][i] = g_active_creature_data[uid].legs[i];
    }
    for(int i = 0; i < g_active_creature_data[uid].feet.size(); i++) {
        j["feet"][i] = g_active_creature_data[uid].feet[i];
    }
    for(int i = 0; i < g_active_creature_data[uid].hands.size(); i++) {
        j["hands"][i] = g_active_creature_data[uid].hands[i];
    }

    json json_item_instances = json::array();

    TraceLog(LOG_INFO, "# g_item_instances %i  ", g_item_instances.size());

    for (auto& [key, inst] : g_item_instances) {
        json instance = {
            {"item_id", inst.item_id},
            {"instance_id", inst.instance_id},
            {"value", inst.value},
            {"type", inst.type},
            {"item_name", inst.item_name},

            {"container_id", inst.container_id},
            {"item_id", inst.item_id},
            {"level", inst.level},
            {"sprite_id", inst.sprite_id},
            {"rarity", inst.rarity},
            {"mod_slots", inst.mod_slots}
        };

        instance["weapon_id"] = inst.weapon_id;
        if(inst.weapon_id != -1) {
            json new_weapon = {
                {"weapon_id", inst.weapon_data.weapon_id},
                {"weapon_name", inst.weapon_data.weapon_name},
                {"cooldown", inst.weapon_data.cooldown},
                //{"spell_id", inst.weapon_data.spell_id},
                {"damage", inst.weapon_data.damage},
                {"recoil", inst.weapon_data.recoil},
                {"knockback", inst.weapon_data.knockback},
                {"mod_slots", inst.weapon_data.mod_slots},
                {"weapon_type", inst.weapon_data.weapon_type},
                {"max_power", inst.weapon_data.max_power},
                {"current_power", inst.weapon_data.current_power},
            };

            if(inst.weapon_data.weapon_type == WEAPON_TYPE_CASTER) {
            

                json caster;// = json::array();

                caster["base"] = {
                    {"part_id", inst.weapon_data.caster_data.base.id},
                    {"part_name", inst.weapon_data.caster_data.base.name},
                    {"accuracy", inst.weapon_data.caster_data.base.accuracy},
                };
                caster["igniter"] = {
                    {"part_id", inst.weapon_data.caster_data.igniter.id},
                    {"part_name", inst.weapon_data.caster_data.igniter.name},
                    {"spell_effect", inst.weapon_data.caster_data.igniter.effect},
                    {"max_power", inst.weapon_data.caster_data.igniter.max_power},
                    {"recoil", inst.weapon_data.caster_data.igniter.recoil},
                };

                caster["coupler"] = {
                    {"part_id", inst.weapon_data.caster_data.coupler.id},
                    {"part_name", inst.weapon_data.caster_data.coupler.name},
                    {"spell_type", inst.weapon_data.caster_data.coupler.type},
                    {"cooldown", inst.weapon_data.caster_data.coupler.cooldown},
                    {"damage", inst.weapon_data.caster_data.coupler.damage},
                    {"pps", inst.weapon_data.caster_data.coupler.pps},
                    {"radius", inst.weapon_data.caster_data.coupler.radius},
                    {"speed", inst.weapon_data.caster_data.coupler.speed},
                };

                caster["rod"] = {
                    {"part_id", inst.weapon_data.caster_data.rod.id},
                    {"part_name", inst.weapon_data.caster_data.rod.name},
                    {"knockback", inst.weapon_data.caster_data.rod.knockback},
                    {"damage", inst.weapon_data.caster_data.rod.damage},
                };

                new_weapon.push_back(caster);
            }
            
            instance["weapon_data"] = new_weapon;
            TraceLog(LOG_INFO, "saving weapon data %s", new_weapon["weapon_name"]);
        }
    
        instance["armor_id"] = inst.armor_id;
        if(inst.armor_id != -1) {

            json new_armor = {
                {"armor_id", inst.armor_data.armor_id},
                {"armor_name", inst.armor_data.armor_name},
                {"defence", inst.armor_data.defence},
                {"magic_defence", inst.armor_data.magic_defence},
                {"mod_slots", inst.armor_data.mod_slots},
                //{"spell_id", inst.armor_data.spell_id},
            };
            
            instance["armor_data"] = new_armor;
            //TraceLog(LOG_INFO, "saving armor data %s", new_armor["armor_name"]);
        }
    
        instance["food_id"] = inst.food_id;
        if(inst.food_id != -1) {

            json new_food = {
                {"food_id", inst.food_data.food_id},
                {"food_name", inst.food_data.food_name},
                {"saturation", inst.food_data.saturation},
            };
            
            instance["food_data"] = new_food;
            //TraceLog(LOG_INFO, "saving food data %s", new_food["food_name"]);
        }
    


        json_item_instances.push_back(instance);
        TraceLog(LOG_INFO, "saving item %i   instance id: %i container iid: %s  sub json size: %i  g_instances size %i", inst.item_id, inst.instance_id, inst.container_id.c_str(), json_item_instances.size(), g_item_instances.size());
    }

    TraceLog(LOG_INFO, "saved instances size %i", json_item_instances.size());
    j["item_instances"] = json_item_instances;    


    json json_persistant_containers = json::array();

    for(int entity = 0; entity < level_data.entity_list.size(); entity++) {
        if(level_data.entity_list[entity]->identifier == "PermContainerEntity" or level_data.entity_list[entity]->identifier == "GroundContainerEntity") {

            if (level_data.entity_list[entity]->is_persistant) {
                BaseContainerEntity* c_entity = dynamic_cast<BaseContainerEntity*>(level_data.entity_list[entity].get());
                
                json container = {
                    {"iid", c_entity->iid},
                    {"is_persistant", c_entity->is_persistant},
                    {"identifier", c_entity->identifier},
                    {"loot_table_id", c_entity->loot_table_id},
                    {"position_x", c_entity->position.x},
                    {"position_y", c_entity->position.y},
                    {"size_x", c_entity->c_area.size.x},
                    {"size_y", c_entity->c_area.size.y},
                    {"sprite_id", c_entity->sprite_id},
                    {"level_index", c_entity->level_index},
                };

                json_persistant_containers.push_back(container);
                TraceLog(LOG_INFO, "saving container  iid %s", c_entity->iid.c_str());

                ContainerData c_data = container.get<ContainerData>();
                g_persistant_containers[c_data.iid] = c_data;
 
            }
        }
    }

    j["persistant_containers"] = json_persistant_containers;


    file<<j.dump(4);
    file.close();

    if(!g_game_data.using_saved_data) {
   
        std::ifstream src("assets/maps/ldtk/test.ldtk", std::ios::binary);
        std::ofstream dst("saves/saved_map.json", std::ios::binary);
        
        dst << src.rdbuf();
        dst.close();
    }

    
    std::ifstream wgfile("saves/saved_map.json");
    if (!wgfile.is_open()) {
        TraceLog(LOG_INFO, "filed to open map save file");
        return;
        }
        
    json wgj;
    wgfile >> wgj;
    wgfile.close(); 


    //NOT SAVING
    for(LDTKLevel &level : g_ldtk_maps.levels) {

        bool already_saved = true;

/*         for(int _l = 0; _l < wgj["levels"].size(); _l++) {
            if(level.identifier == wgj["levels"][_l]["identifier"]) {
                already_saved = true;
            }
        } */

        if (level.is_worldgen and !already_saved) {
            TraceLog(LOG_INFO, "SAVING LEVEL  %s", level.identifier.c_str());
            json level_j;

            level_j["identifier"] = level.identifier;
            level_j["uid"] = level.uid;
            level_j["pxWid"] = level.px_wid;
            level_j["pxHei"] = level.px_hei;
            level_j["is_shelter"] = level.is_shelter;
            level_j["is_sub_map"] = level.is_sub_map;
            level_j["is_worldgen"] = level.is_worldgen;

            level_j["layerInstances"] = json::array();

            for(LDTKLayerInstance layer : level.layer_instances) {
                json layer_j;

                layer_j["__identifier"] = layer.identifier;
                layer_j["__type"] = layer.type;
                layer_j["__tilesetDefUid"] = layer.tileset_def_uid;
                layer_j["__cHei"] = layer.c_hei;
                layer_j["__cWid"] = layer.c_wid;
                layer_j["__gridSize"] = layer.grid_size;

    //TILE GRID
                json grid_array =  json::array();
                for(LDTKGridTile tile : layer.grid_tiles) {
                    json tile_j;
                    tile_j["px"] = {tile.px[0], tile.px[1]};
                    tile_j["src"] = {tile.src[0], tile.src[1]};;
                    tile_j["t"] = tile.t;

                    grid_array.push_back(tile_j);
                }
                layer_j["gridTiles"] = grid_array;
    //-------------------------


    //INT GRID
                json int_grid =  json::array();
                for(int tile : layer.int_grid) {
                    int_grid.push_back(tile);
                }
                layer_j["intGridCsv"] = int_grid;
    //-------------------------

    //ENTITIE INSTANCES  
                layer_j["entityInstances"] = json::array();
                for(LDTKEntityInstance entity : layer.entity_instances) {
                    json entity_j;

                    entity_j["__identifier"] = entity.identifier;

                    entity_j["iid"] = entity.iid;
                    entity_j["width"] = entity.width;
                    entity_j["height"] = entity.height;
                    entity_j["px"] = {entity.px[0], entity.px[1]};

                    
                    entity_j["fieldInstances"] = json::array();
                    for(LDTKFieldInstance field : entity.field_instances) {
                        json field_j;

                        field_j["__identifier"] = field.identifier;

                        if(field.identifier == "DestMapString") {
                            field_j["__value"] = field.value_s;

                        }
                        if (field.identifier == "ReturnPosition") {
                            field_j["__value"]["cx"] = (int)field.value_v.x;
                            field_j["__value"]["cy"] = (int)field.value_v.y;
                        }
                        field_j["__identifier"] = field.identifier;
                        field_j["__identifier"] = field.identifier;

                        entity_j["fieldInstances"].push_back(field_j);

                    }


                    layer_j["entityInstances"].push_back(entity_j);
                }

                //get all environmental entities
                if(layer.identifier == "Entities") {
                    for(LDTKEnvironmentData env_entity : level.environment_data ) {

                        json entity_j;

                        entity_j["__identifier"] = "EnvEntity";
                        entity_j["iid"] = "env";
                        entity_j["width"] = 0;
                        entity_j["height"] = 0;
                        entity_j["px"] = {(int)env_entity.position.x, (int)env_entity.position.y};

                        entity_j["fieldInstances"] = json::array();

                        json field_j;
                        field_j["__identifier"] = "id";
                        field_j["__value"] = env_entity.item_string;
                        
                        entity_j["fieldInstances"].push_back(field_j);


                        layer_j["entityInstances"].push_back(entity_j);
                        //TraceLog(LOG_INFO, "env entity saved");
                    }
                }
    //------------------------


                level_j["layerInstances"].push_back(layer_j);
                
            }
            wgj["levels"].push_back(level_j);
            TraceLog(LOG_INFO, "LEVEL SAVED");
        }
    }

   
    std::ofstream wgout("saves/saved_map.json");
    wgout << wgj.dump();
    wgout.close(); 

    g_game_data.using_saved_data = true;

}


int LoadGame() {
    TraceLog(LOG_INFO, "LOADING SAVED DATA....save.json");

    int uid = GetRandomValue(10000, 1000000);

    g_game_data.using_saved_data = true;

    std::ifstream file(data_save_path);
    if (!file.is_open()) {
        TraceLog(LOG_INFO, "CANNOT OPEN FILE");
        return uid;
    }
    
    json j;
    file>>j;

    g_active_creature_data[uid].creature_id = 0;
    g_active_creature_data[uid].health = j["health"];
    g_active_creature_data[uid].max_health = j["max_health"];
    g_active_creature_data[uid].exp = j["exp"];
    g_active_creature_data[uid].saturation = j["saturation"];
    g_active_creature_data[uid].max_saturation= j["max_saturation"];
    g_active_creature_data[uid].max_stamina= j["max_stamina"];
    g_active_creature_data[uid].current_stamina= j["max_stamina"];
    g_active_creature_data[uid].sprite_sheet_id = j["sprite_sheet_id"];
    g_active_creature_data[uid].portrait_id = j["portrait_id"];
    g_active_creature_data[uid].name = j["name"];
    g_active_creature_data[uid].class_name = j["class_name"];
    
    g_active_creature_data[uid].base_speed = j["base_speed"];
    g_active_creature_data[uid].current_speed = j["base_speed"];

    g_active_creature_data[uid].creature_id = -1;

    std::vector<int> inv;
    for(int i = 0; i < j["inventory"].size(); i++) {
        inv.push_back(j["inventory"][i]);
    }

    std::vector<int> hot;
    for(int i = 0; i < j["hotbar"].size(); i++) {
        hot.push_back(j["hotbar"][i]);
    }

    std::vector<int> p;
    for(int i = 0; i < j["primary"].size(); i++) {
        p.push_back(j["primary"][i]);
    }
///
    std::vector<int> s;
    for(int i = 0; i < j["secondary"].size(); i++) {
        s.push_back(j["secondary"][i]);
    }

    std::vector<int> hd;
    for(int i = 0; i < j["head"].size(); i++) {
        hd.push_back(j["head"][i]);
    }

    std::vector<int> bd;
    for(int i = 0; i < j["body"].size(); i++) {
        bd.push_back(j["body"][i]);
    }

    std::vector<int> lg;
    for(int i = 0; i < j["legs"].size(); i++) {
        lg.push_back(j["legs"][i]);
    }

    std::vector<int> ft;
    for(int i = 0; i < j["feet"].size(); i++) {
        ft.push_back(j["feet"][i]);
    }

    std::vector<int> hs;
    for(int i = 0; i < j["hands"].size(); i++) {
        hs.push_back(j["hands"][i]);
    }

    g_active_creature_data[uid].inventory = inv;
    g_active_creature_data[uid].hotbar = hot;

    g_active_creature_data[uid].primary = p;
    g_active_creature_data[uid].secondary = s;
    g_active_creature_data[uid].head = hd;
    g_active_creature_data[uid].body = bd;
    g_active_creature_data[uid].legs = lg;
    g_active_creature_data[uid].feet = ft;
    g_active_creature_data[uid].hands = hs;


    g_item_instances.clear();
    for(const auto & item : j["item_instances"]) {
        ItemInstanceData instance = item.get<ItemInstanceData>();
        g_item_instances[instance.instance_id] = instance;
    }

    g_persistant_containers.clear();
    for(const auto & container : j["persistant_containers"]) {
        ContainerData c_data = container.get<ContainerData>();
        g_persistant_containers[c_data.iid] = c_data;
    }

    file.close();

    TraceLog(LOG_INFO, "==========LOADING SAVED LDTK MAPS================");

    std::string ldtk_map_path = "saves/saved_map.json";
    int num_maps = load_ldtk_maps(ldtk_map_path);

    TraceLog(LOG_INFO, "==========END LOADING LDTK MAPS================  loaded %i maps", num_maps);

    return uid;

}


void SaveSubSceneState(LevelData &level_data, SubSceneState & sub_state) {

    sub_state.container_data = level_data.container_data;

}

void LoadSubSceneState(SubSceneState & sub_state, LevelData &level_data) {
    level_data.container_data =  sub_state.container_data;
}

void ClearLevelData(LevelData &level_data) {

    level_data.entity_list.clear();
    level_data.spell_list.clear();
    level_data.ui_entities.clear();
    level_data.environment_entities.clear();

    level_data.level_transitions.clear();
    level_data.creature_data.clear();
    level_data.container_data.clear();
    level_data.game_areas.clear();


}



void LoadLevelData(LevelData &level_data) {



    int map_index = 0;
    
    if(g_game_data.is_in_sub_map) {
        map_index = g_game_data.sub_map_index;
    }
    else if(g_game_data.current_map_index == g_game_data.shelter_map_index) {
        map_index = g_game_data.shelter_map_index;
    }

    else if (g_game_data.next_map_index == -1) {
        //LDTKLevel new_level;
        for(int l = 0; l < g_ldtk_maps.levels.size(); l++) {
            if(g_ldtk_maps.levels[l].identifier == g_game_data.level_name_to_create) {
                g_ldtk_maps.levels.erase(g_ldtk_maps.levels.begin() + l);
            }
        }
        
        map_index = g_ldtk_maps.levels.size();
        g_ldtk_maps.levels.emplace_back();

        //find tilesheets
        
        GenerateMap(g_ldtk_maps.levels.back(), g_game_data.level_name_to_create);

        g_game_data.current_map_index = map_index;
        TraceLog(LOG_INFO, "==========GENERATING NEW MAP================");
    }
    else {
        map_index = g_game_data.next_map_index;
        TraceLog(LOG_INFO, "==========FOUND EXISTING MAP================");
    }


    PrecalculateTileCollisionData(level_data);
    PrecalculateShadowData(level_data);



    LDTKLevel &this_level = g_ldtk_maps.levels[map_index];

    TraceLog(LOG_INFO, "\nLOADING LEVEL DATA\n index: %i name: %s", map_index, this_level.identifier.c_str());
    for(int layer = 0; layer < this_level.layer_instances.size(); layer++) {
        //TraceLog(LOG_INFO, "    layer  %s", this_level.layer_instances[layer].identifier.c_str());
    }

    for(int layer_index = 0; layer_index < this_level.layer_instances.size(); layer_index++) {
        if(this_level.layer_instances[layer_index].type == "Entities") {
            int tile_size = this_level.layer_instances[layer_index].grid_size;
            for(int entity_index = 0; entity_index < this_level.layer_instances[layer_index].entity_instances.size(); entity_index++) {
                if(this_level.layer_instances[layer_index].entity_instances[entity_index].identifier == "SpawnPoint") {
                    //TraceLog(LOG_INFO, "SPAWN POINT FOUND");
                    Vector2 sp = {};
                    sp.x = {(float)this_level.layer_instances[layer_index].entity_instances[entity_index].px[0]};
                    sp.y = {(float)this_level.layer_instances[layer_index].entity_instances[entity_index].px[1]};
                    level_data.spawn_position = sp;
                    TraceLog(LOG_INFO, "SPAWN POINT FOUND %0.0f %0.0f", sp.x, sp.y);
                }
                else {
                    //TraceLog(LOG_INFO, "SPAWN POINT NOT FOUND");
                }

                std::string identifier = this_level.layer_instances[layer_index].entity_instances[entity_index].identifier;
                //TraceLog(LOG_INFO, "DATA FOUND FOR %s", identifier.c_str());
                //if(identifier == "LevelTransition" or identifier == "ShelterTransition") {
                if(identifier == "LevelTransition" or identifier == "ShelterTransition" or identifier == "HouseTransition") {
                    LevelTransitionData new_transition;
                    //TraceLog(LOG_INFO, "TRANSITION POINT FOUND %s", identifier.c_str());

                    new_transition.size = {(float)this_level.layer_instances[layer_index].entity_instances[entity_index].width, (float)this_level.layer_instances[layer_index].entity_instances[entity_index].height};
                    new_transition.identifier = this_level.layer_instances[layer_index].entity_instances[entity_index].identifier;
                    new_transition.dest_string = this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[0].value_s;
                    new_transition.position_i.x = this_level.layer_instances[layer_index].entity_instances[entity_index].px[0];
                    new_transition.position_i.y = this_level.layer_instances[layer_index].entity_instances[entity_index].px[1];
                    new_transition.position_f.x = (float)this_level.layer_instances[layer_index].entity_instances[entity_index].px[0] * tile_size;
                    new_transition.position_f.y = (float)this_level.layer_instances[layer_index].entity_instances[entity_index].px[1] * tile_size;
                    new_transition.uid = this_level.layer_instances[layer_index].entity_instances[entity_index].iid;

                    if(new_transition.identifier == "HouseTransition") {
                        new_transition.return_position = this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[1].value_v;
                        //TraceLog(LOG_INFO, "RETURN POSITION, %0.02f %0.02f", new_transition.return_position.x, new_transition.return_position.y);
                    }

                    level_data.level_transitions.push_back(new_transition);
                    TraceLog(LOG_INFO, "TRANSITION dest string ADDED, %s", new_transition.dest_string.c_str());
                }

                if(identifier == "PermContainerEntity"){

                    if( (!g_game_data.using_saved_data or g_game_data.current_map_index != g_game_data.shelter_map_index) or 
                        (g_game_data.is_in_sub_map and !g_game_data.using_saved_data) ) {

                            TraceLog(LOG_INFO, "usd %i  cmi %i == smi %i  (sm %i and usd %i)", 
                                g_game_data.using_saved_data,
                                g_game_data.current_map_index,
                                g_game_data.shelter_map_index,
                                g_game_data.is_in_sub_map,
                                g_game_data.using_saved_data
                            );
                        
                        ContainerData new_container;
                                                
                        new_container.size = {(float)this_level.layer_instances[layer_index].entity_instances[entity_index].width, (float)this_level.layer_instances[layer_index].entity_instances[entity_index].height};
                        new_container.identifier = identifier;
                        new_container.position_i.x = this_level.layer_instances[layer_index].entity_instances[entity_index].px[0];
                        new_container.position_i.y = this_level.layer_instances[layer_index].entity_instances[entity_index].px[1];
                        new_container.position_f.x = (float)this_level.layer_instances[layer_index].entity_instances[entity_index].px[0] * tile_size;
                        new_container.position_f.y = (float)this_level.layer_instances[layer_index].entity_instances[entity_index].px[1] * tile_size;

                        if(g_game_data.is_in_sub_map) {
                            std::string extra  = std::to_string(GetRandomValue(5000, 10000));
                            new_container.iid = this_level.layer_instances[layer_index].entity_instances[entity_index].iid + "subcontainer" + extra ;
                        }
                        else {
                            new_container.iid = this_level.layer_instances[layer_index].entity_instances[entity_index].iid;
                        }
                        
                        new_container.sprite_id = this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[0].value_i;
                        new_container.loot_level = this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[1].value_i;

                        LootTableID table = (LootTableID)this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[2].value_i;
                        new_container.loot_table_id = table;

                        TraceLog(LOG_INFO, "MAKING NEW CONTAINER %s with loot table %i  loot level %i", new_container.iid.c_str(), table, new_container.loot_level);

                        
                        //TraceLog(LOG_INFO, "PERM CONTAINER DATA ADDED IID %s", new_container.iid.c_str());
                        level_data.container_data.push_back(new_container);
                    }
                    //TraceLog(LOG_INFO, "CONTAINER ADDED WITH LT %i SID %i", new_container.loot_table_id, new_container.sprite_id);
                }

                if(identifier == "GroundContainerEntity" and !g_game_data.using_saved_data) {

                    if( (!g_game_data.using_saved_data or g_game_data.current_map_index != g_game_data.shelter_map_index) and
                            !g_game_data.is_in_sub_map ) {
                        ContainerData new_container;
                        //TraceLog(LOG_INFO, "GROUND CONTAINER FOUND %s %i", identifier.c_str(), this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[0].i_list.size());
                        new_container.size = {(float)this_level.layer_instances[layer_index].entity_instances[entity_index].width, (float)this_level.layer_instances[layer_index].entity_instances[entity_index].height};
                        new_container.identifier = this_level.layer_instances[layer_index].entity_instances[entity_index].identifier;
                        new_container.position_i.x = this_level.layer_instances[layer_index].entity_instances[entity_index].px[0];
                        new_container.position_i.y = this_level.layer_instances[layer_index].entity_instances[entity_index].px[1];
                        new_container.position_f.x = (float)this_level.layer_instances[layer_index].entity_instances[entity_index].px[0] * tile_size;
                        new_container.position_f.y = (float)this_level.layer_instances[layer_index].entity_instances[entity_index].px[1] * tile_size;

                        if(g_game_data.is_in_sub_map) {
                            std::string extra  = std::to_string(GetRandomValue(5000, 10000));
                            new_container.iid = this_level.layer_instances[layer_index].entity_instances[entity_index].iid + "subgroundcontainer" + extra ;
                        }
                        else {
                            new_container.iid = this_level.layer_instances[layer_index].entity_instances[entity_index].iid;
                        }
                        
                        for(int item = 0; item < this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[0].i_list.size(); item++) {
                            //TraceLog(LOG_INFO, "GROUND CONTAINER DATA ADDED %i", this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[0].i_list[item]);
                            new_container.item_list.push_back(this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[0].i_list[item]);
                        } 
                        //TraceLog(LOG_INFO, "GROUND CONTAINER DATA ADDED IID %s", new_container.iid.c_str());
                        level_data.container_data.push_back(new_container);
                    }
                }

                if(identifier == "ModuleEntity") {
                    //if(!g_game_data.using_saved_data or g_game_data.current_map_index != g_game_data.shelter_map_index ) {
                        ModuleEntityData new_module;
                        TraceLog(LOG_INFO, "MODULE FOUND %s %i", identifier.c_str(), this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[0].i_list.size());
                        new_module.size = {(float)this_level.layer_instances[layer_index].entity_instances[entity_index].width, (float)this_level.layer_instances[layer_index].entity_instances[entity_index].height};
                        new_module.identifier = this_level.layer_instances[layer_index].entity_instances[entity_index].identifier;
                        new_module.position_i.x = this_level.layer_instances[layer_index].entity_instances[entity_index].px[0];
                        new_module.position_i.y = this_level.layer_instances[layer_index].entity_instances[entity_index].px[1];
                        new_module.position_f.x = (float)this_level.layer_instances[layer_index].entity_instances[entity_index].px[0] * tile_size;
                        new_module.position_f.y = (float)this_level.layer_instances[layer_index].entity_instances[entity_index].px[1] * tile_size;
                        new_module.iid = this_level.layer_instances[layer_index].entity_instances[entity_index].iid;

                        new_module.module_id = this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[0].value_i;
                        level_data.module_data.push_back(new_module);
                    //}
                }


                if(identifier == "MushroomZone") {
                    MushroomZoneData new_zone;
                     
                    TraceLog(LOG_INFO, "MUSHROOM ZONE FOUND %s  ", identifier.c_str());
                    new_zone.position_i.x = this_level.layer_instances[layer_index].entity_instances[entity_index].px[0];
                    new_zone.position_i.y = this_level.layer_instances[layer_index].entity_instances[entity_index].px[1];
                    new_zone.size = {(float)this_level.layer_instances[layer_index].entity_instances[entity_index].width, (float)this_level.layer_instances[layer_index].entity_instances[entity_index].height};
                    new_zone.max_mushrooms = this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[0].value_i;
                    //TraceLog(LOG_INFO, "max mushrooms %i", new_zone.max_mushrooms); 

                   level_data.mushroom_zones.push_back(new_zone);
                }

                if(identifier == "CreatureEntity") {
                    CreatureID creature_id = (CreatureID)this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[0].value_i;
                    CharacterData new_creature = g_creature_data[creature_id];
                    new_creature.spawn_position.x = this_level.layer_instances[layer_index].entity_instances[entity_index].px[0];
                    new_creature.spawn_position.y = this_level.layer_instances[layer_index].entity_instances[entity_index].px[1];
                    TraceLog(LOG_INFO, "NEW CREATURE DATA %i    %s  ", creature_id, g_creature_data[creature_id].name.c_str());

                    level_data.creature_data.push_back(new_creature);
                }

                if(identifier == "DoorEntity") {
                    DoorData new_door;
                    new_door.is_locked = this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[0].value_i;
                    new_door.position.x = this_level.layer_instances[layer_index].entity_instances[entity_index].px[0];
                    new_door.position.y = this_level.layer_instances[layer_index].entity_instances[entity_index].px[1];

                    TraceLog(LOG_INFO, "NEW DOOR DATA locked ? %i ", new_door.is_locked);
                    level_data.door_data.push_back(new_door);
                }
            }
        }
    }

    //WHY DO THIS HERE?
    for(int thing = 0; thing < this_level.environment_data.size(); thing++) {
        //TraceLog(LOG_INFO, "-----environment sprite %s %0.0f %0.0f", this_level.environment_data[thing].item_string.c_str(), this_level.environment_data[thing].position.x, this_level.environment_data[thing].position.y);
        int id = StrToEnviroSpriteId(this_level.environment_data[thing].item_string);

        
        if(id < SPRITE_ENVIRO_GRASS_START or id > SPRITE_ENVIRO_GRASS_END) {
            std::unique_ptr<EnvironmentalEntity> new_entity = std::make_unique<EnvironmentalEntity>(this_level.environment_data[thing].position, id, true);
            level_data.environment_entities.push_back(std::move(new_entity));
        }
        else {
            std::unique_ptr<EnvironmentalEntity> new_entity = std::make_unique<EnvironmentalEntity>(this_level.environment_data[thing].position, id, false);
            level_data.environment_entities.push_back(std::move(new_entity));
        }
        //TraceLog(LOG_INFO, "-----\n");
    }

    //check for saved items 
    TraceLog(LOG_INFO, "\n\nPERSISTANT CONTAINER LIST SIZE %i", g_persistant_containers.size());
    for(const auto & container : g_persistant_containers) {
        TraceLog(LOG_INFO, "LOOKING FOR SAVED SAVED CONTAINER DATA FOR %s", container.second.iid.c_str());
        if(container.second.level_index == g_game_data.current_map_index) {
            TraceLog(LOG_INFO, "SAVED CONTAINER DATA FOUND FOR %s", container.second.iid.c_str());
            ContainerData new_container = container.second;

            level_data.container_data.push_back(new_container);
        }
    }

    TraceLog(LOG_INFO, "\nSUB CONTAINER LIST SIZE %i", g_sub_temp_containers.size());
    for(const auto & container : g_sub_temp_containers) {
        TraceLog(LOG_INFO, "LOOKING FOR SAVED SAVED SUB CONTAINER DATA FOR %s", container.second.iid.c_str());
        if(container.second.level_index == g_game_data.current_map_index) {
            TraceLog(LOG_INFO, "SAVED SUB CONTAINER DATA FOUND FOR %s", container.second.iid.c_str());
            ContainerData new_container = container.second;

            level_data.container_data.push_back(new_container);
        }
    }

    TraceLog(LOG_INFO, "\nEND LOAD LEVEL DATA \n\n");
}



void PrecalculateTileCollisionData(LevelData &level_data) {
    //TraceLog(LOG_INFO, " PRECALCULATING TILE COLLISION DATA ");

    level_data.precalc.map_index = g_game_data.current_map_index;

    if(g_game_data.is_in_sub_map) {
        level_data.precalc.map_index = g_game_data.sub_map_index;
    }


    LDTKLevel this_level = g_ldtk_maps.levels[level_data.precalc.map_index];
    LDTKLayerInstance *col_layer = nullptr;

    level_data.precalc.foreground_layer_index = -1;
    level_data.precalc.collision_layer_index = -1;
    for (int l = 0; l < this_level.layer_instances.size(); l++) {
        if(this_level.layer_instances[l].type == "IntGrid") {
            level_data.precalc.collision_layer_index = l;
            col_layer = &this_level.layer_instances[l];
            TraceLog(LOG_INFO, "            collision layer -- %i",level_data.precalc.collision_layer_index  );
        }
        if(this_level.layer_instances[l].identifier == "Foreground") {
            level_data.precalc.foreground_layer_index = l;
        }
    }

    if(col_layer == nullptr) {
        TraceLog(LOG_INFO, "no collision layer in map data");
    
        return;
    }

    level_data.precalc.tile_size = col_layer->grid_size;
    //TraceLog(LOG_INFO, "            tile size -- %i",level_data.precalc.tile_size  );

    level_data.precalc.inv_tile_size = 1/(float)level_data.precalc.tile_size;
    //TraceLog(LOG_INFO, "            inv tilesize -- %0.5f",level_data.precalc.inv_tile_size  );

    level_data.precalc.map_width = col_layer->c_wid;
    //TraceLog(LOG_INFO, "            collision layer -- %i",level_data.precalc.collision_layer_index  );
    TraceLog(LOG_INFO, "FINISHED PRECALCULATING TILE COLLISION DATA  for map index %i", level_data.precalc.map_index);
}


void PrecalculateShadowData(LevelData &level_data) {
    //TraceLog(LOG_INFO, "CALCULATING SHADOW DATA ");

    level_data.collision_polys.clear();

    LDTKLevel this_level = g_ldtk_maps.levels[level_data.precalc.map_index];
    LDTKLayerInstance *col_layer = nullptr;

    for (int l = 0; l < this_level.layer_instances.size(); l++) {
        if(this_level.layer_instances[l].type == "IntGrid") {
            //TraceLog(LOG_INFO, "            collision layer -- %i", level_data.precalc.collision_layer_index  );
            col_layer = &this_level.layer_instances[l];
        }
    }

    if(col_layer == nullptr) {
        //TraceLog(LOG_INFO, "no collision layer in map data");
        return;
    }

    int tile_size = level_data.precalc.tile_size;
    float inv_tile_size = level_data.precalc.inv_tile_size;
    int map_width = level_data.precalc.map_width;

    for(int tile = 0; tile < col_layer->int_grid.size(); tile++) {
        if(col_layer->int_grid[tile] == 1) {
            //TraceLog(LOG_INFO, "tile data %i", col_layer->int_grid[tile]);

            float x = (tile%level_data.precalc.map_width) * level_data.precalc.tile_size;
            float y = (tile/level_data.precalc.map_width) * level_data.precalc.tile_size;

            //TraceLog(LOG_INFO, "tile position %0.2f %0.2f", x, y);

            Polygon new_poly;

            Vector2 first = {x,y};
            new_poly.points.push_back(first);

            Vector2 seccond = {x + level_data.precalc.tile_size, y};
            new_poly.points.push_back(seccond);

            Vector2 third = {x+ level_data.precalc.tile_size, y + level_data.precalc.tile_size};
            new_poly.points.push_back(third);

            Vector2 fourth = {x, y + level_data.precalc.tile_size};
            new_poly.points.push_back(fourth);

            level_data.collision_polys.push_back(new_poly);
        }
    }
    //TraceLog(LOG_INFO, "FINISHED CALCULATING SHADOW DATA  # polygons %i", level_data.collision_polys.size());
}


void InstanceItemList(std::vector<int> &source_list, std::vector<int> &dest_list, std::string container_id) {

    //TraceLog(LOG_INFO, "instancing item list   size: %i container iid  %s ", source_list.size(), container_id.c_str());

    for(int item = 0; item < source_list.size(); item++) {
        int uid = GetRandomValue(1000, 1000000000);

        g_item_instances[uid] = GenerateItem((ItemID) source_list[item], uid, container_id);
        dest_list.push_back(uid);
    }
}


void InstanceRandomItemsFromList(std::vector<int> &source_list, std::vector<int> &dest_list, std::string container_id, int loot_level) {

    TraceLog(LOG_INFO, "instancing item list   size: %i to container iid  %s ", source_list.size(), container_id.c_str());

    for(int item = 0; item < source_list.size(); item++) {
        int uid = GetRandomValue(1000, 1000000000);

        g_item_instances[uid] = GenerateRandomItem((ItemID) source_list[item], uid, container_id, loot_level);
        dest_list.push_back(uid);
    }
}


ItemInstanceData* InstanceCharacterItem(ItemID item_id, int character_uid) {

    int uid = GetRandomValue(1000, 1000000000);
    g_item_instances[uid] = GenerateItem(item_id, uid, "");
    TraceLog(LOG_INFO, "instancing item %i with uid %i  to character %i ", item_id, uid, character_uid);
    
    int found_spot = -1;
    for(int slot = 0; slot < g_active_creature_data[character_uid].inventory.size(); slot++) {
        if(g_active_creature_data[character_uid].inventory[slot] == -1) {
            found_spot = slot;
            break;
        }
    }

    if(found_spot != -1) {
        g_active_creature_data[character_uid].inventory[found_spot] = uid;
        //TraceLog(LOG_INFO, "item id %i  item uid %i  spot %i",item_id, uid, found_spot);
    }
    else {
        g_active_creature_data[character_uid].inventory.push_back(uid);
        //TraceLog(LOG_INFO, "item id %i  item uid %i  adding to end",item_id, uid);
    }
    return &g_item_instances[uid];
}


ItemInstanceData* InstanceRandomCharacterItem(ItemID item_id, int character_uid, int _level) {

    //TraceLog(LOG_INFO, "instancing item list   size: %i container iid  %s ", source_list.size(), container_id.c_str());
    int uid = GetRandomValue(1000, 1000000000);
    g_item_instances[uid] = GenerateRandomItem(item_id, uid, "", _level);

    int found_spot = -1;
    for(int slot = 0; slot < g_active_creature_data[character_uid].inventory.size(); slot++) {
        if(g_active_creature_data[character_uid].inventory[slot] == -1) {
            found_spot = slot;
            break;
        }
    }

    if(found_spot != -1) {
        g_active_creature_data[character_uid].inventory[found_spot] = uid;
        //TraceLog(LOG_INFO, "item id %i  item uid %i  spot %i",item_id, uid, found_spot);
    }
    else {
        g_active_creature_data[character_uid].inventory.push_back(uid);
        //TraceLog(LOG_INFO, "item id %i  item uid %i  adding to end",item_id, uid);
    }
    return &g_item_instances[uid];
}


PlanID StrToPlanId(const std::string& s) {
    static const std::unordered_map<std::string, PlanID> lookup_table = {
        {"PLAN_ID_NONE",          PlanID::PLAN_ID_NONE},
        {"PLAN_ID_STOVE",         PlanID::PLAN_ID_STOVE},
        {"PLAN_ID_MUSHROOMPRESS", PlanID::PLAN_ID_MUSHROOMPRESS},
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        //TraceLog(LOG_INFO, "Spell ID found %i", it->second);
        return it->second;
    }
    //TraceLog(LOG_INFO, "Spell ID not found ");
    return PlanID::PLAN_ID_NONE;
}



RecipieID StrToRecipieId(const std::string& s) {
    static const std::unordered_map<std::string, RecipieID> lookup_table = {
        {"None",                         RecipieID::RECIPIE_ID_NONE},
        {"RECIPIE_ID_APPLE",             RecipieID::RECIPIE_ID_APPLE},
        {"RECIPIE_ID_MUSHROOMJUICE",     RecipieID::RECIPIE_ID_MUSHROOMJUICE},
        {"RECIPIE_ID_WAND",              RecipieID::RECIPIE_ID_WAND},
        {"RECIPIE_ID_STAFF",             RecipieID::RECIPIE_ID_STAFF},
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        //TraceLog(LOG_INFO, "Spell ID found %i", it->second);
        return it->second;
    }
    //TraceLog(LOG_INFO, "Spell ID not found ");
    return RecipieID::RECIPIE_ID_NONE;
}

ModuleID StrToModuleId(const std::string& s) {

    static const std::unordered_map<std::string, ModuleID> lookup_table = {
        {"None",                        ModuleID::MODULE_ID_NONE},
        {"MODULE_ID_WORKBENCH",         ModuleID::MODULE_ID_WORKBENCH},
        {"MODULE_ID_STOVE",             ModuleID::MODULE_ID_STOVE},
        {"MODULE_ID_MUSHROOMPRESS",     ModuleID::MODULE_ID_MUSHROOMPRESS},
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        //TraceLog(LOG_INFO, "Spell ID found %i", it->second);
        return it->second;
    }
    //TraceLog(LOG_INFO, "Spell ID not found ");
    return ModuleID::MODULE_ID_NONE;
}


/* CharEffectID StrToCharEffectId(const std::string& s) {

    static const std::unordered_map<std::string, CharEffectID> lookup_table = {
        {"CHAREFFECT_SPEED1",       CharEffectID::CHAREFFECT_SPEED1},
        {"CHAREFFECT_SPEED2",       CharEffectID::CHAREFFECT_SPEED2},
        {"CHAREFFECT_SPEED3",       CharEffectID::CHAREFFECT_SPEED3},
        {"CHAREFFECT_SPEED4",       CharEffectID::CHAREFFECT_SPEED4},
        {"CHAREFFECT_SPEED5",       CharEffectID::CHAREFFECT_SPEED5},

        {"CHAREFFECT_POISON1",     CharEffectID::CHAREFFECT_POISON1},
        {"CHAREFFECT_POISON2",     CharEffectID::CHAREFFECT_POISON2},
        {"CHAREFFECT_POISON3",     CharEffectID::CHAREFFECT_POISON3},
        {"CHAREFFECT_POISON4",     CharEffectID::CHAREFFECT_POISON4},
        {"CHAREFFECT_POISON5",     CharEffectID::CHAREFFECT_POISON5},
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        //TraceLog(LOG_INFO, "Spell ID found %i", it->second);
        return it->second;
    }
    //TraceLog(LOG_INFO, "Spell ID not found ");
    return CharEffectID::CHAREFFECT_NONE;
} */

/* CharModID StrToCharModId(const std::string& s) {

    static const std::unordered_map<std::string, CharModID> lookup_table = {
        {"CHARMOD_HEALTH1",       CharModID::CHARMOD_HEALTH1},
        {"CHARMOD_HEALTH2",       CharModID::CHARMOD_HEALTH2},
        {"CHARMOD_HEALTH3",       CharModID::CHARMOD_HEALTH3},
        {"CHARMOD_HEALTH4",       CharModID::CHARMOD_HEALTH4},
        {"CHARMOD_HEALTH5",       CharModID::CHARMOD_HEALTH5},

        {"CHARMOD_SPEED1",     CharModID::CHARMOD_SPEED1},
        {"CHARMOD_SPEED2",     CharModID::CHARMOD_SPEED2},
        {"CHARMOD_SPEED3",     CharModID::CHARMOD_SPEED3},
        {"CHARMOD_SPEED4",     CharModID::CHARMOD_SPEED4},
        {"CHARMOD_SPEED5",     CharModID::CHARMOD_SPEED5},

        {"CHARMOD_STAMINA1",     CharModID::CHARMOD_STAMINA1},
        {"CHARMOD_STAMINA2",     CharModID::CHARMOD_STAMINA2},
        {"CHARMOD_STAMINA3",     CharModID::CHARMOD_STAMINA3},
        {"CHARMOD_STAMINA4",     CharModID::CHARMOD_STAMINA4},
        {"CHARMOD_STAMINA5",     CharModID::CHARMOD_STAMINA5},
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        //TraceLog(LOG_INFO, "Spell ID found %i", it->second);
        return it->second;
    }
    //TraceLog(LOG_INFO, "Spell ID not found ");
    return CharModID::CHARMOD_NONE;
} */

ItemModID StrToItemModId(const std::string& s) {

    static const std::unordered_map<std::string, ItemModID> lookup_table = {
        {"ITEMMOD_NONE",       ItemModID::ITEMMOD_NONE},

        {"ITEMMOD_NUTRITIOUS",     ItemModID::ITEMMOD_NUTRITIOUS},

        {"ITEMMOD_MOLDY",     ItemModID::ITEMMOD_MOLDY},
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        //TraceLog(LOG_INFO, "Spell ID found %i", it->second);
        return it->second;
    }
    //TraceLog(LOG_INFO, "Spell ID not found ");
    return ItemModID::ITEMMOD_NONE;
}


ItemRarity StrToItemRarity(const std::string& s) {

    static const std::unordered_map<std::string, ItemRarity> lookup_table = {
        {"RARITY_COMMON",       ItemRarity::RARITY_COMMON},
        {"RARITY_UNCOMMON",     ItemRarity::RARITY_UNCOMMON},
        {"RARITY_RARE",         ItemRarity::RARITY_RARE},
        {"RARITY_VERYRARE",     ItemRarity::RARITY_VERYRARE},
        {"RARITY_ULTRARARE",    ItemRarity::RARITY_ULTRARARE},
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        //TraceLog(LOG_INFO, "Spell ID found %i", it->second);
        return it->second;
    }
    //TraceLog(LOG_INFO, "Spell ID not found ");
    return ItemRarity::RARITY_COMMON;
}

ItemType StrToItemType(const std::string& s) {
    static const std::unordered_map<std::string, ItemType> lookup_table = {
        {"TYPE_WEAPON",       ItemType::TYPE_WEAPON},
        {"TYPE_HEAD_ARMOR",     ItemType::TYPE_HEAD_ARMOR},
        {"TYPE_BODY_ARMOR",    ItemType::TYPE_BODY_ARMOR},
        {"TYPE_LEG_ARMOR",     ItemType::TYPE_LEG_ARMOR},
        {"TYPE_FEET_ARMOR",    ItemType::TYPE_FEET_ARMOR},
        {"TYPE_HAND_ARMOR",    ItemType::TYPE_HAND_ARMOR},
        {"TYPE_CONSUMEABLE",    ItemType::TYPE_CONSUMEABLE},
        {"TYPE_RESOURCE",    ItemType::TYPE_RESOURCE},
        {"TYPE_ALL",    ItemType::TYPE_ALL},
        {"TYPE_PLAN",    ItemType::TYPE_PLAN},
        {"TYPE_SCROLL",    ItemType::TYPE_SCROLL},
        {"TYPE_FOOD",    ItemType::TYPE_FOOD},
        {"TYPE_ARMOR",    ItemType::TYPE_ARMOR},
        {"TYPE_CHARM",    ItemType::TYPE_CHARM},
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        return it->second;
    }
    return ItemType::TYPE_RESOURCE;  
}




/* SpellID StrToSpellId(const std::string& s) {

    static const std::unordered_map<std::string, SpellID> lookup_table = {
        {"None",                   SpellID::SPELL_ID_NONE},
        {"SPELL_ID_FORCE_1",       SpellID::SPELL_ID_FORCE_1},
        {"SPELL_ID_FORCE_2",       SpellID::SPELL_ID_FORCE_2},
        {"SPELL_ID_FORCE_3",       SpellID::SPELL_ID_FORCE_3},
        {"SPELL_ID_FORCE_4",       SpellID::SPELL_ID_FORCE_4},

        {"SPELL_ID_FIRE_1",        SpellID::SPELL_ID_FIRE_1},
        {"SPELL_ID_FIRE_2",        SpellID::SPELL_ID_FIRE_2},
        {"SPELL_ID_FIRE_3",        SpellID::SPELL_ID_FIRE_3},
        {"SPELL_ID_FIRE_4",        SpellID::SPELL_ID_FIRE_4},


        {"SPELL_ID_LIGHTNING_1",   SpellID::SPELL_ID_LIGHTNING_1},
        {"SPELL_ID_LIGHTNING_2",   SpellID::SPELL_ID_LIGHTNING_2},
        {"SPELL_ID_LIGHTNING_3",   SpellID::SPELL_ID_LIGHTNING_3},
        {"SPELL_ID_LIGHTNING_4",   SpellID::SPELL_ID_LIGHTNING_4},

        {"SPELL_ID_POISON_1",      SpellID::SPELL_ID_POISON_1},
        {"SPELL_ID_POISON_2",      SpellID::SPELL_ID_POISON_2},
        {"SPELL_ID_POISON_3",      SpellID::SPELL_ID_POISON_3},
        {"SPELL_ID_POISON_4",      SpellID::SPELL_ID_POISON_4},
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        //TraceLog(LOG_INFO, "Spell ID found %i", it->second);
        return it->second;
    }
    //TraceLog(LOG_INFO, "Spell ID not found ");
    return SpellID::SPELL_ID_NONE;
}
 */

CASTER_PART StrToCasterPart(const std::string& s) {

    static const std::unordered_map<std::string, CASTER_PART> lookup_table = {
        {"CASTER_PART_BASE",             CASTER_PART::CASTER_PART_BASE},
        {"CASTER_PART_COPLER",           CASTER_PART::CASTER_PART_COPLER},
        {"CASTER_PART_IGNITER",          CASTER_PART::CASTER_PART_IGNITER},
        {"CASTER_PART_ROD",              CASTER_PART::CASTER_PART_ROD},
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        return it->second;
    }
    TraceLog(LOG_INFO, "caster part ID not found ");
    return CASTER_PART::CASTER_PART_NONE; 
}


SPELL_EFFECT StrToSpellEffectId(const std::string& s) {

    static const std::unordered_map<std::string, SPELL_EFFECT> lookup_table = {
        {"SPELL_EFFECT_FORCE",              SPELL_EFFECT::SPELL_EFFECT_FORCE},
        {"SPELL_EFFECT_FIRE",               SPELL_EFFECT::SPELL_EFFECT_FIRE},
        {"SPELL_EFFECT_LIGHTNING",          SPELL_EFFECT::SPELL_EFFECT_LIGHTNING},
        {"SPELL_EFFECT_POISON",             SPELL_EFFECT::SPELL_EFFECT_POISON},
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        return it->second;
    }
    TraceLog(LOG_INFO, "spell effect ID not found ");
    return SPELL_EFFECT::SPELL_EFFECT_NONE;
}


SPELL_TYPE StrToSpellTypeId(const std::string& s) {

    static const std::unordered_map<std::string, SPELL_TYPE> lookup_table = {
        {"SPELL_TYPE_BOLT",             SPELL_TYPE::SPELL_TYPE_BOLT},
        {"SPELL_TYPE_AOE",              SPELL_TYPE::SPELL_TYPE_AOE},
        {"SPELL_TYPE_INSTANT",          SPELL_TYPE::SPELL_TYPE_INSTANT},
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        return it->second;
    }
    TraceLog(LOG_INFO, "spell type ID not found ");
    return SPELL_TYPE::SPELL_TYPE_NONE;
}


WEAPON_TYPE StrToWeaponTypeId(const std::string& s) {

    static const std::unordered_map<std::string, WEAPON_TYPE> lookup_table = {
        {"WEAPON_TYPE_MELE",                                WEAPON_TYPE::WEAPON_TYPE_MELE},
        {"WEAPON_TYPE_RANGED",                              WEAPON_TYPE::WEAPON_TYPE_RANGED},
        {"WEAPON_TYPE_CASTER",                              WEAPON_TYPE::WEAPON_TYPE_CASTER}
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        return it->second;
    }
    TraceLog(LOG_INFO, "WEAPON_TYPE ID not found ");
    return WEAPON_TYPE::WEAPON_TYPE_NONE; 
}


CreatureID StrToCreatureId(const std::string& s) {

    static const std::unordered_map<std::string, CreatureID> lookup_table = {
        {"CREATURE_TESTDUMMY",                  CreatureID::CREATURE_TESTDUMMY},
        {"CREATURE_BUNNY",                      CreatureID::CREATURE_BUNNY},
        {"CREATURE_SCAVENGER",                  CreatureID::CREATURE_SCAVENGER},
        {"CREATURE_TRADER",                     CreatureID::CREATURE_TRADER},
        {"CREATURE_SLIME",                      CreatureID::CREATURE_SLIME},
        {"CREATURE_NPC_1",                      CreatureID::CREATURE_NPC_1},
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        return it->second;
    }
    TraceLog(LOG_INFO, "creature ID not found ");
    return CreatureID::CREATURE_NONE;
}


ItemID StrToItemId(const std::string& s) {

    static const std::unordered_map<std::string, ItemID> lookup_table = {
        {"None",                    ItemID::ITEM_ID_NONE},
        {"ITEM_ID_DAGGER",          ItemID::ITEM_ID_DAGGER},
        {"ITEM_ID_SWORD",           ItemID::ITEM_ID_SWORD},
        {"ITEM_ID_SPEAR",           ItemID::ITEM_ID_SPEAR},
        {"ITEM_ID_AXE",             ItemID::ITEM_ID_AXE},
        {"ITEM_ID_BOW",             ItemID::ITEM_ID_BOW},

        {"ITEM_ID_CASTERBASE",             ItemID::ITEM_ID_CASTERBASE},

        {"ITEM_ID_IGNITER_1",             ItemID::ITEM_ID_IGNITER_1},
        {"ITEM_ID_IGNITER_2",             ItemID::ITEM_ID_IGNITER_2},
        {"ITEM_ID_IGNITER_3",             ItemID::ITEM_ID_IGNITER_3},
        {"ITEM_ID_IGNITER_4",             ItemID::ITEM_ID_IGNITER_4},

        {"ITEM_ID_COUPLER_1",             ItemID::ITEM_ID_COUPLER_1},
        {"ITEM_ID_COUPLER_2",             ItemID::ITEM_ID_COUPLER_2},
        {"ITEM_ID_COUPLER_3",             ItemID::ITEM_ID_COUPLER_3},
        {"ITEM_ID_COUPLER_4",             ItemID::ITEM_ID_COUPLER_4},

        {"ITEM_ID_ROD_1",             ItemID::ITEM_ID_ROD_1},
        {"ITEM_ID_ROD_2",             ItemID::ITEM_ID_ROD_2},
        {"ITEM_ID_ROD_3",             ItemID::ITEM_ID_ROD_3},
        {"ITEM_ID_ROD_4",             ItemID::ITEM_ID_ROD_4},


        
        {"ITEM_ID_MUSHROOM",               ItemID::ITEM_ID_MUSHROOM},
        {"ITEM_ID_MUSHROOM_JUICE",         ItemID::ITEM_ID_MUSHROOM_JUICE},

        {"ITEM_ID_HELMET",                 ItemID::ITEM_ID_HELMET},
        {"ITEM_ID_BOOTS",                  ItemID::ITEM_ID_BOOTS},
        {"ITEM_ID_BODY",                   ItemID::ITEM_ID_BODY},
        {"ITEM_ID_GLOVES",                 ItemID::ITEM_ID_GLOVES},
        {"ITEM_ID_LEGGINGS",               ItemID::ITEM_ID_LEGGINGS},

        {"ITEM_ID_APPLE",                  ItemID::ITEM_ID_APPLE},
        {"ITEM_ID_CHEESE",                 ItemID::ITEM_ID_CHEESE},
        {"ITEM_ID_BREAD",                  ItemID::ITEM_ID_BREAD},
        {"ITEM_ID_MEAT",                   ItemID::ITEM_ID_MEAT},

        {"ITEM_ID_RING",                   ItemID::ITEM_ID_RING},

        {"ITEM_ID_STOVE_PLAN",             ItemID::ITEM_ID_STOVE_PLAN},
        {"ITEM_ID_MUSHROOMPRESS_PLAN",     ItemID::ITEM_ID_MUSHROOMPRESS_PLAN},

        {"ITEM_ID_HAMMER",                 ItemID::ITEM_ID_HAMMER},
        {"ITEM_ID_SAW",                    ItemID::ITEM_ID_SAW},
        {"ITEM_ID_SHOVEL",                 ItemID::ITEM_ID_SHOVEL},
        {"ITEM_ID_TONGS",                  ItemID::ITEM_ID_TONGS},
        {"ITEM_ID_ANVIL",                  ItemID::ITEM_ID_ANVIL},

        {"ITEM_ID_CHARCOAL",               ItemID::ITEM_ID_CHARCOAL},
        {"ITEM_ID_RESIN",                  ItemID::ITEM_ID_RESIN},
        {"ITEM_ID_BONE",                   ItemID::ITEM_ID_BONE},
        {"ITEM_ID_ROPE",                   ItemID::ITEM_ID_ROPE},
        {"ITEM_ID_BUCKET",                 ItemID::ITEM_ID_BUCKET},
    };


    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        return it->second;
    }
    TraceLog(LOG_INFO, "Item ID not found ");
    return ItemID::ITEM_ID_NONE;
}

EnvironmentSpriteID StrToEnviroSpriteId(const std::string& s) {

    static const std::unordered_map<std::string, EnvironmentSpriteID> lookup_table = {
        {"Tree1",                        EnvironmentSpriteID::SPRITE_ENVIRO_TREE1},
        {"Tree2",                        EnvironmentSpriteID::SPRITE_ENVIRO_TREE2},
        {"Grass1",                       EnvironmentSpriteID::SPRITE_ENVIRO_GRASS1},
        {"Grass2",                       EnvironmentSpriteID::SPRITE_ENVIRO_GRASS2},
        //{"RUINS_ENTITY_1",                EnvironmentSpriteID::SPRITE_ENVIRO_DECCO_1},
        {"SPAWN_ENTITY_1",               EnvironmentSpriteID::SPRITE_ENVIRO_DECCO_2},
        {"EXIT_ENTITY_1",                EnvironmentSpriteID::SPRITE_ENVIRO_DECCO_1},        
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        //TraceLog(LOG_INFO, "enviro ID found %i", it->second);
        return it->second;
    }
    TraceLog(LOG_INFO, "Decco ID not found ");
    return EnvironmentSpriteID::SPRITE_ENVIRO_ERROR;
}

std::string EnvironmentalIdToStr(const int id) {

    static const std::unordered_map<int , std::string> lookup_table = {
        {EnvironmentSpriteID::SPRITE_ENVIRO_TREE1,     "Tree1"},
        {EnvironmentSpriteID::SPRITE_ENVIRO_TREE2,     "Tree2"},
        {EnvironmentSpriteID::SPRITE_ENVIRO_GRASS1,     "Grass1"},
        {EnvironmentSpriteID::SPRITE_ENVIRO_GRASS2,     "Grass2"},
        {EnvironmentSpriteID::SPRITE_ENVIRO_DECCO_1,     "DeccoEntity_1"},
        {EnvironmentSpriteID::SPRITE_ENVIRO_DECCO_2,     "DeccoEntity_2"},
    };
    if (auto it = lookup_table.find(id); it != lookup_table.end()) {
        return it->second;
    }
    return "";
}

AIID StrToAiId(const std::string& s) {
    static const std::unordered_map<std::string, AIID> lookup_table = {
        {"AI_FEARFULL",                        AIID::AI_FEARFULL},
        {"AI_PASSIVE",                        AIID::AI_PASSIVE},
        {"AI_TERRITORIAL",                        AIID::AI_TERRITORIAL},
        {"AI_AGGRESSIVE",                        AIID::AI_AGGRESSIVE},
        {"AI_HOSTILE",                        AIID::AI_HOSTILE},
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        return it->second;
    }
    return AIID::AI_PASSIVE;
}

std::string ModuleIdToStr(const int id) {

    static const std::unordered_map<int , std::string> lookup_table = {
        {ModuleID::MODULE_ID_WORKBENCH,     "WORKBENCH"},
         {ModuleID::MODULE_ID_STOVE,     "STOVE"},
          {ModuleID::MODULE_ID_MUSHROOMPRESS,     "MUSHROOMPRESS"},
    };
    if (auto it = lookup_table.find(id); it != lookup_table.end()) {
        return it->second;
    }
    return "";
}


std::string ItemTypeToStr(const int id) {
    static const std::unordered_map<ItemType , std::string> lookup_table = {
        {ItemType::TYPE_ARMOR,          "Armor"},
        {ItemType::TYPE_HEAD_ARMOR,     "Head Armor"},
        {ItemType::TYPE_BODY_ARMOR,     "Body Armor"},
        {ItemType::TYPE_LEG_ARMOR,      "Leg Armor"},
        {ItemType::TYPE_FEET_ARMOR,     "Foot Armor"},
        {ItemType::TYPE_HAND_ARMOR,     "Hand Armor"},
        {ItemType::TYPE_CONSUMEABLE,    "Consumeable"},
        {ItemType::TYPE_RESOURCE,       "Resource"},
        {ItemType::TYPE_PLAN,           "Plan"},
        {ItemType::TYPE_SCROLL,         "Scroll"},
        {ItemType::TYPE_FOOD,            "Food"},
        {ItemType::TYPE_CHARM,          "Charm"},
        {ItemType::TYPE_WEAPON,         "Weapon"},
        {ItemType::TYPE_ALL,            "All"},
    };

    if (auto it = lookup_table.find((ItemType)id); it != lookup_table.end()) {
        //TraceLog(LOG_INFO, "type  %s", it->second.c_str());
        return it->second;
    }
    return "";
}


SpriteSheetID StrToSpriteId(const std::string& s) {

    static const std::unordered_map<std::string, SpriteSheetID> lookup_table = {
        {"SPRITE_APPRENTICE",                        SpriteSheetID::SPRITE_APPRENTICE},
        {"SPRITE_NERD",                        SpriteSheetID::SPRITE_NERD},
        {"SPRITE_TESTDUMMY",                        SpriteSheetID::SPRITE_TESTDUMMY},
        {"SPRITE_BUNNY",                        SpriteSheetID::SPRITE_BUNNY},
        {"SPRITE_SCAVENGER",                        SpriteSheetID::SPRITE_SCAVENGER},
        {"SPRITE_TRADER",                        SpriteSheetID::SPRITE_TRADER},
        {"SPRITE_SLIME",                        SpriteSheetID::SPRITE_SLIME},
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        //TraceLog(LOG_INFO, "enviro ID found %i", it->second);
        return it->second;
    }
    //TraceLog(LOG_INFO, "Spell ID not found ");
    return SpriteSheetID::SPRITE_NONE;

}

void from_json(const json &j, ItemInstanceData &i) {
    j.at("item_id").get_to(i.item_id);
    j.at("instance_id").get_to(i.instance_id);
    j.at("value").get_to(i.value);
    j.at("type").get_to(i.type);
    j.at("item_name").get_to(i.item_name);
    j.at("container_id").get_to(i.container_id);
    j.at("item_id").get_to(i.item_id);
    j.at("level").get_to(i.level);
    j.at("sprite_id").get_to(i.sprite_id);
    j.at("rarity").get_to(i.rarity);
    j.at("mod_slots").get_to(i.mod_slots);

    j.at("weapon_id").get_to(i.weapon_id);     
    WeaponData new_weapon = {};
    if(i.weapon_id != ITEM_ID_NONE) {
        new_weapon.weapon_id = j["weapon_data"]["weapon_id"];
        new_weapon.weapon_name = j["weapon_data"]["weapon_name"];        
        new_weapon.cooldown = j["weapon_data"]["cooldown"];        
        new_weapon.damage = j["weapon_data"]["damage"];
        new_weapon.recoil = j["weapon_data"]["recoil"];        
        new_weapon.knockback = j["weapon_data"]["knockback"];
        new_weapon.mod_slots = j["weapon_data"]["mod_slots"];
        new_weapon.weapon_type = j["weapon_data"]["weapon_type"];
        new_weapon.max_power = j["weapon_data"]["max_power"];
        new_weapon.current_power = j["weapon_data"]["current_power"];

        if(new_weapon.weapon_type == WEAPON_TYPE_CASTER) {
            CasterData caster;

            CasterBaseData base;
            base.name = j["weapon_data"]["caster_data"]["base"]["part_name"];
            base.id = j["weapon_data"]["caster_data"]["base"]["part_id"];
            base.accuracy = j["weapon_data"]["caster_data"]["base"]["accuracy"];

            IgniterData igniter;
            igniter.name = j["weapon_data"]["caster_data"]["igniter"]["part_name"];
            igniter.id = j["weapon_data"]["caster_data"]["igniter"]["part_id"];
            igniter.effect = j["weapon_data"]["caster_data"]["igniter"]["spell_effect"];
            igniter.recoil = j["weapon_data"]["caster_data"]["igniter"]["recoil"];
            igniter.max_power = j["weapon_data"]["caster_data"]["igniter"]["max_power"];

            CouplerData coupler;
            coupler.name = j["weapon_data"]["caster_data"]["coupler"]["part_name"];
            coupler.id = j["weapon_data"]["caster_data"]["coupler"]["part_name"];
            coupler.cooldown = j["weapon_data"]["caster_data"]["coupler"]["cooldown"];
            coupler.damage = j["weapon_data"]["caster_data"]["coupler"]["damage"];
            coupler.pps = j["weapon_data"]["caster_data"]["coupler"]["pps"];
            coupler.radius = j["weapon_data"]["caster_data"]["coupler"]["radius"];
            coupler.speed = j["weapon_data"]["caster_data"]["coupler"]["speed"];
            coupler.type = j["weapon_data"]["caster_data"]["coupler"]["spell_type"];

            RodData rod;
            rod.name = j["weapon_data"]["caster_data"]["rod"]["part_name"];
            rod.id = j["weapon_data"]["caster_data"]["rod"]["part_id"];
            rod.knockback = j["weapon_data"]["caster_data"]["rod"]["knockback"];
            rod.damage = j["weapon_data"]["caster_data"]["rod"]["damage"];

            caster.base = base;
            caster.igniter = igniter;
            caster.coupler = coupler;
            caster.rod = rod;

            new_weapon.caster_data = caster;
        }


        i.weapon_data = new_weapon;
        TraceLog(LOG_INFO, "loaded weapon data found for %s", i.item_name.c_str());
    } 
            
   j.at("armor_id").get_to(i.armor_id);
    ArmorData new_armor = {};
    if(i.armor_id != ITEM_ID_NONE) {
        new_armor.armor_id = j["armor_data"]["armor_id"];
        new_armor.armor_name = j["armor_data"]["armor_name"];
        new_armor.defence = j["armor_data"]["defence"];
        new_armor.magic_defence = j["armor_data"]["magic_defence"];
        //new_armor.spell_id = j["armor_data"]["spell_id"];
        new_armor.mod_slots = j["armor_data"]["mod_slots"];
        i.armor_data = new_armor;
        TraceLog(LOG_INFO, "loaded armor data found for %s", i.item_name.c_str());
    }


    j.at("food_id").get_to(i.food_id);
    FoodData food_data = {};
    if(i.food_id != ITEM_ID_NONE) {
        food_data.food_name = j["food_data"]["food_name"];
        food_data.food_id = j["food_data"]["food_id"];
        food_data.saturation= j["food_data"]["saturation"];
        i.food_data = food_data; 
        TraceLog(LOG_INFO, "loaded food data found for %s", i.item_name.c_str());
    } 

    TraceLog(LOG_INFO, "Item loaded  %i", i.instance_id);
}


void from_json(const json &j, ContainerData &i) {

    j.at("identifier").get_to(i.identifier);
    j.at("iid").get_to(i.iid);
    j.at("loot_table_id").get_to(i.loot_table_id);
    j.at("sprite_id").get_to(i.sprite_id);
    j.at("position_x").get_to(i.position_i.x);
    j.at("position_y").get_to(i.position_i.y);
    j.at("size_x").get_to(i.size.x);
    j.at("size_y").get_to(i.size.y);
    j.at("level_index").get_to(i.level_index);

    //TraceLog(LOG_INFO, "container loaded  %s", i.iid.c_str());
}

void YSortEntities(LevelData & _level_data) {
    _level_data.draw_list.clear();

    //_level_data.draw_list.push_back(g_current_player);

    _level_data.draw_list.push_back(g_current_player.get());

    for (auto &e : _level_data.environment_entities) {
        if(e->is_on_screen) {
            _level_data.draw_list.push_back(e.get());
        }
    }

    for (auto &e : _level_data.entity_list) {
        if(e->is_on_screen) {
            _level_data.draw_list.push_back(e.get());
        }
    }

    for (auto &e : _level_data.spell_list){
        if(e->is_on_screen) {
            _level_data.draw_list.push_back(e.get());
        }
    }

    std::sort(_level_data.draw_list.begin(), _level_data.draw_list.end(),
    [](BaseEntity* a, BaseEntity* b) {
        return a->GetYSort() < b->GetYSort();
    });
    //TraceLog(LOG_INFO, "sorted entities  %i", _level_data.draw_list.size());
}