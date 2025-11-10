#include "../../core/gamedefs.h"

static std::vector<int> blank_list;

CharacterMenu::CharacterMenu() {

    CreateLabel(title_label, {g_screen_center.x, 20 / g_scale}, 30/g_scale, BLACK, "CHARACTER MENU");
    panel_bg = g_ui_panels[PANEL_CHAR_SCREEN];
    panel_rect = {
        .x = 50,
        .y = 20,
        .width = ((float)g_resolution.x - 100),
        .height = ((float)g_resolution.y - 100)
    };

    gpo = {panel_rect.x, panel_rect.y + 80};
    CreateLabel(ground_header_label, {gpo.x + 90, gpo.y - 30}, 30/g_scale, WHITE, "GROUND");
    ground_grid = new ItemGrid(5, 6, 50, {gpo.x + 25, gpo.y + 20}, &shared_data);
    ground_grid->this_grid = GRID_GROUND;
    ground_grid->selecting.Connect( [&](){OnGroundItemSelected();} );
    ground_grid->not_selecting.Connect( [&](){OnGroundItemDeselected();} );
    ground_grid->transfer_item.Connect( [&](){OnTransferItem();} );
    
    cpo = {panel_rect.x + 300, panel_rect.y + 45};
    CreateLabel(character_header_label, {cpo.x + 60, cpo.y - 30}, 30/g_scale, WHITE, "CHARACTER");

    LoadSpriteCentered(character_sprite, g_sprite_sheets[g_player_data.sprite_sheet_id], {cpo.x + 130, cpo.y + 50}, 4, 16.0f, 0.10f);
    ScaleSprite(character_sprite, {5,5});

    
    ipo = {panel_rect.x + 560, panel_rect.y + 80};
    CreateLabel(inventory_label_header, {ipo.x + 60, ipo.y - 30}, 30/g_scale, WHITE, "INVENTORY");

    inventory_grid = new ItemGrid(5, 6, 50, {ipo.x + 25, ipo.y + 20}, &shared_data);
    inventory_grid->this_grid = GRID_INVENTORY;
    inventory_grid->selecting.Connect( [&](){OnInvItemSelected();} );
    inventory_grid->not_selecting.Connect( [&](){OnInvItemDeselected();} );
    inventory_grid->transfer_item.Connect( [&](){OnTransferItem();} );

    hpo = { 350, g_resolution.y - 80};
    hotbar_grid = new ItemGrid(5, 1, 50, {hpo.x, hpo.y}, &shared_data);
    hotbar_grid->this_grid = GRID_HOTBAR;
    hotbar_grid->selecting.Connect( [&](){OnHotbarItemSelected();} );
    hotbar_grid->not_selecting.Connect( [&](){OnHotbarItemDeselected();} );
    hotbar_grid->transfer_item.Connect( [&](){OnTransferItem();} );

}

CharacterMenu::~CharacterMenu() {
    //UnloadSound(button_sound);
    delete ground_grid;
    delete inventory_grid;
    delete hotbar_grid;
    TraceLog(LOG_INFO, "UI DESTRUCTOR:  CHARACTER MENU UI");
}

void CharacterMenu::Draw() {
    DrawRectangleRec({0,0,g_resolution.x,g_resolution.y}, TRANSDARKERGRAY);
    //DrawRectangleRec(panel_rect, RED);
    DrawTexturePro(panel_bg, {0,0,(float)panel_bg.width, (float)panel_bg.height}, panel_rect, {0,0}, 0.0f, WHITE);
    DrawLabel(ground_header_label);
    ground_grid->DrawGrid();
    DrawLabel(inventory_label_header);
    inventory_grid->DrawGrid();

    hotbar_grid->DrawGrid();
    
    ground_grid->DrawItems();
    inventory_grid->DrawItems();
    hotbar_grid->DrawItems();


    DrawLabel(character_header_label);
    DrawSprite(character_sprite);

    DrawCircleV(gpo, 2, RED);
    DrawCircleV(ipo, 2, RED);
    DrawCircleV(cpo, 2, RED);
    DrawCircleV(hpo, 2, RED);
}

void CharacterMenu::DrawHotBarOnly() {
    hotbar_grid->DrawGrid();
    hotbar_grid->DrawItems();
}


void CharacterMenu::Update() {

    ground_grid->Update();
    inventory_grid->Update();
    hotbar_grid->Update();

}

void CharacterMenu::Open() {
    inventory_grid->SetItems(&g_player_data.inventory);
    hotbar_grid->SetItems(&g_player_data.hotbar);

    //std::vector<int> blank_list;
    blank_list.clear();
    blank_list.push_back(-1);
    ground_grid->SetItems(&blank_list);
}

void CharacterMenu::OpenWith(std::vector<int> &list) {
    //g_item_list = list;
    inventory_grid->SetItems(&g_player_data.inventory);
    ground_grid->SetItems(&list);
    hotbar_grid->SetItems(&g_player_data.hotbar);
}


