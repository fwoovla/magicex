#pragma once

#include "gamedefs.h"

void InstanceLevelObjects();



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


    BaseScene *current_scene;
    PauseMenu *pause_menu;

    FadeTransition *fade_transition;

    bool is_transitioning;

    SCENE_ID next_scene_id;

    Label debug_label;

        //bool paused;

    private:
};