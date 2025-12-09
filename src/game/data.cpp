#include "../core/gamedefs.h"
#include "../core/utils.h"

static const std::string save_path = "saves/save.json";


void LoadGameData() {
    TraceLog(LOG_INFO, "LOADING GAME DATA....data.json");


    g_item_type_colors[TYPE_WEAPON] = WEAPONCOLOR;
    g_item_type_colors[TYPE_HEAD_ARMOR] = ARMORCOLOR;
    g_item_type_colors[TYPE_BODY_ARMOR] = ARMORCOLOR;
    g_item_type_colors[TYPE_LEG_ARMOR] = ARMORCOLOR;
    g_item_type_colors[TYPE_FEET_ARMOR] = ARMORCOLOR;
    g_item_type_colors[TYPE_HAND_ARMOR] = ARMORCOLOR;
    g_item_type_colors[TYPE_CONSUMEABLE] = DEFAULTITEMCOLOR;
    g_item_type_colors[TYPE_RESOURCE] = DEFAULTITEMCOLOR;
    g_item_type_colors[TYPE_PLAN] = PLANCOLOR;
    g_item_type_colors[TYPE_SCROLL] = SCROLLCOLOR;
    g_item_type_colors[TYPE_FOOD] = FOODCOLOR;
    g_item_type_colors[TYPE_ALL] = DEFAULTITEMCOLOR;



    std::ifstream cfile("assets/data.json");
    if (!cfile.is_open()) {
        TraceLog(LOG_INFO, "CANNOT OPEN DATA FILE");
        return;
    }
    
    json cj;
    cfile>>cj;

    for(int i = 0; i < cj["base_class"].size(); i++) {
        int health = cj["base_class"][i]["health"];
        int exp = 0;
        float base_speed = cj["base_class"][i]["base_speed"];
        int sprite_sheet_id = cj["base_class"][i]["sprite_sheet_id"];
        int portrait_id = cj["base_class"][i]["portrait_id"];
        std::string class_name = cj["base_class"][i]["class_name"];
        
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


        PlayerData this_class = {
            .health = health,
            .exp = exp,
            .base_speed = base_speed,
            .sprite_sheet_id = sprite_sheet_id,
            .portrait_id = portrait_id,
            .name = "not assigned",
            .class_name = class_name,
            .inventory = inv,
            .hotbar = hot,

            .primary = p,
            .secondary = s,
            .head = hd,
            .body = bd,
            .legs = lg,
            .feet = ft,
            .hands = hs
        };

        g_class_data[i] = this_class;
    }

//------------------item data
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
            //.spell_id = spell_id
            
        };

        TraceLog(LOG_INFO, "Item Data Loaded  id: %i  %s", id, name.c_str());
        g_item_data[(int)id] = new_item;
    }

//---------------------spell data
    for(int i = 0; i < cj["spell_data"].size(); i++) {
        SpellData new_spell;

        std::string sp_id = cj["spell_data"][i]["spell_id"];
        SpellID spell_id = StrToSpellId(sp_id);

        std::string name = cj["spell_data"][i]["spell_name"];

        float lifetime = cj["spell_data"][i]["lifetime"];
        float damage = cj["spell_data"][i]["damage"];
        int speed = cj["spell_data"][i]["speed"];
        int level = cj["spell_data"][i]["level"];
        float radius = cj["spell_data"][i]["radius"];

        new_spell.lifetime = lifetime;
        new_spell.damage = damage;
        new_spell.level = level;
        new_spell.spell_id = spell_id;
        new_spell.radius = radius;
        new_spell.speed = speed;
        new_spell.spell_name = name;
        
        g_spell_data[spell_id] = new_spell;
        TraceLog(LOG_INFO, "Spell Data Loaded  id: %i  %s", spell_id, sp_id.c_str());
    }

//------------------------------------armor data
    for(int i = 0; i < cj["armor_data"].size(); i++) {

        std::string w_id_s = cj["armor_data"][i]["armor_id"];
        ItemID w_id = StrToItemId(w_id_s);

        std::string name = cj["armor_data"][i]["armor_name"];

        std::string sp_id = cj["armor_data"][i]["spell_id"];
        SpellID spell_id = StrToSpellId(sp_id);

        float defence = cj["armor_data"][i]["defence"];

        float magic_defence = cj["armor_data"][i]["magic_defence"];

        ArmorData new_armor = {
            .armor_name= name,
            .armor_id = w_id,
            .defence = defence,
            .magic_defence = magic_defence,               
        };

        TraceLog(LOG_INFO, "Armor Data Loaded  id: %i  %s", w_id, name.c_str());
        g_armor_data[(int)w_id] = new_armor;
    }



