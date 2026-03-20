#include "../../core/gamedefs.h"


SpellGenMeneu::SpellGenMeneu() {

    CreateLabel(title_label, {g_screen_center.x, 20}, FONTSIZE_40, WHITE, "SpellGenMeneu MENU");

    panel_bg = g_ui_panels[PANEL_SPELLGEN_SCREEN];
    panel_rect = {
        .x = 50,
        .y = 60,
        .width = ((float)g_resolution.x - 100),
        .height = ((float)g_resolution.y - 100)
    };

    available_points = 10;

    CreateLabel(points_label, {g_screen_center.x, panel_rect.y + 25}, FONTSIZE_30, WHITE, "points");

    Vector2 r_stat_pos = {panel_rect.x + panel_rect.width - 150};

    CreateLabel(charge_label, {r_stat_pos.x, r_stat_pos.y}, FONTSIZE_30, WHITE, "charge");
    CreateLabel(charge_cost_label, {charge_label.position.x, charge_label.position.y + 25}, FONTSIZE_30, WHITE, "charge cost");
    CreateLabel(damage_label, {r_stat_pos.x, r_stat_pos.y + 50}, FONTSIZE_30, WHITE, "damage");
    CreateLabel(damage_cost_label, {damage_label.position.x, damage_label.position.y + 25}, FONTSIZE_30, WHITE, "damage cost");


}

SpellGenMeneu::~SpellGenMeneu() {
    //UnloadSound(button_sound);
    TraceLog(LOG_INFO, "UI DESTRUCTOR:  SpellGenMeneu ");
}

void SpellGenMeneu::Draw() {
    DrawRectangleRec({0,0,g_resolution.x,g_resolution.y}, TRANSDARKERGRAY);
    DrawTexturePro(panel_bg, {0,0,(float)panel_bg.width, (float)panel_bg.height}, panel_rect, {0,0}, 0.0f, WHITE);
    DrawLabelCentered(title_label);
    DrawLabelCentered(points_label);

    DrawLabel(charge_label);
    DrawLabel(charge_cost_label);

    DrawLabel(damage_label);
    DrawLabel(damage_cost_label);

}

void SpellGenMeneu::Update() {
    points_label.text = "Points: " + std::to_string(available_points);

    std::string cpp = TextFormat("%0.02f" , g_spell_rules.charge_per_point);
    std::string dpp = TextFormat("%0.02f" , g_spell_rules.damage_per_point);
    charge_cost_label.text = "Cost: " + std::to_string(g_spell_rules.charge_cost) + " points:  +/-" + cpp;//std::to_string(g_spell_rules.charge_per_point); 
    damage_cost_label.text = "Cost: " + std::to_string(g_spell_rules.damage_cost) + " points:  +/-" + dpp;//; std::to_string(g_spell_rules.damage_per_point); 


}

void SpellGenMeneu::OpenModule() {

}

void SpellGenMeneu::RecipieSelected() {

}