#include "../core/gamedefs.h"

void ClearLevelData() {
    g_level_data.spawn_position = {0,0};
    //clear transition data

}

void LoadLevelData() {
    LDTKLevel this_level = g_ldtk_maps.levels[g_game_data.current_map_index];

    TraceLog(LOG_INFO, "LOADING LEVEL DATA  %s", this_level.identifier.c_str());

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

                if(this_level.layer_instances[layer_index].entity_instances[entity_index].identifier == "LevelTransition") {
                    LevelTransitionData new_transition;
                    TraceLog(LOG_INFO, "TRANSITION POINT FOUND");

                    new_transition.dest_string = this_level.layer_instances[layer_index].entity_instances[entity_index].field_instances[0].value_s;
                    new_transition.position_i.x = this_level.layer_instances[layer_index].entity_instances[entity_index].px[0];
                    new_transition.position_i.y = this_level.layer_instances[layer_index].entity_instances[entity_index].px[1];
                    new_transition.position_f.x = (float)this_level.layer_instances[layer_index].entity_instances[entity_index].px[0] * tile_size;
                    new_transition.position_f.y = (float)this_level.layer_instances[layer_index].entity_instances[entity_index].px[1] * tile_size;

                    g_level_data.level_transitions.push_back(new_transition);
                    TraceLog(LOG_INFO, "TRANSITION DATA ADDED, %s", new_transition.dest_string.c_str());
                }
            }
        }
    }
}