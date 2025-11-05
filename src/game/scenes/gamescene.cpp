#include "../../core/gamedefs.h"

#define MAX_ZOOM 2.4f
#define MIN_ZOOM 1.80f
#define ZOOM_STEP 0.20f


GameScene::GameScene() {

    scene_id = GAME_SCENE;
    return_scene = NO_SCENE;
    character_menu_visible = false;

    sub_scene = nullptr;




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

        if(g_level_data.game_areas[area_index]->identifier == "ShelterTransition") {
            TransitionArea* t_area = dynamic_cast<TransitionArea*>( g_level_data.game_areas[area_index]);
            //TraceLog(LOG_INFO, "+ connect shelter");
            t_area->area_entered.Connect( [&](){OnShelterTransitionEntered();} );
            t_area->area_activated.Connect( [&](){OnShelterTransitionActivated();} );
        }
        if(g_level_data.game_areas[area_index]->identifier == "HouseTransition") {
            TransitionArea* t_area = dynamic_cast<TransitionArea*>( g_level_data.game_areas[area_index]);
            //TraceLog(LOG_INFO, "+ connect house");

            t_area->area_entered.Connect( [&](){OnHouseTransitionEntered();} );
            t_area->area_activated.Connect( [&](){OnHouseTransitionActivated();} );
        }

    }

    tile_layer = new TileLayer();
    
    ui_layer = new GameUILayer();
    ui_layer->quit_pressed.Connect( [&](){OnQuitPressed();} );


    character_menu = new CharacterMenu();

    g_current_player->position = g_level_data.spawn_position;

    g_camera = { 0 };
    g_camera.target = (Vector2){0,0};
    g_camera.offset = (Vector2){0,0};
    g_camera.rotation = 0.0f;
    g_camera.zoom = 2.0f; 
    g_world2screen = (g_scale * g_camera.zoom);
}


SCENE_ID GameScene::Update() {
    //TraceLog(LOG_INFO, " UPDATE");

    if(g_game_data.is_in_sub_map) {
        sub_scene->Update();
    }
    else {
        if(character_menu_visible) {
            character_menu->Update();
        }
        else {
            //TraceLog(LOG_INFO, "GAME SCENE UPDATE");
            DL_Update(active_entity_list);
            ui_layer->Update();
            for(int i = 0; i < g_level_data.game_areas.size(); i++) {
                g_level_data.game_areas[i]->Update();
            }
            //UpdateGameAreas();
            g_current_player->Update();
            HandleCamera();
        }
    }


    if(g_input.keys_pressed[0] == KEY_E) {
        character_menu_visible = !character_menu_visible;
    }

    return return_scene;
}

void GameScene::Draw() {
}

void GameScene::DrawScene() {

    if(g_game_data.is_in_sub_map) {
        //TraceLog(LOG_INFO, "SUB GAME SCENE DRAW");
        sub_scene->DrawScene();
    }
    else {
        BeginMode2D(g_camera);
        tile_layer->Draw();
        DL_Draw(active_entity_list);
        g_current_player->Draw();
        EndMode2D();
    }    
}

void GameScene::DrawUI() {

    if(g_game_data.is_in_sub_map) {
        //TraceLog(LOG_INFO, "SUB GAME SCENE DRAW");
        sub_scene->DrawUI();
    }
    else {
        if(character_menu_visible) {
            character_menu->Draw();
        }
        else {
            for(int i = 0; i < g_level_data.game_areas.size(); i++) {
                g_level_data.game_areas[i]->Draw();
            }

            ui_layer->Draw();
        }
    }    
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


    g_camera.target =  Vector2Subtract(g_current_player->position, {x_offset_f, y_offset_f} );

    if(g_current_player->position.x - x_offset_f < 0) {
        float x_dif = x_offset_f - g_current_player->position.x;
        //TraceLog(LOG_INFO, "x_dif %0.2f   %0.2f, %0.2f", x_dif, g_camera.target.x, g_camera.target.x);
        g_camera.target.x = g_camera.target.x + x_dif;
        g_camera.target.x = (int)g_camera.target.x;
    }
    else if(g_current_player->position.x + x_offset_f > g_ldtk_maps.levels[g_game_data.current_map_index].px_wid) {
        float x_dif = (x_offset_f + g_current_player->position.x) - g_ldtk_maps.levels[g_game_data.current_map_index].px_wid;
        //TraceLog(LOG_INFO, "x_dif %0.2f   %0.2f, %0.2f", x_dif, g_camera.target.x, g_camera.target.y);
        g_camera.target.x = (int)g_camera.target.x;
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
        g_camera.target.y = (int)g_camera.target.y;

    }

    
}


GameScene::~GameScene() {
    delete ui_layer;
    delete tile_layer;

    DL_Clear(active_entity_list);
    
    TraceLog(LOG_INFO, "SCENE DESTRUCTOR:  GAME");
}

void GameScene::OnQuitPressed() {
    g_game_data.paused = true;
}


void GameScene::OnMapTransitionEntered() {

    TraceLog(LOG_INFO, "TRANSITION ACTIVATED:  %i", g_game_data.current_map_index);
    //return_scene = GAME_SCENE;

}

void GameScene::OnMapTransitionActivated() {

    TraceLog(LOG_INFO, "TRANSITION ACTIVATED:  %i", g_game_data.current_map_index);
    return_scene = GAME_SCENE;

}

void GameScene::OnShelterTransitionEntered() {

    TraceLog(LOG_INFO, "TRANSITION ACTIVATED:  %i", g_game_data.current_map_index);
    //return_scene = SHELTER_SCENE;

}

void GameScene::OnShelterTransitionActivated() {

    TraceLog(LOG_INFO, "TRANSITION ACTIVATED:  %i", g_game_data.current_map_index);
    return_scene = SHELTER_SCENE;

}

void GameScene::OnHouseTransitionEntered() {

    TraceLog(LOG_INFO, "SUB MAP TRANSITION ACTIVATED:  %i", g_game_data.sub_map_index);

/*     g_game_data.is_in_sub_map = true;
    return_level_data = g_level_data;
    sub_scene = new SubScene();
    sub_scene->sub_scene_exited.Connect( [&](){OnSubSceneExited();} ); */

}

void GameScene::OnHouseTransitionActivated() {

    TraceLog(LOG_INFO, "SUB MAP TRANSITION ACTIVATED:  %i", g_game_data.sub_map_index);

    g_game_data.is_in_sub_map = true;
    return_level_data = g_level_data;
    sub_scene = new SubScene();
    sub_scene->sub_scene_exited.Connect( [&](){OnSubSceneExited();} );

}

void GameScene::OnSubSceneExited() {

    TraceLog(LOG_INFO, "SUB MAP EXITED %i", g_game_data.current_map_index);
    g_game_data.is_in_sub_map = false;

    ClearLevelData();
    g_level_data = return_level_data;

    g_current_player->position = g_game_data.sub_return_position;
    TraceLog(LOG_INFO, "+ reset player position");

    g_camera.zoom = 1.0f; 
    
    delete sub_scene;
    sub_scene = nullptr;

    
}
