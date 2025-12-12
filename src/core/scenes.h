#pragma once
#include "basescene.h"

enum TrantitionType {

};

class FadeTransition{
    public:
        FadeTransition(float _durration);
        ~FadeTransition();
        void Update();
        void Draw();

        Label title_label;
        Timer transition_timer;

        float durration;
        float alpha_value;
        float alpha_step;
        int phase;

        Signal transition_ended;
        Signal transition_midpoint;
};

class SplashScreen : public  BaseScene{
    public:
        SplashScreen();
        ~SplashScreen() override;
        SCENE_ID Update() override;
        void Draw() override;
        void DrawScene() override;
        void DrawUI() override;

        Label title_label;

};

class TitleScene : public  BaseScene{
    public:
        TitleScene();
        ~TitleScene() override;
        SCENE_ID Update() override;
        void Draw() override;
        void DrawScene() override;
        void DrawUI() override;

        void OnPlayPressed();
        void OnSettingsPressed();
        void OnQuitPressed();
        void OnContinuePressed();

        Sprite bg_sprite_1;
        TitleUILayer *ui_layer;

};

class StagingScene : public  BaseScene{
    public:
        StagingScene();
        ~StagingScene() override;
        SCENE_ID Update() override;
        void Draw() override;
        void DrawScene() override;
        void DrawUI() override;

        void OnPlayPressed();
        void OnSettingsPressed();
        void OnQuitPressed();
        void OnCharacterSelected();
        void OnPreviousCharacter();
        void OnNextCharacter();

        
        //Sprite bg_sprite_1;

        Texture2D bg_texture;

        StagingUILayer *ui_layer;
};


class ShelterScene : public  BaseScene{
    public:
        ShelterScene();
        ~ShelterScene() override;
        SCENE_ID Update() override;
        void Draw() override;
        void DrawScene() override;
        void DrawUI() override;
        void HandleCamera();

        void OnQuitPressed();
        void OnStartPressed();
        void OnMapSelected();

        void OnContainerOpened();
        
        void OnModuleUsed();

        void OnTransitionAreaEntered();
        void OnTransitionAreaActivated();

        //ShelterUILayer *ui_layer;
        GameUILayer *ui_layer;

        CharacterMenu *character_menu;
        bool character_menu_visible;
    
        ModuleMenu *module_menu;
        bool module_menu_visible;

        MapMenu *map_menu;
        bool show_map_menu;

        TileLayer *tile_layer;

};

class SubScene : public  BaseScene{
    public:
        SubScene(LevelData *_level_data, bool is_new);
        ~SubScene() override;
        SCENE_ID Update() override;
        void Draw() override;
        void DrawScene() override;
        void DrawUI() override;
        void HandleCamera();

        void OnQuitPressed();

        void OnContainerOpened();

        void OnModuleUsed();

        void OnMapTransitionEntered();
        void OnMapTransitionActivated();

        CharacterMenu *character_menu;
        bool character_menu_visible;

        ModuleMenu *module_menu;
        bool module_menu_visible;

        GameUILayer *ui_layer;

        TileLayer *tile_layer;

        Signal sub_scene_exited;

};


class GameScene : public  BaseScene{
    public:
        GameScene();
        ~GameScene() override;
        SCENE_ID Update() override;
        void Draw() override;
        void DrawScene() override;
        void DrawUI() override;

        void HandleCamera();

        void OnQuitPressed();

        void OnContainerOpened();

        void OnModuleUsed();
        
        void OnMapTransitionEntered();
        void OnMapTransitionActivated();

        void OnShelterTransitionEntered();
        void OnShelterTransitionActivated();


        void OnHouseTransitionEntered();
        void OnHouseTransitionActivated();

        void OnSubSceneExited();


        CharacterMenu *character_menu;
        bool character_menu_visible;

        ModuleMenu *module_menu;
        bool module_menu_visible;

        GameUILayer *ui_layer;

        TileLayer *tile_layer;

        //std::unique_ptr<SubScene> sub_scene;
        bool can_delete_sub;

        Vector2 saved_player_position;

};


class EndScene : public  BaseScene{
    public:
        EndScene();
        ~EndScene() override;
        SCENE_ID Update() override;
        void Draw() override;
        void DrawScene() override;
        void DrawUI() override;

        EndUILayer *ui_layer;
        
};

//extern std::unordered_map <std::string, std::unique_ptr< SubScene> > g_sub_scene_instances;