#include "../../core/gamedefs.h"

#define MAX_ZOOM 2.0f
#define MIN_ZOOM 0.5f
#define ZOOM_STEP 0.05f


GameScene::GameScene(int _map_index) {
    ClearLevelData();
    LoadLevelData();

    scene_id = GAME_SCENE;
    return_scene = NO_SCENE;
    character_menu_visible = false;

    tile_layer = new TileLayer();
    //tile_layer->SetTiles( g_tile_sheets[TS_FOREST], _map_index);
    
    ui_layer = new GameUILayer();
    ui_layer->quit_pressed.Connect( [&](){OnQuitPressed();} );


    character_menu = new CharacterMenu();

    //DL_Add(active_entity_list, g_current_player );
    g_current_player->position = g_level_data.spawn_position;
    //g_current_player->position = {100, 100};

    g_camera = { 0 };
    g_camera.target = (Vector2){0,0};
    g_camera.offset = (Vector2){0,0};
    g_camera.rotation = 0.0f;
    g_camera.zoom = 1.5f; 
    g_world2screen = (g_scale * g_camera.zoom);
    
    //UnloadImage(level_image);
}


SCENE_ID GameScene::Update() {
    //GetInputFromPlayer();
    if(character_menu_visible) {
        character_menu->Update();
    }

    ui_layer->Update();
    DL_Update(active_entity_list);
    HandleCamera();
    
/*     if(g_input.keys_pressed[0] == KEY_SPACE) {
        return_scene = END_SCENE;
    } */
    
    if(g_input.keys_pressed[0] == KEY_TAB) {
        g_game_settings.show_debug = !g_game_settings.show_debug;
    }

    if(g_input.keys_pressed[0] == KEY_E) {
        character_menu_visible = !character_menu_visible;
    }

    return return_scene;
}

void GameScene::Draw() {

    //TraceLog(LOG_INFO, "game scene draw");
    DrawRectangle( 0,0, g_resolution.x, g_resolution.y, DARKERGRAY ); 
    BeginMode2D(g_camera);
    tile_layer->Draw();
    //DrawLevel();
    DL_Draw(active_entity_list);
    
    
/*     if(g_input.selecting) {
        DrawRectangleLines(g_input.selected_rect.x, g_input.selected_rect.y, g_input.selected_rect.width, g_input.selected_rect.height, RAYWHITE);
    } */
    
    EndMode2D();
    
    ui_layer->Draw();
    if(character_menu_visible) {
        character_menu->Draw();
    }
}

void GameScene::DrawLevel() {

}

void GameScene::HandleCamera() {

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


    //int tile_size = g_ldtk_maps.default_grid_size;

    g_camera.target =  Vector2Subtract(g_current_player->position, {x_offset_f, y_offset_f} );

    if(g_current_player->position.x - x_offset_f < 0) {
        float x_dif = x_offset_f - g_current_player->position.x;
        //TraceLog(LOG_INFO, "x_dif %0.2f   %0.2f, %0.2f", x_dif, g_camera.target.x, g_camera.target.x);
        g_camera.target.x = g_camera.target.x + x_dif;
    }
    else if(g_current_player->position.x + x_offset_f > g_ldtk_maps.levels[g_game_data.current_map_index].px_wid) {
        float x_dif = (x_offset_f + g_current_player->position.x) - g_ldtk_maps.levels[g_game_data.current_map_index].px_wid;
        //TraceLog(LOG_INFO, "x_dif %0.2f   %0.2f, %0.2f", x_dif, g_camera.target.x, g_camera.target.y);
        g_camera.target.x = g_camera.target.x - x_dif;
    }

    if(g_current_player->position.y - y_offset_f < 0) {
        float y_dif = y_offset_f - g_current_player->position.y;
        //TraceLog(LOG_INFO, "y_dif %0.2f   %0.2f, %0.2f", y_dif, g_camera.target.y, g_camera.target.y);
        g_camera.target.y = g_camera.target.y + y_dif;
    }
    else if(g_current_player->position.y + y_offset_f > g_ldtk_maps.levels[g_game_data.current_map_index].px_hei) {
        float y_dif = (y_offset_f + g_current_player->position.y) - g_ldtk_maps.levels[g_game_data.current_map_index].px_hei;
        //TraceLog(LOG_INFO, "y_dif %0.2f   %0.2f, %0.2f", y_dif, g_camera.target.y, g_camera.target.y);
        g_camera.target.y = g_camera.target.y - y_dif;
    }

    
}


GameScene::~GameScene() {
    delete ui_layer;
    delete tile_layer;
    //delete level_data_array;
    //UnloadTexture(ground_tiles);
    DL_Clear(active_entity_list);
    TraceLog(LOG_INFO, "SCENE DESTRUCTOR:  GAME");
}

void GameScene::OnQuitPressed() {
    g_game_data.paused = true;
}
