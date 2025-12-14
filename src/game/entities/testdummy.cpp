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
    
    LoadSpriteCentered(sprite, g_creature_sprite_sheets[g_character_data[uid].creature_id], position, 4, 16.0f, 0.10f);
    LoadSpriteCentered(shadow_sprite, g_shadow_sprites[SPRITE_SHADOW_CHAR1], position);
    //SetAmination(sprite, ANIM_IDLE);

    collision_radius = 5;
    centered_offset = {0,0};
    ground_point_offset = {0, sprite.frame_size/2};
    collided = false;
    should_delete = false;
    can_switch = true;
    can_take_damage = true;

    
    int _id = ITEM_ID_ERROR;
    auto item_it = g_item_instances.find(g_character_data[uid].primary[0]);
    if(item_it != g_item_instances.end()) {
        Equip(g_character_data[uid].primary[0]);
    }

    Equip(g_character_data[uid].head[0]);
    Equip(g_character_data[uid].body[0]);
    Equip(g_character_data[uid].legs[0]);
    Equip(g_character_data[uid].feet[0]);
    Equip(g_character_data[uid].hands[0]);

    stun_timer.timer_timeout.Connect( [&](){OnStunTimerTimeout();} );

    TraceLog(LOG_INFO, "CREATURE CREATED uid %i   creature id %i", _uid, g_character_data[uid].creature_id);
}

void TestDummyEntity::Update() {
    sprite.position = position;
    weapon_sprite.position = position;
    shadow_sprite.position =  Vector2Add( position, {0, 3});

    if(is_stunned) {
        stun_timer.Update();
    }
}

void TestDummyEntity::Draw() {

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
    TraceLog(LOG_INFO, "trying to equip iteme %i", item_id);

    int _id = ITEM_ID_NONE;

    auto item_it = g_item_instances.find(item_id);
    if(item_it != g_item_instances.end()) {
        if(item_it->second.type == TYPE_WEAPON) {
            g_character_data[uid].max_power += item_it->second.max_power;
            g_character_data[uid].current_power = item_it->second.current_power;
            _id = item_it->second.sprite_id;
            LoadSpriteCentered(weapon_sprite, g_item_sprites[ _id ], position);
            weapon_sprite.center.x -= weapon_sprite.center.x - 3;
            TraceLog(LOG_INFO, "equiping primary weapon %i sprite_id %i", item_id, _id);
        }
        
        if(item_it->second.type >= TYPE_HEAD_ARMOR and item_it->second.type <= TYPE_HAND_ARMOR) {
            _id = item_it->second.sprite_id;
            g_character_data[uid].defence += item_it->second.defence;
            g_character_data[uid].magic_defence += item_it->second.magic_defence;
            TraceLog(LOG_INFO, "equiping armor %i sprite_id %i", item_id, _id);
        }
        
        for(int mod = 0; mod < item_it->second.char_mods.size(); mod++) {
            auto m_itter = g_char_mod_data.find(item_it->second.char_mods[mod]);
            if(m_itter != g_char_mod_data.end()) {
                TraceLog(LOG_INFO, "character mod %i %s", m_itter->second.mod_id, m_itter->second.mod_name.c_str());
                if(m_itter->second.health != -1000){g_character_data[uid].health += m_itter->second.health;}
                if(m_itter->second.speed != -1000){g_character_data[uid].current_speed += m_itter->second.speed;}
            }
        }
    }
    TraceLog(LOG_INFO, "+++++++++++++");
}


bool TestDummyEntity::CanUnEquip(int item_id) {
    return true;
}


void TestDummyEntity::UnEquip(int item_id) {

    auto item_it = g_item_instances.find(item_id);

    if(item_it != g_item_instances.end()) {
        if(item_it->second.type == TYPE_WEAPON) {
            TraceLog(LOG_INFO, "unequiping primary weapon %i %i", item_id, g_character_data[uid].primary[0]);
            //if (g_character_data[uid].primary[0] == -1) {
                Texture2D t;
                LoadSpriteCentered(weapon_sprite, t, position);
                
            //}

            g_character_data[uid].max_power -= item_it->second.max_power;
            g_character_data[uid].current_power -= item_it->second.max_power;

            if(g_character_data[uid].max_power < 0) {
                g_character_data[uid].max_power = 0;
            }

            if(item_it->second.current_power > item_it->second.max_power) {
                item_it->second.current_power = item_it->second.max_power;
            }
            if(g_character_data[uid].current_power < 0) {
                g_character_data[uid].current_power = 0;
            }
        }

        if(item_it->second.type >= TYPE_HEAD_ARMOR and item_it->second.type <= TYPE_HAND_ARMOR) {
            TraceLog(LOG_INFO, "unequiping armor %i", item_id);
            g_character_data[uid].defence -= item_it->second.defence;
            g_character_data[uid].magic_defence -= item_it->second.magic_defence;
        }
        
        for(int mod = 0; mod < item_it->second.char_mods.size(); mod++) {
            auto m_itter = g_char_mod_data.find(item_it->second.char_mods[mod]);
            if(m_itter != g_char_mod_data.end()) {
                TraceLog(LOG_INFO, "character mod %i %s", m_itter->second.mod_id, m_itter->second.mod_name.c_str());
                if(m_itter->second.health != -1000){g_character_data[uid].health -= m_itter->second.health;}
                if(m_itter->second.speed != -1000){g_character_data[uid].current_speed -= m_itter->second.speed;}
            }
        }
    }
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

TestDummyEntity::~TestDummyEntity()
{
    TraceLog(LOG_INFO, "deleting entity!!!!!!!!!!!!!!!!!!!! %i", uid);
}


float TestDummyEntity::GetYSort() {
    return position.y + ground_point_offset.y;
}


void TestDummyEntity::TakeDamage() {
    //TraceLog(LOG_INFO, "taking damage %i", g_character_data[uid].health);
    is_stunned = true;
    sprite.modulate = RED;
    SetAmination(sprite, ANIM_STUN);
    stun_timer.Start(0.5f, true);
}