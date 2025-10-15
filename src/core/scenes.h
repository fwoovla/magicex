#include "basescene.h"

class SplashScreen : public  BaseScene{
    public:
        SplashScreen();
        ~SplashScreen() override;
        SCENE_ID Update() override;
        void Draw() override;

        void OnTimerTimeout();

        Label title_label;
        Timer splash_timer;

        float alpha_value;
        float alpha_step;
};

class TitleScene : public  BaseScene{
    public:
        TitleScene();
        ~TitleScene() override;
        SCENE_ID Update() override;
        void Draw() override;

        void OnPlayPressed();
        void OnSettingsPressed();
        void OnQuitPressed();
        void OnContinuePressed();

        TitleUILayer *ui_layer;

};

class StagingScene : public  BaseScene{
    public:
        StagingScene();
        ~StagingScene() override;
        SCENE_ID Update() override;
        void Draw() override;

        void OnPlayPressed();
        void OnSettingsPressed();
        void OnQuitPressed();
        void OnCharacterSelected();
        void OnPreviousCharacter();
        void OnNextCharacter();
        
        Sprite bg_sprite_1;

        StagingUILayer *ui_layer;
};

class GameScene : public  BaseScene{
    public:
        GameScene(int _map_index);
        ~GameScene() override;
        SCENE_ID Update() override;
        void Draw() override;

        void DrawLevel();
        void HandleCamera();

        CharacterMenu *character_menu;

        bool character_menu_visible;

        GameUILayer *ui_layer;

        TileLayer *tile_layer;

        //Texture2D ground_tiles;

        Texture2D unit_sprite_sheet;

        //Camera2D camera;

        //BaseUnit *unit1;

};

class EndScene : public  BaseScene{
    public:
        EndScene();
        ~EndScene() override;
        SCENE_ID Update() override;
        void Draw() override;

        EndUILayer *ui_layer;
        
};

