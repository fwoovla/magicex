#include "../core/gamedefs.h"

static std::string text = "CHEST";
/////TRANSITION AREA

TransitionArea::~TransitionArea() {
    TraceLog(LOG_INFO, "TransitionArea AREA DESTROYED, %s", identifier.c_str());
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

            CreateLabel(label, {lx, ly}, 20, WHITE, "open");
            if(identifier == "HouseTransition") {
                if(CheckCollisionCircleRec(g_current_player->position, 16, area_rect) ) {
                    g_game_data.sub_return_position = payload_v * 2;
                    g_game_data.sub_map_index = payload_i;
                    g_game_data.sub_map_uid = uid ;
                    in_range = true;
                }
            }
            else {
                if(CheckCollisionCircleRec(g_current_player->position, 16, area_rect) ) {
                        g_game_data.next_map_index = payload_i;
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
        if(in_range) {
            label.default_color = GREEN;
        }
        else {
            label.default_color = WHITE;
        }
        DrawLabelCentered(label);
        DrawLabelCenteredWithBG(label, TRANSDARKERGRAY);

        if(time_pressed > 0) {
            Rectangle rect = {
                .x = label.position.x - 20,
                .y = label.position.y - 30,
                .width = (20 * 2) * time_pressed,
                .height = 8
            };
            DrawRectangleRec(rect, RED);
        }
    }
}


////CONTAINER AREA

ContainerArea::~ContainerArea() {
    TraceLog(LOG_INFO, "ContainerArea AREA DESTROYED, %s", identifier.c_str());
}

void ContainerArea::Update() {
    
    float max_time_to_press = 1.0f;
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

        text = "";
        
        if(identifier == "GroundContainerEntity") {
            int count = 0;
            for(int i = 0; i < item_list.size(); i++) {
                if(item_list[i] != -1) {
                    //text = "ground";
                    text += g_item_instances[item_list[i]].item_name + "\n";
                    //text += g_item_instances[item_list[i]].item_name + "\n";
                }
            }
            text.pop_back();
            max_time_to_press = 0.5f;
        }
        else if(identifier == "Mushroom") { 
            text = "mushroom";
        }
        else {
            text = "open";
        }
        CreateLabel(label, {lx, ly}, 20, WHITE, text.c_str());

        if(CheckCollisionCircleRec(g_current_player->position, 16, area_rect) ) {
            in_range = true;
        }
    }
    else {
        hovered = false;

    }
    if(hovered and g_input.key_use and in_range) {
        time_pressed += 1.0f * GetFrameTime();
        if(time_pressed > max_time_to_press) {
            time_pressed = 0.0f;
            area_activated.EmitSignal();
        }
    }
    else {
        time_pressed = 0.0f;
    }
}

void ContainerArea::Draw() {
    

    if(hovered) {
        if(in_range) {
            label.default_color = GREEN;
        }
        else {
            label.default_color = WHITE;
        }
        DrawLabelCenteredWithBG(label, TRANSDARKERGRAY);

        if(time_pressed > 0) {

            Rectangle rect = {
                .x = label.position.x - 20,
                .y = label.position.y - 30,
                .width = (20 * 2) * time_pressed,
                .height = 8
            };
            DrawRectangleRec(rect, RED);
        }
    }
}