//------------------------------------weapon data
    for(int i = 0; i < cj["weapon_data"].size(); i++) {

        std::string w_id_s = cj["weapon_data"][i]["weapon_id"];
        ItemID w_id = StrToItemId(w_id_s);

        std::string name = cj["weapon_data"][i]["weapon_name"];
        float cooldown = cj["weapon_data"][i]["cooldown"];

        SpellID spell_id = SPELL_ID_NONE;

        std::string sp_id = cj["weapon_data"][i]["spell_id"];
        spell_id = StrToSpellId(sp_id);

        int clip_size = cj["weapon_data"][i]["clip_size"];

        int damage = cj["weapon_data"][i]["damage"];

        WeaponData new_weapon = {
            .weapon_name = name,
            .weapon_id = w_id,
            .cooldown = cooldown,
            .spell_id = spell_id,
            .clip_size = clip_size,
            .ammo_count = clip_size,
            .damage = damage                
        };

        TraceLog(LOG_INFO, "Weapon Data Loaded  id: %i  %s", w_id, name.c_str());
        g_weapon_data[(int)w_id] = new_weapon;
    }



//------------------------------------food data
    for(int i = 0; i < cj["food_data"].size(); i++) {

        std::string f_id_s = cj["food_data"][i]["food_id"];
        ItemID f_id = StrToItemId(f_id_s);

        std::string name = cj["food_data"][i]["food_name"];

        std::string sp_id = cj["food_data"][i]["spell_id"];
        SpellID spell_id = StrToSpellId(sp_id);

        float saturation = cj["food_data"][i]["saturation"];

        FoodData new_food = {
            .food_id = f_id,
            .food_name = name,
            .spell_id = spell_id,
            .saturation = saturation

        };

        TraceLog(LOG_INFO, "FOOD Data Loaded  id: %i  %s", f_id, name.c_str());
        g_food_data[(int)f_id] = new_food;
    }


//------------------------------------plan data
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
        g_plan_data[(int)p_id] = new_plan;
    }

//------------------------------------module data
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


    //--------------------loot tables
    for(int i = 0; i < cj["loot_tables"].size(); i++) {
        int id = i;

        std::vector<int> new_table;
        for(int t = 0; t < cj["loot_tables"][i]["table_data"].size(); t++) {
            ItemID _id = StrToItemId( cj["loot_tables"][i]["table_data"][t] );
            new_table.push_back(_id);
        }

        std::string name = cj["loot_tables"][i]["table_name"];

        g_loot_tables.push_back(new_table);
    }

    cfile.close();
 
    //TraceLog(LOG_INFO, "==========check save data================");

    if(std::filesystem::exists(save_path)) {
        g_game_data.save_available = true;
        TraceLog(LOG_INFO, "SAVE FILE FOUND");
    }
}


