#pragma once

#include "gamedefs.h"

 


class SceneManager{
    public:
    void Init();
    void CleanUp();
    void UpdateScene();
    void DrawScene();
    void ChangeSceneTo(SCENE_ID new_scene);
    
    BaseScene *current_scene;

    private:
};