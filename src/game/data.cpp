#include "../core/gamedefs.h"

void ClearLevelData() {
    g_level_data.spawn_position = {0,0};
    g_level_data.game_areas.clear();
    g_level_data.is_shelter = false;
    g_level_data.level_transitions.clear();
    //clear transition data

}

void LoadLevelData() {


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
                    g_level_data.spawn_position = sp;
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

                    g_level_data.level_transitions.push_back(new_transition);
                    TraceLog(LOG_INFO, "TRANSITION dest string ADDED, %s", new_transition.dest_string.c_str());
                }
            }
        }
    }
}