#include "../../core/gamedefs.h"

//static std::vector<int> blank_list;

CharacterMenu::CharacterMenu() {

    use_ground = false;
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
    ground_grid->accepted_type = TYPE_ALL;
    ground_grid->selecting.Connect( [&](){OnItemSelected();} );
    ground_grid->not_selecting.Connect( [&](){OnItemDeselected();} );
    ground_grid->transfer_item.Connect( [&](){OnTransferItem();} );
    ground_grid->pickup.Connect( [&](){OnPickup();} );

    grid_list.push_back(ground_grid);
    
//stats
    spo = {panel_rect.x + 320, panel_rect.y + 350};
    CreateLabel(health_label, {spo.x, spo.y + 5}, 14, RAYWHITE, "health:");
    CreateLabel(speed_label, {spo.x, spo.y + 20}, 14, RAYWHITE, "speed:");


//character 
    cpo = {panel_rect.x + 300, panel_rect.y + 45};
    CreateLabel(character_header_label, {cpo.x + 60, cpo.y - 30}, 30/g_scale, WHITE, "CHARACTER");

    ppo = {cpo.x + 130, cpo.y + 170}; //portrait offset
    LoadSpriteCentered(character_sprite, g_sprite_sheets[ g_character_data[g_current_player->uid].sprite_sheet_id], ppo, 4, 16.0f, 0.10f);
    ScaleSprite(character_sprite, {5,5});

    CreateLabel(primary_header_label, {ppo.x - 75, ppo.y - 10}, 15/g_scale, WHITE, "primary");
    primary_grid = new ItemGrid(1, 1, 50, {ppo.x - 100, ppo.y}, &shared_data);
    primary_grid->this_grid = GRID_PRIMARY;
    primary_grid->accepted_type = TYPE_WEAPON;
    primary_grid->selecting.Connect( [&](){OnItemSelected();} );
    primary_grid->not_selecting.Connect( [&](){OnItemDeselected();} );
    primary_grid->transfer_item.Connect( [&](){OnTransferItem();} );
    grid_list.push_back(primary_grid);

    
    CreateLabel(secondary_header_label, {ppo.x + 75, ppo.y - 10}, 15/g_scale, WHITE, "secondary");
    secondary_grid = new ItemGrid(1, 1, 50, {ppo.x + 50, ppo.y}, &shared_data);
    secondary_grid->this_grid = GRID_SECONDARY;
    secondary_grid->accepted_type = TYPE_WEAPON;
    secondary_grid->selecting.Connect( [&](){OnItemSelected();} );
    secondary_grid->not_selecting.Connect( [&](){OnItemDeselected();} );
    secondary_grid->transfer_item.Connect( [&](){OnTransferItem();} );
    grid_list.push_back(secondary_grid);

    CreateLabel(head_header_label, {ppo.x + 75, ppo.y - 110}, 15/g_scale, WHITE, "head");
    head_grid = new ItemGrid(1, 1, 50, {ppo.x + 50, ppo.y - 100}, &shared_data);
    head_grid->this_grid = GRID_HEAD;
    head_grid->accepted_type = TYPE_HEAD_ARMOR;
    head_grid->selecting.Connect( [&](){OnItemSelected();} );
    head_grid->not_selecting.Connect( [&](){OnItemDeselected();} );
    head_grid->transfer_item.Connect( [&](){OnTransferItem();} );
    grid_list.push_back(head_grid);

    CreateLabel(body_header_label, {ppo.x - 75, ppo.y - 110}, 15/g_scale, WHITE, "body");
    body_grid = new ItemGrid(1, 1, 50, {ppo.x - 100, ppo.y - 100}, &shared_data);
    body_grid->this_grid = GRID_BODY;
    body_grid->accepted_type = TYPE_BODY_ARMOR;
    body_grid->selecting.Connect( [&](){OnItemSelected();} );
    body_grid->not_selecting.Connect( [&](){OnItemDeselected();} );
    body_grid->transfer_item.Connect( [&](){OnTransferItem();} );
    grid_list.push_back(body_grid);

    CreateLabel(legs_header_label, {ppo.x, ppo.y + 70}, 15/g_scale, WHITE, "legs");
    legs_grid = new ItemGrid(1, 1, 50, {ppo.x - 25, ppo.y + 80}, &shared_data);
    legs_grid->this_grid = GRID_LEGS;
    legs_grid->accepted_type = TYPE_LEG_ARMOR;
    legs_grid->selecting.Connect( [&](){OnItemSelected();} );
    legs_grid->not_selecting.Connect( [&](){OnItemDeselected();} );
    legs_grid->transfer_item.Connect( [&](){OnTransferItem();} );
    grid_list.push_back(legs_grid);

    CreateLabel(feet_header_label, {ppo.x - 75, ppo.y + 70}, 15/g_scale, WHITE, "feet");
    feet_grid = new ItemGrid(1, 1, 50, {ppo.x - 100, ppo.y + 80}, &shared_data);
    feet_grid->this_grid = GRID_FEET;
    feet_grid->accepted_type = TYPE_FEET_ARMOR;
    feet_grid->selecting.Connect( [&](){OnItemSelected();} );
    feet_grid->not_selecting.Connect( [&](){OnItemDeselected();} );
    feet_grid->transfer_item.Connect( [&](){OnTransferItem();} );
    grid_list.push_back(feet_grid);

    CreateLabel(hands_header_label, {ppo.x + 75, ppo.y + 70}, 15/g_scale, WHITE, "hands");
    hands_grid = new ItemGrid(1, 1, 50, {ppo.x + 50, ppo.y + 80}, &shared_data);
    hands_grid->this_grid = GRID_HANDS;
    hands_grid->accepted_type = TYPE_HAND_ARMOR;
    hands_grid->selecting.Connect( [&](){OnItemSelected();} );
    hands_grid->not_selecting.Connect( [&](){OnItemDeselected();} );
    hands_grid->transfer_item.Connect( [&](){OnTransferItem();} );
    grid_list.push_back(hands_grid);



//end character

    ipo = {panel_rect.x + 560, panel_rect.y + 80};
    CreateLabel(inventory_label_header, {ipo.x + 60, ipo.y - 30}, 30/g_scale, WHITE, "INVENTORY");

    inventory_grid = new ItemGrid(5, 6, 50, {ipo.x + 25, ipo.y + 20}, &shared_data);
    inventory_grid->this_grid = GRID_INVENTORY;
    inventory_grid->accepted_type = TYPE_ALL;
    inventory_grid->selecting.Connect( [&](){OnItemSelected();} );
    inventory_grid->not_selecting.Connect( [&](){OnItemDeselected();} );
    inventory_grid->transfer_item.Connect( [&](){OnTransferItem();} );
    grid_list.push_back(inventory_grid);

    hpo = { 350, g_resolution.y - 80};
    hotbar_grid = new ItemGrid(5, 1, 50, {hpo.x, hpo.y}, &shared_data);
    hotbar_grid->this_grid = GRID_HOTBAR;
    hotbar_grid->accepted_type = TYPE_ALL;
    hotbar_grid->selecting.Connect( [&](){OnItemSelected();} );
    hotbar_grid->not_selecting.Connect( [&](){OnItemDeselected();} );
    hotbar_grid->transfer_item.Connect( [&](){OnTransferItem();} );
    grid_list.push_back(hotbar_grid);

}

