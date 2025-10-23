#include "../../core/gamedefs.h"

#define BUTTON_OFFEST 10

MapMenu::MapMenu() {

    CreateLabel(title_label, {g_screen_center.x, 20 / g_scale}, 40/g_scale, BLACK, "CHOOSE MAP");

    panel_rect = {
        .x = g_resolution.x/2 - 100,
        .y = g_resolution.y/2 - 100,
        .width = 200,
        .height = 200
    };

    map_list_pos = {
        .x = panel_rect.x + 20,
        .y = panel_rect.y + 20
    };


/*     for (int i = 0; i < g_maps_data.size(); i++) {
        TraceLog(LOG_INFO, "map name: %s", g_maps_data[i].map_path.c_str());


        Button new_map_button;
        CreateButton(new_map_button, { g_resolution.x/2, g_resolution.y/2 - 50 + (i * (BUTTON_OFFEST + 30)) }, {100, 30}, RAYWHITE, g_maps_data[i].map_path.c_str());
        new_map_button.text_size = 20/g_scale;
        new_map_button.default_color = DARKERGRAY;

        map_buttons.push_back(new_map_button);

    } */
 
}

MapMenu::~MapMenu() {
    //UnloadSound(button_sound);
    TraceLog(LOG_INFO, "UI DESTRUCTOR:  MAP MENU UI");
}

void MapMenu::Draw() {

    DrawRectangleRec(panel_rect, BLACK);
    DrawRectangleLines( panel_rect.x, panel_rect.y, panel_rect.width, panel_rect.height, RAYWHITE );

    for (int i = 0; i < map_buttons.size(); i++) {
        DrawButton(map_buttons[i]);
    }

}

void MapMenu::Update() {
    for (int i = 0; i < map_buttons.size(); i++) {

        if(IsButtonHovered(map_buttons[i], g_scale)){
            if(map_buttons[i].already_hovered == false) {
                //PlaySound(button_sound);
            }
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if(i != g_game_data.shelter_map_index) {
                    TraceLog(LOG_INFO, "MAP BUTTON PRESSED %i", i);
                    g_game_data.current_map_index = i;
                    //play_pressed.EmitSignal();
                    map_selected.EmitSignal();
                }
        }        
        }

    }

}
