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
        inv.push_back(0);
        std::vector<int> hot;
        hot.push_back(1);


        PlayerData this_class = {
            .health = health,
            .exp = exp,
            .base_speed = base_speed,
            .sprite_sheet_id = sprite_sheet_id,
            .portrait_id = portrait_id,
            .name = "not assigned",
            .class_name = class_name,
            .inventory = inv,
            .hotbar = hot
        };

        g_class_data[i] = this_class;
    }

    for(int i = 0; i < cj["item_data"].size(); i++) {
        int id = i;
        int s_id = cj["item_data"][i]["sprite_id"];
        int value = cj["item_data"][i]["value"];
        std::string name = cj["item_data"][i]["item_name"];

        ItemData new_item = {
            .id = id,
            .sprite_id = s_id,
            .value = value,
            .item_name = name
        };

        g_item_data[i] = new_item;
    }

    for(int i = 0; i < cj["loot_tables"].size(); i++) {
        int id = i;

        std::vector<int> new_table;
        for(int t = 0; t < cj["loot_tables"][i]["table_data"].size(); t++) {
            new_table.push_back(cj["loot_tables"][i]["table_data"][t]);
        }

        std::string name = cj["loot_tables"][i]["table_name"];


        g_loot_tables.push_back(new_table);
    }

    cfile.close();
 
    //TraceLog(LOG_INFO, "==========end class data================");
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
    j["inventory"] = {};
    for(int i = 0; i < g_player_data.inventory.size(); i++) {
        j["inventory"][i] = g_player_data.inventory[i];
    }
    for(int i = 0; i < g_player_data.hotbar.size(); i++) {
        j["hotbar"][i] = g_player_data.hotbar[i];
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

    g_player_data.inventory = inv;
    g_player_data.hotbar = hot;

    file.close();

}




void ClearLevelData(LevelData &level_data) {
/*     level_data.spawn_position = {0,0};
    level_data.is_shelter = false;

    for (int i = 0; i <level_data.game_areas.size(); i++) {
        if(level_data.game_areas[i] != nullptr) {
            delete level_data.game_areas[i];
        }
    }
    level_data.game_areas.clear();

    for (int i = 0; i <level_data.containers.size(); i++) {
        if(level_data.containers[i] != nullptr) {
            delete level_data.containers[i];
        }
    }
    level_data.containers.clear();

    level_data.level_transitions.clear();
    level_data.container_data.clear();
    DL_Clear(level_data.entity_list);

 */
    //clear transition data

}

void LoadLevelData(LevelData &level_data) {


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
                if(identifier == "ContainerEntity") {
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
            }
        }
    }
}



void GenerateContainerItemList(int lti, std::vector<int> &list) {
    //int max = GetRandomValue(1, 5);
    int max = g_loot_tables[lti].size();

    for(int i = 0; i < max; i++) {
        //int choice = g_loot_tables[lti][GetRandomValue(0, g_loot_tables[lti].size()-1 )];
        
        list.push_back(g_loot_tables[lti][i]);
    }

}