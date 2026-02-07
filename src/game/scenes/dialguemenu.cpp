#include "../../core/gamedefs.h"

DialogueMenu::DialogueMenu() {

    grid_list.resize(GRID_NUM_GRIDS, nullptr);

    use_ground = false;
    trading = false;

    buy_items_value = 0;
    sell_items_value = 0;

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
    ground_grid = new TradeGrid(5, 6, 50, {gpo.x + 25, gpo.y + 20}, &shared_data);
    ground_grid->this_grid = GRID_GROUND;
    ground_grid->accepted_type = TYPE_ALL;

    ground_grid->selected.Connect( [&](){OnItemSelected();} );
    

    ground_grid->open_details.Connect( [&](){OnOpenDetails();} );
    ground_grid->close_details.Connect( [&](){OnCloseDetails();} );
    grid_list[GRID_GROUND] = ground_grid;
    

//character
    cpo = {panel_rect.x + 300, panel_rect.y + 45};
    CreateLabel(character_header_label, {cpo.x + 60, cpo.y - 30}, FONTSIZE_50, WHITE, "");
    
    ppo = {cpo.x + 130, cpo.y + 80}; //portrait offset

    CreateLabel(character_dialogue_label, {ppo.x, ppo.y + 100}, FONTSIZE_30, WHITE, "");

    
//end character

    ipo = {panel_rect.x + 560, panel_rect.y + 60};
    CreateLabel(inventory_label_header, {ipo.x + 60, ipo.y - 30}, FONTSIZE_50, WHITE, "INVENTORY");

    inventory_grid = new TradeGrid(5, 6, 50, {ipo.x + 25, ipo.y + 20}, &shared_data);
    inventory_grid->this_grid = GRID_INVENTORY;
    inventory_grid->accepted_type = TYPE_ALL;

    inventory_grid->selected.Connect( [&](){OnItemSelected();} );
    inventory_grid->selected.Connect( [&](){OnItemSelected();} );

    inventory_grid->open_details.Connect( [&](){OnOpenDetails();} );
    inventory_grid->close_details.Connect( [&](){OnCloseDetails();} );
    grid_list[GRID_INVENTORY] = inventory_grid;

    details_panel  = new DetailsPanel();
    show_details = false;
    
    response_buttons.resize(5);

    float _y = ppo.y + 150;
    for(Button &button : response_buttons) {
        CreateButton(button, {ppo.x, _y}, {100, 40}, DARKYELLOW, "choice");
        button.default_color = DARKERGRAY;
        button.text_size = FONTSIZE_24;
        _y += 45;
    }

    CreateLabel(sale_label, {ppo.x, panel_rect.height - 80}, FONTSIZE_30, RAYWHITE, "*/*");
    CreateButton(clear_sale_button, {ppo.x, panel_rect.height-50}, {100, 40}, DARKYELLOW, "clear sale");
    CreateButton(buy_button, {ppo.x, panel_rect.height}, {100, 40}, DARKRED, "purchase");
    buy_button.default_color = DARKERGRAY;
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

    DrawLabel(character_header_label);
    DrawLabelCentered(character_dialogue_label);

    for(Button &button : response_buttons) {
        if(button.text != "") {
            DrawButton(button);
        }
    }

    if(trading) {
        DrawLabel(ground_header_label);
        ground_grid->DrawGrid();
        DrawLabel(inventory_label_header);
        inventory_grid->DrawGrid();
           
        ground_grid->DrawItems();
        inventory_grid->DrawItems();
        DrawLabelCentered(sale_label);
        DrawButton(clear_sale_button);
        DrawButton(buy_button);

    }


    if(show_details) {
        details_panel->Draw();
    }

    if(g_game_settings.show_debug) {
    }
}

void DialogueMenu::DrawHotBarOnly() {}


void DialogueMenu::Update() {

    if(g_active_dialogue.tree == nullptr) {
        return;
    }
    
    if(show_details) {
        details_panel->Update();
    }

    ground_grid->Update();
    inventory_grid->Update();

    int option = 0;
    for(Button &button : response_buttons) {

        if(IsButtonHovered(button, g_scale) and button.text != ""){
            if(button.already_hovered == false) {
                //PlaySound(button_sound);
            }
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

                std::string next_node = g_active_dialogue.active_node->options[option].next_node;

                g_active_dialogue.active_node = &g_active_dialogue.tree->nodes[next_node];
                character_dialogue_label.text = g_active_dialogue.active_node->text;
                
                for(Button &button : response_buttons) {
                    button.text = "";
                }

                for(int i = 0; i < g_active_dialogue.active_node->options.size(); i++) {
                    if(i < response_buttons.size()) {
                        response_buttons[i].text = g_active_dialogue.active_node->options[i].text;
                    }
                }
                if(next_node == "shop") {trading = true;}
                else {
                    trading = false;
                    ClearTrade();
                }
            }        
        }
        option++;
    }

    if(IsButtonHovered(clear_sale_button, g_scale) and trading){
        if(clear_sale_button.already_hovered == false) {
            //PlaySound(button_sound);
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            TraceLog(LOG_INFO, "clear sale ");
            ClearTrade();
        }        
    }

    if(IsButtonHovered(buy_button, g_scale) and trading){
        if(clear_sale_button.already_hovered == false) {
            //PlaySound(button_sound);
        }

        if(buy_items_value < sell_items_value) {
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                TraceLog(LOG_INFO, "buy ");

                for(int item_id : items_to_buy) {
                    inventory_grid->AddItem(item_id);
                    ground_grid->RemoveItem(item_id);
                }
                for (int item_id : items_to_sell) {
                    ground_grid->AddItem(item_id);
                    inventory_grid->RemoveItem(item_id);
                }

                ClearTrade();
                
            }        
        }
    }

    sale_label.text = std::to_string(buy_items_value) + "/" + std::to_string(sell_items_value);
}

