#include "../../core/gamedefs.h"

SplashScreen::SplashScreen() {

    double wait_time = 1.0;

    scene_id = SPLASH_SCENE;
    return_scene = NO_SCENE;

    CreateLabel(title_label, {g_screen_center.x, 20 / g_scale}, 40/g_scale, BLACK, "splash");
    splash_timer.Start(wait_time, true);
    splash_timer.timer_timeout.Connect( [&](){OnTimerTimeout();} );
    alpha_value = 0;  
    alpha_step = 255/wait_time;

}


SCENE_ID SplashScreen::Update() {
    splash_timer.Update();
    alpha_value += (float)alpha_step * GetFrameTime();
    if(IsKeyPressed(KEY_SPACE)) {
        return_scene = TITLE_SCENE;
    }
    return return_scene;
}

void SplashScreen::Draw() {
    DrawRectangle( 0,0, g_resolution.x, g_resolution.y, DARKERGRAY ); 
    //DrawTexture(g_sprites[SPRITE_BASE_CHAR], 0, 0, WHITE);
    DrawLabelCentered(title_label);
    DrawRectangle( 0,0, g_game_settings.resolution.x, g_game_settings.resolution.y, {0, 0, 0, (unsigned char)alpha_value} );
}

SplashScreen::~SplashScreen() {
    TraceLog(LOG_INFO, "SCENE DESTRUCTOR:  SPLASH");
}

void SplashScreen::OnTimerTimeout() {
    return_scene = TITLE_SCENE;
}
