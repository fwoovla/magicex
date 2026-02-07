#include "../../core/gamedefs.h"
#include<cmath>
#include <raymath.h>

#define DETECT_RANGE 3
//#define SPEED 25.0f



TestDummyEntity::TestDummyEntity(Vector2 _position, int _uid): CharacterEntity() {
    
    position = _position;
    uid = _uid;
    rotation = 0.0f;
    velocity = {0,0};
    weapon_sprite = {};
    
    LoadSpriteCentered(sprite, g_creature_sprite_sheets[g_active_creature_data[uid].sprite_sheet_id], position, 4, 16.0f, 0.10f);
    LoadSpriteCentered(shadow_sprite, g_shadow_sprites[SPRITE_SHADOW_CHAR1], position);
    SetAmination(sprite, ANIM_IDLE);

    collision_radius = 5;
    centered_offset = {0,0};
    ground_point_offset = {0, sprite.frame_size/2};
    collided = false;
    should_delete = false;
    can_switch = true;
    can_take_damage = true;
    is_stunned = false;

    
    int _id = ITEM_ID_ERROR;
    auto item_it = g_item_instances.find(g_active_creature_data[uid].primary[0]);
    if(item_it != g_item_instances.end()) {
        Equip(g_active_creature_data[uid].primary[0]);
    }

    Equip(g_active_creature_data[uid].head[0]);
    Equip(g_active_creature_data[uid].body[0]);
    Equip(g_active_creature_data[uid].legs[0]);
    Equip(g_active_creature_data[uid].feet[0]);
    Equip(g_active_creature_data[uid].hands[0]);

    stun_timer.timer_timeout.Connect( [&](){OnStunTimerTimeout();} );

    TraceLog(LOG_INFO, "CREATURE CREATED uid %i   creature id %i", _uid, g_active_creature_data[uid].creature_id);
}

void TestDummyEntity::Update() {


}

void TestDummyEntity::Draw() {
    //TraceLog(LOG_INFO, "+++++++draw++++++");
    DrawSprite(shadow_sprite);
    DrawSprite(weapon_sprite);
    DrawSprite(sprite);
    if(g_game_settings.show_debug == true) {
        DrawCircleV( Vector2Add(position, centered_offset), collision_radius, RED);
        DrawCircleV(Vector2Add(position, centered_offset), 1, WHITE);
        DrawCircleV(Vector2Add(position, ground_point_offset), 1, BLUE); 
    }
}

void TestDummyEntity::DrawUI() {

}


bool TestDummyEntity::CanEquip(int item_id) {
    return true;
}


void TestDummyEntity::Equip(int item_id) {

}


bool TestDummyEntity::CanUnEquip(int item_id) {
    return true;
}


void TestDummyEntity::UnEquip(int item_id) {


}


void TestDummyEntity::OnSpellTimerTimeout() {
    can_use_spell = true;
    //TraceLog(LOG_INFO, "can_use_spell");
}

void TestDummyEntity::OnMeleTimerTimeout() {
    can_mele = true;
    //TraceLog(LOG_INFO, "can_use_spell");
}

void TestDummyEntity::OnStunTimerTimeout() {
    is_stunned = false;
    sprite.modulate = WHITE;
    SetAmination(sprite, ANIM_IDLE);
    //TraceLog(LOG_INFO, "un stunned");
}

void TestDummyEntity::OnHungerTimerTimeout() {

}

TestDummyEntity::~TestDummyEntity()
{
    TraceLog(LOG_INFO, "deleting entity!!!!!!!!!!!!!!!!!!!! %i", uid);
}


float TestDummyEntity::GetYSort() {
    return position.y + ground_point_offset.y;
}


void TestDummyEntity::TakeDamage(DamagePayload _payload) {
    TraceLog(LOG_INFO, "%i: taking damage %i", uid, _payload.damage);

    is_stunned = true;
    sprite.modulate = RED;
    SetAmination(sprite, ANIM_STUN);
    stun_timer.Start(0.5f, true);

    std::string damage_string = std::to_string(_payload.damage);


    velocity = _payload.knockback;

    TraceLog(LOG_INFO, "knockback %0.2f  %0.2f", _payload.knockback.x, _payload.knockback.y);

    SpawnCharacterMessage (position, damage_string, DARKRED, 0.3f);

}