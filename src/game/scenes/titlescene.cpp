#include "../../core/gamedefs.h"


TitleScene::TitleScene() {

    scene_id = TITLE_SCENE;
    return_scene = NO_SCENE;

    LoadSprite(bg_sprite_1, g_ldtk_tilesheets[3].texture, {0,0});
    //LoadSprite(bg_sprite_1, g_ui_backgrounds[BG_SHELTER], {0,0});

    auto sheet = g_ldtk_tilesheets.find(3);
    if( sheet != g_ldtk_tilesheets.end() ) {
        int id = sheet->second.uid;
        //TraceLog(LOG_INFO, "tilesheet id %i", id);
    }



    ui_layer = new TitleUILayer();
    ui_layer->play_pressed.Connect( [&](){OnPlayPressed();} );
    ui_layer->settings_pressed.Connect( [&](){OnSettingsPressed();} );
    ui_layer->quit_pressed.Connect( [&](){OnQuitPressed();} );
    ui_layer->continue_pressed.Connect( [&](){OnContinuePressed();} );
}


SCENE_ID TitleScene::Update() {

    ui_layer->Update();
    if(IsKeyPressed(KEY_SPACE)) {
        return_scene = STAGING_SCENE;
    }
    return return_scene;
}

void TitleScene::Draw() {
    DrawRectangle( 0,0, g_resolution.x, g_resolution.y, BLACK ); 
    DrawSprite(bg_sprite_1);
    ui_layer->Draw();

}

TitleScene::~TitleScene() {
    delete ui_layer;
    TraceLog(LOG_INFO, "SCENE DESTRUCTOR:  TITLE");
}

void TitleScene::OnPlayPressed() {

    return_scene = STAGING_SCENE;
    //g_game_data.current_map_index = 0;

}

void TitleScene::OnSettingsPressed() {
}

void TitleScene::OnQuitPressed() {
    
}

void TitleScene::OnContinuePressed() {
    LoadGame();
    g_current_player = new PlayerCharacter({0,0});
    //DL_Add(active_entity_list, g_current_player );
    //g_game_data.is_new_player = false;
    return_scene = SHELTER_SCENE;
    g_game_data.current_map_index = g_game_data.shelter_map_index;
}