#include "../../core/gamedefs.h"

CharacterMenu::CharacterMenu() {

    grid_list.resize(GRID_NUM_GRIDS);
    use_ground = false;
    CreateLabel(title_label, {g_screen_center.x, 20 / g_scale}, 30/g_scale, BLACK, "CHARACTER MENU");
    panel_bg = g_ui_panels[PANEL_CHAR_SCREEN];
    panel_rect = {
        .x = 50,
        .y = 0,
        .width = ((float)g_resolution.x - 100),
        .height = ((float)g_resolution.y - 40)
    };

    gpo = {panel_rect.x, panel_rect.y + 80};
    CreateLabel(ground_header_label, {gpo.x + 90, gpo.y - 30}, FONTSIZE_50, WHITE, "GROUND");
    ground_grid = new ItemGrid(5, 6, 50, {gpo.x + 25, gpo.y + 20}, &shared_data);
    ground_grid->this_grid = GRID_GROUND;
    ground_grid->accepted_type = TYPE_ALL;
    ground_grid->selecting.Connect( [&](){OnItemSelected();} );
    ground_grid->not_selecting.Connect( [&](){OnItemDeselected();} );
    ground_grid->transfer_item.Connect( [&](){OnTransferItem();} );
    ground_grid->pickup.Connect( [&](){OnPickup();} );

    grid_list[GRID_GROUND] = ground_grid;
    
//stats
    spo = {panel_rect.x + 425, panel_rect.y + 320};

    CreateLabel(exp_label, {spo.x, spo.y + 20}, FONTSIZE_30, RAYWHITE, "exp:");

    CreateLabel(health_label, {spo.x, spo.y + 40}, FONTSIZE_30, RAYWHITE, "health:");
    health_label.default_color = PINK;

    CreateLabel(current_power_label, {spo.x, spo.y + 60}, FONTSIZE_30, RAYWHITE, "current power:");
    current_power_label.default_color = CYAN;

    CreateLabel(sat_label, {spo.x, spo.y + 80}, FONTSIZE_30, RAYWHITE, "saturation:");
    sat_label.default_color = GREEN;

    CreateLabel(speed_label, {spo.x - 100, spo.y + 85}, FONTSIZE_24, RAYWHITE, "speed:");
    CreateLabel(defence_label, {spo.x + 10, spo.y + 85}, FONTSIZE_24, RAYWHITE, "defence:");
    CreateLabel(magic_defence_label, {spo.x + 10, spo.y + 105}, FONTSIZE_24, RAYWHITE, "magic defence:");
    
//character 
    cpo = {panel_rect.x + 300, panel_rect.y + 45};
    CreateLabel(character_header_label, {cpo.x + 60, cpo.y - 30}, FONTSIZE_50, WHITE, "CHARACTER");

    ppo = {cpo.x + 130, cpo.y + 130}; //portrait offset

    LoadSpriteCentered(character_sprite, g_character_sprite_sheets[ g_character_data[g_current_player->uid].sprite_sheet_id], ppo, 4, 16.0f, 0.10f);
    ScaleSprite(character_sprite, {5,5});

    CreateLabel(primary_header_label, {ppo.x - 75, ppo.y - 10}, FONTSIZE_24, WHITE, "primary");
    primary_grid = new ItemGrid(1, 1, 50, {ppo.x - 100, ppo.y}, &shared_data);
    primary_grid->this_grid = GRID_PRIMARY;
    primary_grid->accepted_type = TYPE_WEAPON;
    primary_grid->selecting.Connect( [&](){OnItemSelected();} );
    primary_grid->not_selecting.Connect( [&](){OnItemDeselected();} );
    primary_grid->transfer_item.Connect( [&](){OnTransferItem();} );
    //primary_grid->use_item.Connect( [&](){OnUseItem();} );
    primary_grid->pickup.Connect( [&](){OnPickup();} );
    grid_list[GRID_PRIMARY] = primary_grid;

    
    CreateLabel(secondary_header_label, {ppo.x + 75, ppo.y - 10}, FONTSIZE_24, WHITE, "secondary");
    secondary_grid = new ItemGrid(1, 1, 50, {ppo.x + 50, ppo.y}, &shared_data);
    secondary_grid->this_grid = GRID_SECONDARY;
    secondary_grid->accepted_type = TYPE_WEAPON;
    secondary_grid->selecting.Connect( [&](){OnItemSelected();} );
    secondary_grid->not_selecting.Connect( [&](){OnItemDeselected();} );
    secondary_grid->transfer_item.Connect( [&](){OnTransferItem();} );
    //secondary_grid->use_item.Connect( [&](){OnUseItem();} );
    secondary_grid->pickup.Connect( [&](){OnPickup();} );
    grid_list[GRID_SECONDARY] = secondary_grid;

    CreateLabel(head_header_label, {ppo.x + 75, ppo.y - 110}, FONTSIZE_24, WHITE, "head");
    head_grid = new ItemGrid(1, 1, 50, {ppo.x + 50, ppo.y - 100}, &shared_data);
    head_grid->this_grid = GRID_HEAD;
    head_grid->accepted_type = TYPE_HEAD_ARMOR;
    head_grid->selecting.Connect( [&](){OnItemSelected();} );
    head_grid->not_selecting.Connect( [&](){OnItemDeselected();} );
    head_grid->transfer_item.Connect( [&](){OnTransferItem();} );
    //head_grid->use_item.Connect( [&](){OnUseItem();} );
    head_grid->pickup.Connect( [&](){OnPickup();} );
    grid_list[GRID_HEAD] = head_grid;

    CreateLabel(body_header_label, {ppo.x - 75, ppo.y - 110}, FONTSIZE_24, WHITE, "body");
    body_grid = new ItemGrid(1, 1, 50, {ppo.x - 100, ppo.y - 100}, &shared_data);
    body_grid->this_grid = GRID_BODY;
    body_grid->accepted_type = TYPE_BODY_ARMOR;
    body_grid->selecting.Connect( [&](){OnItemSelected();} );
    body_grid->not_selecting.Connect( [&](){OnItemDeselected();} );
    body_grid->transfer_item.Connect( [&](){OnTransferItem();} );
    //body_grid->use_item.Connect( [&](){OnUseItem();} );
    body_grid->pickup.Connect( [&](){OnPickup();} );
    grid_list[GRID_BODY] = body_grid;

    CreateLabel(legs_header_label, {ppo.x, ppo.y + 70}, FONTSIZE_24, WHITE, "legs");
    legs_grid = new ItemGrid(1, 1, 50, {ppo.x - 25, ppo.y + 80}, &shared_data);
    legs_grid->this_grid = GRID_LEGS;
    legs_grid->accepted_type = TYPE_LEG_ARMOR;
    legs_grid->selecting.Connect( [&](){OnItemSelected();} );
    legs_grid->not_selecting.Connect( [&](){OnItemDeselected();} );
    legs_grid->transfer_item.Connect( [&](){OnTransferItem();} );
    //legs_grid->use_item.Connect( [&](){OnUseItem();} );
    legs_grid->pickup.Connect( [&](){OnPickup();} );
    grid_list[GRID_LEGS] = legs_grid;

    CreateLabel(feet_header_label, {ppo.x - 75, ppo.y + 70}, FONTSIZE_24, WHITE, "feet");
    feet_grid = new ItemGrid(1, 1, 50, {ppo.x - 100, ppo.y + 80}, &shared_data);
    feet_grid->this_grid = GRID_FEET;
    feet_grid->accepted_type = TYPE_FEET_ARMOR;
    feet_grid->selecting.Connect( [&](){OnItemSelected();} );
    feet_grid->not_selecting.Connect( [&](){OnItemDeselected();} );
    feet_grid->transfer_item.Connect( [&](){OnTransferItem();} );
    //feet_grid->use_item.Connect( [&](){OnUseItem();} );
    feet_grid->pickup.Connect( [&](){OnPickup();} );
    grid_list[GRID_FEET] = feet_grid;

    CreateLabel(hands_header_label, {ppo.x + 75, ppo.y + 70}, FONTSIZE_24, WHITE, "hands");
    hands_grid = new ItemGrid(1, 1, 50, {ppo.x + 50, ppo.y + 80}, &shared_data);
    hands_grid->this_grid = GRID_HANDS;
    hands_grid->accepted_type = TYPE_HAND_ARMOR;
    hands_grid->selecting.Connect( [&](){OnItemSelected();} );
    hands_grid->not_selecting.Connect( [&](){OnItemDeselected();} );
    hands_grid->transfer_item.Connect( [&](){OnTransferItem();} );
    //hands_grid->use_item.Connect( [&](){OnUseItem();} );
    hands_grid->pickup.Connect( [&](){OnPickup();} );
    grid_list[GRID_HANDS] = hands_grid;



//end character

    ipo = {panel_rect.x + 560, panel_rect.y + 80};
    CreateLabel(inventory_label_header, {ipo.x + 60, ipo.y - 30}, FONTSIZE_50, WHITE, "INVENTORY");

    inventory_grid = new ItemGrid(5, 6, 50, {ipo.x + 25, ipo.y + 20}, &shared_data);
    inventory_grid->this_grid = GRID_INVENTORY;
    inventory_grid->accepted_type = TYPE_ALL;
    inventory_grid->selecting.Connect( [&](){OnItemSelected();} );
    inventory_grid->not_selecting.Connect( [&](){OnItemDeselected();} );
    inventory_grid->transfer_item.Connect( [&](){OnTransferItem();} );
    inventory_grid->use_item.Connect( [&](){OnUseItem();} );
    inventory_grid->putdown_or_equip.Connect( [&](){OnPutDownOrEquip();} );
    grid_list[GRID_INVENTORY] = inventory_grid;

    hpo = { 350, g_resolution.y - 80};
    hotbar_grid = new ItemGrid(5, 1, 50, {hpo.x, hpo.y}, &shared_data);
/*     hotbar_grid->this_grid = GRID_HOTBAR;
    hotbar_grid->accepted_type = TYPE_ALL;
    hotbar_grid->selecting.Connect( [&](){OnItemSelected();} );
    hotbar_grid->not_selecting.Connect( [&](){OnItemDeselected();} );
    hotbar_grid->transfer_item.Connect( [&](){OnTransferItem();} );
    hotbar_grid->use_item.Connect( [&](){OnUseItem();} ); */
    grid_list[GRID_HOTBAR] = hotbar_grid;

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
    DrawTexturePro(panel_bg, {0,0,(float)panel_bg.width, (float)panel_bg.height}, panel_rect, {0,0}, 0.0f, WHITE);

    DrawLineV( {spo.x - 100, spo.y}, {spo.x + 100, spo.y}, RAYWHITE);

    DrawLabelCentered(health_label);
    DrawLabelCentered(exp_label);
    DrawLabelCentered(current_power_label);

    DrawLabel(speed_label);
    DrawLabel(defence_label);
    DrawLabel(magic_defence_label);
    DrawLabel(character_header_label);
  
    DrawLabelCentered(sat_label);
    DrawSprite(character_sprite);


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


    if(g_game_settings.show_debug) {

        DrawCircleV(gpo, 2, RED);
        DrawCircleV(ipo, 2, RED);
        DrawCircleV(cpo, 2, RED);
        DrawCircleV(hpo, 2, RED);
        DrawCircleV(ppo, 2, RED);
        DrawCircleV(spo, 2, RED);
    }
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

    std::string speed = TextFormat("%0.2f", g_character_data[g_current_player->uid].current_speed);
    speed_label.text = "speed:  " + speed;

    std::string max_health = TextFormat("%i", g_character_data[g_current_player->uid].max_health);
    std::string health = TextFormat("%i", g_character_data[g_current_player->uid].health);
    health_label.text = "health:  " + health + "/" + max_health;

    std::string exp = TextFormat("%i", g_character_data[g_current_player->uid].exp);
    exp_label.text = "exp:  " + exp;

    std::string defence = TextFormat("%i", g_character_data[g_current_player->uid].defence);
    defence_label.text = "defence:  " + defence;

    std::string magic_defence = TextFormat("%i", g_character_data[g_current_player->uid].magic_defence);
    magic_defence_label.text = "magic defence:  " + magic_defence;

    std::string max_power = TextFormat("%0.2f", g_character_data[g_current_player->uid].max_power);
    //max_power_label.text = "max power: " + max_power;
    std::string current_power = TextFormat("%0.2f", g_character_data[g_current_player->uid].current_power);
    current_power_label.text = "power:  " + current_power + "/" + max_power;

    std::string max_sat = TextFormat("%0.2f", g_character_data[g_current_player->uid].max_saturation);
    std::string sat = TextFormat("%0.2f", g_character_data[g_current_player->uid].saturation);
    sat_label.text = "sat:  " + sat + "/" + max_sat;
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

    use_ground = true;
    shared_data.dest_cell = {-1,-1};
    shared_data.dest_grid = GRID_NONE;
    shared_data.source_cell = {-1,-1};
    shared_data.source_grid = GRID_NONE;
    shared_data.item_id = -1;
    shared_data.use_id = -1;

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
    for(auto &grid : grid_list) {
        grid->can_select = false;
    }

}

