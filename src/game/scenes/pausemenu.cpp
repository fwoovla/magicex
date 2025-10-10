#include "../../core/gamedefs.h"

PauseMenu::PauseMenu() {

    CreateLabel(title_label, {g_screen_center.x, 20 / g_scale}, 40/g_scale, BLACK, "PAUSED");

    CreateButton(continue_button, {g_screen_center.x, g_screen_center.y}, {150/g_scale , 60/g_scale}, BLUE, "continue");
    continue_button.default_color = DARKBLUE;
    continue_button.text_size = 40/g_scale;

    CreateButton(save_button, {g_screen_center.x, g_screen_center.y+(105/g_scale)}, {150/g_scale , 60/g_scale}, GREEN, "save game");
    save_button.default_color = DARKGREEN;
    save_button.text_size = 40/g_scale;

}

PauseMenu::~PauseMenu() {
    //UnloadSound(button_sound);
    TraceLog(LOG_INFO, "UI DESTRUCTOR:  TITLE UI");
}

void PauseMenu::Draw() {
    DrawRectangleRec({0,0,g_resolution.x,g_resolution.y}, TRANSDARKERGRAY);
    DrawLabelCentered(title_label);
    DrawButton(continue_button);
    DrawButton(save_button);
}

void PauseMenu::Update() {

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

    if(IsButtonHovered(save_button, g_scale)){
        if(save_button.already_hovered == false) {
            //PlaySound(button_sound);
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            TraceLog(LOG_INFO, "SAVE BUTTON PRESSED ");
            //play_pressed.EmitSignal();
            save_pressed.EmitSignal();
        }        
    }
}
