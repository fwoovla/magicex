#include "../../core/gamedefs.h"

SplashScreen::SplashScreen() {

    scene_id = SPLASH_SCENE;
    return_scene = NO_SCENE;

    CreateLabel(title_label, {g_screen_center.x, 20 / g_scale}, 40/g_scale, BLACK, "splash");
}


SCENE_ID SplashScreen::Update() {

    return_scene = TITLE_SCENE;
    if(IsKeyPressed(KEY_SPACE)) {
    }
    return return_scene;
}

void SplashScreen::Draw() {
    DrawRectangle( 0,0, g_resolution.x, g_resolution.y, DARKERGRAY ); 
    DrawLabelCentered(title_label);
}

SplashScreen::~SplashScreen() {
    TraceLog(LOG_INFO, "SCENE DESTRUCTOR:  SPLASH");
}
