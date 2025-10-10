#pragma once

#include "gamedefs.h"

class SceneManager{
    public:
    void Init();
    void CleanUp();
    void UpdateScene();
    void DrawScene();
    void ChangeSceneTo(SCENE_ID new_scene);
    void OnPausePressed();
    void OnSavePressed();
    
    BaseScene *current_scene;
    PauseMenu *pause_menu;

    bool paused;

    private:
};