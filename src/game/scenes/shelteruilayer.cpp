#include "../../core/gamedefs.h"

ShelterUILayer::ShelterUILayer() {

    CreateLabel(title_label, {g_screen_center.x, 20 / g_scale}, 40/g_scale, RAYWHITE, "SHELTER");
    CreateLabel(debug_label, {20 / g_scale}, 40/g_scale, RAYWHITE, "DEBUG");


    CreateButton(quit_button, {g_resolution.x - 10, 10}, {30/g_scale , 30/g_scale}, RED, "X");
    quit_button.default_color = DARKRED;
    quit_button.text_size = 20/g_scale;

    CreateButton(start_button, {g_screen_center.x, g_resolution.y - 40}, {200/g_scale , 100/g_scale}, GREEN, "go!");
    start_button.default_color = DARKGREEN;
    start_button.text_size = 40/g_scale;

    //map_menu = new MapMenu();
    //show_map_menu = false;
}

ShelterUILayer::~ShelterUILayer() {
    //UnloadSound(button_sound);
    //delete map_menu;
    TraceLog(LOG_INFO, "UI DESTRUCTOR:  SHELTER UI");
}

void ShelterUILayer::Draw() {

    DrawLabelCentered(title_label);
    DrawButton(start_button);
    DrawButton(quit_button);

    if(!g_game_settings.show_debug){
        return;
    }
    DrawLabel(debug_label);

}

void ShelterUILayer::Update() {

    if(g_game_settings.show_debug) {
        debug_label.text = TextFormat( "pf %0.2f %0.2f \n pc %i %i \n ct %i %i \n", 
            g_current_player->position.x, g_current_player->position.y,
            (int)(g_current_player->position.x / 16), (int)(g_current_player->position.y / 16),
            (int)g_camera.target.x, (int)g_camera.target.y
          );
    }

    if(IsButtonHovered(quit_button, g_scale)){
        if(quit_button.already_hovered == false) {
            //PlaySound(button_sound);
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            TraceLog(LOG_INFO, "QUIT BUTTON PRESSED ");
            //play_pressed.EmitSignal();
            quit_pressed.EmitSignal();
        }

       
    }

    if(IsButtonHovered(start_button, g_scale)){
        if(start_button.already_hovered == false) {
            //PlaySound(button_sound);
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            TraceLog(LOG_INFO, "START BUTTON PRESSED ");
            //play_pressed.EmitSignal();
            start_pressed.EmitSignal();
            //show_map_menu = true;
        }        
    }

}