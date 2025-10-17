#include "../../core/gamedefs.h"

TitleUILayer::TitleUILayer() {

    CreateLabel(title_label, {g_screen_center.x, 20 / g_scale}, 40/g_scale, BLACK, "title");

    CreateButton(continue_button, {g_screen_center.x, g_screen_center.y-(105/g_scale)}, {200/g_scale , 100/g_scale}, BLUE, "continue");
    continue_button.default_color = DARKBLUE;
    continue_button.text_size = 40/g_scale;
    
    CreateButton(start_button, {g_screen_center.x, g_screen_center.y}, {200/g_scale , 100/g_scale}, GREEN, "new game");
    start_button.default_color = DARKGREEN;
    start_button.text_size = 40/g_scale;

    CreateButton(settings_button, {g_screen_center.x,g_screen_center.y + (105/g_scale)}, {200/g_scale , 100/g_scale}, YELLOW, "settings");
    settings_button.default_color = DARKYELLOW;
    settings_button.text_size = 25/g_scale;

    CreateButton(quit_button, {g_screen_center.x,g_screen_center.y + (210/g_scale)}, {200/g_scale , 100/g_scale}, RED, "exit");

    quit_button.default_color = DARKRED;
    quit_button.text_size = 25/g_scale;

}

TitleUILayer::~TitleUILayer() {
    //UnloadSound(button_sound);
    TraceLog(LOG_INFO, "UI DESTRUCTOR:  TITLE UI");
}

void TitleUILayer::Draw() {
    DrawLabelCentered(title_label);
    DrawButton(start_button);
    DrawButton(settings_button);
    DrawButton(quit_button);

    if(g_game_data.save_available == true) {
        DrawButton(continue_button);
    }
}

void TitleUILayer::Update() {
    //TraceLog(LOG_INFO, "ui draw , %f", g_game_settings.game_scale);


    if(IsButtonHovered(continue_button, g_scale)){
        if(continue_button.already_hovered == false) {
            //PlaySound(button_sound);
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            TraceLog(LOG_INFO, "CONTINUE BUTTON PRESSED ");
            //play_pressed.EmitSignal();
            continue_pressed.EmitSignal();
        }        
    }

    if(IsButtonHovered(start_button, g_scale)){
        if(start_button.already_hovered == false) {
            //PlaySound(button_sound);
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            TraceLog(LOG_INFO, "PLAY BUTTON PRESSED ");
            //play_pressed.EmitSignal();
            play_pressed.EmitSignal();
        }        
    }

    if(IsButtonHovered(settings_button, g_scale)){
        if(settings_button.already_hovered == false) {
            //PlaySound(button_sound);
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            TraceLog(LOG_INFO, "SETTINGS BUTTON PRESSED ");
            //play_pressed.EmitSignal();
            settings_pressed.EmitSignal();
        }        
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
}
