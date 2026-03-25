#include "../../core/gamedefs.h"




DetailsPanel::DetailsPanel() {
    item_data = nullptr;

    padding = 10;
    label_spacing = 20;
    
    panel_rect = {
        .x = g_screen_center.x - 250,
        .y = g_screen_center.y - 250,
        .width = 500,
        .height = 500
    };
    
    stat_pos = {panel_rect.x + padding, panel_rect.y + 120};

    ws_rect = {
            .x = stat_pos.x,
            .y = stat_pos.y,
            .width = panel_rect.width * 0.35f,
            .height = 350.0f
        };

    

    int quarter_x = (panel_rect.width - (padding*2)) /4;

    

    CreateLabel(item_name_label, {panel_rect.x + panel_rect.width/2, panel_rect.y + 30}, FONTSIZE_40, RAYWHITE, "");
    CreateLabel(item_type_label, {panel_rect.x + panel_rect.width/2, panel_rect.y + 70}, FONTSIZE_30, RAYWHITE, "");
    

    CreateLabel(shots_label, {panel_rect.x + panel_rect.width/2, panel_rect.y + 100}, FONTSIZE_30, RAYWHITE, "");
    CreateStatusBar(shots_bar,{shots_label.position.x - 50, shots_label.position.y + label_spacing}, 10, 100.0f, 5.0f, YELLOW);



    CreateLabel(value_label, {g_screen_center.x, panel_rect.y + panel_rect.height - 30}, FONTSIZE_30, RAYWHITE, "");

//food
    CreateLabel(saturation_label, {g_screen_center.x, panel_rect.y + 100}, FONTSIZE_30, RAYWHITE, "");

//weapon

    float ws_quarter_h = ws_rect.height/4;

    CreateLabel(base_label, {stat_pos.x + ws_rect.width/2, stat_pos.y + label_spacing}, FONTSIZE_30, RAYWHITE, "base");
    CreateLabel(base_tier_label, {base_label.position.x, base_label.position.y + 20}, FONTSIZE_30, RAYWHITE, "tier");

    CreateLabel(igniter_label, {stat_pos.x + ws_rect.width/2, stat_pos.y + ws_quarter_h + label_spacing}, FONTSIZE_30, RAYWHITE, "igniter");
    CreateLabel(igniter_tier_label, {igniter_label.position.x, igniter_label.position.y + 20}, FONTSIZE_30, RAYWHITE, "tier");

    CreateLabel(coupler_label, {stat_pos.x + ws_rect.width/2, stat_pos.y + (ws_quarter_h*2) + label_spacing}, FONTSIZE_30, RAYWHITE, "coupler");
    CreateLabel(coupler_tier_label, {coupler_label.position.x, coupler_label.position.y + 20}, FONTSIZE_30, RAYWHITE, "tier");

    CreateLabel(rod_label, {stat_pos.x + ws_rect.width/2, stat_pos.y + (ws_quarter_h*3) + label_spacing}, FONTSIZE_30, RAYWHITE, "rod");
    CreateLabel(rod_tier_label, {rod_label.position.x, rod_label.position.y + 20}, FONTSIZE_30, RAYWHITE, "tier");

    
    float bar_height = 10.0f;
    float bar_width = 100.0f;

//base
    CreateLabel(accuracy_label, { stat_pos.x + (padding*2) + (quarter_x*1.5f), 200}, FONTSIZE_30, RAYWHITE, "accuracy");
    CreateStatusBar(accuracy_bar,{accuracy_label.position.x + quarter_x, accuracy_label.position.y + (bar_height/2) }, g_stat_limits[STAT_ACCURACY].max, 100.0f, bar_height, YELLOW);
    CreateLabel(accuracy_value_label, { accuracy_label.position.x + (quarter_x*2), 200}, FONTSIZE_30, RAYWHITE, "a");

//igniter
    CreateLabel(recoil_label, { stat_pos.x + (padding*2) + (quarter_x*1.5f), 220 }, FONTSIZE_30, RAYWHITE, "recoil");
    CreateStatusBar(recoil_bar,{recoil_label.position.x + quarter_x, recoil_label.position.y + (bar_height/2)}, g_stat_limits[STAT_RECOIL].max, 100.0f, bar_height, YELLOW);
    CreateLabel(recoil_value_label, { recoil_label.position.x + (quarter_x*2), 220}, FONTSIZE_30, RAYWHITE, "r");

    CreateLabel(power_label, {stat_pos.x + (padding*2) + (quarter_x*1.5f), 240}, FONTSIZE_30, RAYWHITE, "max power");
    CreateStatusBar(power_bar,{power_label.position.x + quarter_x, power_label.position.y + (bar_height/2)}, g_stat_limits[STAT_MAX_POWER].max, 100.0f, bar_height, YELLOW);
    CreateLabel(power_value_label, { power_label.position.x + (quarter_x*2), 240}, FONTSIZE_30, RAYWHITE, "p");

//coupler
    CreateLabel(cooldown_label, {stat_pos.x + (padding*2) + (quarter_x*1.5f), 260}, FONTSIZE_30, RAYWHITE, "cooldown");
    CreateStatusBar(cooldown_bar,{cooldown_label.position.x + quarter_x, cooldown_label.position.y + (bar_height/2)}, g_stat_limits[STAT_CHARGE].max, 100.0f, bar_height, YELLOW);
    CreateLabel(cooldown_value_label, { cooldown_label.position.x + (quarter_x*2), 260}, FONTSIZE_30, RAYWHITE, "c");

    CreateLabel(pps_label, {stat_pos.x + (padding*2) + (quarter_x*1.5f), 280}, FONTSIZE_30, RAYWHITE, "pps");
    CreateStatusBar(pps_bar,{pps_label.position.x + quarter_x, pps_label.position.y + (bar_height/2)}, g_stat_limits[STAT_PPS].max, 100.0f, bar_height, YELLOW);
    CreateLabel(pps_value_label, { pps_label.position.x + (quarter_x*2), 280}, FONTSIZE_30, RAYWHITE, "pps");

    CreateLabel(damage_label, {stat_pos.x + (padding*2) + (quarter_x*1.5f), 300}, FONTSIZE_30, RAYWHITE, "damage");
    CreateStatusBar(damage_bar,{damage_label.position.x + quarter_x, damage_label.position.y + (bar_height/2)}, g_stat_limits[STAT_DAMAGE].max, 100.0f, bar_height, YELLOW);
    CreateLabel(damage_value_label, { damage_label.position.x + (quarter_x*2), 300}, FONTSIZE_30, RAYWHITE, "d");

    //rod
    CreateLabel(knockback_label, {stat_pos.x + (padding*2) + (quarter_x*1.5f), 320}, FONTSIZE_30, RAYWHITE, "knockback");
    CreateStatusBar(knockback_bar,{knockback_label.position.x + quarter_x, knockback_label.position.y + (bar_height/2)}, g_stat_limits[STAT_KNOCKBACK].max, 100.0f, bar_height, YELLOW);
    CreateLabel(knockback_value_label, { knockback_label.position.x + (quarter_x*2), 320}, FONTSIZE_30, RAYWHITE, "k");
    


//armor
    CreateLabel(defence_label, {g_screen_center.x, panel_rect.y + 150}, FONTSIZE_30, RAYWHITE, "");
    CreateLabel(magicdefence_label, {g_screen_center.x, panel_rect.y + 180}, FONTSIZE_30, RAYWHITE, "");

    CreateLabel(slot_label, {g_screen_center.x - 100, panel_rect.y + 180}, FONTSIZE_24, RAYWHITE, "");

}

