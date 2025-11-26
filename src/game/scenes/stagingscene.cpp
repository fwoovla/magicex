#include "../../core/gamedefs.h"

StagingScene::StagingScene() {

    scene_id = TITLE_SCENE;
    return_scene = NO_SCENE;

    ui_layer = new StagingUILayer();
    
    ui_layer->play_pressed.Connect( [&](){OnPlayPressed();} );
    ui_layer->settings_pressed.Connect( [&](){OnSettingsPressed();} );
    ui_layer->quit_pressed.Connect( [&](){OnQuitPressed();} );
    ui_layer->character_selected.Connect( [&](){OnCharacterSelected();} );

    ui_layer->character_left_pressed.Connect( [&](){OnPreviousCharacter();} );
    ui_layer->character_right_pressed.Connect( [&](){OnNextCharacter();} );

    bg_texture = g_ui_backgrounds[BG_STAGING];


}


SCENE_ID StagingScene::Update() {

    ui_layer->Update();
    if(IsKeyPressed(KEY_SPACE)) {
        return_scene = GAME_SCENE;
    }
    return return_scene;
}

void StagingScene::Draw() {
}

void StagingScene::DrawScene() {
        DrawTexturePro(
        bg_texture,
        {0,0,(float)bg_texture.width, (float)bg_texture.height},
        {0,0,(float)g_resolution.x, (float)g_resolution.y},
        {0,0},
        0.0f,
        WHITE
    );
}

void StagingScene::DrawUI() {
    //DrawRectangle( 0,0, g_resolution.x, g_resolution.y, {0,0,0,0});
    ui_layer->Draw();
}

StagingScene::~StagingScene() {
    delete ui_layer;
    TraceLog(LOG_INFO, "SCENE DESTRUCTOR:  STAGING");
}

void StagingScene::OnPlayPressed() {

    TraceLog(LOG_INFO, "==========LOADING LDTK MAPS================");

    std::string ldtk_map_path = "assets/maps/ldtk/test.ldtk";
    int num_maps = load_ldtk_maps(ldtk_map_path);

    TraceLog(LOG_INFO, "==========END LOADING LDTK MAPS================  loaded %i maps", num_maps);

    g_game_data.next_map_index = g_game_data.shelter_map_index;
    return_scene = SHELTER_SCENE;
}

void StagingScene::OnSettingsPressed() {
}

void StagingScene::OnQuitPressed() {
    g_game_data.paused = true;
    //return_scene = SPLASH_SCENE;
}

void StagingScene::OnCharacterSelected() {
    TraceLog(LOG_INFO, "CHARACTER SELECTED");
    g_player_data = g_class_data[ui_layer->select_index];
    g_current_player = new PlayerCharacter({0,0});
}


void StagingScene::OnPreviousCharacter() {

}


void StagingScene::OnNextCharacter() {

}