void SaveGame(LevelData &level_data) {

    TraceLog(LOG_INFO, "----------------------SAVING GAME------------------------" );
    g_game_data.using_saved_data = true;
    std::ofstream file(save_path);
    if (!file.is_open()) {
        TraceLog(LOG_INFO, "filed to open save file");
        return;
    }

    json j;

    j["health"] = g_player_data.health;
    j["exp"] = g_player_data.exp;
    j["base_speed"] = g_player_data.base_speed;
    j["sprite_sheet_id"] = g_player_data.sprite_sheet_id;
    j["portrait_id"] = g_player_data.portrait_id;
    j["name"] = g_player_data.name;
    j["class_name"] = g_player_data.class_name;

    //j["inventory"] = {};
    for(int i = 0; i < g_player_data.inventory.size(); i++) {
        j["inventory"][i] = g_player_data.inventory[i];
    }
    for(int i = 0; i < g_player_data.hotbar.size(); i++) {
        j["hotbar"][i] = g_player_data.hotbar[i];
    }

    for(int i = 0; i < g_player_data.primary.size(); i++) {
        j["primary"][i] = g_player_data.primary[i];
    }
    for(int i = 0; i < g_player_data.secondary.size(); i++) {
        j["secondary"][i] = g_player_data.secondary[i];
    }
    for(int i = 0; i < g_player_data.head.size(); i++) {
        j["head"][i] = g_player_data.head[i];
    }
    for(int i = 0; i < g_player_data.body.size(); i++) {
        j["body"][i] = g_player_data.body[i];
    }
    for(int i = 0; i < g_player_data.legs.size(); i++) {
        j["legs"][i] = g_player_data.legs[i];
    }
    for(int i = 0; i < g_player_data.feet.size(); i++) {
        j["feet"][i] = g_player_data.feet[i];
    }
    for(int i = 0; i < g_player_data.hands.size(); i++) {
        j["hands"][i] = g_player_data.hands[i];
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
            {"spell_id", inst.spell_id},
            {"clip_size", inst.clip_size},
            {"ammo_count", inst.ammo_count},
            {"container_id", inst.container_id},
            {"cooldown", inst.cooldown},
            {"item_id", inst.item_id},
            {"icon_id", inst.icon_id},
            {"level", inst.level},
            {"defence", inst.defence},
            {"magic_defence", inst.magic_defence},
            {"sprite_id", inst.sprite_id},
            {"saturation", inst.saturation},
        };
        json_item_instances.push_back(instance);
        TraceLog(LOG_INFO, "saving item %i   instance id: %i container iid: %s  sub json size: %i  g_instances size %i", inst.item_id, inst.instance_id, inst.container_id.c_str(), json_item_instances.size(), g_item_instances.size());
    }

    TraceLog(LOG_INFO, "saved instances size %i", json_item_instances.size());
    j["item_instances"] = json_item_instances;    


    json json_persistant_containers = json::array();

    for(int entity = 0; entity < level_data.entity_list.size(); entity++) {
        if(level_data.entity_list[entity]->identifier == "PermContainerEntity" or level_data.entity_list[entity]->identifier == "GroundContainerEntity") {

            if (level_data.entity_list[entity]->is_persistant) {
                BaseContainerEntity* c_entity = dynamic_cast<BaseContainerEntity*>(level_data.entity_list[entity]);
                
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

    //save map file
    std::ifstream src("assets/maps/ldtk/test.ldtk", std::ios::binary);
    std::ofstream dst("saves/saved_map.ldtk", std::ios::binary);

    dst << src.rdbuf();

}


void LoadGame() {
    TraceLog(LOG_INFO, "LOADING SAVED DATA....save.json");

    g_game_data.using_saved_data = true;

    std::ifstream file(save_path);
    if (!file.is_open()) {
        TraceLog(LOG_INFO, "CANNOT OPEN FILE");
        return;
    }
    
    json j;
    file>>j;

    g_player_data.health = j["health"];
    g_player_data.exp = j["exp"];
    g_player_data.base_speed = j["base_speed"];
    g_player_data.sprite_sheet_id = j["sprite_sheet_id"];
    g_player_data.portrait_id = j["portrait_id"];
    g_player_data.name = j["name"];
    g_player_data.class_name = j["class_name"];

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



    g_player_data.inventory = inv;
    g_player_data.hotbar = hot;

    g_player_data.primary = p;
    g_player_data.secondary = s;
    g_player_data.head = hd;
    g_player_data.body = bd;
    g_player_data.legs = lg;
    g_player_data.feet = ft;
    g_player_data.hands = hs;


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

    std::string ldtk_map_path = "saves/saved_map.ldtk";
    int num_maps = load_ldtk_maps(ldtk_map_path);

    TraceLog(LOG_INFO, "==========END LOADING LDTK MAPS================  loaded %i maps", num_maps);

}



void ClearLevelData(LevelData &level_data) {

    DL_Clear(level_data.entity_list);
    level_data.level_transitions.clear();
    level_data.container_data.clear();
    level_data.game_areas.clear();

}


void ClearSubLevelData() {
    for (auto& [key, value] : g_sub_scene_data) {
        DL_Clear(g_sub_scene_data[key]->entity_list);
        g_sub_scene_data[key]->level_transitions.clear();
        g_sub_scene_data[key]->container_data.clear();
        g_sub_scene_data[key]->game_areas.clear();
    }

    g_sub_scene_data.clear();
}


void LoadLevelData(LevelData &level_data) {


    PrecalculateTileCollisionData(level_data);
    PrecalculateShadowData(level_data);

    int map_index = g_game_data.current_map_index;
    
    if(g_game_data.is_in_sub_map) {
        map_index = g_game_data.sub_map_index;
    }

    LDTKLevel this_level = g_ldtk_maps.levels[map_index];

    TraceLog(LOG_INFO, "LOADING LEVEL DATA %i  %s", map_index, this_level.identifier.c_str());

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
                    TraceLog(LOG_INFO, "SPAWN POINT FOUND");
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
                    //TraceLog(LOG_INFO, "TRANSITION dest string ADDED, %s", new_transition.dest_string.c_str());
                }
                if(identifier == "PermContainerEntity"){

                    if(!g_game_data.using_saved_data or g_game_data.current_map_index != g_game_data.shelter_map_index ) {

                        
                        ContainerData new_container;
                        TraceLog(LOG_INFO, "CONTAINER FOUND %s", identifier.c_str());
                        
                        new_container.size = {(float)this_level.layer_instances[layer_index].entity_instances[entity_index].width, (float)this_level.layer_instances[layer_index].entity_instances[entity_index].height};
                        new_container.identifier = this_level.layer_instances[layer_index].entity_instances[entity_index].identifier;
                        new_container.position_i.x = this_level.layer_instances[layer_index].entity_instances[entity_index].px[0];
                        new_container.position_i.y = this_level.layer_instances[layer_index].entity_instances[entity_index].px[1];
                        new_container.position_f.x = (float)this_level.layer_instances[layer_index].entity_instances[entity_index].px[0] * tile_size;
                        new_container.position_f.y = (float)this_level.layer_instances[layer_index].entity_instances[entity_index].px[1] * tile_size;
                        new_container.iid = this_level.layer_instances[layer_index].entity_instances[entity_index].iid;
                        
                        new_container.sprite_id = this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[0].value_i;
                        new_container.loot_level = this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[1].value_i;

                        for(int t = 0; t < this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[2].i_list.size(); t++) {
                            int lt_id = this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[2].i_list[t];
                            for(int loot = 0; loot < g_loot_tables[lt_id].size(); loot++) {
                                new_container.item_list.push_back(g_loot_tables[lt_id][loot]);
                                TraceLog(LOG_INFO, "item added to container %i", g_loot_tables[lt_id][loot]);
                            }
                        }
                        //TraceLog(LOG_INFO, "PERM CONTAINER DATA ADDED IID %s", new_container.iid.c_str());
                        level_data.container_data.push_back(new_container);
                    }
                    //TraceLog(LOG_INFO, "CONTAINER ADDED WITH LT %i SID %i", new_container.loot_table_id, new_container.sprite_id);
                }

                if(identifier == "GroundContainerEntity" and !g_game_data.using_saved_data) {
                    if(!g_game_data.using_saved_data or g_game_data.current_map_index != g_game_data.shelter_map_index ) {
                        ContainerData new_container;
                        //TraceLog(LOG_INFO, "GROUND CONTAINER FOUND %s %i", identifier.c_str(), this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[0].i_list.size());
                        new_container.size = {(float)this_level.layer_instances[layer_index].entity_instances[entity_index].width, (float)this_level.layer_instances[layer_index].entity_instances[entity_index].height};
                        new_container.identifier = this_level.layer_instances[layer_index].entity_instances[entity_index].identifier;
                        new_container.position_i.x = this_level.layer_instances[layer_index].entity_instances[entity_index].px[0];
                        new_container.position_i.y = this_level.layer_instances[layer_index].entity_instances[entity_index].px[1];
                        new_container.position_f.x = (float)this_level.layer_instances[layer_index].entity_instances[entity_index].px[0] * tile_size;
                        new_container.position_f.y = (float)this_level.layer_instances[layer_index].entity_instances[entity_index].px[1] * tile_size;
                        new_container.iid = this_level.layer_instances[layer_index].entity_instances[entity_index].iid;
                        
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
       
            }
        }
    }


    for(int thing = 0; thing < this_level.environment_data.size(); thing++) {
        //TraceLog(LOG_INFO, "-----environment sprite %s %0.0f %0.0f", this_level.environment_data[thing].item_string.c_str(), this_level.environment_data[thing].position.x, this_level.environment_data[thing].position.y);
        int id = StrToEnviroSpriteId(this_level.environment_data[thing].item_string);

        
        if(id < SPRITE_ENVIRO_GRASS1) {
            EnvironmentalEntity *new_entity = new EnvironmentalEntity(this_level.environment_data[thing].position, id, true);
            level_data.environment_entities.push_back(new_entity);
        }
        else {
            EnvironmentalEntity *new_entity = new EnvironmentalEntity(this_level.environment_data[thing].position, id, false);
            level_data.environment_entities.push_back(new_entity);
        }
        //TraceLog(LOG_INFO, "-----\n");
    }

    TraceLog(LOG_INFO, "PERSISTANT CONTAINER LIST SIZE %i", g_persistant_containers.size());
    for(const auto & container : g_persistant_containers) {
        TraceLog(LOG_INFO, "LOOKING FOR SAVED SAVED CONTAINER DATA %s", container.second.identifier.c_str());
         if(container.second.level_index == g_game_data.current_map_index) {
            TraceLog(LOG_INFO, "SAVED CONTAINER DATA FOUND %s", container.second.iid.c_str());
            ContainerData new_container = container.second;

            level_data.container_data.push_back(new_container);
        }
    }
    TraceLog(LOG_INFO, "END LOAD LEVEL DATA ");
}



