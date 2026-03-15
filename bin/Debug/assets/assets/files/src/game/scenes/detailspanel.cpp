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

    CreateLabel(saturation_label, {g_screen_center.x, panel_rect.y + 100}, FONTSIZE_30, RAYWHITE, "");

    CreateLabel(power_label, {g_screen_center.x, panel_rect.y + 100}, FONTSIZE_30, RAYWHITE, "");
    CreateLabel(pps_label, {g_screen_center.x, panel_rect.y + 130}, FONTSIZE_30, RAYWHITE, "");
    CreateLabel(damage_label, {g_screen_center.x, panel_rect.y + 180}, FONTSIZE_30, RAYWHITE, "");
    CreateLabel(knockback_label, {g_screen_center.x, panel_rect.y + 210}, FONTSIZE_30, RAYWHITE, "");
    CreateLabel(recoil_label, {g_screen_center.x, panel_rect.y + 240}, FONTSIZE_30, RAYWHITE, "");
    CreateLabel(accuracy_label, {g_screen_center.x, panel_rect.y + 270}, FONTSIZE_30, RAYWHITE, "");
    //CreateLabel(cooldown_label, {g_screen_center.x, panel_rect.y + 300}, FONTSIZE_30, RAYWHITE, "");

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
            DrawLabelCentered(pps_label);
        }

        DrawLabel(damage_label);

        if(item_data->weapon_data.recoil != 0) {
            DrawLabel(recoil_label);
        }

        DrawLabel(accuracy_label);

        DrawLabel(knockback_label);

        DrawLabelCentered(slot_label);
    }

    if(item_data->type == TYPE_FOOD) {
        DrawLabelCentered(saturation_label);
    }

    if(item_data->type >= TYPE_HEAD_ARMOR and item_data->type <= TYPE_HAND_ARMOR) {
        DrawLabel(defence_label);
        //DrawLabelCentered(defence_label);

        DrawLabel(magicdefence_label);
        //DrawLabelCentered(magicdefence_label);
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


/*         if(item_data->spell_id != SPELL_ID_NONE) {
            std::string damage = TextFormat("%i", (int)item_data->spell_data.damage);
            damage_label.text = "damage: " + damage;

            std::string knock = TextFormat("%0.2f",item_data->spell_data.knockback);
            knockback_label.text = "knockback: " + knock;
        }
        else {
            std::string damage = TextFormat("%i", item_data->weapon_data.damage);
            damage_label.text = "damage: " + damage;

            std::string knock = TextFormat("%0.2f",item_data->weapon_data.knockback);
            knockback_label.text = "knockback: " + knock;
        } */


        std::string cool = TextFormat("%0.2f",item_data->weapon_data.cooldown);
        cooldown_label.text = "cooldown: " + cool;

        std::string recoil = TextFormat("%0.0f",item_data->weapon_data.recoil);
        recoil_label.text = "recoil: " + recoil;

/*         std::string accuracy = TextFormat("%0.0f", (item_data->weapon_data.accuracy * 100));
        accuracy_label.text = "accuracy: " + accuracy + "%"; */

        int s = item_data->weapon_data.max_power / item_data->weapon_data.pps;
        std::string shots = TextFormat("%i", s );
        pps_label.text = "shots: " + shots;
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
