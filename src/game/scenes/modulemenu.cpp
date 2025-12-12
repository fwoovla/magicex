#include "../../core/gamedefs.h"


ModuleMenu::ModuleMenu() {

    selected_button_index = -1;
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

    CreateLabel(recipie_header, rpo, FONTSIZE_50, WHITE, "RECIPIES");
    CreateLabel(recipie_label, {rpo.x, rpo.y + 50}, FONTSIZE_50, WHITE, "");

    CreateLabel(ingredient_header, inpo, FONTSIZE_50, WHITE, "REQUIRED");
    CreateLabel(ingredient_label, {inpo.x, inpo.y + 50}, FONTSIZE_40, WHITE, "");

    CreateLabel(inventory_header, ipo, FONTSIZE_50, WHITE, "INVENTORY");

    inventory_grid = new ItemGrid(5, 6, 50, {ipo.x + 50, ipo.y + 50}, &shared_data);

    CreateButton(craft_button, {inpo.x + 100, panel_rect.height }, {100, 30}, YELLOW, "CRAFT");
    craft_button.default_color = DARKYELLOW;
    craft_button.text_size = FONTSIZE_30;
}

ModuleMenu::~ModuleMenu() {
    //UnloadSound(button_sound);
    delete inventory_grid;
    TraceLog(LOG_INFO, "UI DESTRUCTOR:  MODULEMENU ");
}

void ModuleMenu::Draw() {
    DrawRectangleRec({0,0,g_resolution.x,g_resolution.y}, TRANSDARKERGRAY);
    DrawTexturePro(panel_bg, {0,0,(float)panel_bg.width, (float)panel_bg.height}, panel_rect, {0,0}, 0.0f, WHITE);
    DrawLabelCentered(title_label);
    DrawLabel(recipie_header);
    DrawLabel(ingredient_header);
    DrawLabel(recipie_label);

    if(selected_button_index >= 0) {
        if(button_lookup[selected_button_index] == 1) {
            ingredient_label.default_color = GREEN;
            DrawButton(craft_button);
        }
        else {
            ingredient_label.default_color = RED;
        }
    }

    DrawLabel(ingredient_label);
    DrawLabel(inventory_header);

    for(int i = 0; i < recipie_buttons.size(); i ++) {
        if(i == selected_button_index) {
            recipie_buttons[i].default_color.a = 150;
        }
        else {
            recipie_buttons[i].default_color.a = 60;
        }
        DrawButton(recipie_buttons[i]);
    }

    inventory_grid->DrawGrid();
    inventory_grid->DrawItems();

    //DrawCircleV(rpo, 2, RED);
    //DrawCircleV(ipo, 2, RED);
    //DrawCircleV(inpo, 2, RED);
}

void ModuleMenu::Update() {
    recipie_label.text = "";
    ingredient_label.text = "";

    if(selected_button_index >= 0) {
        if(!plan_selected) {

            auto r_itter = g_recipie_data.find(g_module_data[module_id].recipies[selected_button_index]);
            if(r_itter != g_recipie_data.end()) {
                
                for(int i = 0; i < r_itter->second.ingredients.size(); i++) {
                    auto i_itter = g_item_data.find(r_itter->second.ingredients[i]);
                    if(i_itter != g_item_data.end()) {
                        ingredient_label.text += "- " + i_itter->second.item_name + "\n";
                    }
                }   
            }
        }
        else {
            int plan_index = selected_button_index -  (button_lookup.size() - 2);
            //TraceLog(LOG_INFO, "looking for plan %i", plan_index);
            auto p_itter = g_plan_data.find(g_module_data[module_id].accepted_plans[selected_button_index - (button_lookup.size() - 2)]);
            if(p_itter != g_plan_data.end()) {
                //TraceLog(LOG_INFO, "plan found %s", p_itter->second.plan_name.c_str());
                
                for(int item = 0; item < p_itter->second.ingredients.size(); item++) {
                    
                    //TraceLog(LOG_INFO, "ingredient found? %i", p_itter->second.ingredients[item]);

                    auto i_itter = g_item_data.find(p_itter->second.ingredients[item]);
                    if(i_itter != g_item_data.end()) {
                        
                        //TraceLog(LOG_INFO, "ingredient found %s", i_itter->second.item_name.c_str());
                        ingredient_label.text += "- " + i_itter->second.item_name + "\n";
                    }
                }
            }
        }
    }
        
    for(int b = 0; b <recipie_buttons.size(); b++) {

        if(IsButtonHovered(recipie_buttons[b], g_scale)){
            if(recipie_buttons[b].already_hovered == false) {
            }
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                selected_button_index = b;

                plan_selected = false;
                for(int i = 0; i < plan_indexes.size(); i ++) {
                    if(plan_indexes[i] == selected_button_index) {
                        plan_selected = true;
                    }
                }
                if(plan_selected) {
                    TraceLog(LOG_INFO, "selected plan ingredient list size %i", module_data.accepted_plans.size());
                }
                else{
                    TraceLog(LOG_INFO, "selected recipie %i", module_data.recipies[b]);
                }
            }        
        }
    }

    if(IsButtonHovered(craft_button, g_scale)){
        if(craft_button.already_hovered == false) {
            //TraceLog(LOG_INFO, "selected hovered");
            //recipie_selected.EmitSignal();
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            //TraceLog(LOG_INFO, "crafting recipie %i", module_data.recipies[selected_button_index]);
            RecipieSelected();
            //recipie_selected.EmitSignal();
        }        
    }
}

