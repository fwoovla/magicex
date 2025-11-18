#include "../core/gamedefs.h"
#include "../core/utils.h"


void LoadGameData() {
    TraceLog(LOG_INFO, "LOADING GAME DATA....data.json");

    std::ifstream cfile("assets/data.json");
    if (!cfile.is_open()) {
        TraceLog(LOG_INFO, "CANNOT OPEN DATA FILE");
        return;
    }
    
    json cj;
    cfile>>cj;
    //TraceLog(LOG_INFO, "==========LOADING GAME DATA FROM FILE================");
    //TraceLog(LOG_INFO, "==========loading class data================");

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
        ItemType type = cj["item_data"][i]["item_type"];

/*         SpellID spell_id = SPELL_ID_NONE;

        if(type == TYPE_WEAPON) {
            std::string sp_id = cj["item_data"][i]["spell_id"];
            spell_id = StrToSpellId(sp_id);
        } */

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

        float lifetime = cj["spell_data"][i]["lifetime"];
        float damage = cj["spell_data"][i]["damage"];
        int speed = cj["spell_data"][i]["speed"];
        int level = cj["spell_data"][i]["level"];

        new_spell.lifetime = lifetime;
        new_spell.damage = damage;
        new_spell.level = level;
        new_spell.spell_id = spell_id;
        
        g_spell_data[spell_id] = new_spell;
        TraceLog(LOG_INFO, "Spell Data Loaded  id: %i  %s", spell_id, sp_id.c_str());
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


        WeaponData new_weapon = {
            .weapon_name = name,
            .weapon_id = w_id,
            .cooldown = cooldown,
            .spell_id = spell_id            
        };

        TraceLog(LOG_INFO, "Weapon Data Loaded  id: %i  %s", w_id, name.c_str());
        g_weapon_data[(int)w_id] = new_weapon;

    }


    //--------------------loot tables
    for(int i = 0; i < cj["loot_tables"].size(); i++) {
        int id = i;

        std::vector<ItemID> new_table;
        for(int t = 0; t < cj["loot_tables"][i]["table_data"].size(); t++) {
            new_table.push_back(cj["loot_tables"][i]["table_data"][t]);
        }

        std::string name = cj["loot_tables"][i]["table_name"];


        g_loot_tables.push_back(new_table);
    }

    cfile.close();
 
    //TraceLog(LOG_INFO, "==========check save data================");

    std::string save_path = "assets/save.json";
    if(std::filesystem::exists(save_path)) {
        g_game_data.save_available = true;
        TraceLog(LOG_INFO, "SAVE FILE FOUND");
    }


    TraceLog(LOG_INFO, "==========LOADING LDTK MAPS================");

    std::string ldtk_map_dir = "assets/maps/ldtk";
    int num_maps = load_ldtk_maps();

    TraceLog(LOG_INFO, "==========END LOADING LDTK MAPS================  loaded %i maps", num_maps);

}


void SaveGame() {
    std::string save_path = "assets/save.json";
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


    file<<j.dump(4);

    file.close();

}


void LoadGame() {
    TraceLog(LOG_INFO, "LOADING SAVED DATA....save.json");

    std::string save_path = "assets/save.json";
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

    file.close();

}




void ClearLevelData(LevelData &level_data) {

    DL_Clear(level_data.entity_list);
    level_data.level_transitions.clear();
    level_data.container_data.clear();
    level_data.game_areas.clear();

}

