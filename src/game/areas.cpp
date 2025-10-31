#include "../core/gamedefs.h"

TransitionArea::~TransitionArea() {
    
}

void TransitionArea::Update() {
    
    Rectangle area_rect = {
        .x = position.x,
        .y = position.y,
        .width = (float)size.x,
        .height = (float)size.y
    };
    
    //collided = true;
    //if(CheckCollisionPointRec( g_current_player->position, area_rect) ) {
    if(CheckCollisionPointRec( g_input.world_mouse_position, area_rect) ) {
            hovered = true;
            if(identifier == "HouseTransition") {
                g_game_data.sub_map_index = payload_i;
                g_game_data.sub_return_position = payload_v * g_ldtk_maps.default_grid_size;
                CreateLabel(label, {position.x + (size.x * 0.5f), position.y + (size.y * 0.5f)}, 10, WHITE, "ENTER");
            }
            else {
                if(identifier == "ShelterTransition") {
                    CreateLabel(label, {position.x + (size.x * 0.5f), position.y + (size.y * 0.5f)}, 10, WHITE, "ENTER");
                }
                else {
                    CreateLabel(label, {position.x + (size.x * 0.5f), position.y + (size.y * 0.5f)}, 10, WHITE, "EXIT");
                }
                g_game_data.next_map_index = payload_i;
            }
        //TraceLog(LOG_INFO, "AREA Collided");
    }
    else {
        hovered = false;

    }


    if(hovered and g_input.key_use) {
        time_pressed += 1.0f * GetFrameTime();
        if(time_pressed > 1.0f) {
            time_pressed = 1.0f;
            area_activated.EmitSignal();
        }
    }
    else {
        time_pressed = 0.0f;
    }


    //TraceLog(LOG_INFO, "AREA UPDATE %0.0f %0.0f ", position.x, position.y);
}

void TransitionArea::Draw() {
    if(hovered) {
        if(g_game_settings.show_debug) {
            DrawRectangleV(position, {(float)size.x, (float)size.y}, BLUE);
        }
        DrawLabelCentered(label);

        if(time_pressed > 0) {

            Rectangle rect = {
                .x = position.x,
                .y = position.y + size.y,
                .width = size.x * time_pressed,
                .height = 4
            };
            
            DrawRectangleRec(rect, YELLOW);
            DrawRectangleLines(rect.x-1, rect.y-1, rect.width+1, rect.height+1, BLACK);
        }

    }
}


void UpdateGameAreas() {

/*     for(int area_index = 0; area_index < g_level_data.game_areas.size(); area_index++) {
        //TraceLog(LOG_INFO, "updating area at %0.0f %0.0f", g_game_areas[area_index].position.x, g_game_areas[area_index].position.y);
        if(g_level_data.game_areas[area_index].identifier == "LevelTransition" or g_level_data.game_areas[area_index].identifier == "ShelterTransition" or g_level_data.game_areas[area_index].identifier == "HouseTransition") {
            int dest_map_index = g_game_data.current_map_index;
            Rectangle area_rect = {
                .x = g_level_data.game_areas[area_index].position.x,
                .y = g_level_data.game_areas[area_index].position.y,
                .width = (float)g_level_data.game_areas[area_index].size,
                .height = (float)g_level_data.game_areas[area_index].size
            };
            g_level_data.game_areas[area_index].collided = false;
            if(CheckCollisionPointRec( g_current_player->position, area_rect) ) {

                g_level_data.game_areas[area_index].collided = true;

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
                    g_game_data.next_map_index = dest_map_index;
                    //g_game_data.current_map_index = dest_map_index;
                }
                g_level_data.game_areas[area_index].entity_entered.EmitSignal();
            }
        }
    } */
}


void DrawGameAreas(Color color) {
/*     for(int area_index = 0; area_index < g_level_data.game_areas.size(); area_index++) {
        Vector2 pos = {g_level_data.game_areas[area_index].position.x, g_level_data.game_areas[area_index].position.y};
        //TraceLog(LOG_INFO, "drawing area at %0.0f %0.0f  size %i", pos.x, pos.y, g_game_areas[area_index].size);

        DrawRectangleV( pos, {(float)g_level_data.game_areas[area_index].size, (float)g_level_data.game_areas[area_index].size}, color);      
    } */
}