#include "../../core/gamedefs.h"
#include<cmath>
#include <raymath.h>

#define DETECT_RANGE 3
//#define SPEED 25.0f

enum AnimationState {
    IDLE = 0,
    WALK = 1,
    RUN = 2,
};

PlayerCharacter::PlayerCharacter(Vector2 _position, int _uid): CharacterEntity() {

    position = _position;
    uid = _uid;
    rotation = 0.0f;
    velocity = {0,0};
    
    LoadSpriteCentered(sprite, g_sprite_sheets[g_character_data[uid].sprite_sheet_id], position, 4, 16.0f, 0.10f);
    //LoadSpriteCentered(sprite, g_sprite_sheets[g_player_data.sprite_sheet_id], position, 4, 16.0f, 0.10f);

    int _id = ITEM_ID_ERROR;
    auto item_it = g_item_instances.find(g_character_data[uid].primary[0]);
    //auto item_it = g_item_instances.find(g_player_data.primary[0]);
    if(item_it != g_item_instances.end()) {
        _id = item_it->second.sprite_id;
        LoadSpriteCentered(weapon_sprite, g_item_sprites[_id], position);
    }
    else {
        Texture2D blank;
        LoadSpriteCentered(weapon_sprite, blank, position);
    }
    
    collision_radius = 5;
    centered_offset = {0,0};
    ground_point_offset = {0, sprite.frame_size/2};
    collided = false;
    should_delete = false;
    can_switch = true;
    shot_timer.wait_time = 0.5f;
    shot_timer.timer_timeout.Connect( [&](){OnShotTimerTimeout();} );
    can_shoot = true;
}

void PlayerCharacter::Update() {
    //TraceLog(LOG_INFO, "player update");
    CheckInput();

    collided = false;
    
    Vector2 previous_position = position;

    if(velocity.x != 0 or velocity.y != 0) {
        position = Vector2Add(position, velocity * GetFrameTime());
        SetAmination(sprite, RUN);

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
        SetAmination(sprite, IDLE);
    }
    if(g_input.world_mouse_position.x  < position.x){
        //if(velocity.x < -1) {
        sprite.source.width = -sprite.size.x;
    }
    else {
        //else if(velocity.x > 1){
        sprite.source.width = sprite.size.x;
    }

    sprite.position = position;
    weapon_sprite.position = position;

    if(!can_shoot) {
        shot_timer.Update();
    }
}

void PlayerCharacter::Draw() {
    //TraceLog(LOG_INFO, "player draw");

    DrawSprite(sprite);
    DrawSprite(weapon_sprite);
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

    float speed = g_character_data[uid].base_speed;

    if(g_input.key_sprint) {
        speed = speed + (speed * 0.8f);
    }

    //TraceLog(LOG_INFO, "player speed : %0.04f", speed);

    velocity = Vector2Lerp(velocity, input_dir * speed, .15);
    if(abs(velocity.x) < 4.0f) {
        velocity.x = {0.0};
    }
    if (abs(velocity.y) < 4.0f) {
        velocity.y = {0.0};
    }

    Vector2 pp = GetWorldToScreen2D( position, g_camera);
    pp = {pp.x * g_scale, pp.y*g_scale};
    weapon_sprite.rotation = GetAngleFromTo(pp, g_input.screen_mouse_position) * RAD2DEG;

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

    if(g_input.mouse_left_down and can_shoot) {

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
            shot_timer.Start(spell_it->second.cooldown, true);
            can_shoot = false;
        }
    }
}


bool PlayerCharacter::CanEquip(int item_id) {
    return true;
}


void PlayerCharacter::Equip(int item_id) {
    TraceLog(LOG_INFO, "trying to equip primary weapon %i", item_id);

    int _id = ITEM_ID_NONE;

    auto item_it = g_item_instances.find(item_id);
    if(item_it != g_item_instances.end()) {
        if(item_it->second.type == TYPE_WEAPON) {

            g_character_data[uid].max_power += item_it->second.max_power;
            g_character_data[uid].current_power = item_it->second.current_power;
            _id = item_it->second.sprite_id;
            LoadSpriteCentered(weapon_sprite, g_item_sprites[ _id ], position);
            TraceLog(LOG_INFO, "equiping primary weapon %i sprite_id %i", item_id, _id);
            //}
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
                if(m_itter->second.speed != -1000){g_character_data[uid].base_speed += m_itter->second.speed;}
            }
        }
    }
    else if(_id == ITEM_ID_NONE) {
        Texture2D t;
        LoadSpriteCentered(weapon_sprite, t, position);
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
            if (g_character_data[uid].primary[0] == -1) {
                Texture2D t;
                LoadSpriteCentered(weapon_sprite, t, position);
            }

            g_character_data[uid].max_power -= item_it->second.max_power;

            if(g_character_data[uid].max_power < 0) {
                g_character_data[uid].max_power = 0;
            }

            item_it->second.current_power = g_character_data[uid].current_power;
            g_character_data[uid].current_power -= item_it->second.max_power;
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
                if(m_itter->second.speed != -1000){g_character_data[uid].base_speed -= m_itter->second.speed;}
            }
        }
    }
}


void PlayerCharacter::OnShotTimerTimeout() {
    can_shoot = true;
    TraceLog(LOG_INFO, "can shoot");
}


PlayerCharacter::~PlayerCharacter()
{
    TraceLog(LOG_INFO, "deleting player!!!!!!!!!!!!!!!!!!!!");
}


float PlayerCharacter::GetYSort() {
    return position.y + ground_point_offset.y;
}