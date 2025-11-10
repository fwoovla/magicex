#include "../../core/gamedefs.h"

#define MAX_ZOOM 2.4f
#define MIN_ZOOM 1.80f
#define ZOOM_STEP 0.20f


GameScene::GameScene() {

    scene_id = GAME_SCENE;
    return_scene = NO_SCENE;
    character_menu_visible = false;

    sub_scene = nullptr;


    //ClearLevelData(level_data);
    LoadLevelData(level_data);
    InstanceLevelObjects(level_data);


    for(int area_index = 0; area_index < level_data.game_areas.size(); area_index++) {
        if(level_data.game_areas[area_index]->identifier == "LevelTransition") {
            TransitionArea* t_area = dynamic_cast<TransitionArea*>(level_data.game_areas[area_index]);
            //TraceLog(LOG_INFO, "+ connect map");
            t_area->area_entered.Connect( [this](){OnMapTransitionEntered();} );
            t_area->area_activated.Connect( [this](){OnMapTransitionActivated();} );
        }

        if(level_data.game_areas[area_index]->identifier == "ShelterTransition") {
            TransitionArea* t_area = dynamic_cast<TransitionArea*>(level_data.game_areas[area_index]);
            //TransitionArea* t_area = dynamic_cast<TransitionArea*>( g_level_data.game_areas[area_index]);
            //TraceLog(LOG_INFO, "+ connect shelter");
            t_area->area_entered.Connect( [this](){OnShelterTransitionEntered();} );
            t_area->area_activated.Connect( [this](){OnShelterTransitionActivated();} );
        }
        if(level_data.game_areas[area_index]->identifier == "HouseTransition") {
            TransitionArea* t_area = dynamic_cast<TransitionArea*>(level_data.game_areas[area_index]);
            //TraceLog(LOG_INFO, "+ connect house");

            t_area->area_entered.Connect( [this](){OnHouseTransitionEntered();} );
            t_area->area_activated.Connect( [this](){OnHouseTransitionActivated();} );
        }
        if(level_data.game_areas[area_index]->identifier == "ContainerEntity") {
            TraceLog(LOG_INFO, "container area identified");
            ContainerArea* c_area = dynamic_cast<ContainerArea*>(level_data.game_areas[area_index]);
            c_area->area_activated.Connect( [this](){OnContainerOpened();} );
        }

    }

    tile_layer = new TileLayer();
    
    ui_layer = new GameUILayer();
    ui_layer->quit_pressed.Connect( [this](){OnQuitPressed();} );


    character_menu = new CharacterMenu();
    character_menu->Open();

    g_current_player->position = level_data.spawn_position;

    g_camera = { 0 };
    g_camera.target = (Vector2){0,0};
    g_camera.offset = (Vector2){0,0};
    g_camera.rotation = 0.0f;
    g_camera.zoom = 2.0f; 
    g_world2screen = (g_scale * g_camera.zoom);
}


SCENE_ID GameScene::Update() {
    //TraceLog(LOG_INFO, " UPDATE");
    if(can_delete_sub) {
        delete sub_scene;
        sub_scene = nullptr;
        can_delete_sub = false;
    }

    if(g_game_data.is_in_sub_map) {
        sub_scene->Update();
    }
    else {
        if(character_menu_visible) {
            character_menu->Update();
        }
        else {
            //TraceLog(LOG_INFO, "GAME SCENE UPDATE");
            for(int i = 0; i < level_data.game_areas.size(); i++) {
                level_data.game_areas[i]->Update();
            }
            DL_Update(level_data.entity_list);
            ui_layer->Update();
            g_current_player->Update();
            HandleCamera();
        }
    }


    if(g_input.keys_pressed[0] == KEY_E) {
        character_menu_visible = !character_menu_visible;
        if(character_menu_visible) {
                std::vector<int> list;
                list.push_back(1);

                character_menu->Open();
            }
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
        DL_Draw(level_data.entity_list);
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
            for(int i = 0; i < level_data.game_areas.size(); i++) {
                level_data.game_areas[i]->Draw();
            }
            ui_layer->Draw();
            character_menu->DrawHotBarOnly();
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

    //DL_Clear(entity_draw_list);
    
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

}

void GameScene::OnHouseTransitionActivated() {

    TraceLog(LOG_INFO, "SUB MAP TRANSITION ACTIVATED:  %i", g_game_data.sub_map_index);

    g_game_data.is_in_sub_map = true;
    sub_scene = new SubScene();
    sub_scene->sub_scene_exited.Connect( [this](){OnSubSceneExited();} );

}

void GameScene::OnSubSceneExited() {
    TraceLog(LOG_INFO, "SUB MAP EXITED %i", g_game_data.current_map_index);
    g_game_data.is_in_sub_map = false;

    g_current_player->position = g_game_data.sub_return_position;
    TraceLog(LOG_INFO, "+ reset player position");

    g_camera.zoom = 1.0f; 
    
    character_menu->Open();

    can_delete_sub = true;
}


void GameScene::OnContainerOpened() {
    if(character_menu_visible) {
        return;
    }
    TraceLog(LOG_INFO, "CONTAINER OPEN:  %i", g_game_data.loot_table_id);
    character_menu->OpenWith(*g_game_data.loot_table);
    character_menu_visible = true;
}