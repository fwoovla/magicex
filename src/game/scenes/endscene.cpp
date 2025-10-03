#include "../../core/gamedefs.h"


EndScene::EndScene() {

    scene_id = GAME_SCENE;
    return_scene = NO_SCENE;

    ui_layer = new EndUILayer();
}


SCENE_ID EndScene::Update() {
    ui_layer->Update();
    if(IsKeyPressed(KEY_SPACE)) {
        return_scene = SPLASH_SCENE;
    }

    return return_scene;
}

void EndScene::Draw() {
    DrawRectangle( 0,0, g_game_settings.resolution.x, g_game_settings.resolution.y, DARKERGRAY ); 
    ui_layer->Draw();
}

EndScene::~EndScene() {
    delete ui_layer;
    TraceLog(LOG_INFO, "SCENE DESTRUCTOR:  END");
}