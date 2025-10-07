#pragma once
#include "gamedefs.h"


struct UnitPortrait{
    int id;
    Texture2D texture;
};

class BaseUILayer{
    public:
    virtual ~BaseUILayer(){};
    virtual void Update() = 0;
    virtual void Draw() = 0;
};


class TitleUILayer : public BaseUILayer {

    public:
    TitleUILayer();
    ~TitleUILayer() override;
    void Update() override;
    void Draw() override;

    Label title_label;
    Button start_button;
    Button settings_button;
    Button quit_button;

    Signal play_pressed;
    Signal settings_pressed;
    Signal quit_pressed;
};

class StagingUILayer : public BaseUILayer {

    public:
    StagingUILayer();
    ~StagingUILayer() override;
    
    void Update() override;
    void Draw() override;

    Label title_label;

    Button start_button;
    Button settings_button;
    Button quit_button;

    //Rectangle character_selection_rect;

    Vector2 spo; //select_panel_offest
    Button character_left_button;
    Button character_right_button;
    Sprite character_panel_sprite;
    AnimatedSprite chatacter_sprite;

    Button select_chatacter_button;
    Label character_label;
    
    int select_index;
    bool is_selecting;

    Signal play_pressed;
    Signal settings_pressed;
    Signal quit_pressed;
    Signal character_selected;
    Signal character_left_pressed;
    Signal character_right_pressed;

    std::vector<UnitPortrait > portraits;

};

class GameUILayer : public BaseUILayer {

    public:
    GameUILayer();
    ~GameUILayer() override;
    void Update() override;
    void Draw() override;

    void DrawDebug();

    Label title_label;

    Label debug_input_smp;
    Label debug_input_wmp;

    Label debug_input_mbl;
    Label debug_input_mbld;
    Label debug_input_mblscp;
    Label debug_input_mblwcp;

    Label debug_input_mbr;
    Label debug_input_mbrd;
    Label debug_input_mbrscp;
    Label debug_input_mbrwcp;

    Label debug_input_mw;

    Label debug_input_mm;
    Label debug_input_mmd;

    Label debug_scale;
    Label debug_zoom_level;
    Label debug_camera_target;
    Label debug_w2s;
    Label debug_selected_rect;


};


class EndUILayer : public BaseUILayer {

    public:
    EndUILayer();
    ~EndUILayer() override;
    
    void Update() override;
    void Draw() override;

    Label title_label;
};

/*  class GameMenu : public BaseUILayer {

    public:
    GameMenu();
    ~GameMenu() override;
    void Update() override;
    void Draw() override;
};

 class PartsMenu : public BaseUILayer {

    public:
    PartsMenu();
    ~PartsMenu() override;
    void Update() override;
    void Draw() override;
};

 class ShipyardMenu : public BaseUILayer {

    public:
    ShipyardMenu();
    ~ShipyardMenu() override;
    void Update() override;
    void Draw() override;
};







 */
