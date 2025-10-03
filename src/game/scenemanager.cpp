#include "../core/gamedefs.h"

char level1_data[] = "assets/map4.png";

void SceneManager::UpdateScene()
{
    if(current_scene == nullptr){
        return;
    }
    SCENE_ID new_scene = NO_SCENE;
    
    new_scene = current_scene->Update();
    switch (new_scene) {
        case NO_SCENE:
            break;

        default:
            ChangeSceneTo(new_scene);
    }
}


void SceneManager::Init() {
    current_scene = new SplashScreen();
    //current_scene = new GameScene(level1_data);
}

void SceneManager::CleanUp() {
    delete current_scene;
    TraceLog(LOG_INFO, "cleaning up scene manager");
}


void SceneManager::DrawScene() {
    current_scene->Draw();
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