DetailsPanel::~DetailsPanel() {
    //UnloadSound(button_sound);
    TraceLog(LOG_INFO, "UI DESTRUCTOR:  DETAILS PANEL UI");
}

void DetailsPanel::Draw() {
    //TraceLog(LOG_INFO, "DETAILS PANEL draw");
    DrawRectangleRec(panel_rect, DARKGRAY);

    DrawLabelCentered(item_name_label);
    DrawLabelCentered(item_type_label);
    DrawLabelCentered(value_label);

    if(item_data->type == TYPE_WEAPON) {
        
        if(item_data->weapon_data.max_power != 0) {
            DrawLabelCentered(shots_label);
            DrawStatusBar(shots_bar);    
        }
        if(item_data->weapon_data.weapon_type == WEAPON_TYPE_CASTER) {
            for(std::unique_ptr<SpellDetailsPanel> &panel : spell_details) {
                panel->Draw();    
            }
            TraceLog(LOG_INFO, "\n");
        }
/* 
        DrawRectangleRec(ws_rect, DARKERGRAY);

        DrawLabel(damage_label);
        DrawStatusBar(damage_bar);
        DrawLabel(damage_value_label);
     
        
        DrawLabelCentered(base_label);
        DrawLabelCentered(base_tier_label);
        DrawLabelCentered(base_quality_label);
        DrawLine(stat_pos.x, ws_rect.y + (ws_rect.height*0.25f), stat_pos.x + ws_rect.width, ws_rect.y + (ws_rect.height*0.25f), WHITE);
        DrawLabelCentered(igniter_label);
        DrawLabelCentered(igniter_tier_label);
        DrawLabelCentered(igniter_quality_label);
        DrawLine(stat_pos.x, ws_rect.y + (ws_rect.height*0.5f), stat_pos.x + ws_rect.width, ws_rect.y + (ws_rect.height*0.5f), WHITE);
        DrawLabelCentered(coupler_label);
        DrawLabelCentered(coupler_tier_label);
        DrawLabelCentered(coupler_quality_label);
        DrawLine(stat_pos.x, ws_rect.y + (ws_rect.height*0.75f), stat_pos.x + ws_rect.width, ws_rect.y + (ws_rect.height*0.75f), WHITE);
        DrawLabelCentered(rod_label);
        DrawLabelCentered(rod_tier_label);
        DrawLabelCentered(rod_quality_label);

        DrawLabel(accuracy_label);
        DrawStatusBar(accuracy_bar);
        DrawLabel(accuracy_value_label);

        DrawLabel(recoil_label);
        DrawStatusBar(recoil_bar);
        DrawLabel(recoil_value_label);

        DrawLabel(power_label);
        DrawStatusBar(power_bar);
        DrawLabel(power_value_label);

        DrawLabel(cooldown_label);
        DrawStatusBar(cooldown_bar);
        DrawLabel(cooldown_value_label);
             
        
        DrawLabel(pps_label);
        DrawStatusBar(pps_bar);
        DrawLabel(pps_value_label);

        DrawLabel(knockback_label);
        DrawStatusBar(knockback_bar); 
        DrawLabel(knockback_value_label);
 */
        //DrawLabelCentered(slot_label);
    }



    if(item_data->type == TYPE_FOOD) {
        DrawLabelCentered(saturation_label);
    }



    if(item_data->type >= TYPE_HEAD_ARMOR and item_data->type <= TYPE_HAND_ARMOR) {
        DrawLabel(defence_label);
        //DrawLabelCentered(defence_label);

        DrawLabel(magicdefence_label);
        //DrawLabelCentered(magicdefence_label);
        //DrawLabelCentered(slot_label);
    }

}

