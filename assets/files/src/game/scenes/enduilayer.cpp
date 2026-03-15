#include "../../core/gamedefs.h"

EndUILayer::EndUILayer() {

    CreateLabel(title_label, {g_screen_center.x, 20 / g_scale}, 40/g_scale, BLACK, "end");
}

EndUILayer::~EndUILayer() {
    //UnloadSound(button_sound);
    TraceLog(LOG_INFO, "UI DESTRUCTOR:  END UI");
}

void EndUILayer::Draw() {
    DrawLabelCentered(title_label);
}

void EndUILayer::Update() {

}