void PrecalculateTileCollisionData(LevelData &level_data) {
    TraceLog(LOG_INFO, " PRECALCULATING TILE COLLISION DATA ");

    level_data.precalc.map_index = g_game_data.current_map_index;

    if(g_game_data.is_in_sub_map) {
        level_data.precalc.map_index = g_game_data.sub_map_index;
        //TraceLog(LOG_INFO, "            map index sub map -- %i",level_data.precalc.map_index  );
    }
    //TraceLog(LOG_INFO, "            map index -- %i",level_data.precalc.map_index  );

    LDTKLevel this_level = g_ldtk_maps.levels[level_data.precalc.map_index];
    LDTKLayerInstance *col_layer = nullptr;

    for (int l = 0; l < this_level.layer_instances.size(); l++) {
        if(this_level.layer_instances[l].type == "IntGrid") {
            level_data.precalc.collision_layer_index = l;
            //TraceLog(LOG_INFO, "            collision layer -- %i",level_data.precalc.collision_layer_index  );
            col_layer = &this_level.layer_instances[l];
        }
        if(this_level.layer_instances[l].identifier == "Foreground") {
            level_data.precalc.foreground_layer_index = l;
        }
    }

    if(col_layer == nullptr) {
        TraceLog(LOG_INFO, "no collision layer in map data");
        //level_data.tile_precalc.collision_layer_index = -1;
    }

    level_data.precalc.tile_size = col_layer->grid_size;
    //TraceLog(LOG_INFO, "            tile size -- %i",level_data.precalc.tile_size  );

    level_data.precalc.inv_tile_size = 1/(float)level_data.precalc.tile_size;
    //TraceLog(LOG_INFO, "            inv tilesize -- %i",level_data.precalc.collision_layer_index  );

    level_data.precalc.map_width = col_layer->c_wid;
    //TraceLog(LOG_INFO, "            collision layer -- %i",level_data.precalc.collision_layer_index  );

    TraceLog(LOG_INFO, "FINISHED PRECALCULATING TILE COLLISION DATA ");
}