void LoadLevelData(LevelData &level_data) {


    PrecalculateTileCollisionData(level_data);


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
                    TraceLog(LOG_INFO, "SPAWN POINT FOUND");
                    Vector2 sp = {};
                    sp.x = {(float)this_level.layer_instances[layer_index].entity_instances[entity_index].px[0]};
                    sp.y = {(float)this_level.layer_instances[layer_index].entity_instances[entity_index].px[1]};
                    level_data.spawn_position = sp;
                }

                std::string identifier = this_level.layer_instances[layer_index].entity_instances[entity_index].identifier;
                //if(identifier == "LevelTransition" or identifier == "ShelterTransition") {
                if(identifier == "LevelTransition" or identifier == "ShelterTransition" or identifier == "HouseTransition") {
                    LevelTransitionData new_transition;
                    TraceLog(LOG_INFO, "TRANSITION POINT FOUND %s", identifier.c_str());

 

                    new_transition.size = {(float)this_level.layer_instances[layer_index].entity_instances[entity_index].width, (float)this_level.layer_instances[layer_index].entity_instances[entity_index].height};
                    new_transition.identifier = this_level.layer_instances[layer_index].entity_instances[entity_index].identifier;
                    new_transition.dest_string = this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[0].value_s;
                    new_transition.position_i.x = this_level.layer_instances[layer_index].entity_instances[entity_index].px[0];
                    new_transition.position_i.y = this_level.layer_instances[layer_index].entity_instances[entity_index].px[1];
                    new_transition.position_f.x = (float)this_level.layer_instances[layer_index].entity_instances[entity_index].px[0] * tile_size;
                    new_transition.position_f.y = (float)this_level.layer_instances[layer_index].entity_instances[entity_index].px[1] * tile_size;

                    if(new_transition.identifier == "HouseTransition") {
                        new_transition.return_position = this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[1].value_v;
                        TraceLog(LOG_INFO, "RETURN POSITION, %0.02f %0.02f", new_transition.return_position.x, new_transition.return_position.y);
                    }

                    level_data.level_transitions.push_back(new_transition);
                    TraceLog(LOG_INFO, "TRANSITION dest string ADDED, %s", new_transition.dest_string.c_str());
                }
                if(identifier == "PermContainerEntity") {
                    ContainerData new_container;
                    TraceLog(LOG_INFO, "CONTAINER FOUND %s", identifier.c_str());

                    new_container.size = {(float)this_level.layer_instances[layer_index].entity_instances[entity_index].width, (float)this_level.layer_instances[layer_index].entity_instances[entity_index].height};
                    new_container.identifier = this_level.layer_instances[layer_index].entity_instances[entity_index].identifier;
                    new_container.position_i.x = this_level.layer_instances[layer_index].entity_instances[entity_index].px[0];
                    new_container.position_i.y = this_level.layer_instances[layer_index].entity_instances[entity_index].px[1];
                    new_container.position_f.x = (float)this_level.layer_instances[layer_index].entity_instances[entity_index].px[0] * tile_size;
                    new_container.position_f.y = (float)this_level.layer_instances[layer_index].entity_instances[entity_index].px[1] * tile_size;

                    new_container.sprite_id = this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[0].value_i;
                    new_container.loot_table_id = this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[1].value_i;
                    level_data.container_data.push_back(new_container);
                    TraceLog(LOG_INFO, "CONTAINER ADDED WITH LT %i SID %i", new_container.loot_table_id, new_container.sprite_id);
                }

                if(identifier == "GroundContainerEntity") {
                    ContainerData new_container;
                    TraceLog(LOG_INFO, "GROUND CONTAINER FOUND %s %i", identifier.c_str(), this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[0].i_list.size());
                    new_container.size = {(float)this_level.layer_instances[layer_index].entity_instances[entity_index].width, (float)this_level.layer_instances[layer_index].entity_instances[entity_index].height};
                    new_container.identifier = this_level.layer_instances[layer_index].entity_instances[entity_index].identifier;
                    new_container.position_i.x = this_level.layer_instances[layer_index].entity_instances[entity_index].px[0];
                    new_container.position_i.y = this_level.layer_instances[layer_index].entity_instances[entity_index].px[1];
                    new_container.position_f.x = (float)this_level.layer_instances[layer_index].entity_instances[entity_index].px[0] * tile_size;
                    new_container.position_f.y = (float)this_level.layer_instances[layer_index].entity_instances[entity_index].px[1] * tile_size;

                    for(int item = 0; item < this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[0].i_list.size(); item++) {
                        TraceLog(LOG_INFO, "GROUND CONTAINER DATA ADDED %i", this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[0].i_list[item]);
                        new_container.item_list.push_back(this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[0].i_list[item]);
                    } 
                    level_data.container_data.push_back(new_container);
                }
            }
        }
    }
}



