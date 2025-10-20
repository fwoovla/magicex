#include "../core/gamedefs.h"

char level1_data[] = "assets/map4.png";

void SceneManager::UpdateScene()
{
    GetInputFromPlayer();
    if(current_scene == nullptr){
        return;
    }
    SCENE_ID new_scene = NO_SCENE;
    
    if(g_game_data.paused == false) {
        new_scene = current_scene->Update();
    }
    else {
        pause_menu->Update();
    }

    switch (new_scene) {
        case NO_SCENE:
            break;

        default:
            ChangeSceneTo(new_scene);
    }

    if(g_input.keys_pressed[0] == KEY_P) {
        g_game_data.paused = !g_game_data.paused;    
    }
}


void SceneManager::Init() {
    current_scene = new SplashScreen();
    g_game_data.current_scene_id = SPLASH_SCENE;

    pause_menu = new PauseMenu();
    pause_menu->continue_pressed.Connect( [&](){OnPausePressed();} );
    pause_menu->save_pressed.Connect( [&](){OnSavePressed();} );
    pause_menu->back_to_menu_pressed.Connect( [&](){OnBackToMenuPressed();} );
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
}

void SceneManager::ChangeSceneTo(SCENE_ID new_scene) {
    
    
    delete current_scene;

    switch (new_scene) {
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
            current_scene = new GameScene(g_game_data.current_map_index);
            break;

        case END_SCENE:
            current_scene = new EndScene();
            break;

        default:
            break;
    }
    g_game_data.current_scene_id = new_scene;
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
    ChangeSceneTo(TITLE_SCENE);
}