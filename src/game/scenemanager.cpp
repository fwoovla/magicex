#include "../core/gamedefs.h"

char level1_data[] = "assets/map4.png";

void SceneManager::UpdateScene()
{
    GetInputFromPlayer();
    if(current_scene == nullptr){
        return;
    }


    SetCursorPosition(g_input.screen_mouse_position / g_scale);
    //SetCursorPosition(g_input.screen_mouse_position);



    SCENE_ID new_scene_id = NO_SCENE;
        
    if(g_game_data.paused == false) {
        new_scene_id = current_scene->Update();
    }
    else {
        pause_menu->Update();
    }
    if(!is_transitioning) {
        switch (new_scene_id) {
            case NO_SCENE:
                break;

            default:
            TransitionSceneTo(new_scene_id);

            //ChangeSceneTo(new_scene_id);
        }
    }

    if(g_input.keys_pressed[0] == KEY_P) {
        g_game_data.paused = !g_game_data.paused;            }

    if(g_input.keys_pressed[0] == KEY_TAB) {
        g_game_settings.show_debug = !g_game_settings.show_debug;
    }

    if(is_transitioning) {
        fade_transition->Update();
    }

    if(g_game_settings.show_debug) {
        debug_label.text = TextFormat( "player pos ff %0.2f %0.2f \n player cell %i %i \n cti %i %i \ncamera target %f %f", 
        g_current_player->position.x, g_current_player->position.y,
        (int)(g_current_player->position.x / 16), (int)(g_current_player->position.y / 16),
        (int)g_camera.target.x, (int)g_camera.target.y,
        g_camera.target.x, g_camera.target.y
    );
    }


}


void SceneManager::Init() {
    current_scene = std::make_unique<SplashScreen>();

    g_game_data.current_scene_id = SPLASH_SCENE;

    is_transitioning = false;

    pause_menu = new PauseMenu();
    pause_menu->continue_pressed.Connect( [&](){OnPausePressed();} );
    pause_menu->save_pressed.Connect( [&](){OnSavePressed();} );
    pause_menu->back_to_menu_pressed.Connect( [&](){OnBackToMenuPressed();} );

    LoadSpriteCentered( g_cursor.sprite, g_sprite_sheets[SPRITE_CROSSHAIR], {0,0} );
    CreateLabel(debug_label, {20 / g_scale}, 40/g_scale, RAYWHITE, "DEBUG");
}


void SceneManager::CleanUp() {
    //delete current_scene;
    //delete pause_menu;
    TraceLog(LOG_INFO, "cleaning up scene manager");
}


void SceneManager::DrawScene() {

    current_scene->DrawScene();

}

void SceneManager::DrawUI() {

    current_scene->DrawUI();

    if(g_game_data.paused == true) {
        pause_menu->Draw();
    }
    DrawSprite(g_cursor.sprite);
    //BeginMode2D(g_camera);
    //EndMode2D();

    if(is_transitioning) {
        fade_transition->Draw();
    }
    if(g_game_settings.show_debug) {
        DrawLabel(debug_label);
    }
}


void SceneManager::ChangeSceneTo(SCENE_ID new_scene_id) {
    
    
    current_scene.reset();
    g_game_data.current_map_index = g_game_data.next_map_index;

    switch (new_scene_id) {
        case SPLASH_SCENE:
            current_scene = std::make_unique<SplashScreen>();
            break;

        case TITLE_SCENE:
            current_scene = std::make_unique<TitleScene>();
            break;

        case STAGING_SCENE:
            current_scene = std::make_unique<StagingScene>();
            break;

        case SHELTER_SCENE:
            current_scene = std::make_unique<ShelterScene>();
            break;

        case GAME_SCENE:
            current_scene = std::make_unique<GameScene>();
            break;

        case END_SCENE:
            current_scene = std::make_unique<EndScene>();
            break;

        default:
            break;
    }
    g_game_data.current_scene_id = new_scene_id;
}


void SceneManager::TransitionSceneTo(SCENE_ID new_scene_id) {
    next_scene_id = new_scene_id;
    is_transitioning = true;
    fade_transition = new FadeTransition(1);
    fade_transition->transition_ended.Connect( [&](){OnTransitionEnded();} );
    fade_transition->transition_midpoint.Connect( [&](){OnTransitionMidpoint();} );
}



