#include "../../core/gamedefs.h"

GameUILayer::GameUILayer() {

    CreateLabel(title_label, {g_screen_center.x, 20 / g_scale}, 40/g_scale, RAYWHITE, "...");
    CreateLabel(debug_zoom_level, {20, 320 / g_scale}, 20/g_scale, RAYWHITE, "zoom...");

    CreateButton(quit_button, {g_resolution.x - 10, 10}, {30/g_scale , 30/g_scale}, RED, "X");
    quit_button.default_color = DARKRED;
    quit_button.text_size = 20/g_scale;

    //CreateLabel(power_label, {20, g_resolution.y - 40}, FONTSIZE_50, RAYWHITE, "power...");
    //CreateLabel(sat_label, {20, g_resolution.y - 85}, FONTSIZE_50, RAYWHITE, "SAT...");
    
    CreateStatusBar(health_bar,{20, g_resolution.y - 60}, g_active_creature_data[g_current_player->uid].max_health, 100.0f, 10.0f, RED);
    CreateStatusBar(power_bar,{20, g_resolution.y - 45}, g_active_creature_data[g_current_player->uid].max_power, 100.0f, 10.0f, WEAPONCOLOR);
    last_max_power = power_bar.max_value;

    CreateStatusBar(saturation_bar,{20, g_resolution.y - 30}, g_active_creature_data[g_current_player->uid].max_saturation, 100.0f, 10.0f, GREEN);
    CreateStatusBar(stamina_bar,{20, g_resolution.y - 15}, g_active_creature_data[g_current_player->uid].max_stamina, 100.0f, 10.0f, RAYWHITE);
}

GameUILayer::~GameUILayer() {
    //UnloadSound(button_sound);
    TraceLog(LOG_INFO, "UI DESTRUCTOR:  GAME UI");
}

void GameUILayer::Draw() {

    DrawButton(quit_button);
    //DrawLabel(power_label);
    //DrawLabel(sat_label);
    DrawStatusBar(health_bar);
    DrawStatusBar(saturation_bar);
    DrawStatusBar(power_bar);
    DrawStatusBar(stamina_bar);

    if(!g_game_settings.show_debug){
        return;
    }


    DrawLabelCentered(title_label);

    DrawLabel(debug_zoom_level);

}

void GameUILayer::Update() {
    if(last_max_power != g_active_creature_data[g_current_player->uid].max_power) {
        //max power changed
        CreateStatusBar(power_bar,{20, g_resolution.y - 45}, g_active_creature_data[g_current_player->uid].max_power, 100.0f, 10.0f, WEAPONCOLOR);
        last_max_power = power_bar.max_value;
    }

    health_bar.current_value = g_active_creature_data[g_current_player->uid].health;
    power_bar.current_value = g_active_creature_data[g_current_player->uid].current_power;
    saturation_bar.current_value = g_active_creature_data[g_current_player->uid].saturation;
    stamina_bar.current_value = g_active_creature_data[g_current_player->uid].current_stamina;

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