void CharacterMenu::OnItemDeselected() {
    for(auto &grid :grid_list) {
        grid->can_select = true;
    }

}


void CharacterMenu::OnPickup() {
    TraceLog(LOG_INFO, "pickup");
    int item_id = shared_data.item_id;
    int source_grid = shared_data.source_grid;

    int dest_grid = GRID_INVENTORY;
    if(source_grid == GRID_INVENTORY) {
        dest_grid = GRID_GROUND;
    }

    Vector2 source_cell = shared_data.source_cell;
    Vector2 dest_cell = shared_data.dest_cell;

    TraceLog(LOG_INFO, "dest cell %0.0f %0.0f", dest_cell.x, dest_cell.y);
    TraceLog(LOG_INFO, "source cell %0.0f %0.0f", source_cell.x, source_cell.y);
    TraceLog(LOG_INFO, "move %i  from %i %i", item_id, source_grid, dest_grid);

    if(grid_list[dest_grid]->HasRoom()) {
        grid_list[dest_grid]->AddItem(item_id);
        grid_list[source_grid]->RemoveItem(source_cell);
        if(source_grid != GRID_GROUND and source_grid != GRID_INVENTORY and source_grid != GRID_SECONDARY) {
            g_current_player->UnEquip(item_id);
        }
    }
}