void PrecalculateShadowData(LevelData &level_data) {
    TraceLog(LOG_INFO, "CALCULATING SHADOW DATA ");

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
    TraceLog(LOG_INFO, "FINISHED CALCULATING SHADOW DATA  # polygons %i", level_data.collision_polys.size());
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

    //TraceLog(LOG_INFO, "instancing item list   size: %i container iid  %s ", source_list.size(), container_id.c_str());

    for(int item = 0; item < source_list.size(); item++) {
        int uid = GetRandomValue(1000, 1000000000);

        g_item_instances[uid] = GenerateRandomItem((ItemID) source_list[item], uid, container_id, loot_level);
        dest_list.push_back(uid);
    }
}


void InstancePlayerItem(ItemID item_id) {

    //TraceLog(LOG_INFO, "instancing item list   size: %i container iid  %s ", source_list.size(), container_id.c_str());
    
    int uid = GetRandomValue(1000, 1000000000);

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
        new_instance.container_id = "";
        new_instance.sprite_id = item_id;
        new_instance.icon_id = item_id;
            
        g_item_instances[uid] = new_instance;

        int found_spot = -1;
        for(int slot = 0; slot < g_player_data.inventory.size(); slot++) {
            if(g_player_data.inventory[slot] == -1) {
                found_spot = slot;
                break;
            }
        }

        if(found_spot != -1) {
            g_player_data.inventory[found_spot] = uid;
            //TraceLog(LOG_INFO, "item id %i  item uid %i  spot %i",item_id, uid, found_spot);
        }
        else {
            g_player_data.inventory.push_back(uid);
            //TraceLog(LOG_INFO, "item id %i  item uid %i  adding to end",item_id, uid);
        }

        //dest_list.push_back(uid);
        
    }
}

