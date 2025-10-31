#include "../core/gamedefs.h"

char level1_data[] = "assets/map4.png";

void SceneManager::UpdateScene()
{
    GetInputFromPlayer();
    if(current_scene == nullptr){
        return;
    }


    SetCursorPosition(g_input.screen_mouse_position / g_scale);



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


}


void SceneManager::Init() {
    current_scene = new SplashScreen();
    g_game_data.current_scene_id = SPLASH_SCENE;

    is_transitioning = false;

    pause_menu = new PauseMenu();
    pause_menu->continue_pressed.Connect( [&](){OnPausePressed();} );
    pause_menu->save_pressed.Connect( [&](){OnSavePressed();} );
    pause_menu->back_to_menu_pressed.Connect( [&](){OnBackToMenuPressed();} );

    LoadSpriteCentered( g_cursor.sprite, g_sprite_sheets[SPRITE_CROSSHAIR], {0,0} );
}


void SceneManager::CleanUp() {
    delete current_scene;
    delete pause_menu;
    TraceLog(LOG_INFO, "cleaning up scene manager");
}


void SceneManager::DrawScene() {

    current_scene->Draw();

    if(g_game_data.paused == true) {
        pause_menu->Draw();
    }
    DrawSprite(g_cursor.sprite);
    //BeginMode2D(g_camera);
    //EndMode2D();

    if(is_transitioning) {
        fade_transition->Draw();
    }
}

void SceneManager::ChangeSceneTo(SCENE_ID new_scene_id) {
    
    
    delete current_scene;
    g_game_data.current_map_index = g_game_data.next_map_index;

    switch (new_scene_id) {
        case SPLASH_SCENE:
            current_scene = new SplashScreen();
            break;

        case TITLE_SCENE:
            current_scene = new TitleScene();
            break;

        case STAGING_SCENE:
            current_scene = new StagingScene();
            break;

        case SHELTER_SCENE:
            current_scene = new ShelterScene();
            break;

        case GAME_SCENE:
            current_scene = new GameScene();
            break;

        case END_SCENE:
            current_scene = new EndScene();
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



void InstanceLevelObjects() {
    TraceLog(LOG_INFO, "instacing game objects");


    for(int t_index = 0; t_index < g_level_data.level_transitions.size(); t_index++) {
        Vector2 t_posisition = g_level_data.level_transitions[t_index].position_i;

        TransitionArea *new_area = new TransitionArea();

        new_area->identifier = g_level_data.level_transitions[t_index].identifier;
        new_area->position = t_posisition;
        new_area->size = {g_level_data.level_transitions[t_index].size.x, g_level_data.level_transitions[t_index].size.y};
        new_area->payload_s = g_level_data.level_transitions[t_index].dest_string;
        
        if(g_level_data.level_transitions[t_index].identifier == "HouseTransition") {
            new_area->payload_v = g_level_data.level_transitions[t_index].return_position;
        }

        for(int level_index = 0; level_index < g_ldtk_maps.levels.size(); level_index++) {
            if(g_ldtk_maps.levels[level_index].identifier == new_area->payload_s) {
                new_area->payload_i = level_index;
            }
        }

        //g_game_areas.push_back(new_area);
        g_level_data.game_areas.push_back(new_area);
    } 
}