void SceneManager::OnPausePressed() {
    g_game_data.paused = !g_game_data.paused;
}

void SceneManager::OnSavePressed() {
    SaveGame();
    TraceLog(LOG_INFO, "GAME SAVED ");
}

void SceneManager::OnBackToMenuPressed() {
    //SaveGame();
    if (g_current_player != nullptr) {
        delete g_current_player;
    }
    g_game_data.paused = false;
    g_game_data.next_map_index = g_game_data.shelter_map_index;
    g_game_data.is_in_sub_map = false;
    ChangeSceneTo(TITLE_SCENE);
}


void SceneManager::OnTransitionMidpoint() {
    TraceLog(LOG_INFO, "trans midpoint ");
    ChangeSceneTo(next_scene_id);
}

void SceneManager::OnTransitionEnded() {
    delete fade_transition;
    is_transitioning = false;
    TraceLog(LOG_INFO, "trans destroy ");
}



void InstanceLevelObjects(LevelData &level_data) {
    TraceLog(LOG_INFO, "instacing game objects");

//transition areas
    for(int t_index = 0; t_index < level_data.level_transitions.size(); t_index++) {
        Vector2 t_posisition = level_data.level_transitions[t_index].position_i;

        TransitionArea *new_area = new TransitionArea();

        new_area->identifier = level_data.level_transitions[t_index].identifier;
        new_area->position = t_posisition;
        new_area->size = {level_data.level_transitions[t_index].size.x, level_data.level_transitions[t_index].size.y};
        new_area->payload_s = level_data.level_transitions[t_index].dest_string;
        
        if(level_data.level_transitions[t_index].identifier == "HouseTransition") {
            new_area->payload_v = level_data.level_transitions[t_index].return_position;
        }

        for(int level_index = 0; level_index < g_ldtk_maps.levels.size(); level_index++) {
            if(g_ldtk_maps.levels[level_index].identifier == new_area->payload_s) {
                new_area->payload_i = level_index;
            }
        }

        //g_game_areas.push_back(new_area);
        //_draw_list.push_back(std::unique_ptr<BaseEntity>(new_entity));
        level_data.game_areas.push_back(new_area);
    }

//containers
    for(int c_index = 0; c_index < level_data.container_data.size(); c_index++) {

        

        Vector2 pos = level_data.container_data[c_index].position_i;        
        
        if(level_data.container_data[c_index].identifier == "PermContainerEntity") {
            TraceLog(LOG_INFO, "instacing container");
            int spi = level_data.container_data[c_index].sprite_id;
            int lti = level_data.container_data[c_index].loot_table_id;
            PermContainerEntity *new_container = new PermContainerEntity(pos, spi, lti);
            //g_level_data.containers.push_back(new_container);
            DL_Add(level_data.entity_list, new_container);
            
            new_container->identifier = level_data.container_data[c_index].identifier;
            new_container->c_area.identifier = level_data.container_data[c_index].identifier;
            new_container->c_area.position = pos;
            new_container->loot_table_id = level_data.container_data[c_index].loot_table_id;
            new_container->c_area.loot_table_id = level_data.container_data[c_index].loot_table_id;
            GenerateContainerItemList(new_container->loot_table_id, new_container->c_area.item_list);
            new_container->c_area.size = {level_data.container_data[c_index].size.x, level_data.container_data[c_index].size.y};
            
            //level_data.game_areas.push_back(&new_container->c_area);
        }


        if(level_data.container_data[c_index].identifier == "GroundContainerEntity") {
            int spi = g_item_data[ level_data.container_data[c_index].item_list[0]].sprite_id;
            GroundContainerEntity *new_container = new GroundContainerEntity(pos, spi);
            //g_level_data.containers.push_back(new_container);
            DL_Add(level_data.entity_list, new_container);
            
            new_container->identifier = level_data.container_data[c_index].identifier;
            new_container->c_area.identifier = level_data.container_data[c_index].identifier;
            new_container->c_area.position = pos;
            new_container->c_area.item_list = level_data.container_data[c_index].item_list;
            new_container->c_area.size = {level_data.container_data[c_index].size.x, level_data.container_data[c_index].size.y};
            
            TraceLog(LOG_INFO, "instacing ground container %i %s", spi, new_container->c_area.identifier.c_str());
            //level_data.game_areas.push_back(&new_container->c_area);
        }
    }

}