void DialogueMenu::Open() {
    inventory_grid->SetItems(&g_active_creature_data[g_current_player->uid].inventory);

    use_ground = false;
    trading = false;
    show_details = false;

    ClearTrade();

    shared_data.dest_cell = {-1,-1};
    shared_data.dest_grid = GRID_NONE;
    shared_data.source_cell = {-1,-1};
    shared_data.source_grid = GRID_NONE;
    shared_data.item_id = -1;
    shared_data.use_id = -1;
    shared_data.showing_details = false;

    blank_list.clear();
    blank_list.push_back(-1);
    default_iid = "ground" + std::to_string(GetRandomValue(1000, 1000000));
    ground_grid->container_iid = default_iid;
    ground_grid->SetItems(&blank_list);
}


void DialogueMenu::OpenWith(NpcEntity  *npc_entity) {
    g_active_dialogue.active_node = nullptr;
    g_active_dialogue.tree = nullptr;
    int sprite_id = g_active_creature_data[npc_entity->uid].sprite_sheet_id;

    TraceLog(LOG_INFO, "OPENING DIALOGUE WITH %i  sprite %i", npc_entity->uid, sprite_id);

    inventory_grid->SetItems(&g_active_creature_data[g_current_player->uid].inventory);

    use_ground = false;
    trading = false;
    show_details = false;

    ClearTrade();

    ground_grid->container_iid = "npc_" + std::to_string(npc_entity->uid);
    ground_grid->SetItems(&g_active_creature_data[npc_entity->uid].inventory);

    
    for(Button &button : response_buttons) {
        button.text = "";
    }

    auto d_itter = g_dialogues.find(g_active_creature_data[npc_entity->uid].dialogue_id);
    if(d_itter != g_dialogues.end()) {
        g_active_dialogue.tree = &d_itter->second;
        g_active_dialogue.active_node = &g_active_dialogue.tree->nodes[g_active_dialogue.tree->start_node];
        character_dialogue_label.text = g_active_dialogue.active_node->text;

        TraceLog(LOG_INFO, "----DIALOGUE OPTIONS %i", g_active_dialogue.active_node->options.size());

        for(int r = 0; r < g_active_dialogue.active_node->options.size(); r++) {
            TraceLog(LOG_INFO, "----DIALOGUE OPTION");
            if(r < response_buttons.size()) {
                response_buttons[r].text = g_active_dialogue.active_node->options[r].text;
            }
        }

        TraceLog(LOG_INFO, "----DIALOGUE FOUND");
    }
    else {
        TraceLog(LOG_INFO, "!!! DIALOGUE NOT FOUND !!!!");
    }

    LoadSpriteCentered(character_sprite, g_creature_sprite_sheets[ g_active_creature_data[npc_entity->uid].sprite_sheet_id], ppo, 4, 16.0f, 0.10f);
    ScaleSprite(character_sprite, {5,5});

    character_header_label.text = g_active_creature_data[npc_entity->uid].name;
}


void DialogueMenu::OnItemSelected() {

    if(shared_data.source_grid == GRID_GROUND) {
        items_to_buy.push_back(shared_data.item_id);
        buy_items_value += g_item_instances[shared_data.item_id].value;
    }
    else if(shared_data.source_grid == GRID_INVENTORY) {
        items_to_sell.push_back(shared_data.item_id);
        sell_items_value += g_item_instances[shared_data.item_id].value;
    }

    if(buy_items_value <= sell_items_value) {
        buy_button.focus_color = DARKGREEN;
    }
    else {
        buy_button.focus_color = DARKRED;
    }
}


void DialogueMenu::OnItemUnSelected() {
    if(shared_data.source_grid == GRID_GROUND) {
        for(int &item :items_to_buy) {
            if(item == shared_data.item_id) {item = -1;}
        }
        //items_to_buy.erase(shared_data.item_id);
        buy_items_value -= g_item_instances[shared_data.item_id].value;
    }
    else if(shared_data.source_grid == GRID_INVENTORY) {
        for(int &item :items_to_sell) {
            if(item == shared_data.item_id) {item = -1;}
        }
        //items_to_sell.push_back(shared_data.item_id);
        sell_items_value -= g_item_instances[shared_data.item_id].value;
    }

    if(buy_items_value <= sell_items_value) {
        buy_button.focus_color = DARKGREEN;
    }
    else {
        buy_button.focus_color = DARKRED;
    }
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

void DialogueMenu::ClearTrade() {
    for(TradeGrid *grid : grid_list) {
        if(grid != nullptr) {
            grid->selected_cells.clear();
        }
    }
    buy_items_value = 0;
    sell_items_value = 0;
    buy_button.focus_color = DARKRED;
    items_to_buy.clear();
    items_to_sell.clear();

}
