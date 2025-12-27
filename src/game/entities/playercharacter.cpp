#include "../../core/gamedefs.h"
#include<cmath>
#include <raymath.h>

#define DETECT_RANGE 3
//#define SPEED 25.0f



PlayerCharacter::PlayerCharacter(Vector2 _position, int _uid): CharacterEntity() {

    uid = _uid;
    data = &g_character_data[uid];
    character_entity = this;
    position = _position;
    rotation = 0.0f;
    velocity = {0,0};
    
    LoadSpriteCentered(sprite, g_character_sprite_sheets[g_character_data[uid].sprite_sheet_id], position, 4, 16.0f, 0.10f);
    LoadSpriteCentered(shadow_sprite, g_shadow_sprites[SPRITE_SHADOW_CHAR1], position);
        
    collision_radius = 5;
    centered_offset = {0, 3};
    ground_point_offset = {0, sprite.frame_size/2};
    collided = false;
    should_delete = false;
    can_switch = true;
    can_take_damage = true;
    can_reload = true;

    spell_timer.timer_timeout.Connect( [&](){OnSpellTimerTimeout();} );
    can_use_spell = true;

    mele_timer.timer_timeout.Connect( [&](){OnMeleTimerTimeout();} );
    can_mele = true;

    hunger_timer.timer_timeout.Connect( [&](){OnHungerTimerTimeout();} );
    hunger_rate = .1;
    hunger_timer.Start(10, false);

    int _id = ITEM_ID_ERROR;
    auto item_it = g_item_instances.find(g_character_data[uid].primary[0]);
    if(item_it != g_item_instances.end()) {
        Equip(g_character_data[uid].primary[0]);

    }
    else {
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
    
    Vector2 next_position = position;

    if(velocity.x != 0 or velocity.y != 0) {
        next_position = Vector2Add(next_position, velocity * GetFrameTime());
        SetAmination(sprite, ANIM_RUN);

        CollisionResult result;
        result.collision_dir = {0,0};

        if(CollideAndSlide(this, result, next_position) == false) {
            position = next_position;
        }
        else {
            //TraceLog(LOG_INFO, "COLLIDED, %0.0f %0.0f \n", result.collision_dir.x, result.collision_dir.y);
            //velocity = {0,0};
            if(result.collision_dir.x == 0) {
                position.x = next_position.x;
            }
            else {
                velocity.x = 0.0f;
                position.x -= (.2 * result.collision_dir.x);
            }
            if(result.collision_dir.y == 0) {
                position.y = next_position.y;
            }
            else {
                velocity.y = 0.0f;
                position.y -= (.2 * result.collision_dir.y);
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
    hunger_timer.Update();

    if(weapon_state == WSTATE_MELE) {
        weapon_sprite.rotation =  Lerp(weapon_sprite.rotation, weapon_sprite.rotation + (400 * swing_dir), 0.1f);
        Vector2 t_pos = Vector2Add(Vector2Rotate( {8, 0}, weapon_sprite.rotation * DEG2RAD), position);
        CollisionResult result;
        if(CollideWithEntity(t_pos, 2, result)) {

            DamagePayload new_payload;
            new_payload.damage = current_primary_data->damage;
            new_payload.attacker_id = uid;
            new_payload.knockback = Vector2Rotate( {current_primary_data->knockback, 0}, weapon_sprite.rotation * DEG2RAD);
            result.collider->TakeDamage(new_payload);
        }
        if(weapon_sprite.rotation >=  weapon_end_rotation - 5 and weapon_sprite.rotation <=  weapon_end_rotation + 5) {
            weapon_state = WSTATE_IDLE;
        }
    }
    if(weapon_state == WSTATE_IDLE) {
        Vector2 pp = GetWorldToScreen2D( position, g_camera);
        pp = {pp.x * g_scale, pp.y*g_scale};
        weapon_sprite.rotation = GetAngleFromTo(pp, g_input.screen_mouse_position) * RAD2DEG;
    }
    //TraceLog(LOG_INFO, "sprite rot  %0.2f", weapon_sprite.rotation);
}

void PlayerCharacter::Draw() {
    
    DrawSprite(shadow_sprite);
    DrawSprite(weapon_sprite);
    DrawSprite(sprite);

    if(weapon_state == WSTATE_MELE) {
    }
    if(g_game_settings.show_debug == true) {

/*     Rectangle checker_rect = {
        .x = position.x - collision_radius,
        .y = position.y - collision_radius + 5,
        .width = collision_radius * 2,
        .height = collision_radius * 2
    }; */
        //DrawRectangleRec(checker_rect, RED);
        DrawCircleV(Vector2Add(position, centered_offset), collision_radius, RED);
        //DrawCircleV(Vector2Add(position, centered_offset), 2, WHITE);
        DrawCircleV(Vector2Add(position, ground_point_offset), 2, BLUE); 

        DrawCircleV(position,3, RED);
        Vector2 t_pos = Vector2Add(Vector2Rotate( {8, 0}, weapon_sprite.rotation * DEG2RAD), position);
        DrawCircleV( t_pos, 3, BLUE);
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

    if(g_input.key_sprint and g_character_data[uid].current_stamina > 0.1) {
        speed = speed + (speed * 0.8f);
        g_character_data[uid].current_stamina -= 5.0f * GetFrameTime();
        if(g_character_data[uid].current_stamina < 0.0f) {
            g_character_data[uid].current_stamina = 0.0f;
        }
    }
    else {
        g_character_data[uid].current_stamina += 2.0f * GetFrameTime();
        if(g_character_data[uid].current_stamina > g_character_data[uid].max_stamina) {
            g_character_data[uid].current_stamina = g_character_data[uid].max_stamina;
        }
    }

    velocity = Vector2Lerp(velocity, input_dir * speed, .15);
    if(abs(velocity.x) < 4.0f) {
        velocity.x = {0.0};
    }
    if (abs(velocity.y) < 4.0f) {
        velocity.y = {0.0};
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

    if(g_input.key_reload and can_reload) {            
        //TraceLog(LOG_INFO, "reload");
        for(int item = 0; item < g_character_data[uid].inventory.size(); item++) {
            int instance_id = g_character_data[uid].inventory[item];
            if(g_item_instances[instance_id].item_id == ITEM_ID_MUSHROOM_JUICE) {
                TraceLog(LOG_INFO, "reloading");
                can_reload = false;
                g_character_data[uid].current_power = g_character_data[uid].max_power;
                g_character_data[uid].inventory[item] = -1;
                g_item_instances.erase(instance_id);

                SpawnCharacterMessage (position, "reload", WHITE, 0.3f);
                break;
            }
        }
    }
    if(!g_input.key_reload) {
        can_reload = true;
    }

    if(g_input.mouse_right_down and can_mele) {

        if(current_primary_data != nullptr) {
            //TraceLog(LOG_INFO, "mele attack");
            mele_timer.Start(current_primary_data->cooldown, true);
            can_mele = false;
            weapon_state = WSTATE_MELE;
            weapon_end_rotation = weapon_sprite.rotation + (400 * swing_dir);

            //velocity = Vector2Rotate( {-current_primary_data->recoil, 0}, weapon_sprite.rotation * DEG2RAD);
        }
    }

    if(g_input.mouse_left_down and can_use_spell) {
        if(current_primary_data != nullptr) {
            if(current_primary_data->spell_id != -1) {
                
                if(g_character_data[uid].current_power < g_spell_data[current_primary_data->spell_id].pps) {
                    return;
                }
                
                //TraceLog(LOG_INFO, "casting spell id %i  %s", current_primary_data->spell_id, g_spell_data[current_primary_data->spell_id].spell_name.c_str());
                //TraceLog(LOG_INFO, "power %0.2f/  %0.2f", g_character_data[uid].current_power, g_character_data[uid].max_power);

                NewSpellPayload payload;
                payload.position = position;
                payload.rotation = weapon_sprite.rotation;
                payload.shooter_id = uid;

                if(g_game_data.is_in_sub_map) {
                        SpawnSpell(*g_sub_scene, payload, &g_spell_data[current_primary_data->spell_id]);
                }
                else {
                        SpawnSpell(*g_current_scene, payload, &g_spell_data[current_primary_data->spell_id]);
                }
                g_character_data[uid].current_power -= g_spell_data[current_primary_data->spell_id].pps;
                current_primary_data->current_power = g_character_data[uid].current_power; 
                spell_timer.Start(g_spell_data[current_primary_data->spell_id].cooldown, true);
                can_use_spell = false;
                velocity = Vector2Rotate( {-g_spell_data[current_primary_data->spell_id].recoil, 0}, weapon_sprite.rotation * DEG2RAD);
            }
        }
    }
}


bool PlayerCharacter::CanEquip(int item_id) {
    return true;
}


void PlayerCharacter::Equip(int item_id) {
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
            if(item_it->second.char_mods[mod].health != -1000){g_character_data[uid].max_health += item_it->second.char_mods[mod].health;}
            if(item_it->second.char_mods[mod].speed != -1000){g_character_data[uid].current_speed += item_it->second.char_mods[mod].speed;}
            if(item_it->second.char_mods[mod].stamina != -1000){g_character_data[uid].max_stamina += item_it->second.char_mods[mod].stamina;}
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
                if(item_it->second.char_mods[mod].health != -1000){g_character_data[uid].max_health -= item_it->second.char_mods[mod].health;}
                if(item_it->second.char_mods[mod].speed != -1000){g_character_data[uid].current_speed -= item_it->second.char_mods[mod].speed;}
                if(item_it->second.char_mods[mod].stamina != -1000){g_character_data[uid].max_stamina -= item_it->second.char_mods[mod].stamina;}
        }
    }
}


void PlayerCharacter::OnSpellTimerTimeout() {
    can_use_spell = true;
    //TraceLog(LOG_INFO, "can_use_spell");
}

void PlayerCharacter::OnMeleTimerTimeout() {
    can_mele = true;
    //TraceLog(LOG_INFO, "can_mele");
}

void PlayerCharacter::OnStunTimerTimeout() {
    is_stunned = false;
    //TraceLog(LOG_INFO, "is_stunned false");
}

void PlayerCharacter::OnHungerTimerTimeout() {
    g_character_data[uid].saturation -= hunger_rate;
    if(g_character_data[uid].saturation < 0) {
        g_character_data[uid].saturation = 0;
        DamagePayload new_payload;
        TakeDamage(new_payload);
    }
    //TraceLog(LOG_INFO, "hunger");
}

PlayerCharacter::~PlayerCharacter()
{
    TraceLog(LOG_INFO, "deleting player!!!!!!!!!!!!!!!!!!!!");
}


float PlayerCharacter::GetYSort() {
    return position.y + ground_point_offset.y;
}

void PlayerCharacter::TakeDamage(DamagePayload _payload) {
    TraceLog(LOG_INFO, "%i: taking damage %i", uid, _payload.damage);
    
    std::string damage_string = std::to_string(_payload.damage);
    SpawnCharacterMessage (position, damage_string, DARKRED, 0.3f);
}