void GenerateContainerItemList(int lti, std::vector<int> &list) {

    //TraceLog(LOG_INFO, "GENERATING ITEM INSTANCES ");

    int max = g_loot_tables[lti].size();

    for(int i = 0; i < max; i++) {        

        int uid = GetRandomValue(1000, 100000000);

        ItemInstanceData new_instance;
        new_instance.instance_id = uid;
        new_instance.item_id = (ItemID)g_loot_tables[lti][i];

        auto item_it = g_item_data.find(g_loot_tables[lti][i]);
        if(item_it != g_item_data.end()) {

            new_instance.item_name = item_it->second.item_name; 
            new_instance.type = item_it->second.type;
            new_instance.value = item_it->second.value;
            new_instance.clip_size = 0;
            new_instance.ammo_count = 0;
            new_instance.spell_id = item_it->second.spell_id;
            
            g_item_instances[uid] = new_instance;
            //TraceLog(LOG_INFO, "----item uid %i   g_item_instance size %i", uid, g_item_instances.size());
            list.push_back(uid);
        }
 
    }
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



/* ItemTypeColors IdToItemTypeColor(const std::string& s) {
    static const std::unordered_map<std::string, ItemTypeColors> lookup_table = {
        {"TYPE_COLOR_ARMOR",      ItemTypeColors::TYPE_COLOR_ARMOR},
        {"TYPE_COLOR_DEFAULT",    ItemTypeColors::TYPE_COLOR_DEFAULT},
        {"TYPE_COLOR_PLAN",       ItemTypeColors::TYPE_COLOR_PLAN},
        {"TYPE_COLOR_SCROLL",     ItemTypeColors::TYPE_COLOR_SCROLL},
        {"TYPE_COLOR_WEAPON",     ItemTypeColors::TYPE_COLOR_WEAPON}

    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        //TraceLog(LOG_INFO, "Spell ID found %i", it->second);
        return it->second;
    }
    //TraceLog(LOG_INFO, "Spell ID not found ");
    return ItemTypeColors::TYPE_COLOR_DEFAULT;
} */


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
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        return it->second;
    }
    return ItemType::TYPE_RESOURCE;  
}

SpellID StrToSpellId(const std::string& s) {

    static const std::unordered_map<std::string, SpellID> lookup_table = {
        {"None",                          SpellID::SPELL_ID_NONE},
        {"SPELL_ID_MAGICMISSLE",        SpellID::SPELL_ID_MAGICMISSLE},
        {"SPELL_ID_FIREBALL",           SpellID::SPELL_ID_FIREBALL},
        {"SPELL_ID_LIGHTNING",          SpellID::SPELL_ID_LIGHTNING},
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        //TraceLog(LOG_INFO, "Spell ID found %i", it->second);
        return it->second;
    }
    //TraceLog(LOG_INFO, "Spell ID not found ");
    return SpellID::SPELL_ID_NONE;

}


