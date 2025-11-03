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

            float lx = g_input.screen_mouse_position.x * g_inv_scale;
            float ly = (g_input.screen_mouse_position.y - 50) * g_inv_scale;

            if(identifier == "HouseTransition") {
                g_game_data.sub_map_index = payload_i;
                g_game_data.sub_return_position = payload_v * g_ldtk_maps.default_grid_size;
                CreateLabel(label, {lx, ly}, 20, WHITE, "ENTER");
            }
            else {
                if(identifier == "ShelterTransition") {
                    CreateLabel(label, {lx, ly}, 20, WHITE, "ENTER SHELTER");
                }
                else {
                    CreateLabel(label, {lx, ly}, 20, WHITE, "EXIT");
                }
                g_game_data.next_map_index = payload_i;
            }
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
}

void TransitionArea::Draw() {
    if(hovered) {
        if(g_game_settings.show_debug) {
            int grid_size = g_ldtk_maps.default_grid_size;
/* 
            float lx = position.x;
            float ly = position.y;

            float rlx = (position.x * g_world2screen) + (g_camera.target.x * g_screen2world);
            float rly = (position.y * g_world2screen) + (g_camera.target.y * g_screen2world);
            //(m_pos.y * g_screen2world) + (g_camera.target.y);
            DrawRectangleV( {lx, ly}, {(float)size.x * g_scale, (float)size.y * g_scale}, BLUE);
            DrawRectangleV( {rlx, rly}, {(float)size.x * g_scale, (float)size.y * g_scale}, RED);
        }
         */
        //DrawCircle( lx , ly, 100, WHITE  );
        DrawLabelCentered(label);
        DrawLabelCenteredWithBG(label, BLACK);


        if(time_pressed > 0) {

            Rectangle rect = {
                .x = label.position.x - size.x,
                .y = label.position.y + size.y,
                .width = (size.x * 2) * time_pressed,
                .height = 6
            };
            
            //DrawRectangle(rect.x-1, rect.y-1, rect.width+2, rect.height+4, BLACK);
            DrawRectangleRec(rect, GOLD);
            //DrawRectangleLines(rect.x-2, rect.y-2, rect.width+2, rect.height+2, BLACK);
        }

    }
}


