#include "../../core/gamedefs.h"




DetailsPanel::DetailsPanel() {
    item_data = nullptr;
    panel_rect = {
        .x = g_screen_center.x - 200,
        .y = g_screen_center.y - 200,
        .width = 400,
        .height = 400
    };


    CreateLabel(item_name_label, {g_screen_center.x, panel_rect.y + 30}, FONTSIZE_40, RAYWHITE, "");
    CreateLabel(item_type_label, {g_screen_center.x, panel_rect.y + 70}, FONTSIZE_30, RAYWHITE, "");
    
    CreateLabel(value_label, {g_screen_center.x, (g_screen_center.y + 200) - 30}, FONTSIZE_30, RAYWHITE, "");

//food
    CreateLabel(saturation_label, {g_screen_center.x, panel_rect.y + 100}, FONTSIZE_30, RAYWHITE, "");

//weapon
    CreateLabel(power_label, {g_screen_center.x, panel_rect.y + 90}, FONTSIZE_30, RAYWHITE, "");
    CreateStatusBar(power_bar,{g_screen_center.x - 50, panel_rect.y + 100}, g_stat_limits[STAT_MAX_POWER].max, 100.0f, 5.0f, YELLOW);


    CreateLabel(accuracy_label, {g_screen_center.x +75, panel_rect.y + 150}, FONTSIZE_30, RAYWHITE, "accuracy");
    CreateStatusBar(accuracy_bar,{g_screen_center.x +75, panel_rect.y + 170}, g_stat_limits[STAT_ACCURACY].max, 100.0f, 5.0f, GREEN);


    CreateLabel(recoil_label, {g_screen_center.x +75, panel_rect.y + 190}, FONTSIZE_30, RAYWHITE, "recoil");
    CreateStatusBar(recoil_bar,{g_screen_center.x +75, panel_rect.y + 210}, g_stat_limits[STAT_RECOIL].max, 100.0f, 5.0f, GREEN);

    
    CreateLabel(damage_label, {g_screen_center.x +75, panel_rect.y + 230}, FONTSIZE_30, RAYWHITE, "damage");
    CreateStatusBar(damage_bar,{g_screen_center.x +75, panel_rect.y + 250}, g_stat_limits[STAT_DAMAGE].max, 100.0f, 5.0f, GREEN);
    
    CreateLabel(cooldown_label, {g_screen_center.x +75, panel_rect.y + 270}, FONTSIZE_30, RAYWHITE, "cooldown");
    CreateStatusBar(cooldown_bar,{g_screen_center.x +75, panel_rect.y + 290}, g_stat_limits[STAT_COOLDOWN].max, 100.0f, 5.0f, BLUE);

    CreateLabel(pps_label, {g_screen_center.x +75, panel_rect.y + 310}, FONTSIZE_30, RAYWHITE, "pps");
    CreateStatusBar(pps_bar,{g_screen_center.x +75, panel_rect.y + 330}, g_stat_limits[STAT_PPS].max, 100.0f, 5.0f, GREEN);


    CreateLabel(knockback_label, {g_screen_center.x +75, panel_rect.y + 350}, FONTSIZE_30, RAYWHITE, "knockback");
    CreateStatusBar(knockback_bar,{g_screen_center.x +75, panel_rect.y + 370}, g_stat_limits[STAT_KNOCKBACK].max, 100.0f, 5.0f, RED);





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
        //DrawLabel(power_label);
        if(item_data->weapon_data.max_power != 0) {
            DrawLabelCentered(power_label);
            //DrawLabel(pps_label);
            DrawStatusBar(power_bar);    
        }


        DrawRectangle(
            accuracy_label.position.x - 10,
            accuracy_label.position.y,
            120,
            240,
            DARKERGRAY
        );

        DrawLabel(damage_label);
        DrawStatusBar(damage_bar);

        DrawLabel(recoil_label);
        DrawStatusBar(recoil_bar);

        DrawLabel(accuracy_label);
        DrawStatusBar(accuracy_bar);

        DrawLabel(cooldown_label);
        DrawStatusBar(cooldown_bar);
        
        DrawLabel(pps_label);
        DrawStatusBar(pps_bar);

        DrawLabel(knockback_label);
        DrawStatusBar(knockback_bar);

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
        std::string mpower = TextFormat("%i",(int)item_data->weapon_data.max_power);
        std::string cpower = TextFormat("%i",(int)item_data->weapon_data.current_power);
        power_label.text = "power: " + cpower + "/" + mpower;



        if(item_data->weapon_data.weapon_type == WEAPON_TYPE_CASTER) {

            item_name_label.default_color = g_spell_effect_colors[item_data->weapon_data.caster_data.igniter.effect];

            //std::string damage = TextFormat("%i", (int)item_data->weapon_data.caster_data.coupler.damage);
            //damage_label.text = "damage: " + damage;
            damage_bar.current_value = item_data->weapon_data.caster_data.coupler.damage;

            //std::string knock = TextFormat("%0.2f",item_data->weapon_data.caster_data.rod.knockback);
            //knockback_label.text = "knockback: " + knock;
            knockback_bar.current_value = item_data->weapon_data.caster_data.rod.knockback;

            //std::string cool = TextFormat("%0.2f", item_data->weapon_data.caster_data.coupler.cooldown);
            //cooldown_label.text = "cooldown: " + cool;
            cooldown_bar.current_value = item_data->weapon_data.caster_data.coupler.cooldown;

            //std::string recoil = TextFormat("%0.0f", item_data->weapon_data.caster_data.igniter.recoil);
            //recoil_label.text = "recoil: " + recoil;
            recoil_bar.current_value = item_data->weapon_data.caster_data.igniter.recoil;

            //std::string accuracy = TextFormat("%0.0f", item_data->weapon_data.caster_data.base.accuracy);
            //accuracy_label.text = "accuracy: " + accuracy;
            accuracy_bar.current_value = item_data->weapon_data.caster_data.base.accuracy;

            pps_bar.current_value = item_data->weapon_data.caster_data.coupler.pps;
        }
        else {
            //std::string damage = TextFormat("%i", item_data->weapon_data.damage);
            //damage_label.text = "damage: " + damage;
            damage_bar.current_value = item_data->weapon_data.caster_data.coupler.damage;

            //std::string knock = TextFormat("%0.2f",item_data->weapon_data.knockback);
            //knockback_label.text = "knockback: " + knock;
            knockback_bar.current_value = item_data->weapon_data.caster_data.rod.knockback;


            //std::string cool = TextFormat("%0.2f",item_data->weapon_data.cooldown);
            //cooldown_label.text = "cooldown: " + cool;
            cooldown_bar.current_value = item_data->weapon_data.cooldown;

            //std::string recoil = TextFormat("%0.0f",item_data->weapon_data.recoil);
            //recoil_label.text = "recoil: " + recoil;
            recoil_bar.current_value = item_data->weapon_data.caster_data.igniter.recoil;
        }


        

        

/*         std::string accuracy = TextFormat("%0.0f", (item_data->weapon_data.accuracy * 100));
        accuracy_label.text = "accuracy: " + accuracy + "%"; */

        //int s = item_data->weapon_data.max_power / item_data->weapon_data.pps;
        //std::string shots = TextFormat("%i", s );
        //pps_label.text = "shots: " + shots;
        //TraceLog(LOG_INFO, "SHOTS %s", shots.c_str());

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
    //TraceLog(LOG_INFO, "DETAILS PANEL open %s ", item_data->item_name.c_str());
}
