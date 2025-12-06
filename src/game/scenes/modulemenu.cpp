#include "../../core/gamedefs.h"


ModuleMenu::ModuleMenu() {

    selected_button_index = 0;
    CreateLabel(title_label, {g_screen_center.x, 20 / g_scale}, 40/g_scale, WHITE, "MODULE MENU");

    panel_bg = g_ui_panels[PANEL_MODULE_SCREEN];
    panel_rect = {
        .x = 50,
        .y = 60,
        .width = ((float)g_resolution.x - 100),
        .height = ((float)g_resolution.y - 100)
    };

    rpo = {panel_rect.x + 30, panel_rect.y + 50};
    inpo = {panel_rect.x + 250, panel_rect.y + 50};
    ipo = {panel_rect.x + 470, panel_rect.y + 50};

    CreateLabel(recipie_header, rpo, 24, WHITE, "RECIPIES");
    CreateLabel(recipie_label, {rpo.x, rpo.y + 50}, 24, WHITE, "");

    CreateLabel(ingredient_header, inpo, 24, WHITE, "REQUIRED");
    CreateLabel(ingredient_label, {inpo.x, inpo.y + 50}, 14, WHITE, "");

    CreateLabel(inventory_header, ipo, 24, WHITE, "INVENTORY");

    inventory_grid = new ItemGrid(5, 6, 50, {ipo.x + 50, ipo.y + 50}, &shared_data);

    CreateButton(craft_button, {inpo.x + 100, panel_rect.height }, {100, 30}, YELLOW, "CRAFT");
    craft_button.default_color = DARKYELLOW;
    craft_button.text_size = 20;
}

ModuleMenu::~ModuleMenu() {
    //UnloadSound(button_sound);
    TraceLog(LOG_INFO, "UI DESTRUCTOR:  MODULEMENU ");
}

void ModuleMenu::Draw() {
    DrawRectangleRec({0,0,g_resolution.x,g_resolution.y}, TRANSDARKERGRAY);
    DrawTexturePro(panel_bg, {0,0,(float)panel_bg.width, (float)panel_bg.height}, panel_rect, {0,0}, 0.0f, WHITE);
    DrawLabelCentered(title_label);
    DrawLabel(recipie_header);
    DrawLabel(ingredient_header);
    DrawLabel(recipie_label);
    DrawLabel(ingredient_label);
    DrawLabel(inventory_header);

    for(int i = 0; i < recipie_buttons.size(); i ++) {
        DrawButton(recipie_buttons[i]);
    }

    inventory_grid->DrawGrid();
    inventory_grid->DrawItems();

    if(button_lookup[selected_button_index] == 1) {
        DrawButton(craft_button);
    }
    //DrawCircleV(rpo, 2, RED);
    //DrawCircleV(ipo, 2, RED);
    //DrawCircleV(inpo, 2, RED);
}

void ModuleMenu::Update() {
    recipie_label.text = "";
    ingredient_label.text = "";

    auto r_itter = g_recipie_data.find(g_module_data[module_id].recipies[selected_button_index]);
    if(r_itter != g_recipie_data.end()) {

        for(int i = 0; i < r_itter->second.ingredients.size(); i++) {
            auto i_itter = g_item_data.find(r_itter->second.ingredients[i]);
            if(i_itter != g_item_data.end()) {
                    ingredient_label.text += i_itter->second.item_name + "\n";
            }
        }   
    }        

    for(int b = 0; b <recipie_buttons.size(); b++) {

        if(IsButtonHovered(recipie_buttons[b], g_scale)){
            if(recipie_buttons[b].already_hovered == false) {
                //TraceLog(LOG_INFO, "selected hovered");
                //recipie_selected.EmitSignal();
            }
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                selected_button_index = b;
                TraceLog(LOG_INFO, "selected recipie %i", module_data.recipies[b]);
                //RecipieSelected();
                //recipie_selected.EmitSignal();
            }        
        }
    }

    if(IsButtonHovered(craft_button, g_scale)){
            if(craft_button.already_hovered == false) {
                //TraceLog(LOG_INFO, "selected hovered");
                //recipie_selected.EmitSignal();
            }
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                //selected_button_index = b;
                TraceLog(LOG_INFO, "crafting recipie %i", module_data.recipies[selected_button_index]);
                RecipieSelected();
                //recipie_selected.EmitSignal();
            }        
        }

}