void CharacterMenu::OnGroundItemSelected() {
    ground_grid->can_select = false;
    inventory_grid->can_select = false;
    hotbar_grid->can_select = false;
}

void CharacterMenu::OnGroundItemDeselected() {
    ground_grid->can_select = true;
    inventory_grid->can_select = true;
    hotbar_grid->can_select = true;
    
}


void CharacterMenu::OnInvItemSelected() {
    ground_grid->can_select = false;
    inventory_grid->can_select = false;
    hotbar_grid->can_select = false;
}

void CharacterMenu::OnInvItemDeselected() {
    ground_grid->can_select = true;
    inventory_grid->can_select = true;
    hotbar_grid->can_select = true;
}

void CharacterMenu::OnHotbarItemSelected() {
    ground_grid->can_select = false;
    inventory_grid->can_select = false;
    hotbar_grid->can_select = false;
}

void CharacterMenu::OnHotbarItemDeselected() {
    ground_grid->can_select = true;
    inventory_grid->can_select = true;
    hotbar_grid->can_select = true;
}

void CharacterMenu::OnTransferItem() {
    TraceLog(LOG_INFO, "\ntransfer items \ndest %0.0f %0.0f", shared_data.dest_cell.x, shared_data.dest_cell.y);
    TraceLog(LOG_INFO, "source %0.0f %0.0f", shared_data.source_cell.x, shared_data.source_cell.y);
    TraceLog(LOG_INFO, "move %i  from %i %i\n", shared_data.item_id, shared_data.source_grid, shared_data.dest_grid);

    if(shared_data.source_grid == GRID_GROUND) {
        //from ground
        if(shared_data.dest_grid == GRID_INVENTORY) {
            //from ground to inv
            if(inventory_grid->CanAddItem(shared_data.dest_cell)) {
                //is a free cell
                inventory_grid->AddItem(shared_data.item_id, shared_data.dest_cell);
                ground_grid->RemoveItem(shared_data.source_cell);
            }
            else {
                //not a free cell
                ground_grid->AddItem(shared_data.item_id, shared_data.source_cell);
            }
        }
        if(shared_data.dest_grid == GRID_HOTBAR) {
            //from ground to hotbar
            if(hotbar_grid->CanAddItem(shared_data.dest_cell)) {
                //is a free cell
                hotbar_grid->AddItem(shared_data.item_id, shared_data.dest_cell);
                ground_grid->RemoveItem(shared_data.source_cell);
            }
            else {
                //not a free cell
                ground_grid->AddItem(shared_data.item_id, shared_data.source_cell);
            }
        }
    }

    if(shared_data.source_grid == GRID_INVENTORY) {
        //from inv
        if(shared_data.dest_grid == GRID_GROUND) {
            //from inv to ground
            if(ground_grid->CanAddItem(shared_data.dest_cell)) {
                //is a free cell
                ground_grid->AddItem(shared_data.item_id, shared_data.dest_cell);
                inventory_grid->RemoveItem(shared_data.source_cell);
            }
            else {
                //not a free cell
                inventory_grid->AddItem(shared_data.item_id, shared_data.source_cell);
            }
        }
        if(shared_data.dest_grid == GRID_HOTBAR) {
            //from inv to hotbar
            if(hotbar_grid->CanAddItem(shared_data.dest_cell)) {
                //is a free cell
                hotbar_grid->AddItem(shared_data.item_id, shared_data.dest_cell);
                inventory_grid->RemoveItem(shared_data.source_cell);
            }
            else {
                //not a free cell
                inventory_grid->AddItem(shared_data.item_id, shared_data.source_cell);
            }
            
        }
    }

    if(shared_data.source_grid == GRID_HOTBAR) {
        //from inv
        if(shared_data.dest_grid == GRID_GROUND) {
            //from hotbar to ground
            if(ground_grid->CanAddItem(shared_data.dest_cell)) {
                //is a free cell
                ground_grid->AddItem(shared_data.item_id, shared_data.dest_cell);
                hotbar_grid->RemoveItem(shared_data.source_cell);
            }
            else {
                //not a free cell
                hotbar_grid->AddItem(shared_data.item_id, shared_data.source_cell);
            }
        }
        if(shared_data.dest_grid == GRID_INVENTORY) {
            //from hotbar to inv
            if(inventory_grid->CanAddItem(shared_data.dest_cell)) {
                //is a free cell
                inventory_grid->AddItem(shared_data.item_id, shared_data.dest_cell);
                hotbar_grid->RemoveItem(shared_data.source_cell);
            }
            else {
                //not a free cell
                TraceLog(LOG_INFO, "\nnot a fre cell \nsource %0.0f %0.0f", shared_data.source_cell.x, shared_data.source_cell.y);

                hotbar_grid->AddItem(shared_data.item_id, shared_data.source_cell);
            }
        }
    }
    shared_data.dest_cell = {-1,-1};
    shared_data.dest_grid = GRID_NONE;
    shared_data.source_cell = {-1,-1};
    shared_data.source_grid = GRID_NONE;
}