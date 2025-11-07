#pragma once
#include "gamedefs.h"

enum INVENTORYGRIDS {
    NONE,
    GROUND,
    INVENTORY,
    HOTBAR
};


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

struct SharedItemData {
    int item_id;
    //std::vector<int> *source_list;
    //std::vector<int> *dest_list;
    INVENTORYGRIDS source_grid;
    INVENTORYGRIDS dest_grid;
    Vector2 source_cell;
    Vector2 dest_cell;
};

class ItemGrid {
    public:
    ItemGrid(int c, int r, int s, Vector2 p, SharedItemData *sd);
    ~ItemGrid();
    void Update();
    void DrawGrid();
    void DrawItems();
    void SetItems(std::vector<int> *list);

    bool CanAddItem(Vector2 dest_cell);
    void AddItem(int item_id, Vector2 dest_cell);
    bool CanRemoveItem(Vector2 source_cell);
    void RemoveItem(Vector2 source_cell);

    INVENTORYGRIDS this_grid;

    SharedItemData *shared_data;

    int rows;
    int cols;
    int grid_size;
    Vector2 position;

    bool can_select;
    bool cell_hovered;
    bool cell_selected;
    Vector2 hovered_cell;
    Vector2 selected_cell;
    //Vector2 return_position;

    std::vector<Sprite> item_sprites;

    std::vector<int> *item_list;

    Label label;

    Signal selecting;
    Signal not_selecting;
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
    void Open();
    void OpenWith(std::vector<int> &list);

    void OnGroundItemSelected();
    void OnGroundItemDeselected();

    void OnInvItemSelected();
    void OnInvItemDeselected();

    void OnHotbarItemSelected();
    void OnHotbarItemDeselected();


    SharedItemData shared_data;
    INVENTORYGRIDS source_grid;
    INVENTORYGRIDS dest_grid;

    Label title_label;

    Rectangle panel_rect;
    Texture2D panel_bg;

    Label ground_header_label;
    Label character_header_label;
    Label inventory_label_header;

    
    Vector2 gpo; // ground position offset
    ItemGrid *ground_grid;
    std::vector<int> g_item_list;
    
    Vector2 cpo;
    Sprite character_panel_sprite;
    AnimatedSprite character_sprite;
    Label character_label;
    Label character_stat_label;
    //Rectangle ground_bounding_rect;
    //Sprite ground_panel_sprite;
    //AnimatedSprite ground_sprite;
    //Label character_label;
    //Label character_stat_label;

    Vector2 hpo;
    ItemGrid *hotbar_grid;
    
    Vector2 ipo;
    ItemGrid *inventory_grid;

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

