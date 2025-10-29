#pragma once

#include "gamedefs.h"

void InstanceLevelObjects();



class SceneManager{
    public:
    void Init();
    void CleanUp();
    void UpdateScene();
    void DrawScene();
    void ChangeSceneTo(SCENE_ID new_scene);

    
    void OnPausePressed();
    void OnSavePressed();
    void OnBackToMenuPressed();


    BaseScene *current_scene;
    PauseMenu *pause_menu;

        //bool paused;

    private:
};