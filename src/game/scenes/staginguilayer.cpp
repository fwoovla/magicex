#include "../../core/gamedefs.h"

StagingUILayer::StagingUILayer() {

    select_index = 0;
    is_selecting = true;

    CreateLabel(title_label, {g_screen_center.x, 20 / g_scale}, 40/g_scale, BLACK, "staging");

    CreateButton(start_button, {g_resolution.x - 80, 100}, {200/g_scale , 100/g_scale}, GREEN, "go!");
    start_button.default_color = DARKGREEN;
    start_button.text_size = 20/g_scale;

    CreateButton(settings_button, {g_resolution.x - 80, 10}, {120/g_scale , 30/g_scale}, YELLOW, "settings");
    settings_button.default_color = DARKYELLOW;
    settings_button.text_size = 20/g_scale;

    CreateButton(quit_button, {g_resolution.x - 10, 10}, {30/g_scale , 30/g_scale}, RED, "X");
    quit_button.default_color = DARKRED;
    quit_button.text_size = 20/g_scale;

    LoadSpriteCentered(character_panel_sprite, g_ui_panels[PANEL_GRAY], g_screen_center);
    //LoadSprite(chatacter_sprite, g_sprite_sheets[g_player_data.sprite_sheet_id], g_screen_center, 4, 24.0f, 0.10f);
    LoadSpriteCentered(chatacter_sprite, g_sprite_sheets[0], {g_screen_center.x, g_screen_center.y-50}, 4, 24.0f, 0.10f);
    ScaleSprite(chatacter_sprite, {4,4});

     CreateButton(select_chatacter_button, {g_screen_center.x, g_screen_center.y+50}, {200/g_scale , 50/g_scale}, GREEN, "choose character");
    select_chatacter_button.default_color = DARKGREEN;
    select_chatacter_button.text_size = 20/g_scale; 

    CreateLabel(character_label,{g_screen_center.x, g_screen_center.y+25},20, RAYWHITE, g_player_data.class_name.c_str());


    CreateButton(character_left_button, {g_screen_center.x - 20 - character_panel_sprite.size.x/2 , g_screen_center.y}, {30/g_scale , 100/g_scale}, YELLOW, "<");
    character_left_button.default_color = DARKYELLOW;
    character_left_button.text_size = 20/g_scale; 

    CreateButton(character_right_button, {g_screen_center.x + 20 + character_panel_sprite.size.x/2 , g_screen_center.y}, {30/g_scale , 100/g_scale}, YELLOW, ">");
    character_right_button.default_color = DARKYELLOW;
    character_right_button.text_size = 20/g_scale; 

}

StagingUILayer::~StagingUILayer() {
    //UnloadSound(button_sound);
    TraceLog(LOG_INFO, "UI DESTRUCTOR:  STAGING UI");
}

void StagingUILayer::Draw() {
    DrawLabelCentered(title_label);

    DrawButton(start_button);
    DrawButton(settings_button);
    DrawButton(quit_button);
    
    DrawCircleV(g_screen_center, 2, RED);
    
    if(is_selecting) {
        
        DrawSprite(character_panel_sprite);
        DrawSprite(chatacter_sprite);
        DrawButton(select_chatacter_button);
        DrawLabelCentered(character_label);
        DrawButton(character_right_button);
        DrawButton(character_left_button);
    }
}

void StagingUILayer::Update() {

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


    if(is_selecting) {
        character_label.text = g_class_data[select_index].class_name;
        if(IsButtonHovered(select_chatacter_button, g_scale)){
            if(select_chatacter_button.already_hovered == false) {
                //PlaySound(button_sound);
            }
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                TraceLog(LOG_INFO, "CHARACTER SELECTED UI");
                //play_pressed.EmitSignal();
                character_selected.EmitSignal();
            }        
        }

        if(IsButtonHovered(character_left_button, g_scale)){
            if(character_left_button.already_hovered == false) {
                //PlaySound(button_sound);
            }
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                TraceLog(LOG_INFO, "PREVIOUS CHARACTER");
                //play_pressed.EmitSignal();
                character_left_pressed.EmitSignal();
            }        
        }

        if(IsButtonHovered(character_right_button, g_scale)){
            if(character_right_button.already_hovered == false) {
                //PlaySound(button_sound);
            }
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                TraceLog(LOG_INFO, "NEXT CHARACTER");
                //play_pressed.EmitSignal();
                character_right_pressed.EmitSignal();
            }        
        }
    }

}

