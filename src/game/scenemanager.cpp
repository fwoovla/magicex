#include "../core/gamedefs.h"

char level1_data[] = "assets/map4.png";

void SceneManager::UpdateScene()
{
    GetInputFromPlayer();
    if(current_scene == nullptr){
        return;
    }
    SCENE_ID new_scene = NO_SCENE;
    
    if(paused == false) {
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
        paused = !paused;    
    }
}


void SceneManager::Init() {
    current_scene = new SplashScreen();
    pause_menu = new PauseMenu();
    paused = false;
    pause_menu->continue_pressed.Connect( [&](){OnPausePressed();} );
    pause_menu->save_pressed.Connect( [&](){OnSavePressed();} );
    //current_scene = new GameScene(level1_data);
}

void SceneManager::CleanUp() {
    delete current_scene;
    delete pause_menu;
    TraceLog(LOG_INFO, "cleaning up scene manager");
}


void SceneManager::DrawScene() {

    current_scene->Draw();

    if(paused == true) {
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

        case GAME_SCENE:
            current_scene = new GameScene(level1_data);
            break;

        case END_SCENE:
            current_scene = new EndScene();
            break;

        default:
            break;
    }
}

void SceneManager::OnPausePressed() {
    paused = !paused;
}

void SceneManager::OnSavePressed() {
    SaveGame();
    TraceLog(LOG_INFO, "GAME SAVED ");
}