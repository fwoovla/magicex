#include "../../core/gamedefs.h"

#define MAX_ZOOM 2.0f
#define MIN_ZOOM 0.5f
#define ZOOM_STEP 0.05f



ShelterScene::ShelterScene() {
    scene_id = GAME_SCENE;
    return_scene = NO_SCENE;
   
    ui_layer = new ShelterUILayer();
    ui_layer->quit_pressed.Connect( [&](){OnQuitPressed();} );
    ui_layer->start_pressed.Connect( [&](){OnStartPressed();} );
    
    LoadSprite(bg_sprite_1, g_ui_backgrounds[BG_SHELTER], {0,0});
    
    map_menu = new MapMenu();
    map_menu->map_selected.Connect( [&](){OnMapSelected();} );

    show_map_menu = false;

}


SCENE_ID ShelterScene::Update() {


    
    if(show_map_menu == true) {
        map_menu->Update();
    }
    else {
        ui_layer->Update();
    }

    //DL_Update(active_entity_list);
    
    return return_scene;
}

void ShelterScene::Draw() {

    DrawRectangle( 0,0, g_resolution.x, g_resolution.y, DARKERGRAY ); 
    DrawSprite(bg_sprite_1);
    
    
    if(show_map_menu == true) {
        map_menu->Draw();
    }
    else {
        ui_layer->Draw();
    }

}





ShelterScene::~ShelterScene() {
    delete ui_layer;

    TraceLog(LOG_INFO, "SCENE DESTRUCTOR:  SHELTER");
}

void ShelterScene::OnQuitPressed() {
    g_game_data.paused = true;
}

void ShelterScene::OnStartPressed() {
    show_map_menu = true;
    //return_scene = GAME_SCENE;
}

void ShelterScene::OnMapSelected() {
    return_scene = GAME_SCENE;    
}