void PrecalculateTileCollisionData(LevelData &level_data) {
    TraceLog(LOG_INFO, " PRECALCULATING TILE COLLISION DATA ");

    level_data.precalc.map_index = g_game_data.current_map_index;

    if(g_game_data.is_in_sub_map) {
        level_data.precalc.map_index = g_game_data.sub_map_index;
        TraceLog(LOG_INFO, "            map index sub map -- %i",level_data.precalc.map_index  );
    }
    TraceLog(LOG_INFO, "            map index -- %i",level_data.precalc.map_index  );

    LDTKLevel this_level = g_ldtk_maps.levels[level_data.precalc.map_index];
    LDTKLayerInstance *col_layer = nullptr;

    for (int l = 0; l < this_level.layer_instances.size(); l++) {
        if(this_level.layer_instances[l].type == "IntGrid") {
            level_data.precalc.collision_layer_index = l;
            TraceLog(LOG_INFO, "            collision layer -- %i",level_data.precalc.collision_layer_index  );
            col_layer = &this_level.layer_instances[l];
        }
    }

    if(col_layer == nullptr) {
        TraceLog(LOG_INFO, "no collision layer");
        //level_data.tile_precalc.collision_layer_index = -1;
    }

    level_data.precalc.tile_size = col_layer->grid_size;
    TraceLog(LOG_INFO, "            tile size -- %i",level_data.precalc.tile_size  );

    level_data.precalc.inv_tile_size = 1/(float)level_data.precalc.tile_size;
    TraceLog(LOG_INFO, "            inv tilesize -- %i",level_data.precalc.collision_layer_index  );

    level_data.precalc.map_width = col_layer->c_wid;
    TraceLog(LOG_INFO, "            collision layer -- %i",level_data.precalc.collision_layer_index  );

    TraceLog(LOG_INFO, "FINISHED PRECALCULATING TILE COLLISION DATA ");
}



void GenerateContainerItemList(int lti, std::vector<int> &list) {
    int max = g_loot_tables[lti].size();

    for(int i = 0; i < max; i++) {        
        list.push_back(g_loot_tables[lti][i]);
    }
}



SpellID StrToSpellId(const std::string& s) {

    static const std::unordered_map<std::string, SpellID> lookup_table = {
        {"None",                        SpellID::SPELL_ID_NONE},
        {"SPELL_ID_MAGICMISSLE_1",        SpellID::SPELL_ID_MAGICMISSLE_1},
        {"SPELL_ID_MAGICMISSLE_2",        SpellID::SPELL_ID_MAGICMISSLE_2},
        {"SPELL_ID_MAGICMISSLE_3",        SpellID::SPELL_ID_MAGICMISSLE_3},
        {"SPELL_ID_FIREBALL_1",           SpellID::SPELL_ID_FIREBALL_1},
        {"SPELL_ID_FIREBALL_2",           SpellID::SPELL_ID_FIREBALL_2},
        {"SPELL_ID_FIREBALL_3",           SpellID::SPELL_ID_FIREBALL_3},
        {"SPELL_ID_LIGHTNING_1",          SpellID::SPELL_ID_LIGHTNING_1},
        {"SPELL_ID_LIGHTNING_2",          SpellID::SPELL_ID_LIGHTNING_2},
        {"SPELL_ID_LIGHTNING_3",          SpellID::SPELL_ID_LIGHTNING_3},
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        TraceLog(LOG_INFO, "Spell ID found %i", it->second);
        return it->second;
    }
    TraceLog(LOG_INFO, "Spell ID not found ");
    return SpellID::SPELL_ID_NONE;

}