void DetailsPanel::Update() {
    if(item_data == nullptr) {
        return;
    }

    item_name_label.text = item_data->item_name;
    item_type_label.text = ItemTypeToStr(item_data->type);
    item_type_label.default_color = g_item_type_colors[item_data->type];
    value_label.text = "$" + std::to_string(item_data->value);

    if(item_data->type == TYPE_FOOD) {
        std::string sat = TextFormat("%0.2f", item_data->food_data.saturation);
        saturation_label.text = "saturation: " + sat;
    }

    if(item_data->type == TYPE_WEAPON) {

        float mp = item_data->weapon_data.max_power;
        float cp = item_data->weapon_data.current_power;
        float pps = item_data->weapon_data.pps;

        int max_shots = mp / pps;
        int current_shots = cp / pps;

        shots_label.text = "charges: " + std::to_string(current_shots) + "/" + std::to_string(max_shots);
        shots_bar.max_value = (float)max_shots;
        shots_bar.current_value = (float)current_shots;
/* 
        TraceLog(LOG_INFO,"\n------------------");
        TraceLog(LOG_INFO,"%0.2f mp",mp);
        TraceLog(LOG_INFO,"%0.2f cp",cp);
        TraceLog(LOG_INFO,"%0.2f pps",pps);
        TraceLog(LOG_INFO,"------------------\n");

 */
        if(item_data->weapon_data.weapon_type == WEAPON_TYPE_CASTER) {
            for(std::unique_ptr<SpellDetailsPanel> &panel : spell_details) {
                panel->Update();    
            }
            TraceLog(LOG_INFO, "\n");

        }
        else {

        }

    }


    if(item_data->type >= TYPE_HEAD_ARMOR and item_data->type <= TYPE_HAND_ARMOR) {
        std::string defence = TextFormat("%i", item_data->armor_data.defence);
        defence_label.text = "defence: " + defence;

        std::string mdefence = TextFormat("%i", item_data->armor_data.magic_defence);
        magicdefence_label.text = "magic defence: " + mdefence;
    }

    slot_label.text = "";
    for(int slot = 0; slot < item_data->mod_slots; slot++) {
        slot_label.text += "--empty slot--\n"; 
    }

}



void DetailsPanel::OpenPanel(ItemInstanceData *_data) {
    item_data = _data;

    if(_data->type == TYPE_WEAPON) {
        if(_data->weapon_data.weapon_type ==WEAPON_TYPE_CASTER) {
                
            Vector2 position = (Vector2){stat_pos.x, stat_pos.y + 80 + 80};
            Vector2 size = (Vector2){panel_rect.width - (padding * 2), 75};

            std::unique_ptr<SpellDetailsPanel> new_details = std::make_unique<SpellDetailsPanel>(
                &_data->weapon_data.wand_data.spell,
                position,
                size.x,
                size.y

            );

            spell_details.push_back( std::move(new_details));
            
        }
    }
    //TraceLog(LOG_INFO, "DETAILS PANEL open %s ", item_data->item_name.c_str());
}

void DetailsPanel::ClosePanel() {
    spell_details.clear();
}