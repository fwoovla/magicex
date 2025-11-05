#include "../../core/gamedefs.h"

CharacterMenu::CharacterMenu() {

    CreateLabel(title_label, {g_screen_center.x, 20 / g_scale}, 30/g_scale, BLACK, "CHARACTER MENU");
    //LoadSpriteCentered(panel_bg, g_ui_panels[PANEL_CHAR_SCREEN], {g_screen_center.x, g_screen_center.y});
    panel_bg = g_ui_panels[PANEL_CHAR_SCREEN];
    panel_rect = {
        .x = 50,
        .y = 50,
        .width = ((float)g_resolution.x - 100),
        .height = ((float)g_resolution.y - 100)
    };
    //ScaleSprite(panel_bg, {2, 2});

    
    
    gpo = {panel_rect.x, panel_rect.y + 80};
    CreateLabel(ground_header_label, {gpo.x + 90, gpo.y - 30}, 30/g_scale, WHITE, "GROUND");
    ground_grid = new ItemGrid(6, 7, 50, gpo);
    
    cpo = {panel_rect.x + 300, panel_rect.y + 45};
    CreateLabel(character_header_label, {cpo.x + 60, cpo.y - 30}, 30/g_scale, WHITE, "CHARACTER");

    //LoadSpriteCentered(character_panel_sprite, g_ui_panels[PANEL_WOOD_LARGE], {cpo.x + 130, cpo.y + 100});
    LoadSpriteCentered(character_sprite, g_sprite_sheets[g_player_data.sprite_sheet_id], {cpo.x + 130, cpo.y + 50}, 4, 16.0f, 0.10f);
    ScaleSprite(character_sprite, {5,5});
    //CreateLabel(character_label,{cpo.x, cpo.y+200}, 20, RAYWHITE, g_player_data.class_name.c_str());
    //CreateLabel(character_stat_label,{cpo.x, cpo.y+250}, 14, RAYWHITE, "");

    
    ipo = {panel_rect.x + 560, panel_rect.y + 80};
    CreateLabel(inventory_label_header, {ipo.x + 60, ipo.y - 30}, 30/g_scale, WHITE, "INVENTORY");
    inventory_grid = new ItemGrid(6, 7, 50, ipo);

}

CharacterMenu::~CharacterMenu() {
    //UnloadSound(button_sound);
    delete ground_grid;
    delete inventory_grid;
    TraceLog(LOG_INFO, "UI DESTRUCTOR:  CHARACTER MENU UI");
}

void CharacterMenu::Draw() {
    DrawRectangleRec({0,0,g_resolution.x,g_resolution.y}, TRANSDARKERGRAY);
    //DrawRectangleRec(panel_rect, RED);
    DrawTexturePro(panel_bg, {0,0,(float)panel_bg.width, (float)panel_bg.height}, panel_rect, {0,0}, 0.0f, WHITE);
    //DrawLabelCentered(title_label);
    DrawLabel(ground_header_label);
    ground_grid->Draw();

    DrawLabel(inventory_label_header);
    inventory_grid->Draw();


    DrawLabel(character_header_label);
    //DrawSprite(character_panel_sprite);
    DrawSprite(character_sprite);
    //DrawLabel(character_label);
    //DrawLabel(character_stat_label);

    DrawCircleV(gpo, 2, RED);
    DrawCircleV(ipo, 2, RED);
    DrawCircleV(cpo, 2, RED);
}

void CharacterMenu::Update() {

    ground_grid->Update();
    inventory_grid->Update();

}
