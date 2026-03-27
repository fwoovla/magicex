#include "../../core/gamedefs.h"


SpellGenMeneu::SpellGenMeneu() {

    CreateLabel(title_label, {g_screen_center.x, 20}, FONTSIZE_40, WHITE, "SpellGenMeneu MENU");
    wand_equipped = false;

    
    panel_bg = g_ui_panels[PANEL_SPELLGEN_SCREEN];
    panel_rect = {
        .x = 50,
        .y = 60,
        .width = ((float)g_resolution.x - 100),
        .height = ((float)g_resolution.y - 100)
    };

    center = {
        .x = panel_rect.x + (panel_rect.width/2),
        .y = panel_rect.y + (panel_rect.height/2)
    };


    CreateButton(generate_button, {center.x, panel_rect.y + panel_rect.height - 20}, {200, 30}, YELLOW, "Generate Spell");
    generate_button.text_size = FONTSIZE_30;

    CreateLabel(error_label, center, FONTSIZE_50, WHITE, "!must have wand equipped!");

    CreateLabel(points_label, {g_screen_center.x, panel_rect.y + 25}, FONTSIZE_30, WHITE, "points");

    Vector2 charge_stat_pos = {panel_rect.x + 20, panel_rect.y + 150};

    charge_interface = new SpellStatInterface("charge", charge_stat_pos, {280, 40}, &shared_spell);
    charge_interface->this_stat = SPELLSTAT_CHARGE;
    charge_interface->stat_changed.Connect( [&](){OnStatChanged();} );

    damage_interface = new SpellStatInterface("damage", {charge_stat_pos.x, charge_stat_pos.y + 60}, {280, 40}, &shared_spell);
    damage_interface->this_stat = SPELLSTAT_DAMAGE;
    damage_interface->stat_changed.Connect( [&](){OnStatChanged();} );

    preview = new SpellPreview({center.x, center.y - 50}, &shared_spell);
    //preview.shared_spell =  &shared_spell;
    //preview.center = center;
}

SpellGenMeneu::~SpellGenMeneu() {
    //UnloadSound(button_sound);
    delete charge_interface;
    delete damage_interface;
    delete preview;
    TraceLog(LOG_INFO, "UI DESTRUCTOR:  SpellGenMeneu ");
}

void SpellGenMeneu::Draw() {
    if(!wand_equipped) {
        DrawLabelCenteredWithBG(error_label, DARKRED);
        return;
    }


    

    DrawRectangleRec({0,0,g_resolution.x,g_resolution.y}, TRANSDARKERGRAY);
    DrawTexturePro(panel_bg, {0,0,(float)panel_bg.width, (float)panel_bg.height}, panel_rect, {0,0}, 0.0f, WHITE);
    DrawLabelCentered(title_label);
    DrawLabelCentered(points_label);
    DrawButton(generate_button);

    DrawSprite(wand_sprite);

    charge_interface->Draw();
    damage_interface->Draw();
    preview->Draw();


}

void SpellGenMeneu::Update() {
    if(!wand_equipped) {
        return;
    }
    points_label.text = "Points: " + std::to_string(shared_spell.points_available);

    charge_interface->Update();
    damage_interface->Update();
    preview->Update();

    if(IsButtonHovered(generate_button, g_scale)){
        if(generate_button.already_hovered == false) {
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            GenerateSpell();
        }
    }
}

void SpellGenMeneu::OnStatChanged() {
    
    SPELL_STAT stat_id = shared_spell.source_stat;
    
    int dir = 1;
    if(!shared_spell.add_stat) {
        dir = -1;
    }
    
    int cost = g_spell_rules.stats[stat_id].cost;
    float step = g_spell_rules.stats[stat_id].step;
    Limit limit = g_spell_rules.stats[stat_id].limit;

    TraceLog(LOG_INFO, "stat changed  %i  %i cost %i step %0.02f min %0.02f max %0.02f", shared_spell.source_stat,
        shared_spell.add_stat, cost, step, limit.min, limit.max);

    int prev_points = shared_spell.working_spell.stat_points[stat_id];
    int new_points = prev_points + (cost*dir);

        
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

void SpellGenMeneu::OpenModule(int instance_id) {

    wand_equipped = false;

    if(instance_id == -1) {
        TraceLog(LOG_INFO, "not item  " );
        return;
    }
    if(g_item_instances[instance_id].type != TYPE_WEAPON) {
        TraceLog(LOG_INFO, "not weapon  " );
        return;
    }
    if(g_item_instances[instance_id].weapon_data.weapon_type != WEAPON_TYPE_CASTER) {
        TraceLog(LOG_INFO, "not wand  " );
        return;
    }
    
    SpellData *this_spell = &g_item_instances[instance_id].weapon_data.wand_data.spell;

    wand_equipped = true;
    SpellBuildData this_build;
    this_build.is_exploding = this_spell->is_exploding;
    this_build.is_lingering = this_spell->is_lingering;
    this_build.spell_delivery_id = this_spell->delivery_type;
    this_build.spell_effect_id = this_spell->effect_type;

    shared_spell.working_spell = this_build;
    shared_spell.points_total = 10;
    shared_spell.points_available = shared_spell.points_total;
    shared_spell.working_spell.stat_points.resize(SPELLSTAT_COUNT);

    LoadSpriteCentered(wand_sprite, g_item_instances[instance_id].icon_texture, {center.x, center.y - 150});
    ScaleSprite(wand_sprite, {4,4});

    preview->SetSpellPreview(instance_id, this_spell);

    TraceLog(LOG_INFO, "spell menu open  " );    

}

void SpellGenMeneu::GenerateSpell() {
    ui_closed.EmitSignal();
    SpawnCharacterMessage (g_current_player->position, "Spell Created!", RAYWHITE, 1.0f);
    

}