void CharacterMenu::OnPutDownOrEquip() {
    TraceLog(LOG_INFO, "put down or equip item %i  %i", shared_data.item_id, grid_list.size());
    int item_id = shared_data.item_id;
    int source_grid = shared_data.source_grid;
    int dest_grid =  GRID_GROUND;
    Vector2 source_cell = shared_data.source_cell;
    Vector2 dest_cell = shared_data.dest_cell;


    for(int i = 0; i < grid_list.size(); i++) {
        //TraceLog(LOG_INFO, "put down or equip item list / %i   type %i", i, grid_list[i]->this_grid);

        if(grid_list[i]->accepted_type == g_item_instances[shared_data.item_id].type) {
            dest_grid = i;
            break;
        }
    }


    TraceLog(LOG_INFO, "dest cell %0.0f %0.0f", dest_cell.x, dest_cell.y);
    TraceLog(LOG_INFO, "source cell %0.0f %0.0f", source_cell.x, source_cell.y);
    TraceLog(LOG_INFO, "move %i  from %i %i", item_id, source_grid, dest_grid);

    if(grid_list[dest_grid]->HasRoom()) {
        grid_list[dest_grid]->AddItem(item_id);
        grid_list[source_grid]->RemoveItem(shared_data.source_cell);
        if(dest_grid != GRID_GROUND and dest_grid != GRID_INVENTORY and dest_grid != GRID_SECONDARY) {
            g_current_player->Equip(item_id);    
        }
    }
}