ItemID StrToItemId(const std::string& s) {

    static const std::unordered_map<std::string, ItemID> lookup_table = {
        {"None",                    ItemID::ITEM_ID_NONE},
        {"ITEM_ID_DAGGER",          ItemID::ITEM_ID_DAGGER},
        {"ITEM_ID_SWORD",           ItemID::ITEM_ID_SWORD},
        {"ITEM_ID_SPEAR",           ItemID::ITEM_ID_SPEAR},
        {"ITEM_ID_AXE",             ItemID::ITEM_ID_AXE},
        {"ITEM_ID_BOW",             ItemID::ITEM_ID_BOW},
        {"ITEM_ID_WAND",            ItemID::ITEM_ID_WAND},
        {"ITEM_ID_STAFF",           ItemID::ITEM_ID_STAFF},


        {"ITEM_ID_MUSHROOM",           ItemID::ITEM_ID_MUSHROOM},
        {"ITEM_ID_MUSHROOM_JUICE",     ItemID::ITEM_ID_MUSHROOM_JUICE},


        {"ITEM_ID_BOOTS",    ItemID::ITEM_ID_BOOTS},
        {"ITEM_ID_BODY",     ItemID::ITEM_ID_BODY},
        {"ITEM_ID_GLOVES",   ItemID::ITEM_ID_GLOVES},
        {"ITEM_ID_LEGGINGS",   ItemID::ITEM_ID_LEGGINGS},


        {"ITEM_ID_MAGICMMISSLE_SCROLL",          ItemID::ITEM_ID_MAGICMMISSLE_SCROLL},
        {"ITEM_ID_FIREBALL_SCROLL",          ItemID::ITEM_ID_FIREBALL_SCROLL},
        {"ITEM_ID_LIGHTNING_SCROLL",          ItemID::ITEM_ID_LIGHTNING_SCROLL},

        {"ITEM_ID_APPLE",           ItemID::ITEM_ID_APPLE},
        {"ITEM_ID_CHEESE",          ItemID::ITEM_ID_CHEESE},
        {"ITEM_ID_BREAD",           ItemID::ITEM_ID_BREAD},
        {"ITEM_ID_MEAT",            ItemID::ITEM_ID_MEAT},
        {"ITEM_ID_RING",            ItemID::ITEM_ID_RING},
        {"ITEM_ID_STOVE_PLAN",            ItemID::ITEM_ID_STOVE_PLAN},
        {"ITEM_ID_MUSHROOMPRESS_PLAN",    ItemID::ITEM_ID_MUSHROOMPRESS_PLAN},
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
        {"Grass1",                        EnvironmentSpriteID::SPRITE_ENVIRO_GRASS1},
        {"Grass2",                        EnvironmentSpriteID::SPRITE_ENVIRO_GRASS2},
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        //TraceLog(LOG_INFO, "enviro ID found %i", it->second);
        return it->second;
    }
    //TraceLog(LOG_INFO, "Spell ID not found ");
    return EnvironmentSpriteID::SPRITE_ENVIRO_ERROR;

}

void from_json(const json &j, ItemInstanceData &i) {
    j.at("item_id").get_to(i.item_id);
    j.at("instance_id").get_to(i.instance_id);
    j.at("value").get_to(i.value);
    j.at("type").get_to(i.type);
    j.at("item_name").get_to(i.item_name);
    j.at("spell_id").get_to(i.spell_id);
    j.at("clip_size").get_to(i.clip_size);
    j.at("ammo_count").get_to(i.ammo_count);
    j.at("container_id").get_to(i.container_id);
    j.at("cooldown").get_to(i.cooldown);
    j.at("item_id").get_to(i.item_id);
    j.at("icon_id").get_to(i.icon_id);
    j.at("level").get_to(i.level);
    j.at("defence").get_to(i.defence);
    j.at("magic_defence").get_to(i.magic_defence);
    j.at("sprite_id").get_to(i.sprite_id);
    j.at("saturation").get_to(i.saturation);

    //TraceLog(LOG_INFO, "Item loaded  %i", i.instance_id);
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

    _level_data.draw_list.push_back(g_current_player);

    for (auto e : _level_data.environment_entities)
        _level_data.draw_list.push_back(e);


    for (auto e : _level_data.entity_list)
    _level_data.draw_list.push_back(e);

 /*    if(e->y_sort){
    }

    for (auto e : _level_data.entity_list)
    if(!e->y_sort){
        _level_data.draw_list.push_back(e);
    } */

    std::sort(_level_data.draw_list.begin(), _level_data.draw_list.end(),
    [](BaseEntity* a, BaseEntity* b) {
        return a->GetYSort() < b->GetYSort();
    });
}