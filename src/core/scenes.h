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

        
        Sprite bg_sprite_1;

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

        void OnTransitionAreaEntered();
        void OnTransitionAreaActivated();


        Sprite bg_sprite_1;

        ShelterUILayer *ui_layer;

        TileLayer *tile_layer;

        MapMenu *map_menu;

        bool show_map_menu;

};

class SubScene : public  BaseScene{
    public:
        SubScene();
        ~SubScene() override;
        SCENE_ID Update() override;
        void Draw() override;
        void DrawScene() override;
        void DrawUI() override;
        void HandleCamera();

        void OnQuitPressed();

        void OnMapTransitionEntered();
        void OnMapTransitionActivated();

        Sprite bg_sprite_1;

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

        void OnMapTransitionEntered();
        void OnMapTransitionActivated();

        void OnShelterTransitionEntered();
        void OnShelterTransitionActivated();


        void OnHouseTransitionEntered();
        void OnHouseTransitionActivated();

        void OnSubSceneExited();


        CharacterMenu *character_menu;

        bool character_menu_visible;

        GameUILayer *ui_layer;

        TileLayer *tile_layer;

        SubScene *sub_scene;

        Vector2 saved_player_position;

        LevelData return_level_data;

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

