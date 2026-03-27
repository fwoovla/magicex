#include "../../core/gamedefs.h"


SpellPreview::SpellPreview(Vector2 _center, SharedSpellData *shared_data) {
    center = _center;
    shared_spell = shared_data;
    generated_spell = nullptr;
    //wand_sprite = {};
    int spacing = 30;

    CreateLabel(delivery_label, {center.x, center.y + (spacing * 1)}, FONTSIZE_40, WHITE, "delivery");
    CreateLabel(charge_label, {center.x, center.y + (spacing * 3)}, FONTSIZE_40, WHITE, "charge");
    CreateLabel(damage_label, {center.x, center.y + (spacing * 4)}, FONTSIZE_40, WHITE, "damage");
    CreateLabel(duration_label, {center.x, center.y + (spacing * 5)}, FONTSIZE_40, WHITE, "duration");
    CreateLabel(radius_label, {center.x, center.y + (spacing * 6)}, FONTSIZE_40, WHITE, "radius");
}

SpellPreview::~SpellPreview() {

    //UnloadSound(button_sound);

    TraceLog(LOG_INFO, "UI DESTRUCTOR:  SpellPreview ");
}

void SpellPreview::Draw() {
        if(generated_spell == nullptr) {
        return;
    }
    //DrawSprite(wand_sprite);
    DrawLabelCentered(charge_label);
    DrawLabelCentered(damage_label);
    DrawLabelCentered(duration_label);
    DrawLabelCentered(delivery_label);
    DrawLabelCentered(radius_label);
}

void SpellPreview::Update() {
    if(generated_spell == nullptr) {
        return;
    }

//charge
    //value from points
    float charge_vfp =
    (shared_spell->spell_build.stat_points[SPELLSTAT_CHARGE] * g_spell_rules.stats[SPELLSTAT_CHARGE].step)/g_spell_rules.stats[SPELLSTAT_CHARGE].cost;

    float charge_base = g_spell_rules.stats[SPELLSTAT_CHARGE].base;
    float charge_total = charge_vfp;
    //generated_spell->chargetime = (charge_base + charge_vfp);

    std::string charge_string = TextFormat("%0.02f", charge_total * g_spell_rules.stats[SPELLSTAT_CHARGE].scale);
    charge_label.text = "charge " + charge_string;

//damage
    float damage_vfp =
    (shared_spell->spell_build.stat_points[SPELLSTAT_DAMAGE] * g_spell_rules.stats[SPELLSTAT_DAMAGE].step)/g_spell_rules.stats[SPELLSTAT_DAMAGE].cost;
    float damage_base = g_spell_rules.stats[SPELLSTAT_DAMAGE].base;
    generated_spell->damage = (damage_base + damage_vfp);

    std::string damage_string = TextFormat("%0.02f", generated_spell->damage * g_spell_rules.stats[SPELLSTAT_DAMAGE].scale);
    damage_label.text = "damage " + damage_string;

//duration
    float duration_vfp =
    (shared_spell->spell_build.stat_points[SPELLSTAT_DURATION] * g_spell_rules.stats[SPELLSTAT_DURATION].step)/g_spell_rules.stats[SPELLSTAT_DURATION].cost;
    float duration_base = g_spell_rules.stats[SPELLSTAT_DURATION].base;
    generated_spell->durration = (duration_base + duration_vfp);

    std::string duration_string = TextFormat("%0.02f", generated_spell->durration * g_spell_rules.stats[SPELLSTAT_DURATION].scale);
    duration_label.text = "durration " + duration_string;


//delivery
    SPELL_DELIVERY delivery = shared_spell->spell_build.spell_delivery_id;

    if(delivery == DELIVERY_INSTANT) {
        delivery_label.text = "Instant";
        generated_spell->delivery_type = DELIVERY_INSTANT;
    }
    else if(delivery == DELIVERY_PROJECTILE) {
        delivery_label.text = "Projectile";
        generated_spell->delivery_type = DELIVERY_PROJECTILE;
    }

//radius
    float radius_vfp =
    (shared_spell->spell_build.stat_points[SPELLSTAT_RADIUS] * g_spell_rules.stats[SPELLSTAT_DURATION].step)/g_spell_rules.stats[SPELLSTAT_RADIUS].cost;
    float radius_base = g_spell_rules.stats[SPELLSTAT_RADIUS].base;
    generated_spell->radius = (radius_base + radius_vfp);

    std::string radius_string = TextFormat("%0.02f", generated_spell->radius * g_spell_rules.stats[SPELLSTAT_RADIUS].scale);
    radius_label.text = "radius " + radius_string;

}

void SpellPreview::SetSpellPreview(int instance_id, SpellData *spell_data) {
    generated_spell = spell_data;
}