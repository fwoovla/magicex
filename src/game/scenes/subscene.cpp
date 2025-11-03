#include "../../core/gamedefs.h"

#define MAX_ZOOM 2.0f
#define MIN_ZOOM 0.80f
#define ZOOM_STEP 0.20f



SubScene::SubScene() {
    scene_id = SUB_SCENE;
    return_scene = NO_SCENE;


    ClearLevelData();
    LoadLevelData();
    InstanceLevelObjects();

    for(int area_index = 0; area_index < g_level_data.game_areas.size(); area_index++) {
        if(g_level_data.game_areas[area_index]->identifier == "LevelTransition") {
            TransitionArea* t_area = dynamic_cast<TransitionArea*>( g_level_data.game_areas[area_index]);
            //TraceLog(LOG_INFO, "+ connect map");
            t_area->area_entered.Connect( [&](){OnMapTransitionEntered();} );
            t_area->area_activated.Connect( [&](){OnMapTransitionActivated();} );
        }
/*         if(g_level_data.game_areas[area_index]->identifier == "LevelTransition") {
            g_level_data.game_areas[area_index]->entity_entered.Connect( [&](){OnMapTransitionEntered();} );
        } */
    }

    ui_layer = new GameUILayer();
    ui_layer->quit_pressed.Connect( [&](){OnQuitPressed();} );
    
    tile_layer = new TileLayer();

    //LoadSprite(bg_sprite_1, g_ui_backgrounds[BG_SHELTER], {0,0});
    
    g_current_player->position = g_level_data.spawn_position;

    g_camera = { 0 };
    g_camera.target = (Vector2){0,0};
    g_camera.offset = (Vector2){0,0};
    g_camera.rotation = 0.0f;
    g_camera.zoom = 2.4f; 
    g_world2screen = (g_scale * g_camera.zoom);
}


SCENE_ID SubScene::Update() {
    //TraceLog(LOG_INFO, "SUB SCENE UPDATE, %i", g_game_data.sub_map_index);
    ui_layer->Update();
    
    g_current_player->Update();
    //DL_Update(active_entity_list);
    for(int i = 0; i < g_level_data.game_areas.size(); i++) {
        g_level_data.game_areas[i]->Update();
    }
    HandleCamera();

    return return_scene;
}

void SubScene::Draw() {
    //TraceLog(LOG_INFO, "SUB SCENE DRAW");

    DrawRectangle( 0,0, g_resolution.x, g_resolution.y, BLACK ); 
    //DrawSprite(bg_sprite_1);
    BeginMode2D(g_camera);
    tile_layer->Draw();
    g_current_player->Draw();
    //DL_Draw(active_entity_list);
    
    for(int i = 0; i < g_level_data.game_areas.size(); i++) {
        g_level_data.game_areas[i]->Draw();
    }

    EndMode2D();

    ui_layer->Draw();
}


void SubScene::DrawScene() {
    //TraceLog(LOG_INFO, "SUB SCENE DRAW");

    //DrawRectangle( 0,0, g_resolution.x, g_resolution.y, BLACK ); 
    //DrawSprite(bg_sprite_1);
    BeginMode2D(g_camera);
    tile_layer->Draw();
    g_current_player->Draw();
    //DL_Draw(active_entity_list);
    
/*     for(int i = 0; i < g_level_data.game_areas.size(); i++) {
        g_level_data.game_areas[i]->Draw();
    } */

    EndMode2D();

    //ui_layer->Draw();
}


void SubScene::DrawUI() {
    //TraceLog(LOG_INFO, "SUB SCENE DRAW");
    //DrawRectangle( 0,0, g_resolution.x, g_resolution.y, BLACK ); 

    for(int i = 0; i < g_level_data.game_areas.size(); i++) {
        g_level_data.game_areas[i]->Draw();
    }
    ui_layer->Draw();

    /*     

    //DrawSprite(bg_sprite_1);
    BeginMode2D(g_camera);
    tile_layer->Draw();
    g_current_player->Draw();
    //DL_Draw(active_entity_list);
    


    EndMode2D(); */

}


SubScene::~SubScene() {
    delete ui_layer;
    delete tile_layer;

    DL_Clear(active_entity_list);
    //g_game_areas.clear();
    TraceLog(LOG_INFO, "SCENE DESTRUCTOR:  SUB SCENE");
}

void SubScene::OnQuitPressed() {
    g_game_data.paused = true;
}


void SubScene::OnMapTransitionEntered() {


    TraceLog(LOG_INFO, "SUB TRANSITION ACTIVATED:  %i", g_game_data.sub_map_index);
    //sub_scene_exited.EmitSignal();
    //return_scene = GAME_SCENE;

}

void SubScene::OnMapTransitionActivated() {


    TraceLog(LOG_INFO, "SUB TRANSITION ACTIVATED:  %i", g_game_data.sub_map_index);
    sub_scene_exited.EmitSignal();
    //return_scene = GAME_SCENE;

}


void SubScene::HandleCamera() {

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