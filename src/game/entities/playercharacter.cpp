#include "../../core/gamedefs.h"
#include<cmath>
#include <raymath.h>

#define DETECT_RANGE 3
//#define SPEED 25.0f



PlayerCharacter::PlayerCharacter(Vector2 _position, int _uid): CharacterEntity() {

    position = _position;
    uid = _uid;
    rotation = 0.0f;
    velocity = {0,0};
    //weapon_sprite = {};
    //shadow_sprite = {};
    
    LoadSpriteCentered(sprite, g_character_sprite_sheets[g_character_data[uid].sprite_sheet_id], position, 4, 16.0f, 0.10f);
    LoadSpriteCentered(shadow_sprite, g_shadow_sprites[SPRITE_SHADOW_CHAR1], position);
        
    collision_radius = 5;
    centered_offset = {0,0};
    ground_point_offset = {0, sprite.frame_size/2};
    collided = false;
    should_delete = false;
    can_switch = true;
    can_take_damage = true;

    //spell_timer.wait_time = 0.5f;
    spell_timer.timer_timeout.Connect( [&](){OnSpellTimerTimeout();} );
    can_use_spell = true;

    mele_timer.timer_timeout.Connect( [&](){OnMeleTimerTimeout();} );
    can_mele = true;

    int _id = ITEM_ID_ERROR;
    auto item_it = g_item_instances.find(g_character_data[uid].primary[0]);
    if(item_it != g_item_instances.end()) {
        Equip(g_character_data[uid].primary[0]);
        //_id = item_it->second.sprite_id;
        //LoadSpriteCentered(weapon_sprite, g_item_sprites[_id], position);
    }
    else {
        //Texture2D blank;
        //LoadSpriteCentered(weapon_sprite, blank, position);
    }

    Equip(g_character_data[uid].head[0]);
    Equip(g_character_data[uid].body[0]);
    Equip(g_character_data[uid].legs[0]);
    Equip(g_character_data[uid].feet[0]);
    Equip(g_character_data[uid].hands[0]);
}

void PlayerCharacter::Update() {
    CheckInput();

    collided = false;
    
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
    else {
        SetAmination(sprite, ANIM_IDLE);
    }
    if(g_input.world_mouse_position.x  < position.x){
        sprite.source.width = -sprite.size.x;
        swing_dir = -1;
    }
    else {
        sprite.source.width = sprite.size.x;
        swing_dir = 1;
    }

    sprite.position = position;
    weapon_sprite.position = position;
    shadow_sprite.position =  Vector2Add( position, {0, 3});

    if(!can_use_spell) {
        spell_timer.Update();
    }
    if(!can_mele) {
        mele_timer.Update();
    }

    if(weapon_state == WSTATE_MELE) {
        weapon_sprite.rotation =  Lerp(weapon_sprite.rotation, weapon_sprite.rotation + (400 * swing_dir), 0.1f);
        Vector2 t_pos = Vector2Add(Vector2Rotate( {8, 0}, weapon_sprite.rotation * DEG2RAD), position);
        CollisionResult result;
        if(CollideWithEntity(t_pos, 2, result)) {
            result.collider->TakeDamage();
        }
        if(weapon_sprite.rotation >=  weapon_end_rotation - 5 and weapon_sprite.rotation <=  weapon_end_rotation + 5) {
            weapon_state = WSTATE_IDLE;
        }
    }
    //TraceLog(LOG_INFO, "sprite rot  %0.2f", weapon_sprite.rotation);
}

void PlayerCharacter::Draw() {
    
    DrawSprite(shadow_sprite);
    DrawSprite(weapon_sprite);
    DrawSprite(sprite);

    //DrawCircleV(position,3, RED);
    //Vector2 t_pos = Vector2Add(Vector2Rotate( {8, 0}, weapon_sprite.rotation * DEG2RAD), position);
    //t_pos = Vector2Add(t_pos, position);
    //DrawCircleV( t_pos, 3, BLUE);

    if(weapon_state == WSTATE_MELE) {
    }
    if(g_game_settings.show_debug == true) {
        DrawCircleV( Vector2Add(position, centered_offset), collision_radius, RED);
        DrawCircleV(Vector2Add(position, centered_offset), 1, WHITE);
        DrawCircleV(Vector2Add(position, ground_point_offset), 1, BLUE); 
    }
}

void PlayerCharacter::DrawUI() {
    

}