CharacterMenu::~CharacterMenu() {
    //UnloadSound(button_sound);
    delete ground_grid;
    delete inventory_grid;
    delete hotbar_grid;

    delete primary_grid;
    delete secondary_grid;
    delete head_grid;
    delete body_grid;
    delete legs_grid;
    delete feet_grid;
    delete hands_grid;

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

    //hotbar_grid->DrawGrid();

    primary_grid->DrawGrid();
    DrawLabelCentered(primary_header_label);

    secondary_grid->DrawGrid();
    DrawLabelCentered(secondary_header_label);

    head_grid->DrawGrid();
    DrawLabelCentered(head_header_label);

    body_grid->DrawGrid();
    DrawLabelCentered(body_header_label);

    legs_grid->DrawGrid();
    DrawLabelCentered(legs_header_label);

    feet_grid->DrawGrid();
    DrawLabelCentered(feet_header_label);

    hands_grid->DrawGrid();
    DrawLabelCentered(hands_header_label);
    
    ground_grid->DrawItems();
    inventory_grid->DrawItems();
    //hotbar_grid->DrawItems();

    primary_grid->DrawItems();
    secondary_grid->DrawItems();
    head_grid->DrawItems();
    body_grid->DrawItems();
    legs_grid->DrawItems();
    feet_grid->DrawItems();
    hands_grid->DrawItems();


    DrawLabel(character_header_label);
    DrawSprite(character_sprite);

    DrawLineV( {spo.x - 10, spo.y}, {spo.x + 220, spo.y}, RAYWHITE);
    DrawLabel(health_label);
    DrawLabel(speed_label);

/*     DrawCircleV(gpo, 2, RED);
    DrawCircleV(ipo, 2, RED);
    DrawCircleV(cpo, 2, RED);
    DrawCircleV(hpo, 2, RED);
    DrawCircleV(ppo, 2, RED); */
}

