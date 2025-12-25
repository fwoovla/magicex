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
    
    LoadSpriteCentered(sprite, g_creature_sprite_sheets[g_character_data[uid].sprite_sheet_id], position, 4, 16.0f, 0.10f);
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

    velocity = Vector2Lerp(velocity, {0.0f,0.0f}, .15);

    if(abs(velocity.x) < 4.0f) {
        velocity.x = {0.0};
    }
    if (abs(velocity.y) < 4.0f) {
        velocity.y = {0.0};
    }

    Vector2 previous_position = position;

    if(velocity.x != 0 or velocity.y != 0) {
        position = Vector2Add(position, velocity * GetFrameTime());
        SetAmination(sprite, ANIM_RUN);

        CollisionResult result;
        result.collision_dir = {0,0};

        if(CollideAndSlide(this, result, 2) == true) {
            //TraceLog(LOG_INFO, "COLLIDED, %0.0f %0.0f \n", result.collision_dir.x, result.collision_dir.y);
            if(result.collision_dir.x != 0) {
                position.x = previous_position.x;
                velocity.x = 0.0f;
            }
            if(result.collision_dir.y != 0) {
                position.y = previous_position.y;
                velocity.y = 0.0f;
            }
        }
    }

    sprite.position = position;
    weapon_sprite.position = position;
    shadow_sprite.position =  Vector2Add( position, {0, 3});

    if(is_stunned) {
        stun_timer.Update();
    }
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
    TraceLog(LOG_INFO, "%i trying to equip item %i", uid, item_id);

    int _id = ITEM_ID_NONE;

    auto item_it = g_item_instances.find(item_id);
    if(item_it != g_item_instances.end()) {
        if(item_it->second.type == TYPE_WEAPON) {
            g_character_data[uid].max_power += item_it->second.max_power;
            g_character_data[uid].current_power = item_it->second.current_power;
            _id = item_it->second.sprite_id;
            LoadSpriteCentered(weapon_sprite, g_item_sprites[ _id ], position);
            weapon_sprite.center.x -= weapon_sprite.center.x - 3;
            current_primary_data = &item_it->second;
            TraceLog(LOG_INFO, "equiping primary weapon %i sprite_id %i", item_id, _id);
        }
        
        if(item_it->second.type >= TYPE_HEAD_ARMOR and item_it->second.type <= TYPE_HAND_ARMOR) {
            _id = item_it->second.sprite_id;
            g_character_data[uid].defence += item_it->second.defence;
            g_character_data[uid].magic_defence += item_it->second.magic_defence;
            TraceLog(LOG_INFO, "equiping armor %i sprite_id %i", item_id, _id);
        }
        
        for(int mod = 0; mod < item_it->second.char_mods.size(); mod++) {
            TraceLog(LOG_INFO, "character mod %i %s", item_it->second.char_mods[mod].mod_id, item_it->second.char_mods[mod].mod_name.c_str());
            if(item_it->second.char_mods[mod].health != -1000){g_character_data[uid].health += item_it->second.char_mods[mod].health;}
            if(item_it->second.char_mods[mod].speed != -1000){g_character_data[uid].current_speed += item_it->second.char_mods[mod].speed;}
            if(item_it->second.char_mods[mod].stamina != -1000){g_character_data[uid].max_stamina += item_it->second.char_mods[mod].stamina;}
            
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
            TraceLog(LOG_INFO, "%i unequiping primary weapon %i %i", uid, item_id, g_character_data[uid].primary[0]);
            Texture2D t;
            LoadSpriteCentered(weapon_sprite, t, position);
            current_primary_data = nullptr;
                
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
                TraceLog(LOG_INFO, "character mod %i %s", item_it->second.char_mods[mod].mod_id, item_it->second.char_mods[mod].mod_name.c_str());
                if(item_it->second.char_mods[mod].health != -1000){g_character_data[uid].health -= item_it->second.char_mods[mod].health;}
                if(item_it->second.char_mods[mod].speed != -1000){g_character_data[uid].current_speed -= item_it->second.char_mods[mod].speed;}
                if(item_it->second.char_mods[mod].stamina != -1000){g_character_data[uid].max_stamina -= item_it->second.char_mods[mod].stamina;}
            
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