void PlayerCharacter::CheckInput() {

    Rectangle screen_rect = {
        .x = (position.x - g_camera.target.x) * g_world2screen,
        .y = (position.y - g_camera.target.y) * g_world2screen,
        .width = sprite.size.x * g_world2screen,
        .height = sprite.size.y * g_world2screen,
    };

    Rectangle world_rect = {position.x, position.y, sprite.size.x, sprite.size.y};

    Vector2 input_dir = {0,0};

    if(g_input.key_up) {
        input_dir.y = -1;
    }

    if(g_input.key_down) {
        input_dir.y = 1;
    }

    if(g_input.key_left) {
        input_dir.x = -1;
    }

    if(g_input.key_right) {
        input_dir.x = 1;
    }
    
    input_dir = Vector2Normalize(input_dir);

    float speed = g_character_data[uid].current_speed;

    if(g_input.key_sprint) {
        speed = speed + (speed * 0.8f);
    }

    velocity = Vector2Lerp(velocity, input_dir * speed, .15);
    if(abs(velocity.x) < 4.0f) {
        velocity.x = {0.0};
    }
    if (abs(velocity.y) < 4.0f) {
        velocity.y = {0.0};
    }

    if(weapon_state == WSTATE_IDLE) {
        Vector2 pp = GetWorldToScreen2D( position, g_camera);
        pp = {pp.x * g_scale, pp.y*g_scale};
        weapon_sprite.rotation = GetAngleFromTo(pp, g_input.screen_mouse_position) * RAD2DEG;
    }
    
    if(g_input.key_switch_weapon and can_switch == true) {
        if(CanEquip(g_character_data[uid].secondary[0]) and CanUnEquip(g_character_data[uid].primary[0])) {
            TraceLog(LOG_INFO, "switching primary weapon");
            UnEquip(g_character_data[uid].primary[0]);
            Equip(g_character_data[uid].secondary[0]);
            int temp_primary = g_character_data[uid].primary[0];
            g_character_data[uid].primary[0] = g_character_data[uid].secondary[0];
            g_character_data[uid].secondary[0] = temp_primary;
            can_switch = false;
        }
    }
    if(!g_input.key_switch_weapon) {
        can_switch = true;
    }

    if(g_input.mouse_right_down and can_mele) {
        int item_id = -1;
        auto item_it = g_item_instances.find(g_character_data[uid].primary[0]);
        if(item_it != g_item_instances.end()) {
            item_id = item_it->second.item_id;
            TraceLog(LOG_INFO, "mele attack");
            mele_timer.Start(item_it->second.cooldown, true);
            can_mele = false;
            weapon_state = WSTATE_MELE;
            weapon_end_rotation = weapon_sprite.rotation + (400 * swing_dir);
        }
    }

    if(g_input.mouse_left_down and can_use_spell) {

        int item_id = -1;
        auto item_it = g_item_instances.find(g_character_data[uid].primary[0]);
        if(item_it != g_item_instances.end()) {
            item_id = item_it->second.item_id;
        }

        if(item_id == -1) {
            return;
        }

        auto spell_it = g_spell_data.find(item_it->second.spell_id);
        if(spell_it != g_spell_data.end()) {
            if(g_character_data[uid].current_power < spell_it->second.pps) {
                return;
            }
            if(g_game_data.is_in_sub_map) {
                SpawnSpell(spell_it->second , *g_sub_scene, {.position = position,.rotation = weapon_sprite.rotation,.shooter_id = 0});
            }
            else {
                SpawnSpell(spell_it->second , *g_current_scene, {.position = position,.rotation = weapon_sprite.rotation,.shooter_id = 0});
            }
            g_character_data[uid].current_power -= spell_it->second.pps;
            item_it->second.current_power = g_character_data[uid].current_power;
            spell_timer.Start(spell_it->second.cooldown, true);
            can_use_spell = false;
        }
    }
}


bool PlayerCharacter::CanEquip(int item_id) {
    return true;
}


void PlayerCharacter::Equip(int item_id) {
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


bool PlayerCharacter::CanUnEquip(int item_id) {
    return true;
}


void PlayerCharacter::UnEquip(int item_id) {

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


void PlayerCharacter::OnSpellTimerTimeout() {
    can_use_spell = true;
    //TraceLog(LOG_INFO, "can_use_spell");
}

void PlayerCharacter::OnMeleTimerTimeout() {
    can_mele = true;
    //TraceLog(LOG_INFO, "can_use_spell");
}

void PlayerCharacter::OnStunTimerTimeout() {
    is_stunned = false;
    //TraceLog(LOG_INFO, "can_use_spell");
}

PlayerCharacter::~PlayerCharacter()
{
    TraceLog(LOG_INFO, "deleting player!!!!!!!!!!!!!!!!!!!!");
}


float PlayerCharacter::GetYSort() {
    return position.y + ground_point_offset.y;
}

void PlayerCharacter::TakeDamage() {
    TraceLog(LOG_INFO, "taking damage %i", g_character_data[uid].health);
}