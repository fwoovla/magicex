#include "../../core/gamedefs.h"


TitleScene::TitleScene() {

    scene_id = TITLE_SCENE;
    return_scene = NO_SCENE;

    ui_layer = new TitleUILayer();
    ui_layer->play_pressed.Connect( [&](){OnPlayPressed();} );
    ui_layer->settings_pressed.Connect( [&](){OnSettingsPressed();} );
    ui_layer->quit_pressed.Connect( [&](){OnQuitPressed();} );
}


SCENE_ID TitleScene::Update() {

    ui_layer->Update();
    if(IsKeyPressed(KEY_SPACE)) {
        return_scene = STAGING_SCENE;
    }
    return return_scene;
}

void TitleScene::Draw() {
    DrawRectangle( 0,0, g_resolution.x, g_resolution.y, DARKERGRAY ); 
    ui_layer->Draw();

}

TitleScene::~TitleScene() {
    delete ui_layer;
    TraceLog(LOG_INFO, "SCENE DESTRUCTOR:  TITLE");
}

void TitleScene::OnPlayPressed() {
    return_scene = STAGING_SCENE;

}

void TitleScene::OnSettingsPressed() {
}

void TitleScene::OnQuitPressed() {
}