void CharacterMenu::OnTransferItem() {

    int item_id = shared_data.item_id;
    int source_grid = shared_data.source_grid;
    int dest_grid =  shared_data.dest_grid;
    Vector2 source_cell = shared_data.source_cell;
    Vector2 dest_cell = shared_data.dest_cell;

    TraceLog(LOG_INFO, "\ntransfer items \ndest cell %0.0f %0.0f", dest_cell.x, dest_cell.y);
    TraceLog(LOG_INFO, "source cell %0.0f %0.0f", source_cell.x, source_cell.y);
    TraceLog(LOG_INFO, "move %i  from %i %i", item_id, source_grid, dest_grid);

    if(source_grid != -1 and dest_grid != -1) {
        if(grid_list[dest_grid]->CanAddItem(item_id, dest_cell)) {
            grid_list[dest_grid]->AddItem(item_id, dest_cell);
            grid_list[source_grid]->RemoveItem(source_cell);
            

            if(dest_grid != GRID_GROUND and dest_grid != GRID_INVENTORY and dest_grid != GRID_SECONDARY) {
                g_current_player->Equip(item_id);    
            }
            
            if(source_grid != GRID_GROUND and source_grid != GRID_INVENTORY and source_grid != GRID_SECONDARY) {
                g_current_player->UnEquip(item_id);
                
            }
        }
        else {
            grid_list[source_grid]->AddItem(item_id, source_cell);
        }
    }


    shared_data.dest_cell = {-1,-1};
    shared_data.dest_grid = GRID_NONE;
    shared_data.source_cell = {-1,-1};
    shared_data.source_grid = GRID_NONE;
    shared_data.item_id = -1;
    shared_data.use_id = -1;

    TraceLog(LOG_INFO, "---------------------------------");
}

