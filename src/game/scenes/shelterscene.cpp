#include "../../core/gamedefs.h"

#define MAX_ZOOM 2.0f
#define MIN_ZOOM 0.5f
#define ZOOM_STEP 0.05f



ShelterScene::ShelterScene() {
    ClearLevelData();
    LoadLevelData();

    scene_id = GAME_SCENE;
    return_scene = NO_SCENE;
   
    ui_layer = new ShelterUILayer();
    ui_layer->quit_pressed.Connect( [&](){OnQuitPressed();} );
    ui_layer->start_pressed.Connect( [&](){OnStartPressed();} );
    
    tile_layer = new TileLayer();
    //tile_layer->SetTiles( g_tile_sheets[TS_SHELTER], g_game_data.shelter_map_index);

    LoadSprite(bg_sprite_1, g_ui_backgrounds[BG_SHELTER], {0,0});
    
    map_menu = new MapMenu();
    map_menu->map_selected.Connect( [&](){OnMapSelected();} );

    show_map_menu = false;

    DL_Add(active_entity_list, g_current_player );
    g_current_player->position = g_level_data.spawn_position;

    g_camera = { 0 };
    g_camera.target = (Vector2){0,0};
    g_camera.offset = (Vector2){0,0};
    g_camera.rotation = 0.0f;
    g_camera.zoom = 1.5f; 
    g_world2screen = (g_scale * g_camera.zoom);

    

}


SCENE_ID ShelterScene::Update() {


    
    if(show_map_menu == true) {
        map_menu->Update();
    }
    else {
        ui_layer->Update();
    }

    //DL_Update(active_entity_list);
    DL_Update(active_entity_list);
    HandleCamera();

    return return_scene;
}

void ShelterScene::Draw() {

    DrawRectangle( 0,0, g_resolution.x, g_resolution.y, BLACK ); 
    //DrawSprite(bg_sprite_1);
    BeginMode2D(g_camera);
    tile_layer->Draw();
    DL_Draw(active_entity_list);

    EndMode2D();
    
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
    TraceLog(LOG_INFO, "MAP SELECTED:  %i", g_game_data.current_map_index);
}

void ShelterScene::HandleCamera() {

    Vector2 worldPosBeforeZoom = GetScreenToWorld2D(g_input.world_mouse_position, g_camera);

    g_camera.zoom += g_input.mouse_wheel * ZOOM_STEP;
    if(g_camera.zoom < MIN_ZOOM) {
        g_camera.zoom = MIN_ZOOM;
    }
    if(g_camera.zoom > MAX_ZOOM) {
        g_camera.zoom = MAX_ZOOM;
    }

    CalculateViewport();

    float x_offset_f = g_viewport.x_offset_f;
    float y_offset_f = g_viewport.y_offset_f;


    g_camera.target = Vector2Subtract(g_current_player->position, {x_offset_f, y_offset_f} );


}