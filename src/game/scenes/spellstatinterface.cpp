#include "../../core/gamedefs.h"




SpellStatInterface::SpellStatInterface(std::string stat_name, Vector2 p, Vector2 s, SharedSpellData *shared_data) {
    stat_string =  stat_name;
    shared_spell = shared_data;
    position = p;
    size = s;

    Vector2 center = {
        .x = position.x + (size.x/2),
        .y = position.y + (size.y/2)
    };

    bar_start = {
        .x = (center.x - size.x/2) + (size.y + 10),
        .y = position.y + (size.y/4) + 10
    };


    bar_end = {
        .x = center.x + (size.x/2) - (size.y + 10),
        .y = position.y + (size.y/4) + 10
    };
    

    CreateStatusBar(bar, bar_start, 10, bar_end.x - bar_start.x, size.y/2, GREEN);


    CreateButton(minus_button, {p.x + 20 , p.y + (size.y/2)}, {size.y, size.y}, YELLOW, "-" );
    minus_button.text_size = FONTSIZE_30;
    CreateButton(plus_button, {p.x + size.x - 20 , p.y + (size.y/2)}, {size.y, size.y}, YELLOW, "+" );
    plus_button.text_size = FONTSIZE_30;

    CreateLabel(stat_name_label, {center.x, center.y - 10}, FONTSIZE_30, RAYWHITE, stat_string);
    CreateLabel(points_label, bar_end, FONTSIZE_30, RAYWHITE, "?/?");
    CreateLabel(stat_value_label, {center.x, center.y + 20}, FONTSIZE_30, RAYWHITE, "?");

}

SpellStatInterface::~SpellStatInterface() {
    //UnloadSound(button_sound);

    TraceLog(LOG_INFO, "UI DESTRUCTOR:  SpellStatInterface  UI");
}

void SpellStatInterface::Draw() {
    Rectangle rect = {
        .x = position.x,
        .y = position.y,
        .width = size.x,
        .height = size.y
    };


    DrawRectangleRec(rect, DARKERGRAY);

    DrawStatusBar(bar);
    for (int i = 0; i <= bar.max_value; i++) {
        float x = bar.position.x + i * bar.unit_ratio;
        DrawLine(x, bar.position.y, x, bar.position.y + bar.height, DARKGRAY);
    }

    DrawLabelCentered(stat_name_label);
    //DrawLabelCentered(stat_value_label);
    
    DrawButton(plus_button);
    DrawButton(minus_button);
    if(g_game_settings.show_debug) {
        DrawLabel(points_label);   
    }
}

void SpellStatInterface::Update() {

    int spent_points = shared_spell->spell_build.stat_points[this_stat];
    int unspent_points = shared_spell->points_available;
    int total_points = shared_spell->points_total;
    float step = g_spell_rules.stats[this_stat].step;

    if(unspent_points < 0) {
        unspent_points = 0;
    }
   
    float added_value = (spent_points * step)/ g_spell_rules.stats[this_stat].cost;
    float stat_base = g_spell_rules.stats[this_stat].base;
    float accumulated_stat = (stat_base + added_value);// * g_spell_rules.stats[this_stat].scale;
    
    bar.max_value = shared_spell->points_total;
    bar.current_value = spent_points;
    
    
/*     
    stat_name_label.text = stat_string + " " + as ; */
    
    float min_value = g_spell_rules.stats[this_stat].limit.min;
    float max_value = g_spell_rules.stats[this_stat].limit.max;

    float over_value = accumulated_stat + step;
    float under_value = accumulated_stat - step;

    if(IsButtonHovered(minus_button, g_scale)){
        if(minus_button.already_hovered == false) {
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

            if(step > 0) {
                if(under_value >= min_value) {
                    shared_spell->add_stat = false;
                    shared_spell->source_stat = this_stat;
                    stat_changed.EmitSignal();
                }
            }
            else {
                if(under_value <= max_value) {
                    shared_spell->add_stat = false;
                    shared_spell->source_stat = this_stat;
                    stat_changed.EmitSignal();
                }
            }
        }
    }

    if(IsButtonHovered(plus_button, g_scale)){
        if(plus_button.already_hovered == false) {
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

            if(step > 0) {
                if(over_value <= max_value) {           
                    shared_spell->add_stat = true;
                    shared_spell->source_stat = this_stat;
                    stat_changed.EmitSignal();
                }
            }
            else {
                if(over_value >= min_value) {
                    shared_spell->add_stat = true;
                    shared_spell->source_stat = this_stat;
                    stat_changed.EmitSignal();
                }
            }
        }
        
    }

    if(g_game_settings.show_debug) {
        std::string as = TextFormat("%0.02f",  accumulated_stat * g_spell_rules.stats[this_stat].scale);
        std::string over = std::to_string(over_value);
        std::string under = std::to_string(under_value);
        std::string st = std::to_string(g_spell_rules.stats[this_stat].step);
        points_label.text = "under " + under + "      over " + over + "  as " + as + " step " + st; 
    }       
   
}