void CharacterMenu::OnUseItem() {
    int item_id = shared_data.item_id;
    int source_grid = shared_data.source_grid;
    int dest_grid =  shared_data.dest_grid;
    Vector2 source_cell = shared_data.source_cell;
    Vector2 dest_cell = shared_data.dest_cell;

    TraceLog(LOG_INFO, "--------------using %i  -------------------", item_id);
    TraceLog(LOG_INFO, "--------------source grid %i  dest grid %i-------------------", source_grid, dest_grid);
    TraceLog(LOG_INFO, "--------------source cell %0.0f %0.0f-------------------", source_cell.x, source_cell.y);
    TraceLog(LOG_INFO, "--------------dest cell %0.0f %0.0f-------------------", dest_cell.x, dest_cell.y);


    auto source_itter = g_item_instances.find(shared_data.item_id);
    if(source_itter != g_item_instances.end()) {
        if(source_itter->second.type == TYPE_FOOD) {
            TraceLog(LOG_INFO, "-------using food  %s-------", source_itter->second.item_name.c_str());
            TraceLog(LOG_INFO, "------- saturation  %0.02f-------", source_itter->second.saturation);
            g_character_data[g_current_player->uid].saturation += source_itter->second.saturation;
            if(g_character_data[g_current_player->uid].saturation > g_character_data[g_current_player->uid].max_saturation) {
                g_character_data[g_current_player->uid].saturation = g_character_data[g_current_player->uid].max_saturation;
            }

            grid_list[source_grid]->RemoveItem(source_cell);
        }
    }

    for(auto &grid :grid_list) {
        grid->can_select = true;
    }

    shared_data.dest_cell = {-1,-1};
    shared_data.dest_grid = GRID_NONE;
    shared_data.source_cell = {-1,-1};
    shared_data.source_grid = GRID_NONE;
    shared_data.item_id = -1;
    shared_data.use_id = -1;


}