#include "../../core/gamedefs.h"

GameUILayer::GameUILayer() {

    CreateLabel(title_label, {g_screen_center.x, 20 / g_scale}, 40/g_scale, RAYWHITE, "...");
    CreateLabel(debug_zoom_level, {20, 320 / g_scale}, FONTSIZE_20, RAYWHITE, "zoom...");

    CreateButton(quit_button, {g_resolution.x - 10, 10}, {30/g_scale , 30/g_scale}, RED, "X");
    quit_button.default_color = DARKRED;
    quit_button.text_size = 20/g_scale;

    CreateButton(debug_poi_layer_button, {100, 450 / g_scale}, {150/g_scale , 40/g_scale}, ORANGE, "poi layer");
    debug_poi_layer_button.text_size = FONTSIZE_24;

    CreateButton(debug_toggle_poi_layer_button, {100, 520 / g_scale}, {200/g_scale , 40/g_scale}, ORANGE, "toggle poi layer");
    debug_toggle_poi_layer_button.text_size = FONTSIZE_24;
    
    
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
    DrawStatusBar(health_bar);
    DrawStatusBar(saturation_bar);
    DrawStatusBar(power_bar);
    DrawStatusBar(stamina_bar);

    if(!g_game_settings.show_debug){
        return;
    }


    DrawLabelCentered(title_label);

    DrawLabel(debug_zoom_level);

    DrawButton(debug_poi_layer_button);
    DrawButton(debug_toggle_poi_layer_button);

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

    if(IsButtonHovered(debug_poi_layer_button, g_scale)){
        if(debug_poi_layer_button.already_hovered == false) {
            //PlaySound(button_sound);
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            g_debug_data.poi_layer++;
            g_debug_data.poi_layer = g_debug_data.poi_layer % 4;
            debug_poi_layer_button.text = "layer " + std::to_string(g_debug_data.poi_layer);
            
        }        
    }

    if(IsButtonHovered(debug_toggle_poi_layer_button, g_scale)){
        if(debug_toggle_poi_layer_button.already_hovered == false) {
            //PlaySound(button_sound);
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            g_debug_data.show_poi_layer = !g_debug_data.show_poi_layer;
            
        }        
    }

    debug_zoom_level.text = TextFormat("zoom %0.2f", g_camera.zoom);
}