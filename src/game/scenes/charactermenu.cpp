#include "../../core/gamedefs.h"

CharacterMenu::CharacterMenu() {

    CreateLabel(title_label, {g_screen_center.x, 20 / g_scale}, 40/g_scale, BLACK, "CHARACTER MENU");

}

CharacterMenu::~CharacterMenu() {
    //UnloadSound(button_sound);
    TraceLog(LOG_INFO, "UI DESTRUCTOR:  CHARACTER MENU UI");
}

void CharacterMenu::Draw() {
    DrawRectangleRec({0,0,g_resolution.x,g_resolution.y}, TRANSDARKERGRAY);
    DrawLabelCentered(title_label);
}

void CharacterMenu::Update() {

}
