#include "../../core/gamedefs.h"

StagingUILayer::StagingUILayer() {

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

    CreateButton(add_to_team_button, {50, 10}, {100/g_scale , 50/g_scale}, GREEN, "add to squad");
    add_to_team_button.default_color = DARKGREEN;
    add_to_team_button.text_size = 20/g_scale;

    CreateButton(advertize_button, {50, g_resolution.y-50}, {100/g_scale , 50/g_scale}, YELLOW, "advertize");
    advertize_button.default_color = DARKYELLOW;
    advertize_button.text_size = 20/g_scale;

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
    DrawButton(add_to_team_button);
    DrawButton(advertize_button);

    DrawPortraits();
}

void StagingUILayer::Update() {
    PopulatePortraits();

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

    if(IsButtonHovered(add_to_team_button, g_scale)){
        if(add_to_team_button.already_hovered == false) {
            //PlaySound(button_sound);
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            TraceLog(LOG_INFO, "add to team BUTTON PRESSED ");
            //play_pressed.EmitSignal();
            add_to_team_pressed.EmitSignal();
        }        
    }

    if(IsButtonHovered(advertize_button, g_scale)){
        if(advertize_button.already_hovered == false) {
            //PlaySound(button_sound);
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            TraceLog(LOG_INFO, "advertize BUTTON PRESSED ");
            //play_pressed.EmitSignal();
            advertize_pressed.EmitSignal();
        }        
    }
}

void StagingUILayer::PopulatePortraits() {

    if(active_unit_list.size() == portraits.size()) {
        //TraceLog(LOG_INFO, " ALL PORTRAITS MADE: %i", portraits.size());
        return;
    }

    portraits.clear();

    for(int i = 0; i < active_unit_list.size(); i++) {

        BaseUnit *unit =  dynamic_cast<BaseUnit *>(active_unit_list[i]);

        UnitPortrait new_portrait {
            .id = unit->data.id,
            .texture = g_portrait_sprites[unit->data.portrait_id]
        };

        portraits.push_back(new_portrait);
        TraceLog(LOG_INFO, "PORTRAITS: %i", portraits.size());
    }

}

void StagingUILayer::DrawPortraits() {
    for(int i = 0; i < portraits.size(); i++) {
        DrawTextureRec(portraits[i].texture, {0,0, 64, 64}, {200, 50.0f + (i* 50)}, WHITE);
    }
}
