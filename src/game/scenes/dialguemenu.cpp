#include "../../core/gamedefs.h"

DialogueMenu::DialogueMenu() {

    grid_list.resize(GRID_NUM_GRIDS, nullptr);

    use_ground = false;
    CreateLabel(title_label, {g_screen_center.x, 20 / g_scale}, 30/g_scale, BLACK, "HI THERE");
    panel_bg = g_ui_panels[PANEL_CHAR_SCREEN];
    panel_rect = {
        .x = 50,
        .y = 0,
        .width = ((float)g_resolution.x - 100),
        .height = ((float)g_resolution.y - 40)
    };

    gpo = {panel_rect.x, panel_rect.y + 80};
    CreateLabel(ground_header_label, {gpo.x + 90, gpo.y - 30}, FONTSIZE_50, WHITE, "TRADE");
    ground_grid = new ItemGrid(5, 6, 50, {gpo.x + 25, gpo.y + 20}, &shared_data);
    ground_grid->this_grid = GRID_GROUND;
    ground_grid->accepted_type = TYPE_ALL;

    ground_grid->selecting.Connect( [&](){OnItemSelected();} );
    ground_grid->not_selecting.Connect( [&](){OnItemDeselected();} );
    ground_grid->transfer_item.Connect( [&](){OnTransferItem();} );
    ground_grid->pickup.Connect( [&](){OnPickup();} );

    ground_grid->open_details.Connect( [&](){OnOpenDetails();} );
    ground_grid->close_details.Connect( [&](){OnCloseDetails();} );
    grid_list[GRID_GROUND] = ground_grid;
    

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

    inventory_grid->open_details.Connect( [&](){OnOpenDetails();} );
    inventory_grid->close_details.Connect( [&](){OnCloseDetails();} );
    grid_list[GRID_INVENTORY] = inventory_grid;

    details_panel  = new DetailsPanel();
    show_details = false;
}

DialogueMenu::~DialogueMenu() {
    //UnloadSound(button_sound);
    delete ground_grid;
    delete inventory_grid;

    delete details_panel;

    TraceLog(LOG_INFO, "UI DESTRUCTOR:  CHARACTER MENU UI");
}

void DialogueMenu::Draw() {
    DrawRectangleRec({0,0,g_resolution.x,g_resolution.y}, TRANSDARKERGRAY);
    DrawTexturePro(panel_bg, {0,0,(float)panel_bg.width, (float)panel_bg.height}, panel_rect, {0,0}, 0.0f, WHITE);


    DrawSprite(character_sprite);


    DrawLabel(ground_header_label);
    ground_grid->DrawGrid();
    DrawLabel(inventory_label_header);
    inventory_grid->DrawGrid();



    ground_grid->DrawItems();
    inventory_grid->DrawItems();
    //hotbar_grid->DrawItems();

    if(show_details) {
        details_panel->Draw();
    }

    if(g_game_settings.show_debug) {
    }
}

void DialogueMenu::DrawHotBarOnly() {
    //hotbar_grid->DrawGrid();
    //hotbar_grid->DrawItems();
}


void DialogueMenu::Update() {
    
    if(show_details) {
        details_panel->Update();
    }

    ground_grid->Update();
    inventory_grid->Update();
    //hotbar_grid->Update();

}

void DialogueMenu::Open() {
    inventory_grid->SetItems(&g_character_data[g_current_player->uid].inventory);

    use_ground = true;
    shared_data.dest_cell = {-1,-1};
    shared_data.dest_grid = GRID_NONE;
    shared_data.source_cell = {-1,-1};
    shared_data.source_grid = GRID_NONE;
    shared_data.item_id = -1;
    shared_data.use_id = -1;
    shared_data.showing_details = false;
    show_details = false;

    blank_list.clear();
    blank_list.push_back(-1);
    default_iid = "ground" + std::to_string(GetRandomValue(1000, 1000000));
    ground_grid->container_iid = default_iid;
    ground_grid->SetItems(&blank_list);
}


void DialogueMenu::OpenWith(NpcEntity  *npc_entity) {
    TraceLog(LOG_INFO, "OPENING DIALOGUE WITH %i", npc_entity->uid);

    inventory_grid->SetItems(&g_character_data[g_current_player->uid].inventory);

    use_ground = false;
    ground_grid->container_iid = "npc_" + std::to_string(npc_entity->uid);
    ground_grid->SetItems(& g_creature_data[npc_entity->uid].inventory);
    show_details = false;

}