void CharacterMenu::DrawHotBarOnly() {
    //hotbar_grid->DrawGrid();
    //hotbar_grid->DrawItems();
}


void CharacterMenu::Update() {
    

    ground_grid->Update();
    inventory_grid->Update();
    //hotbar_grid->Update();

    primary_grid->Update();
    secondary_grid->Update();
    head_grid->Update();
    body_grid->Update();
    legs_grid->Update();
    feet_grid->Update();
    hands_grid->Update();

    std::string speed = TextFormat("%0.2f", g_character_data[g_current_player->uid].base_speed);
    speed_label.text = "speed: " + speed;

    std::string health = TextFormat("%i", g_character_data[g_current_player->uid].health);
    health_label.text = "health: " + health;

}

void CharacterMenu::Open() {
     TraceLog(LOG_INFO, "opening character menu with no container");
    inventory_grid->SetItems(&g_character_data[g_current_player->uid].inventory);
    hotbar_grid->SetItems(&g_character_data[g_current_player->uid].hotbar);

    primary_grid->SetItems(&g_character_data[g_current_player->uid].primary);
    secondary_grid->SetItems(&g_character_data[g_current_player->uid].secondary);
    head_grid->SetItems(&g_character_data[g_current_player->uid].head);
    body_grid->SetItems(&g_character_data[g_current_player->uid].body);
    legs_grid->SetItems(&g_character_data[g_current_player->uid].legs);
    feet_grid->SetItems(&g_character_data[g_current_player->uid].feet);
    hands_grid->SetItems(&g_character_data[g_current_player->uid].hands);

    //std::vector<int> blank_list;
    use_ground = true;
    blank_list.clear();
    blank_list.push_back(-1);
    default_iid = "ground" + std::to_string(GetRandomValue(1000, 1000000));
    ground_grid->container_iid = default_iid;
    ground_grid->SetItems(&blank_list);
}


void CharacterMenu::OpenWith(BaseContainerEntity *container) {
    TraceLog(LOG_INFO, "OPENING CONTAINER %s", container->iid.c_str());
    inventory_grid->SetItems(&g_character_data[g_current_player->uid].inventory);
    hotbar_grid->SetItems(&g_character_data[g_current_player->uid].hotbar);

    primary_grid->SetItems(&g_character_data[g_current_player->uid].primary);
    secondary_grid->SetItems(&g_character_data[g_current_player->uid].secondary);
    head_grid->SetItems(&g_character_data[g_current_player->uid].head);
    body_grid->SetItems(&g_character_data[g_current_player->uid].body);
    legs_grid->SetItems(&g_character_data[g_current_player->uid].legs);
    feet_grid->SetItems(&g_character_data[g_current_player->uid].feet);
    hands_grid->SetItems(&g_character_data[g_current_player->uid].hands);

    use_ground = false;
    ground_grid->container_iid = container->iid;
    ground_grid->SetItems(&container->c_area.item_list);

}


void CharacterMenu::OnItemSelected() {
    ground_grid->can_select = false;
    inventory_grid->can_select = false;
    hotbar_grid->can_select = false;

    primary_grid->can_select = false;
    secondary_grid->can_select = false;
    head_grid->can_select = false;
    body_grid->can_select = false;
    legs_grid->can_select = false;
    feet_grid->can_select = false;
    hands_grid->can_select = false;
}

