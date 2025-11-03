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
            int grid_size = g_ldtk_maps.default_grid_size;
            if(identifier == "HouseTransition") {
                g_game_data.sub_map_index = payload_i;
                g_game_data.sub_return_position = payload_v * g_ldtk_maps.default_grid_size;
                float lx = g_input.screen_mouse_position.x / g_scale;
                float ly = g_input.screen_mouse_position.y / g_scale;
                CreateLabel(label, {lx, ly}, 20, WHITE, "ENTER");
            }
            else {
                if(identifier == "ShelterTransition") {
                    float lx = g_input.screen_mouse_position.x / g_scale;
                    float ly = g_input.screen_mouse_position.y / g_scale;

                    CreateLabel(label, {lx, ly}, 20, WHITE, "ENTER SHELTER");
                }
                else {
                    TraceLog(LOG_INFO, "camera targert   %0.0f %0.0f", g_camera.target.x, g_camera.target.y);
                    
                    float lx = g_input.screen_mouse_position.x / g_scale;
                    float ly = g_input.screen_mouse_position.y / g_scale;

                    CreateLabel(label, {lx, ly}, 20, WHITE, "EXIT");
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
            //DrawRectangleV(position, {(float)size.x, (float)size.y}, BLUE);
        }
        int grid_size = g_ldtk_maps.default_grid_size;
        float lx = g_input.screen_mouse_position.x / g_scale;
        float ly = g_input.screen_mouse_position.y / g_scale;
        
        //DrawCircle( lx , ly, 100, WHITE  );
        DrawLabelCentered(label);
        TraceLog(LOG_INFO, "AREA UPDATE %0.0f %0.0f ", lx, ly);

        if(time_pressed > 0) {

            Rectangle rect = {
                .x = label.position.x - size.x,
                .y = label.position.y + size.y,
                .width = (size.x * 2) * time_pressed,
                .height = 4
            };
            
            DrawRectangleRec(rect, YELLOW);
            DrawRectangleLines(rect.x-1, rect.y-1, rect.width+1, rect.height+1, BLACK);
        }

    }
}


