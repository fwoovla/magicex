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

    shared_spell.points_total = 20;
    shared_spell.points_available = shared_spell.points_total;
    shared_spell.working_spell.stat_points.resize(SPELLSTAT_COUNT);
    

    CreateLabel(points_label, {g_screen_center.x, panel_rect.y + 25}, FONTSIZE_30, WHITE, "points");

    Vector2 r_stat_pos = {panel_rect.width - 260, panel_rect.y + 150};

    CreateLabel(charge_label, {r_stat_pos.x, r_stat_pos.y}, FONTSIZE_30, WHITE, "charge");
    CreateLabel(charge_cost_label, {charge_label.position.x, charge_label.position.y + 25}, FONTSIZE_30, WHITE, "charge cost");
    CreateLabel(damage_label, {r_stat_pos.x, r_stat_pos.y + 50}, FONTSIZE_30, WHITE, "damage");
    CreateLabel(damage_cost_label, {damage_label.position.x, damage_label.position.y + 25}, FONTSIZE_30, WHITE, "damage cost");

    charge_interface = new SpellStatInterface("charge", r_stat_pos, {290, 40}, &shared_spell);
    charge_interface->this_stat = SPELLSTAT_CHARGE;
    charge_interface->stat_changed.Connect( [&](){OnStatChanged();} );

    damage_interface = new SpellStatInterface("damage", {r_stat_pos.x, r_stat_pos.y + 60}, {290, 40}, &shared_spell);
    damage_interface->this_stat = SPELLSTAT_DAMAGE;
    damage_interface->stat_changed.Connect( [&](){OnStatChanged();} );

}

SpellGenMeneu::~SpellGenMeneu() {
    //UnloadSound(button_sound);
    delete charge_interface;
    TraceLog(LOG_INFO, "UI DESTRUCTOR:  SpellGenMeneu ");
}

void SpellGenMeneu::Draw() {
    DrawRectangleRec({0,0,g_resolution.x,g_resolution.y}, TRANSDARKERGRAY);
    DrawTexturePro(panel_bg, {0,0,(float)panel_bg.width, (float)panel_bg.height}, panel_rect, {0,0}, 0.0f, WHITE);
    DrawLabelCentered(title_label);
    DrawLabelCentered(points_label);

    //DrawLabelCentered(charge_label);
    //DrawLabelCentered(charge_cost_label);
    charge_interface->Draw();
    damage_interface->Draw();

    //DrawLabelCentered(damage_label);
    //DrawLabelCentered(damage_cost_label);

}

void SpellGenMeneu::Update() {
    points_label.text = "Points: " + std::to_string(shared_spell.points_available);

    std::string cpp = TextFormat("%0.02f" , g_spell_rules.stats[SPELLSTAT_CHARGE].step);
    std::string dpp = TextFormat("%0.02f" , g_spell_rules.stats[SPELLSTAT_DAMAGE].step);
    charge_cost_label.text = "Cost: " + std::to_string(g_spell_rules.stats[SPELLSTAT_CHARGE].cost) + " points:  +/-" + cpp;
    damage_cost_label.text = "Cost: " + std::to_string(g_spell_rules.stats[SPELLSTAT_DAMAGE].cost) + " points:  +/-" + dpp;

    charge_interface->Update();
    damage_interface->Update();

}

void SpellGenMeneu::OnStatChanged() {
    TraceLog(LOG_INFO, "stat changed  %i  %i ", shared_spell.source_stat, shared_spell.add_stat);


    SPELL_STAT stat_id = shared_spell.source_stat;

    
    int dir = 1;
    if(!shared_spell.add_stat) {
        dir = -1;
    }
    
    int cost = g_spell_rules.stats[stat_id].cost;
    float step = g_spell_rules.stats[stat_id].step;
    Limit limit = g_spell_rules.stats[stat_id].limit;
    
/*     if(shared_spell.points_available < cost) {
        return;
    } */

    int prev_points = shared_spell.working_spell.stat_points[stat_id];
    int new_points = prev_points + (1*dir);

        
    if(abs(prev_points) > abs(new_points) ) { //we are "taking back" points
        shared_spell.points_available += cost;
    }
    else {
        if(shared_spell.points_available < cost) {
            return;
        }
        shared_spell.points_available -= cost;
    }
        
    shared_spell.working_spell.stat_points[stat_id] = abs(new_points);
}

void SpellGenMeneu::OpenModule() {

    shared_spell.working_spell = {};
    shared_spell.working_spell.stat_points.resize(SPELLSTAT_COUNT);
    shared_spell.points_total = 20;



}