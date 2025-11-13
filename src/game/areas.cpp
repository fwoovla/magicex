#include "../core/gamedefs.h"

static std::string text = "CHEST";
/////TRANSITION AREA

TransitionArea::~TransitionArea() {
    
}

void TransitionArea::Update() {
    
    Rectangle area_rect = {
        .x = position.x,
        .y = position.y,
        .width = (float)size.x,
        .height = (float)size.y
    };

    if(CheckCollisionPointRec( g_input.world_mouse_position, area_rect) ) {
            hovered = true;
            in_range = false;
            int grid_size = g_ldtk_maps.default_grid_size;

            float lx = g_input.screen_mouse_position.x * g_inv_scale;
            float ly = (g_input.screen_mouse_position.y - 50) * g_inv_scale;

            CreateLabel(label, {lx, ly}, 20, WHITE, "DOOR");
            if(identifier == "HouseTransition") {
                if(CheckCollisionCircleRec(g_current_player->position, 16, area_rect) ) {
                    g_game_data.sub_return_position = payload_v * g_ldtk_maps.default_grid_size;
                    g_game_data.sub_map_index = payload_i;
                    //label.text += "\n\n\nopen";
                    in_range = true;
                }
            }
            else {
                if(CheckCollisionCircleRec(g_current_player->position, 16, area_rect) ) {
                        g_game_data.next_map_index = payload_i;
                        label.text += "\n\n\n'space'";
                        in_range = true;
                }
            }
    }
    else {
        hovered = false;

    }
    if(hovered and g_input.key_use and in_range) {
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
        DrawLabelCentered(label);
        DrawLabelCenteredWithBG(label, BLACK);


        if(time_pressed > 0) {

            Rectangle rect = {
                .x = label.position.x - size.x,
                .y = label.position.y + size.y,
                .width = (size.x * 2) * time_pressed,
                .height = 6
            };
            
            DrawRectangleRec(rect, RED);
        }

    }
}


////CONTAINER AREA

ContainerArea::~ContainerArea() {
    
}

void ContainerArea::Update() {
    

    Rectangle area_rect = {
        .x = position.x,
        .y = position.y,
        .width = (float)size.x,
        .height = (float)size.y
    };
    //TraceLog(LOG_INFO, "CONTAINER AREA update, %s", identifier.c_str());
    if(CheckCollisionPointRec( g_input.world_mouse_position, area_rect) ) {
        //TraceLog(LOG_INFO, "CONTAINER AREA update, %s", identifier.c_str());
        hovered = true;
        in_range = false;

        int grid_size = g_ldtk_maps.default_grid_size;

        float lx = g_input.screen_mouse_position.x * g_inv_scale;
        float ly = (g_input.screen_mouse_position.y - 50) * g_inv_scale;

        if(identifier == "GroundContainerEntity") {
            //TraceLog(LOG_INFO, "CONTAINER AREA update");
            int count = 0;
            for(int i = 0; i < item_list.size(); i++) {
                if(item_list[i] != -1) {
                    count++;
                }
            }
            if(count == 0) {
                //list_empty.EmitSignal();
                is_empty = true;
                return;
            }
            text = "loot";
        }
        else {
            //TraceLog(LOG_INFO, "CONTAINER AREA update");
            text = "open";
        }
        CreateLabel(label, {lx, ly}, 20, WHITE, text.c_str());

        if(CheckCollisionCircleRec(g_current_player->position, 16, area_rect) ) {
            in_range = true;
            label.text += "\n\n\n'space'";
        }
    }
    else {
        hovered = false;

    }
    if(hovered and g_input.key_use and in_range) {
        time_pressed += 1.0f * GetFrameTime();
        if(time_pressed > 1.0f) {
            time_pressed = 1.0f;
            //g_game_data.loot_table_id = loot_table_id;
            //g_game_data.loot_table = &item_list;
            area_activated.EmitSignal();
        }
    }
    else {
        time_pressed = 0.0f;
    }
}

void ContainerArea::Draw() {
    

    if(hovered) {
        //TraceLog(LOG_INFO, "CONTAINER DRAW");
        //DrawLabelCentered(label);
        DrawLabelCenteredWithBG(label, BLACK);


        if(time_pressed > 0) {

            Rectangle rect = {
                .x = label.position.x - size.x,
                .y = label.position.y + size.y,
                .width = (size.x * 2) * time_pressed,
                .height = 6
            };
            
            DrawRectangleRec(rect, RED);
        }

    }
}
