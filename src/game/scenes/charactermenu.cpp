#include "../../core/gamedefs.h"


CharacterMenu::CharacterMenu() {

    source_grid = NONE;
    dest_grid = NONE;

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
    ground_grid->this_grid = GROUND;
    ground_grid->selecting.Connect( [&](){OnGroundItemSelected();} );
    ground_grid->not_selecting.Connect( [&](){OnGroundItemDeselected();} );
    
    cpo = {panel_rect.x + 300, panel_rect.y + 45};
    CreateLabel(character_header_label, {cpo.x + 60, cpo.y - 30}, 30/g_scale, WHITE, "CHARACTER");

    LoadSpriteCentered(character_sprite, g_sprite_sheets[g_player_data.sprite_sheet_id], {cpo.x + 130, cpo.y + 50}, 4, 16.0f, 0.10f);
    ScaleSprite(character_sprite, {5,5});

    
    ipo = {panel_rect.x + 560, panel_rect.y + 80};
    CreateLabel(inventory_label_header, {ipo.x + 60, ipo.y - 30}, 30/g_scale, WHITE, "INVENTORY");

    inventory_grid = new ItemGrid(5, 6, 50, {ipo.x + 25, ipo.y + 20}, &shared_data);
    inventory_grid->this_grid = INVENTORY;
    inventory_grid->selecting.Connect( [&](){OnInvItemSelected();} );
    inventory_grid->not_selecting.Connect( [&](){OnInvItemDeselected();} );

    hpo = { 350, g_resolution.y - 50};
    hotbar_grid = new ItemGrid(5, 1, 50, {hpo.x, hpo.y}, &shared_data);
    hotbar_grid->this_grid = HOTBAR;
    hotbar_grid->selecting.Connect( [&](){OnHotbarItemSelected();} );
    hotbar_grid->not_selecting.Connect( [&](){OnHotbarItemDeselected();} );

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

void CharacterMenu::Update() {

    ground_grid->Update();
    inventory_grid->Update();
    hotbar_grid->Update();

}

void CharacterMenu::Open() {
    inventory_grid->SetItems(&g_player_data.inventory);
    hotbar_grid->SetItems(&g_player_data.hotbar);
}

void CharacterMenu::OpenWith(std::vector<int> &list) {
    g_item_list = list;
    inventory_grid->SetItems(&g_player_data.inventory);
    ground_grid->SetItems(&g_item_list);
    hotbar_grid->SetItems(&g_player_data.hotbar);
}

void CharacterMenu::OnGroundItemSelected() {
    source_grid = GROUND;
    ground_grid->can_select = false;
    inventory_grid->can_select = false;
    hotbar_grid->can_select = false;
}

void CharacterMenu::OnGroundItemDeselected() {
    ground_grid->can_select = true;
    inventory_grid->can_select = true;
    hotbar_grid->can_select = true;

    TraceLog(LOG_INFO, "hp %0.0f %0.0f", shared_data.dest_cell.x, shared_data.dest_cell.y);
    TraceLog(LOG_INFO, "sp %0.0f %0.0f", shared_data.source_cell.x, shared_data.source_cell.y);
    TraceLog(LOG_INFO, "move %i  from %i %i\n", shared_data.item_id, shared_data.source_grid, shared_data.dest_grid);
    
}

void CharacterMenu::OnInvItemSelected() {
    source_grid = INVENTORY;
    ground_grid->can_select = false;
    inventory_grid->can_select = false;
    hotbar_grid->can_select = false;
}

void CharacterMenu::OnInvItemDeselected() {
    ground_grid->can_select = true;
    inventory_grid->can_select = true;
    hotbar_grid->can_select = true;

    TraceLog(LOG_INFO, "hp %0.0f %0.0f", shared_data.dest_cell.x, shared_data.dest_cell.y);
    TraceLog(LOG_INFO, "sp %0.0f %0.0f", shared_data.source_cell.x, shared_data.source_cell.y);
    TraceLog(LOG_INFO, "move %i  from %i %i\n", shared_data.item_id, shared_data.source_grid, shared_data.dest_grid);
}

void CharacterMenu::OnHotbarItemSelected() {
    source_grid = HOTBAR;
    ground_grid->can_select = false;
    inventory_grid->can_select = false;
    hotbar_grid->can_select = false;
}

void CharacterMenu::OnHotbarItemDeselected() {
    ground_grid->can_select = true;
    inventory_grid->can_select = true;
    hotbar_grid->can_select = true;

    TraceLog(LOG_INFO, "hp %0.0f %0.0f", shared_data.dest_cell.x, shared_data.dest_cell.y);
    TraceLog(LOG_INFO, "sp %0.0f %0.0f", shared_data.source_cell.x, shared_data.source_cell.y);
    TraceLog(LOG_INFO, "move %i  from %i %i\n", shared_data.item_id, shared_data.source_grid, shared_data.dest_grid);
}