void ModuleMenu::OpenModule() {
    inventory_grid->SetItems(&g_player_data.inventory);
    selected_button_index = 0;
    recipie_buttons.clear();
    button_lookup.clear();

    module_id = g_game_data.current_module_id;

    auto itter = g_module_data.find(module_id);
    if(itter == g_module_data.end()) {
        TraceLog(LOG_INFO, " no module data available");
        return;
    }

    module_data = g_module_data[module_id];

    title_label.text = module_data.module_name.c_str();
    TraceLog(LOG_INFO, " MODULEMENU %s  recipies %i", module_data.module_name.c_str(), module_data.recipies.size());

    int y_index = 0;
    for(int i = 0; i < module_data.recipies.size(); i++) {

        auto r_itter = g_recipie_data.find(module_data.recipies[i]);
        if(r_itter != g_recipie_data.end()) {
            TraceLog(LOG_INFO, "recipie data available");
            
            TraceLog(LOG_INFO, " recipies %s", r_itter->second.recipie_name.c_str());
            
            Button new_button;
            CreateButton(new_button, {rpo.x + 75, rpo.y + 50 + ( 40 * y_index)} , {100, 30}, BLANK, r_itter->second.recipie_name.c_str());
            new_button.text_size = 20;
            new_button.default_color = BLANK;
            
            int ingredients_types_found = 0;
            int num_ingredients_types = r_itter->second.ingredients.size();
            bool can_craft = false;

            for(int ingredient = 0; ingredient < r_itter->second.ingredients.size(); ingredient++) {

                int ingredient_id = r_itter->second.ingredients[ingredient];
                TraceLog(LOG_INFO, " ingredient id %i", ingredient_id);


                for(int item = 0; item < g_player_data.inventory.size(); item++) {

                    auto i_itter = g_item_instances.find(g_player_data.inventory[item]);
                    if(i_itter != g_item_instances.end()) {
                        if(ingredient_id == i_itter->second.item_id) {
                            TraceLog(LOG_INFO, " found ingredient %i", i_itter->second.item_id);
                            ingredients_types_found++;
                            break;
                        }
                    }
                }
            }
            if(ingredients_types_found < num_ingredients_types) {
                new_button.text_color = DARKRED;
                new_button.text_color_focus = RED;
            }
            else {
                new_button.text_color = DARKGREEN;
                new_button.text_color_focus = GREEN; 
                can_craft = true;
            }
            recipie_buttons.push_back(new_button);
            button_lookup.push_back(can_craft);
            y_index ++;
        }
    }
    
}

void ModuleMenu::RecipieSelected() {

    if(button_lookup[selected_button_index] == 0) {
        TraceLog(LOG_INFO, "cannot craft item not enough resources");
        return;
    }

    auto r_itter = g_recipie_data.find(module_data.recipies[selected_button_index]);
    if(r_itter == g_recipie_data.end()) {
        TraceLog(LOG_INFO, "recipie not found");
        return;
    }
 
    
    TraceLog(LOG_INFO, "crafting", r_itter->second.produces);
    
    InstancePlayerItem(r_itter->second.produces);
    
    int instance_id;
    int item_id;

    for(int item = 0; item < g_player_data.inventory.size(); item++) {
        auto i_itter = g_item_instances.find(g_player_data.inventory[item]);
        if(i_itter != g_item_instances.end()) {
            item_id = i_itter->second.item_id;
            instance_id = i_itter->second.instance_id;

            for(int ingredient = 0; ingredient < r_itter->second.ingredients.size(); ingredient++) {
                if(item_id == r_itter->second.ingredients[ingredient]) {
                    g_item_instances.erase(instance_id);
                    g_player_data.inventory[item] = -1;
                    break;
                }
            }
        }
    }


    for(int item = 0; item < g_player_data.inventory.size(); item++) {
        TraceLog(LOG_INFO, "player has %i in inventory", g_player_data.inventory[item]);        
    }
    OpenModule();
}