#include "../../core/gamedefs.h"


ModuleMenu::ModuleMenu() {

    CreateLabel(title_label, {g_screen_center.x, 20 / g_scale}, 40/g_scale, WHITE, "MODULE MENU");

    panel_bg = g_ui_panels[PANEL_MODULE_SCREEN];
    panel_rect = {
        .x = 50,
        .y = 60,
        .width = ((float)g_resolution.x - 100),
        .height = ((float)g_resolution.y - 100)
    };

    rpo = {panel_rect.x + 20, panel_rect.y + 50};
    ipo = {panel_rect.x + 320, panel_rect.y + 50};

    CreateLabel(recipie_header, rpo, 24, WHITE, "RECIPIES");
    CreateLabel(recipie_label, {rpo.x, rpo.y + 50}, 24, WHITE, "");

    CreateLabel(ingredient_header, ipo, 24, WHITE, "INGREDIENTS REQUIRED");
    CreateLabel(ingredient_label, {ipo.x, ipo.y + 50}, 24, WHITE, "");


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

    DrawCircleV(rpo, 2, RED);
    DrawCircleV(ipo, 2, RED);

}

void ModuleMenu::Update() {
    recipie_label.text = "";
    ingredient_label.text = "";
    for(int r = 0; r < g_module_data[module_id].recipies.size(); r++) {

        auto r_itter = g_recipie_data.find(g_module_data[module_id].recipies[r]);
        if(r_itter != g_recipie_data.end()) {
            //TraceLog(LOG_INFO, "recipie data available");
            recipie_label.text += r_itter->second.recipie_name + "\n";

            for(int i = 0; i < r_itter->second.ingredients.size(); i++) {
                auto i_itter = g_item_data.find(r_itter->second.ingredients[i]);
                if(i_itter != g_item_data.end()) {
                    ingredient_label.text += i_itter->second.item_name + "\n";
                }
            }   
        }        
    }

}

void ModuleMenu::OpenModule() {
    module_id = g_game_data.current_module_id;

    auto itter = g_module_data.find(module_id);
    if(itter == g_module_data.end()) {
        TraceLog(LOG_INFO, " no module data available");
        return;
    }

    title_label.text = itter->second.module_name.c_str();
    TraceLog(LOG_INFO, " MODULEMENU %s  recipies %i", g_module_data[module_id].module_name.c_str(), g_module_data[module_id].recipies.size());

    for(int i = 0; i < itter->second.recipies.size(); i++) {
        TraceLog(LOG_INFO, " recipies %s", g_recipie_data[itter->second.recipies[i]].recipie_name.c_str());

    }
}