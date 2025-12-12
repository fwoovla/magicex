#include "../../core/gamedefs.h"

StagingUILayer::StagingUILayer() {
    
    CreateLabel(title_label, {g_screen_center.x, 20 / g_scale}, 40/g_scale, BLACK, "staging");
    
    CreateButton(settings_button, {g_resolution.x - 80, 10}, {120/g_scale , 30/g_scale}, YELLOW, "settings");
    settings_button.default_color = DARKYELLOW;
    settings_button.text_size = 20/g_scale;
    
    CreateButton(quit_button, {g_resolution.x - 10, 10}, {30/g_scale , 30/g_scale}, RED, "X");
    quit_button.default_color = DARKRED;
    quit_button.text_size = 20/g_scale;
    
    CreateButton(start_button, {g_screen_center.x, g_resolution.y - 40}, {200/g_scale , 100/g_scale}, GREEN, "go!");
    start_button.default_color = DARKGREEN;
    start_button.text_size = 40/g_scale;
    
    //=============character info===============
    cpo = {(float)g_screen_center.x * 0.30f, (float)g_screen_center.y * 0.5f};
    character_bounding_rect = {
        10,
        10,
        180,
        g_resolution.y-20
    };

    LoadSpriteCentered(character_panel_sprite, g_ui_panels[PANEL_WOOD_LARGE], cpo);
    LoadSpriteCentered(character_sprite, g_sprite_sheets[SPRITE_NERD], {cpo.x, cpo.y - 30}, 4, 16.0f, 0.10f);
    ScaleSprite(character_sprite, {3,3});

    CreateLabel(character_label,{cpo.x, cpo.y+25},20, RAYWHITE, g_class_data[SPRITE_NERD].class_name.c_str());
    CreateLabel(character_stat_label,{20, cpo.y+85},14, RAYWHITE, "");
    
    
    //=======================select panel=====================
    spo = {g_screen_center.x , g_screen_center.y - (100/g_scale) };

    select_index = 0;
    is_selecting = true;

    LoadSpriteCentered(select_character_panel_sprite, g_ui_panels[PANEL_WOOD_LARGE], spo);
    LoadSpriteCentered(select_chatacter_sprite, g_sprite_sheets[0], {spo.x, spo.y - 30}, 4, 16.0f, 0.10f);
    ScaleSprite(select_chatacter_sprite, {3,3});

    CreateButton(select_chatacter_button, {spo.x, g_resolution.y -50}, {200/g_scale , 50/g_scale}, GREEN, "choose character");
    select_chatacter_button.default_color = DARKGREEN;
    select_chatacter_button.text_size = 20/g_scale; 

    CreateLabel(select_character_label,{spo.x, spo.y+25},18, RAYWHITE, g_class_data[SPRITE_NERD].class_name.c_str());

    CreateButton(select_character_left_button, {spo.x - 20 - select_character_panel_sprite.size.x/2 , spo.y}, {30/g_scale , 100/g_scale}, YELLOW, "<");
    select_character_left_button.default_color = DARKYELLOW;
    select_character_left_button.text_size = 20/g_scale; 

    CreateButton(select_character_right_button, {spo.x + 20 + select_character_panel_sprite.size.x/2 , spo.y}, {30/g_scale , 100/g_scale}, YELLOW, ">");
    select_character_right_button.default_color = DARKYELLOW;
    select_character_right_button.text_size = 20/g_scale; 

//========================================================================

}

StagingUILayer::~StagingUILayer() {
    //UnloadSound(button_sound);
    TraceLog(LOG_INFO, "UI DESTRUCTOR:  STAGING UI");
}

void StagingUILayer::Draw() {
    DrawLabelCentered(title_label);

    DrawButton(settings_button);
    DrawButton(quit_button);
    
    //DrawCircleV(g_screen_center, 2, RED);
    
    if(is_selecting) {
        DrawSelectPanel();
        

    }
    else {
        DrawCharacterInfo();
    }
}

void StagingUILayer::Update() {

/*     if(!g_game_data.is_new_player){
        is_selecting = false;
    } */

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
        UpdateSelectPanel();
        
    }
    else {
        UpdateCharacterInfo();
        
    }

}

void StagingUILayer::UpdateSelectPanel() {
    select_character_label.text = g_class_data[select_index].class_name;
    character_label.text = g_class_data[select_index].class_name;

    if(IsButtonHovered(select_chatacter_button, g_scale)){
        if(select_chatacter_button.already_hovered == false) {
            //PlaySound(button_sound);
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            TraceLog(LOG_INFO, "CHARACTER SELECTED UI");
            is_selecting = false;
            //play_pressed.EmitSignal();
            character_selected.EmitSignal();
        }        
    }

    if(IsButtonHovered(select_character_left_button, g_scale)){
        if(select_character_left_button.already_hovered == false) {
            //PlaySound(button_sound);
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            TraceLog(LOG_INFO, "PREVIOUS CHARACTER");
            select_index--;
            if(select_index  < 0) {
                select_index = g_class_data.size() - 1;
            }
            //TraceLog(LOG_INFO, "index: %i", select_index);
            LoadSpriteCentered(select_chatacter_sprite, g_sprite_sheets[select_index], {spo.x, spo.y - 30}, 4, 16.0f, 0.10f);
            ScaleSprite(select_chatacter_sprite, {3,3});

            LoadSpriteCentered(character_sprite, g_sprite_sheets[select_index], {cpo.x, cpo.y - 30}, 4, 16.0f, 0.10f);
            ScaleSprite(character_sprite, {3,3});
            //play_pressed.EmitSignal();
            character_left_pressed.EmitSignal();
        }        
    }

    if(IsButtonHovered(select_character_right_button, g_scale)){
        if(select_character_right_button.already_hovered == false) {
            //PlaySound(button_sound);
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            TraceLog(LOG_INFO, "NEXT CHARACTER");
            select_index++;
            if(select_index  > g_class_data.size() - 1) {
                select_index = 0;
            }
            //TraceLog(LOG_INFO, "index: %i", select_index);
            LoadSpriteCentered(select_chatacter_sprite, g_sprite_sheets[select_index], {spo.x, spo.y - 30}, 4, 16.0f, 0.10f);
            ScaleSprite(select_chatacter_sprite, {3,3});

            LoadSpriteCentered(character_sprite, g_sprite_sheets[select_index], {cpo.x, cpo.y - 30}, 4, 16.0f, 0.10f);
            ScaleSprite(character_sprite, {3,3});
            //play_pressed.EmitSignal();
            character_right_pressed.EmitSignal();
        }        
    }
}

void StagingUILayer::UpdateCharacterInfo() {
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

    character_stat_label.text = TextFormat("health: %i\nspeed: %0.2f\nexp: %i", g_class_data[select_index].health, g_class_data[select_index].base_speed, g_class_data[select_index].exp);
}

void StagingUILayer::DrawSelectPanel() {
    DrawSprite(select_character_panel_sprite);
    DrawSprite(select_chatacter_sprite);
    DrawButton(select_chatacter_button);
    DrawLabelCentered(select_character_label);
    DrawButton(select_character_right_button);
    DrawButton(select_character_left_button);
}

void StagingUILayer::DrawCharacterInfo() {
    DrawRectangleRec(character_bounding_rect, DARKERGRAY);
    DrawRectangleLines( character_bounding_rect.x, character_bounding_rect.y,character_bounding_rect.width,character_bounding_rect.height, RAYWHITE);
    DrawSprite(character_panel_sprite);
    DrawSprite(character_sprite);
    DrawLabelCentered(character_label);
    DrawLabel(character_stat_label);
    DrawButton(start_button);
}