void DialogueMenu::OnItemSelected() {
    for(auto &grid : grid_list) {
        if(grid != nullptr) {
            grid->can_select = false;
        }
    }

}

void DialogueMenu::OnItemDeselected() {
    for(auto &grid :grid_list) {
        if(grid != nullptr) {
            grid->can_select = true;
        }
    }

}


void DialogueMenu::OnPickup() {
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


void DialogueMenu::OnPutDownOrEquip() {
    /* int item_id = shared_data.item_id;
    int source_grid = shared_data.source_grid;
    int dest_grid =  GRID_GROUND;
    Vector2 source_cell = shared_data.source_cell;
    Vector2 dest_cell = shared_data.dest_cell;
    
    TraceLog(LOG_INFO, "put down or equip item %i", shared_data.item_id);

    auto i_itter = g_item_instances.find(shared_data.item_id);
    if(i_itter == g_item_instances.end()) {
        TraceLog(LOG_INFO, "item not found!!!");
        return;
    }



    for(int i = 0; i < grid_list.size(); i++) {
        TraceLog(LOG_INFO, "put down or equip item list / %i   type %i   ?  %i", i, grid_list[i]->accepted_type, g_item_instances[shared_data.item_id].type);

        if(grid_list[i]->accepted_type == g_item_instances[shared_data.item_id].type) {
            dest_grid = i;
            TraceLog(LOG_INFO, "found grid!!!");
            break;
        }
    }

    TraceLog(LOG_INFO, "inserting into grid %i", dest_grid);
    TraceLog(LOG_INFO, "dest cell %0.0f %0.0f", dest_cell.x, dest_cell.y);
    TraceLog(LOG_INFO, "source cell %0.0f %0.0f", source_cell.x, source_cell.y);
    TraceLog(LOG_INFO, "try to move %i  from %i %i", item_id, source_grid, dest_grid);

    if(grid_list[dest_grid]->HasRoom()) {
        grid_list[dest_grid]->AddItem(item_id);
        grid_list[source_grid]->RemoveItem(shared_data.source_cell);
        if(dest_grid != GRID_GROUND and dest_grid != GRID_INVENTORY and dest_grid != GRID_SECONDARY) {
            g_current_player->Equip(item_id);    
        }
        TraceLog(LOG_INFO, "move succsessfull!!!");
    }
    else if (dest_grid != GRID_GROUND) {
        dest_grid = GRID_GROUND;
        if(grid_list[dest_grid]->HasRoom()) {
            TraceLog(LOG_INFO, "moving to ground!!!");
            grid_list[dest_grid]->AddItem(item_id);
            grid_list[source_grid]->RemoveItem(shared_data.source_cell);
        }
    } */
    TraceLog(LOG_INFO, "---------------------------------\n");
}



void DialogueMenu::OnTransferItem() {

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
                //g_current_player->Equip(item_id);    
            }
            if(source_grid != GRID_GROUND and source_grid != GRID_INVENTORY and source_grid != GRID_SECONDARY) {
                //g_current_player->UnEquip(item_id);
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
    shared_data.showing_details = false;

    TraceLog(LOG_INFO, "---------------------------------\n");
}

void DialogueMenu::OnUseItem() {
/*     int item_id = shared_data.item_id;
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
            TraceLog(LOG_INFO, "------- saturation  %0.02f-------", source_itter->second.food_data.saturation);
            g_character_data[g_current_player->uid].saturation += source_itter->second.food_data.saturation;
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
    shared_data.showing_details = false; */

}


void DialogueMenu::OnOpenDetails() {
    if(!show_details) {
        show_details = true;
        shared_data.showing_details = true;
        details_panel->OpenPanel(&g_item_instances[shared_data.item_id]);
        TraceLog(LOG_INFO, "--------------open details %i------------------- %i ", shared_data.item_id, shared_data.source_grid);
    }
}


void DialogueMenu::OnCloseDetails() {
    if(show_details) {
        show_details = false;
        
        shared_data.dest_cell = {-1,-1};
        shared_data.dest_grid = GRID_NONE;
        shared_data.item_id = -1;
        shared_data.use_id = -1; 
        shared_data.source_cell = {-1,-1};
        shared_data.source_grid = GRID_NONE;
        shared_data.showing_details = false;
        TraceLog(LOG_INFO, "--------------close details %i  ------------------- %i", shared_data.item_id, shared_data.source_grid);
    }
}