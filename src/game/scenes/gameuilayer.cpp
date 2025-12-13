#include "../../core/gamedefs.h"

GameUILayer::GameUILayer() {

    CreateLabel(title_label, {g_screen_center.x, 20 / g_scale}, 40/g_scale, RAYWHITE, "...");
    CreateLabel(debug_zoom_level, {20, 320 / g_scale}, 20/g_scale, RAYWHITE, "zoom...");

    CreateButton(quit_button, {g_resolution.x - 10, 10}, {30/g_scale , 30/g_scale}, RED, "X");
    quit_button.default_color = DARKRED;
    quit_button.text_size = 20/g_scale;


    CreateLabel(power_label, {20, g_resolution.y - 40}, FONTSIZE_50, RAYWHITE, "power...");
    CreateLabel(sat_label, {20, g_resolution.y - 85}, FONTSIZE_50, RAYWHITE, "SAT...");
}

GameUILayer::~GameUILayer() {
    //UnloadSound(button_sound);
    TraceLog(LOG_INFO, "UI DESTRUCTOR:  GAME UI");
}

void GameUILayer::Draw() {

    DrawButton(quit_button);
    DrawLabel(power_label);
    DrawLabel(sat_label);

    if(!g_game_settings.show_debug){
        return;
    }
    DrawLabelCentered(title_label);

    DrawLabel(debug_zoom_level);

}

void GameUILayer::Update() {
    std::string power = TextFormat("%0.2f", g_character_data[g_current_player->uid].current_power);
    power_label.text = power;

    std::string sat = TextFormat("%0.2f", g_character_data[g_current_player->uid].saturation);
    sat_label.text = sat;

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

    if(!g_game_settings.show_debug){
        return;
    }
    debug_zoom_level.text = TextFormat("zoom %0.2f", g_camera.zoom);
}