#pragma once
#include "gamedefs.h"

enum INVENTORYGRIDS {
    GRID_NONE = -1,
    GRID_GROUND,
    GRID_INVENTORY,
    GRID_HOTBAR,
    GRID_PRIMARY,
    GRID_SECONDARY,
    GRID_HEAD,
    GRID_BODY,
    GRID_LEGS,
    GRID_FEET,
    GRID_HANDS,
    GRID_NUM_GRIDS
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
    //int source_index
};

class ItemGrid {
    public:
    ItemGrid(int c, int r, int s, Vector2 p, SharedItemData *sd);
    ~ItemGrid();
    void Update();
    void DrawGrid();
    void DrawItems();
    void SetItems(std::vector<int> *list);

    bool CanAddItem(int item_id, Vector2 dest_cell);
    bool HasRoom();
    void AddItem(int item_id, Vector2 dest_cell);
    void AddItem(int item_id);
    bool CanRemoveItem(Vector2 source_cell);
    void RemoveItem(Vector2 source_cell);

    std::string CreateDetails(ItemInstanceData &item_data);

    INVENTORYGRIDS this_grid;
    ItemType accepted_type;

    SharedItemData *shared_data;

    int rows;
    int cols;
    int grid_size;
    float hovered_time;
    Vector2 position;

    bool can_select;
    bool cell_hovered;
    bool cell_selected;
    bool show_details;
    Vector2 hovered_cell;
    Vector2 last_hovered_cell;
    Vector2 selected_cell;

    std::string container_iid;
    //Vector2 return_position;

    std::vector<Sprite> item_sprites;

    std::vector<int> *item_list;

    Label name_label;
    Label details_label;

    Signal selecting;
    Signal not_selecting;
    Signal transfer_item;
    Signal pickup;
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


class ModuleMenu : public BaseUILayer {

    public:
    ModuleMenu();
    ~ModuleMenu() override;
    void Update() override;
    void Draw() override;
    void OpenModule();
    void RecipieSelected();

    Label title_label;

    Rectangle panel_rect;
    Texture2D panel_bg;

    int module_id;

    ModuleData module_data;

    Vector2 rpo;
    Vector2 inpo;
    Vector2 ipo;

    Label recipie_header;
    Label ingredient_header;
    Label inventory_header;

    Label recipie_label;
    Label ingredient_label;
    
    std::vector<Button> recipie_buttons;
    std::vector<int> button_lookup;
    std::vector<int> plan_indexes;
    int selected_button_index;
    bool plan_selected;

    Signal recipie_hovered;
    Signal recipie_selected;

    ItemGrid *inventory_grid;

    Button craft_button;

    SharedItemData shared_data;
};


class CharacterMenu : public BaseUILayer {

    public:
    CharacterMenu();
    ~CharacterMenu() override;
    void Update() override;
    void Draw() override;
    void DrawHotBarOnly();
    void Open();
    void OpenWith(BaseContainerEntity *container);

    void OnItemSelected();
    void OnItemDeselected();

    void OnTransferItem();

    void OnPickup();


    bool use_ground;
    std::vector<int> blank_list;
    std::string default_iid;


    SharedItemData shared_data;
    std::vector<ItemGrid *> grid_list;

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
    Vector2 ppo; //portrait offset
    Sprite character_panel_sprite;
    AnimatedSprite character_sprite;
    Label character_label;
    Label character_stat_label;

    Label primary_header_label;
    ItemGrid *primary_grid;
    Label secondary_header_label;
    ItemGrid *secondary_grid;
    Label head_header_label;
    ItemGrid *head_grid;
    Label body_header_label;
    ItemGrid *body_grid;
    Label legs_header_label;
    ItemGrid *legs_grid;
    Label hands_header_label;
    ItemGrid *hands_grid;
    Label feet_header_label;
    ItemGrid *feet_grid;

    Vector2 hpo;
    ItemGrid *hotbar_grid;
    
    Vector2 ipo;
    ItemGrid *inventory_grid;


    Vector2 spo; 
    Label speed_label;
    Label health_label;
    Label exp_label;
    Label defence_label;
    Label magic_defence_label;
    Label max_power_label;
    Label current_power_label;
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

    Label power_label;

};


class EndUILayer : public BaseUILayer {

    public:
    EndUILayer();
    ~EndUILayer() override;
    
    void Update() override;
    void Draw() override;

    Label title_label;
};

