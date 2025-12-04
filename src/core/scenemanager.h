#pragma once

#include "gamedefs.h"

void InstanceLevelObjects(LevelData &level_data);

extern std::unique_ptr<BaseScene> g_current_scene;
extern std::unique_ptr<SubScene> g_sub_scene;

class SceneManager{
    public:
    void Init();
    void CleanUp();
    void UpdateScene();
    void DrawScene();
    void DrawUI();
    void ChangeSceneTo(SCENE_ID new_scene_id);
    void TransitionSceneTo(SCENE_ID new_scene_id);

    
    void OnPausePressed();
    void OnSavePressed();
    void OnBackToMenuPressed();

    void OnTransitionEnded();
    void OnTransitionMidpoint();

    /* std::unique_ptr<BaseScene> current_scene; */
    //BaseScene *current_scene;
    PauseMenu *pause_menu;

    FadeTransition *fade_transition;

    bool is_transitioning;

    SCENE_ID next_scene_id;

    Label debug_label;

        //bool paused;

    private:
};


/* void InstanceSavedObjects(LevelData &level_data);

void InstanceNewObjects(LevelData &level_data); */