void ModuleMenu::OpenModule() {
    inventory_grid->SetItems(&g_character_data[g_current_player->uid].inventory);
    selected_button_index = -1;
    recipie_buttons.clear();
    button_lookup.clear();
    plan_selected = false;

    module_id = g_game_data.current_module_id;

    auto itter = g_module_data.find(module_id);
    if(itter == g_module_data.end()) {
        TraceLog(LOG_INFO, " no module data available");
        return;
    }

    module_data = itter->second;

    title_label.text = module_data.module_name.c_str();
    TraceLog(LOG_INFO, " MODULEMENU %s  recipies %i", module_data.module_name.c_str(), module_data.recipies.size());

    int y_index = 0;
    for(int i = 0; i < module_data.recipies.size(); i++) {

        auto r_itter = g_recipie_data.find(module_data.recipies[i]);
        if(r_itter != g_recipie_data.end()) {
            TraceLog(LOG_INFO, "recipie data available");
            
            TraceLog(LOG_INFO, " recipies %s", r_itter->second.recipie_name.c_str());
            Button new_button;
            CreateButton(new_button, {rpo.x + 78, rpo.y + 50 + ( 40 * y_index)} , {200, 40}, BLANK, r_itter->second.recipie_name.c_str());
            new_button.text_size = FONTSIZE_20;
            new_button.default_color = BLANK;
            
            int ingredients_types_found = 0;
            int num_ingredients_types = r_itter->second.ingredients.size();
            bool can_craft = false;

            for(int ingredient = 0; ingredient < r_itter->second.ingredients.size(); ingredient++) {

                int ingredient_id = r_itter->second.ingredients[ingredient];
                TraceLog(LOG_INFO, " ingredient id %i", ingredient_id);


                for(int item = 0; item < g_character_data[g_current_player->uid].inventory.size(); item++) {

                    auto i_itter = g_item_instances.find(g_character_data[g_current_player->uid].inventory[item]);
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
                new_button.focus_color = TRANSRED;
                new_button.default_color = TRANSDARKRED;
            }
            else {
                new_button.text_color = DARKGREEN;
                new_button.text_color_focus = GREEN; 
                new_button.focus_color = TRANSGREEN;
                new_button.default_color = TRANSDARKGREEN;
                can_craft = true;
            }
            recipie_buttons.push_back(new_button);
            button_lookup.push_back(can_craft);
            y_index ++;
        }
    }

    TraceLog(LOG_INFO, "plans  available %i", module_data.accepted_plans.size());
    for(int i = 0; i < module_data.accepted_plans.size(); i++) {

        TraceLog(LOG_INFO, "index %i  plan id %i", i, module_data.accepted_plans[i]);

        auto p_itter = g_plan_data.find(module_data.accepted_plans[i]);
        if(p_itter != g_plan_data.end()) {
            TraceLog(LOG_INFO, "plan data available for %s %i    %i", p_itter->second.plan_name.c_str(), p_itter->second.plan_id, module_data.accepted_plans[i]);

            std::string module_name = "module not found";

            auto m_itter = g_module_data.find(p_itter->second.module_id);
            if(m_itter != g_module_data.end()) {
                module_name = m_itter->second.module_name;
            }
            else {
                return;
            }

            Button new_button;
            CreateButton(new_button, {rpo.x + 78, rpo.y + 50 + ( 40 * y_index)} , {200, 40}, BLANK, module_name.c_str());
            new_button.text_size = 20;
            new_button.default_color = BLANK;

            int ingredients_types_found = 0;
            int num_ingredients_types = p_itter->second.ingredients.size();
            bool can_craft = false;

            TraceLog(LOG_INFO, " ingredient list size  %i",  p_itter->second.ingredients.size());
            for(int ingredient = 0; ingredient < p_itter->second.ingredients.size(); ingredient++) {

                TraceLog(LOG_INFO, " ingredient id      %i", p_itter->second.ingredients[ingredient]);
                int ingredient_id = p_itter->second.ingredients[ingredient];
                TraceLog(LOG_INFO, " ingredient id %i", ingredient_id);


                for(int item = 0; item < g_character_data[g_current_player->uid].inventory.size(); item++) {

                    auto i_itter = g_item_instances.find(g_character_data[g_current_player->uid].inventory[item]);
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
                new_button.focus_color = TRANSRED;
                new_button.default_color = TRANSDARKRED;
            }
            else {
                new_button.text_color = DARKGREEN;
                new_button.text_color_focus = GREEN; 
                new_button.focus_color = TRANSGREEN;
                new_button.default_color = TRANSDARKGREEN;
                can_craft = true;
            }
            recipie_buttons.push_back(new_button);
            button_lookup.push_back(can_craft);
            plan_indexes.push_back(button_lookup.size() - 1);
            y_index ++;
        }
    }
}

void ModuleMenu::RecipieSelected() {
    if(selected_button_index < 0) {
        return;
    }

    if(button_lookup[selected_button_index] == 0) {
        TraceLog(LOG_INFO, "cannot craft item not enough resources");
        return;
    }

    if(!plan_selected) {

        auto r_itter = g_recipie_data.find(module_data.recipies[selected_button_index]);
        if(r_itter == g_recipie_data.end()) {
            TraceLog(LOG_INFO, "recipie not found");
            return;
        }

        TraceLog(LOG_INFO, "crafting %i", r_itter->second.produces);
        
        InstanceCharacterItem(r_itter->second.produces, g_current_player->uid);
        
        int instance_id;
        int item_id;

        for(int item = 0; item < g_character_data[g_current_player->uid].inventory.size(); item++) {
            auto i_itter = g_item_instances.find(g_character_data[g_current_player->uid].inventory[item]);
            if(i_itter != g_item_instances.end()) {
                item_id = i_itter->second.item_id;
                instance_id = i_itter->second.instance_id;

                for(int ingredient = 0; ingredient < r_itter->second.ingredients.size(); ingredient++) {
                    if(item_id == r_itter->second.ingredients[ingredient]) {
                        g_item_instances.erase(instance_id);
                        g_character_data[g_current_player->uid].inventory[item] = -1;
                        break;
                    }
                }
            }
        }
    }
    else {
        auto p_itter = g_plan_data.find(module_data.accepted_plans[selected_button_index - (button_lookup.size() - 2)]);
        if(p_itter == g_plan_data.end()) {
            TraceLog(LOG_INFO, "plan not found");
            return;
        }

        TraceLog(LOG_INFO, "crafting module  %i --------- COMMING SOON!!!!!", p_itter->second.module_id);
        
        //InstancePlayerItem(r_itter->second.produces);
        
        int instance_id;
        int item_id;

        for(int item = 0; item < g_character_data[g_current_player->uid].inventory.size(); item++) {
            auto i_itter = g_item_instances.find(g_character_data[g_current_player->uid].inventory[item]);
            if(i_itter != g_item_instances.end()) {
                item_id = i_itter->second.item_id;
                instance_id = i_itter->second.instance_id;

                for(int ingredient = 0; ingredient < p_itter->second.ingredients.size(); ingredient++) {
                    if(item_id == p_itter->second.ingredients[ingredient]) {
                        g_item_instances.erase(instance_id);
                        g_character_data[g_current_player->uid].inventory[item] = -1;
                        break;
                    }
                }
            }
        }

    }


    for(int item = 0; item < g_character_data[g_current_player->uid].inventory.size(); item++) {
        TraceLog(LOG_INFO, "player has %i in inventory", g_character_data[g_current_player->uid].inventory[item]);        
    }
    OpenModule();
}