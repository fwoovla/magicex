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

class ItemGrid {
    public:
    ItemGrid(int c, int r, int s, Vector2 p);
    ~ItemGrid();
    void Update();
    void Draw();

    int rows;
    int cols;
    int grid_size;
    Vector2 position;

    bool cell_hovered;
    Vector2 hovered_cell;
};



class PauseMenu : public BaseUILayer {

    public:
    PauseMenu();
    ~PauseMenu() override;
    void Update() override;
    void Draw() override;

    Label title_label;

    Button back_to_menu_button;
    Signal back_to_menu_pressed;

    Button continue_button;
    Signal continue_pressed;

    Button save_button;
    Signal save_pressed;

};

class MapMenu : public BaseUILayer {

    public:
    MapMenu();
    ~MapMenu() override;
    void Update() override;
    void Draw() override;

    Label title_label;

    Rectangle panel_rect;

    Vector2 map_list_pos;
    std::vector<Button> map_buttons;

    Signal map_selected;

};

class CharacterMenu : public BaseUILayer {

    public:
    CharacterMenu();
    ~CharacterMenu() override;
    void Update() override;
    void Draw() override;

    Label title_label;

    Rectangle panel_rect;
    Texture2D panel_bg;

    Label ground_header_label;
    Label character_header_label;
    Label inventory_label_header;

    
    Vector2 gpo; // ground position offset
    ItemGrid *ground_grid;
    
    Vector2 cpo;
    Sprite character_panel_sprite;
    AnimatedSprite character_sprite;
    Label character_label;
    Label character_stat_label;
    ItemGrid *inventory_grid;
    //Rectangle ground_bounding_rect;
    //Sprite ground_panel_sprite;
    //AnimatedSprite ground_sprite;
    //Label character_label;
    //Label character_stat_label;

    Vector2 ipo;

};

class TitleUILayer : public BaseUILayer {

    public:
    TitleUILayer();
    ~TitleUILayer() override;
    void Update() override;
    void Draw() override;

    Label title_label;

    Button continue_button;
    Signal continue_pressed;

    Button start_button;
    Signal play_pressed;

    Button settings_button;
    Signal settings_pressed;

    Button quit_button;
    Signal quit_pressed;

};

class StagingUILayer : public BaseUILayer {

    public:
    StagingUILayer();
    ~StagingUILayer() override;
    
    void Update() override;
    void UpdateSelectPanel();
    void UpdateCharacterInfo();
    void Draw() override;
    void DrawSelectPanel();
    void DrawCharacterInfo();

    Label title_label;

    Button start_button;
    Signal play_pressed;

    Button settings_button;
    Signal settings_pressed;

    Button quit_button;
    Signal quit_pressed;

    //Rectangle character_selection_rect;

    Vector2 spo; //select_panel_offest
    Button select_character_left_button;
    Button select_character_right_button;
    Sprite select_character_panel_sprite;
    AnimatedSprite select_chatacter_sprite;
    Button select_chatacter_button;
    Label select_character_label;
    
    int select_index;
    bool is_selecting;
    Signal character_selected;
    Signal character_left_pressed;
    Signal character_right_pressed;

    Vector2 cpo;
    Rectangle character_bounding_rect;
    Sprite character_panel_sprite;
    AnimatedSprite character_sprite;
    Label character_label;
    Label character_stat_label;



    std::vector<UnitPortrait > portraits;

};

class ShelterUILayer : public BaseUILayer {

    public:
    ShelterUILayer();
    ~ShelterUILayer() override;
    void Update() override;
    void Draw() override;

    //void DrawDebug();
   
    Button start_button;
    Signal start_pressed;

    Button quit_button;
    Signal quit_pressed;

    Label title_label;
    //Label debug_label;

};


class GameUILayer : public BaseUILayer {

    public:
    GameUILayer();
    ~GameUILayer() override;
    void Update() override;
    void Draw() override;

    //void DrawDebug();

    Button quit_button;
    Signal quit_pressed;

    Label title_label;

    Label debug_zoom_level;



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
