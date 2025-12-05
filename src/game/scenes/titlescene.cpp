#include "../../core/gamedefs.h"


TitleScene::TitleScene() {

    scene_id = TITLE_SCENE;
    return_scene = NO_SCENE;

    //LoadSprite(bg_sprite_1, g_ui_backgrounds[BG_TITLE], {0,0});
    //ScaleSprite(bg_sprite_1, {2,2});


    ui_layer = new TitleUILayer();
    ui_layer->play_pressed.Connect( [&](){OnPlayPressed();} );
    ui_layer->settings_pressed.Connect( [&](){OnSettingsPressed();} );
    ui_layer->quit_pressed.Connect( [&](){OnQuitPressed();} );
    ui_layer->continue_pressed.Connect( [&](){OnContinuePressed();} );

    HideCursor();
}


SCENE_ID TitleScene::Update() {

    ui_layer->Update();
    if(IsKeyPressed(KEY_SPACE)) {
        return_scene = STAGING_SCENE;
    }
    return return_scene;
}

void TitleScene::Draw() {

}


void TitleScene::DrawScene() {
        Rectangle source = {
        .x = 0.0f,
        .y = 0.0f,
        .width = (float)g_ui_backgrounds[BG_TITLE].width,
        .height = (float)g_ui_backgrounds[BG_TITLE].height
    };

    Rectangle dest = {
        .x = 0.0f,
        .y = 0.0f,
        .width = (float)g_resolution.x,
        .height = (float)g_resolution.y
    };

    DrawTexturePro(g_ui_backgrounds[BG_TITLE], source, dest,{0.0f,0.0f}, 0.0f, WHITE);
    //DrawSprite(bg_sprite_1);

}


void TitleScene::DrawUI() {
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
    ToggleFullscreen();
}

void TitleScene::OnQuitPressed() {
    
}

void TitleScene::OnContinuePressed() {

    LoadGame();

    g_current_player = new PlayerCharacter({0,0});
    return_scene = SHELTER_SCENE;
    g_game_data.next_map_index = g_game_data.shelter_map_index;
}