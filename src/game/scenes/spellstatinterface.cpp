#include "../../core/gamedefs.h"




SpellStatInterface::SpellStatInterface(std::string stat_name, Vector2 p, Vector2 s, SharedSpellData *shared_data) {
    shared_spell = shared_data;
    position = p;
    size = s;

    Vector2 center = {
        .x = position.x + (size.x/2),
        .y = position.y + (size.y/2)
    };

    bar_pos = {
        .x = position.x + size.y + 20,
        .y = position.y + (size.y/4)
    };

    CreateButton(minus_button, {p.x + 20 , p.y + (size.y/2)}, {size.y, size.y}, YELLOW, "-" );
    minus_button.text_size = FONTSIZE_30;
    CreateButton(plus_button, {p.x + size.x - 20 , p.y + (size.y/2)}, {size.y, size.y}, YELLOW, "+" );
    plus_button.text_size = FONTSIZE_30;

    CreateLabel(stat_name_label, {center.x, center.y - 10}, FONTSIZE_30, RAYWHITE, stat_name);
    CreateLabel(points_label, center, FONTSIZE_30, RAYWHITE, "?/?");
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
        
    for(int point = 0; point < tick_rects.size(); point++) {
        Color tick_color = DARKGREEN;

        if(point < shared_spell->working_spell.stat_points[this_stat]) {
            tick_color = GREEN;
        }

        DrawRectangleRec(tick_rects[point], tick_color);
    }
    

    DrawLabelCentered(stat_name_label);
    //DrawLabelCentered(points_label);
    DrawLabelCentered(stat_value_label);

    DrawButton(plus_button);
    DrawButton(minus_button);



    //DrawRectangleV(bar_pos, {5, 5}, GREEN);
}

void SpellStatInterface::Update() {


    
    int spent_points = shared_spell->working_spell.stat_points[this_stat];
    int unspent_points = shared_spell->points_available;
    int total_points = shared_spell->points_total;

    if(unspent_points < 0) {
        unspent_points = 0;
    }
    
    std::string p = std::to_string(spent_points);
    std::string mp = std::to_string(unspent_points);
    points_label.text = p + " / " + mp;

    float stat_value = shared_spell->working_spell.stat_points[this_stat] * g_spell_rules.stats[this_stat].step;
    float stat_base = g_spell_rules.stats[this_stat].base;

    tick_rects.clear();
    for(int point = 0; point < spent_points + unspent_points; point++) {
        Rectangle new_tick;

        new_tick.x = bar_pos.x + (15 * point);
        new_tick.y = bar_pos.y + 5;
        new_tick.width = 10;
        new_tick.height = 10;

        tick_rects.push_back(new_tick);
    }


    std::string v = TextFormat("%0.02f", (stat_base + stat_value) * g_spell_rules.stats[this_stat].scale );
    stat_value_label.text = v;

    if(IsButtonHovered(minus_button, g_scale)){
        if(minus_button.already_hovered == false) {
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if(spent_points > 0) {
                shared_spell->add_stat = false;
                shared_spell->source_stat = this_stat;
                stat_changed.EmitSignal();
            }
        }
    }

    if(IsButtonHovered(plus_button, g_scale)){
        if(plus_button.already_hovered == false) {
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if(unspent_points > 0) {
                shared_spell->add_stat = true;
                shared_spell->source_stat = this_stat;
                stat_changed.EmitSignal();
            }
        }
    }
}
