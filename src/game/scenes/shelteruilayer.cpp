#include "../../core/gamedefs.h"

ShelterUILayer::ShelterUILayer() {

    CreateLabel(title_label, {g_screen_center.x, 20 / g_scale}, 40/g_scale, RAYWHITE, "SHELTER");
    //CreateLabel(debug_label, {20 / g_scale}, 40/g_scale, RAYWHITE, "DEBUG");


    CreateButton(quit_button, {g_resolution.x - 10, 10}, {30/g_scale , 30/g_scale}, RED, "X");
    quit_button.default_color = DARKRED;
    quit_button.text_size = 20/g_scale;

    CreateButton(start_button, {g_screen_center.x, 20}, {100/g_scale , 50/g_scale}, GREEN, "go!");
    start_button.default_color = DARKGREEN;
    start_button.text_size = 30/g_scale;
}

ShelterUILayer::~ShelterUILayer() {
    TraceLog(LOG_INFO, "UI DESTRUCTOR:  SHELTER UI");
}

void ShelterUILayer::Draw() {

    DrawLabelCentered(title_label);
    DrawButton(start_button);
    DrawButton(quit_button);

    if(!g_game_settings.show_debug){
        return;
    }
}

void ShelterUILayer::Update() {

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
        }        
    }
}