/* ItemID StrToWeaponID(const std::string& s) {

    static const std::unordered_map<std::string, ItemID> lookup_table = {
        {"ITEM_ID_MAGICMISSLE_WAND_1",          ItemID::ITEM_ID_MAGICMISSLE_WAND_1},
        {"ITEM_ID_MAGICMISSLE_WAND_2",         ItemID::ITEM_ID_MAGICMISSLE_WAND_2},
        {"ITEM_ID_MAGICMISSLE_WAND_3",         ItemID::ITEM_ID_MAGICMISSLE_WAND_3},
        {"ITEM_ID_FIREBALL_WAND_1",         ItemID::ITEM_ID_FIREBALL_WAND_1},
        {"ITEM_ID_FIREBALL_WAND_2",         ItemID::ITEM_ID_FIREBALL_WAND_2},
        {"ITEM_ID_FIREBALL_WAND_3",         ItemID::ITEM_ID_FIREBALL_WAND_3},
        {"ITEM_ID_LIGHTNING_WAND_1",         ItemID::ITEM_ID_LIGHTNING_WAND_1},
        {"ITEM_ID_LIGHTNING_WAND_2",         ItemID::ITEM_ID_LIGHTNING_WAND_2},
        {"ITEM_ID_LIGHTNING_WAND_3",         ItemID::ITEM_ID_LIGHTNING_WAND_3},


        {"ITEM_ID_MAGICMISSLE_STAFF_1",         ItemID::ITEM_ID_MAGICMISSLE_STAFF_1},
        {"ITEM_ID_MAGICMISSLE_STAFF_2",         ItemID::ITEM_ID_MAGICMISSLE_STAFF_2},
        {"ITEM_ID_MAGICMISSLE_STAFF_3",         ItemID::ITEM_ID_MAGICMISSLE_STAFF_3},
        {"ITEM_ID_FIREBALL_STAFF_1",         ItemID::ITEM_ID_FIREBALL_STAFF_1},
        {"ITEM_ID_FIREBALL_STAFF_2",         ItemID::ITEM_ID_FIREBALL_STAFF_2},
        {"ITEM_ID_FIREBALL_STAFF_3",         ItemID::ITEM_ID_FIREBALL_STAFF_3},
        {"ITEM_ID_LIGHTNING_STAFF_1",         ItemID::ITEM_ID_LIGHTNING_STAFF_1},
        {"ITEM_ID_LIGHTNING_STAFF_2",         ItemID::ITEM_ID_LIGHTNING_STAFF_2},
        {"ITEM_ID_LIGHTNING_STAFF_3",         ItemID::ITEM_ID_LIGHTNING_STAFF_3},
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        return it->second;
    }
    TraceLog(LOG_INFO, "Item ID not found ");
    return ItemID::ITEM_ID_NONE;

} */

