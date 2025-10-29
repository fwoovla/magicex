#include "../core/gamedefs.h"


//std::vector<Area> g_game_areas;


void UpdateGameAreas() {

    for(int area_index = 0; area_index < g_level_data.game_areas.size(); area_index++) {
        //TraceLog(LOG_INFO, "updating area at %0.0f %0.0f", g_game_areas[area_index].position.x, g_game_areas[area_index].position.y);
        if(g_level_data.game_areas[area_index].identifier == "LevelTransition" or g_level_data.game_areas[area_index].identifier == "ShelterTransition" or g_level_data.game_areas[area_index].identifier == "HouseTransition") {
            int dest_map_index = g_game_data.current_map_index;
            Rectangle area_rect = {
                .x = g_level_data.game_areas[area_index].position.x,
                .y = g_level_data.game_areas[area_index].position.y,
                .width = (float)g_level_data.game_areas[area_index].size,
                .height = (float)g_level_data.game_areas[area_index].size
            };
            if(CheckCollisionPointRec( g_current_player->position, area_rect) ) {
                for(int level_index = 0; level_index < g_ldtk_maps.levels.size(); level_index++) {

                    if(g_ldtk_maps.levels[level_index].identifier == g_level_data.game_areas[area_index].payload_s) {
                        dest_map_index = level_index;
                    }
                }
                if(g_level_data.game_areas[area_index].identifier == "HouseTransition") {
                    TraceLog(LOG_INFO, "return position %0.0f %0.0f", g_level_data.game_areas[area_index].payload_v.x, g_level_data.game_areas[area_index].payload_v.y);
                    g_game_data.sub_map_index = dest_map_index;
                    g_game_data.sub_return_position = g_level_data.game_areas[area_index].payload_v * g_ldtk_maps.default_grid_size;
                }
                else {
                    g_game_data.current_map_index = dest_map_index;
                }
                g_level_data.game_areas[area_index].entity_entered.EmitSignal();
            }
        }
    }
}


void DrawGameAreas(Color color) {
    for(int area_index = 0; area_index < g_level_data.game_areas.size(); area_index++) {
        Vector2 pos = {g_level_data.game_areas[area_index].position.x, g_level_data.game_areas[area_index].position.y};
        //TraceLog(LOG_INFO, "drawing area at %0.0f %0.0f  size %i", pos.x, pos.y, g_game_areas[area_index].size);
        DrawRectangleV( pos, {(float)g_level_data.game_areas[area_index].size, (float)g_level_data.game_areas[area_index].size}, color);      
    }
}