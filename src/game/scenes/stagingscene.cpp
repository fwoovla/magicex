#include "../../core/gamedefs.h"



//std::vector<BaseEntity *> recruit_unit_list;
//BaseEntity *active_unit_list[DRAW_LIST_SIZE] = {nullptr};
//BaseEntity *recruit_unit_list[DRAW_LIST_SIZE] = {nullptr};


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
    //ui_layer->advertize_pressed.Connect( [&](){OnAdvertizePressed();} );


    LoadSprite(bg_sprite_1, g_ui_backgrounds[BG_STAGING], {0,0});

    //active_unit_list.push_back(new BaseUnit({20,0}, units_data[0]));
    //active_unit_list.push_back(new BaseUnit({20,50}, units_data[0]));
    //active_unit_list.push_back(new BaseUnit({20,100}, units_data[0]));
    //active_unit_list.push_back(new BaseUnit({20,150}, units_data[0]));


}


SCENE_ID StagingScene::Update() {

    ui_layer->Update();
    if(IsKeyPressed(KEY_SPACE)) {
        return_scene = GAME_SCENE;
    }
    return return_scene;
}

void StagingScene::Draw() {
    DrawRectangle( 0,0, g_resolution.x, g_resolution.y, BLACK);
    DrawSprite(bg_sprite_1);
    //DL_Draw(active_unit_list);
    ui_layer->Draw();
}

StagingScene::~StagingScene() {
    delete ui_layer;
    //UnloadTexture(bg_sprite_1.texture);
    TraceLog(LOG_INFO, "SCENE DESTRUCTOR:  STAGING");
}

void StagingScene::OnPlayPressed() {
    return_scene = GAME_SCENE;
}

void StagingScene::OnSettingsPressed() {
}

void StagingScene::OnQuitPressed() {
    return_scene = SPLASH_SCENE;
}

void StagingScene::OnCharacterSelected() {
    TraceLog(LOG_INFO, "CHARACTER SELECTED");
    g_player_data = g_class_data[ui_layer->select_index];
}


void StagingScene::OnPreviousCharacter() {

}


void StagingScene::OnNextCharacter() {

}