ItemID StrToItemId(const std::string& s) {

    static const std::unordered_map<std::string, ItemID> lookup_table = {
        {"None",                    ItemID::ITEM_ID_NONE},
        {"ITEM_ID_DAGGER",          ItemID::ITEM_ID_DAGGER},
        {"ITEM_ID_SWORD",           ItemID::ITEM_ID_SWORD},
        {"ITEM_ID_SPEAR",           ItemID::ITEM_ID_SPEAR},
        {"ITEM_ID_AXE",                         ItemID::ITEM_ID_AXE},
        {"ITEM_ID_BOW",                         ItemID::ITEM_ID_BOW},

        {"ITEM_ID_MAGICMISSLE_WAND_1",          ItemID::ITEM_ID_MAGICMISSLE_WAND_1},
        {"ITEM_ID_MAGICMISSLE_WAND_2",         ItemID::ITEM_ID_MAGICMISSLE_WAND_2},
        {"ITEM_ID_MAGICMISSLE_WAND_3",         ItemID::ITEM_ID_MAGICMISSLE_WAND_3},
        {"ITEM_ID_FIREBALL_WAND_1",         ItemID::ITEM_ID_FIREBALL_WAND_1},
        {"ITEM_ID_FIREBALL_WAND_2",         ItemID::ITEM_ID_FIREBALL_WAND_2},
        {"ITEM_ID_FIREBALL_WAND_3",         ItemID::ITEM_ID_FIREBALL_WAND_3},
        {"ITEM_ID_LIGHTNING_WAND_1",         ItemID::ITEM_ID_LIGHTNING_WAND_1},
        {"ITEM_ID_LIGHTNING_WAND_2",         ItemID::ITEM_ID_LIGHTNING_WAND_2},
        {"ITEM_ID_LIGHTNING_WAND_3",         ItemID::ITEM_ID_LIGHTNING_WAND_3},


        {"ITEM_ID_MAGICMISSLE_STAFF_1",         ItemID::ITEM_ID_MAGICMISSLE_STAFF_1},
        {"ITEM_ID_MAGICMISSLE_STAFF_2",         ItemID::ITEM_ID_MAGICMISSLE_STAFF_2},
        {"ITEM_ID_MAGICMISSLE_STAFF_3",         ItemID::ITEM_ID_MAGICMISSLE_STAFF_3},
        {"ITEM_ID_FIREBALL_STAFF_1",         ItemID::ITEM_ID_FIREBALL_STAFF_1},
        {"ITEM_ID_FIREBALL_STAFF_2",         ItemID::ITEM_ID_FIREBALL_STAFF_2},
        {"ITEM_ID_FIREBALL_STAFF_3",         ItemID::ITEM_ID_FIREBALL_STAFF_3},
        {"ITEM_ID_LIGHTNING_STAFF_1",         ItemID::ITEM_ID_LIGHTNING_STAFF_1},
        {"ITEM_ID_LIGHTNING_STAFF_2",         ItemID::ITEM_ID_LIGHTNING_STAFF_2},
        {"ITEM_ID_LIGHTNING_STAFF_3",         ItemID::ITEM_ID_LIGHTNING_STAFF_3},

        {"ITEM_ID_MUSHROOM",           ItemID::ITEM_ID_MUSHROOM},
        {"ITEM_ID_MUSHROOM_JUICE",     ItemID::ITEM_ID_MUSHROOM_JUICE},


        {"ITEM_ID_LEATHERBOOTS",    ItemID::ITEM_ID_LEATHERBOOTS},
        {"ITEM_ID_LEATHERVEST",     ItemID::ITEM_ID_LEATHERVEST},
        {"ITEM_ID_LEATHERGLOVES",   ItemID::ITEM_ID_LEATHERGLOVES},
        {"ITEM_ID_LEATHERLEGGINGS",   ItemID::ITEM_ID_LEATHERLEGGINGS},


        {"ITEM_ID_MAGICMMISSLE_SCROLL",          ItemID::ITEM_ID_MAGICMMISSLE_SCROLL},
        {"ITEM_ID_FIREBALL_SCROLL",          ItemID::ITEM_ID_FIREBALL_SCROLL},
        {"ITEM_ID_LIGHTNING_SCROLL",          ItemID::ITEM_ID_LIGHTNING_SCROLL},

        {"ITEM_ID_APPLE",           ItemID::ITEM_ID_APPLE},
        {"ITEM_ID_CHEESE",          ItemID::ITEM_ID_CHEESE},
        {"ITEM_ID_BREAD",           ItemID::ITEM_ID_BREAD},
        {"ITEM_ID_MEAT",            ItemID::ITEM_ID_MEAT},
        {"ITEM_ID_RING",            ItemID::ITEM_ID_RING},
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        return it->second;
    }
    TraceLog(LOG_INFO, "Item ID not found ");
    return ItemID::ITEM_ID_NONE;
}