#include "../core/gamedefs.h"


std::vector<Area> g_game_areas;


void UpdateGameAreas() {
    for(int area_index = 0; area_index < g_game_areas.size(); area_index++) {
        //TraceLog(LOG_INFO, "updating area at %0.0f %0.0f", g_game_areas[area_index].position.x, g_game_areas[area_index].position.y);
        
    }
}



void DrawGameAreas(Color color) {
    for(int area_index = 0; area_index < g_game_areas.size(); area_index++) {
        Vector2 pos = {g_game_areas[area_index].position.x, g_game_areas[area_index].position.y};
        TraceLog(LOG_INFO, "drawing area at %0.0f %0.0f  size %i", pos.x, pos.y, g_game_areas[area_index].size);

        DrawRectangleV( pos, {(float)g_game_areas[area_index].size, (float)g_game_areas[area_index].size}, color);      
    }
}