void CharacterMenu::OnItemDeselected() {
    ground_grid->can_select = true;
    inventory_grid->can_select = true;
    hotbar_grid->can_select = true;
    
    primary_grid->can_select = true;
    secondary_grid->can_select = true;
    head_grid->can_select = true;
    body_grid->can_select = true;
    legs_grid->can_select = true;
    feet_grid->can_select = true;
    hands_grid->can_select = true;
}


void CharacterMenu::OnPickup() {
    TraceLog(LOG_INFO, "pickup");
    int source_index = -1;
    int dest_index = -1;

    for(int i = 0; i < grid_list.size(); i++) {
        if(grid_list[i]->this_grid == shared_data.source_grid) {
            source_index = i;
        }
    }

    if(inventory_grid->HasRoom()) {
        TraceLog(LOG_INFO, "pickup %i", source_index);
        inventory_grid->AddItem(shared_data.item_id);
        grid_list[source_index]->RemoveItem(shared_data.source_cell);
    }
    else {
    }
}

void CharacterMenu::OnTransferItem() {

    int source_index = -1;
    int dest_index = -1;

    for(int i = 0; i < grid_list.size(); i++) {
        if(grid_list[i]->this_grid == shared_data.source_grid) {
            source_index = i;
        }
        if(grid_list[i]->this_grid == shared_data.dest_grid) {
            dest_index = i;
        }
    }

    TraceLog(LOG_INFO, "\ntransfer items \ndest %0.0f %0.0f", shared_data.dest_cell.x, shared_data.dest_cell.y);
    TraceLog(LOG_INFO, "source %0.0f %0.0f", shared_data.source_cell.x, shared_data.source_cell.y);
    TraceLog(LOG_INFO, "move %i  from %i %i", shared_data.item_id, shared_data.source_grid, shared_data.dest_grid);
    TraceLog(LOG_INFO, "source index %i dest index %i", source_index, dest_index);

    if(source_index != -1 and dest_index != -1) {
        if(grid_list[dest_index]->CanAddItem(shared_data.item_id, shared_data.dest_cell)) {
            grid_list[dest_index]->AddItem(shared_data.item_id, shared_data.dest_cell);

            if(shared_data.dest_grid == GRID_PRIMARY or
                shared_data.dest_grid == GRID_SECONDARY or
                shared_data.dest_grid == GRID_HEAD or
                shared_data.dest_grid == GRID_BODY or
                shared_data.dest_grid == GRID_LEGS or
                shared_data.dest_grid == GRID_FEET or 
                shared_data.dest_grid == GRID_HANDS) {
                if(g_current_player->CanEquip(shared_data.item_id)) {g_current_player->Equip(shared_data.item_id);}}

            if(shared_data.source_grid == GRID_PRIMARY or
                shared_data.source_grid == GRID_SECONDARY or
                shared_data.source_grid == GRID_HEAD or
                shared_data.source_grid == GRID_BODY or
                shared_data.source_grid == GRID_LEGS or
                shared_data.source_grid == GRID_FEET or 
                shared_data.source_grid == GRID_HANDS) {
                if(g_current_player->CanUnEquip(shared_data.item_id)) {g_current_player->UnEquip(shared_data.item_id);}}

            grid_list[source_index]->RemoveItem(shared_data.source_cell);
        }
        else {
            grid_list[source_index]->AddItem(shared_data.item_id, shared_data.source_cell);
        }
    }

/*     if(shared_data.dest_grid == GRID_PRIMARY or
       shared_data.dest_grid == GRID_SECONDARY or
       shared_data.dest_grid == GRID_HEAD or
       shared_data.dest_grid == GRID_BODY or
       shared_data.dest_grid == GRID_LEGS or
       shared_data.dest_grid == GRID_FEET or 
       shared_data.dest_grid == GRID_HANDS
    ) {
        if(g_current_player->CanEquip(shared_data.item_id)) {
            
            g_current_player->Equip(shared_data.item_id);
        }
    } */

    shared_data.dest_cell = {-1,-1};
    shared_data.dest_grid = GRID_NONE;
    shared_data.source_cell = {-1,-1};
    shared_data.source_grid = GRID_NONE;

    TraceLog(LOG_INFO, "---------------------------------");
}