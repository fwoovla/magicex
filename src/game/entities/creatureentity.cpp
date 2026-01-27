#include "../../core/gamedefs.h"
#include<cmath>
#include <raymath.h>

#define DETECT_RANGE 3
//#define SPEED 25.0f


CreatureEntity::CreatureEntity(Vector2 _position, int _uid): CharacterEntity() {
    
    uid = _uid;
    data = &g_character_data[uid];
    character_entity = this;

    position = _position;
    rotation = 0.0f;
    velocity = {0,0};
    weapon_sprite = {};
    

    LoadSpriteCentered(sprite, g_creature_sprite_sheets[data->sprite_sheet_id], position, 4, 16.0f, 0.10f);
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
    target_creature = nullptr;
    weapon_state = WSTATE_IDLE;
    action_state = ACTION_IDLE;
    detect_state = DETECT_NONE;
    path_index = 0;
    obstructed_count = 0;
    detect_range = 100.0f;
    attack_range = 200.0f;

    detect_timer.timer_timeout.Connect( [&](){OnDetectTimerTimeout();} );
    detect_timer.Start((GetRandomValue(1, 20) * .1), false);

    action_timer.timer_timeout.Connect( [&](){OnActionTimerTimeout();} );
    action_timer.Start((GetRandomValue(1, 20) * .1), false);

    spell_timer.timer_timeout.Connect( [&](){OnSpellTimerTimeout();} );
    can_use_spell = true;
    should_use_spell = true;

    mele_timer.timer_timeout.Connect( [&](){OnMeleTimerTimeout();} );
    can_mele = true;
    should_mele = true;


    int _id = ITEM_ID_ERROR;
    auto item_it = g_item_instances.find(data->primary[0]);
    if(item_it != g_item_instances.end()) {
        Equip(data->primary[0]);
        current_primary_data = &item_it->second;
    }

    Equip(data->head[0]);
    Equip(data->body[0]);
    Equip(data->legs[0]);
    Equip(data->feet[0]);
    Equip(data->hands[0]);

    stun_timer.timer_timeout.Connect( [&](){OnStunTimerTimeout();} );

    TraceLog(LOG_INFO, "CREATURE CREATED uid %i   creature id %i  name %s speed  %0.2f", _uid, data->creature_id, data->name.c_str(), data->base_speed);
}

void CreatureEntity::Update() {

    is_on_screen = IsOnScreen(position, {sprite.frame_size, sprite.frame_size});

    Vector2 input_dir = {0,0};

    if(detect_state == DETECT_NONE) {
        //nothing right now
    }

    if(detect_state == DETECT_PLAYER and target_creature) {
        //TraceLog(LOG_INFO, "hostility %i", data->ai_data.hostility);
        if(data->ai_data.hostility > AI_TERRITORIAL) {
            input_dir =  GetDirToPlayer();
        }
        if(weapon_state == WSTATE_IDLE) {

            weapon_sprite.rotation = GetAngleFromTo(position, target_creature->position) * RAD2DEG;
        }
        if(weapon_state == WSTATE_MELE) {
            weapon_sprite.rotation =  Lerp(weapon_sprite.rotation, weapon_sprite.rotation + (400 * swing_dir), 0.1f);
            Vector2 t_pos = Vector2Add(Vector2Rotate( {8, 0}, weapon_sprite.rotation * DEG2RAD), position);
            CollisionResult result;
            if(CollideWithEntity(t_pos, 2, result)) {

                DamagePayload new_payload;
                new_payload.damage = current_primary_data->weapon_data.damage;
                new_payload.attacker_id = uid;
                new_payload.knockback = Vector2Rotate( {current_primary_data->weapon_data.knockback, 0}, weapon_sprite.rotation * DEG2RAD);
                result.collider->TakeDamage(new_payload);
            }
            if(weapon_sprite.rotation >=  weapon_end_rotation - 5 and weapon_sprite.rotation <=  weapon_end_rotation + 5) {
                weapon_state = WSTATE_IDLE;
            }
        }
        if(GetRandomValue(0,10) > 5 and should_mele) {

            if(current_primary_data != nullptr) {
                TraceLog(LOG_INFO, "mele attack  %0.2f", current_primary_data->weapon_data.cooldown);
                mele_timer.Start(current_primary_data->weapon_data.cooldown, true);
                can_mele = false;
                should_mele = false;
                weapon_state = WSTATE_MELE;
                weapon_end_rotation = weapon_sprite.rotation + (400 * swing_dir);

                //velocity = Vector2Rotate( {-current_primary_data->recoil, 0}, weapon_sprite.rotation * DEG2RAD);
            }
        }

        if(GetRandomValue(0,10) > 5 and should_use_spell) {
            if(current_primary_data != nullptr) {
                if(current_primary_data->spell_id != -1) {
                    TraceLog(LOG_INFO, "casting spell");
                    
                    if(g_character_data[uid].current_power < current_primary_data->weapon_data.pps) {
                        return;
                    }
                    
                    
                    NewSpellPayload payload;
                    payload.position = position;
                    payload.rotation = weapon_sprite.rotation;
                    payload.shooter_id = uid;
                    payload.target_position = target_creature->position;
                    payload.spread = current_primary_data->weapon_data.spread;
                    TraceLog(LOG_INFO, "casting spell id %i  shots %i", current_primary_data->spell_id, current_primary_data->weapon_data.shots);
                    //TraceLog(LOG_INFO, "power %0.2f/  %0.2f   rotation %0.2f   ", g_character_data[uid].current_power, g_character_data[uid].max_power, payload.rotation);

                    for(int shot = 0; shot <= current_primary_data->weapon_data.shots; shot++) {
                        if(g_game_data.is_in_sub_map) {
                            SpawnSpell(*g_sub_scene, payload, &current_primary_data->spell_data);

                        }
                        else {
                            SpawnSpell(*g_current_scene, payload, &current_primary_data->spell_data);
                        }
                    }
                    g_character_data[uid].current_power -= current_primary_data->weapon_data.pps;
                    current_primary_data->weapon_data.current_power = g_character_data[uid].current_power; 
                    spell_timer.Start(current_primary_data->weapon_data.cooldown, true);
                    can_use_spell = false;
                    should_use_spell = false;
                    //weapon_state = WSTATE_MELE;
                    velocity = Vector2Rotate( {-current_primary_data->weapon_data.recoil, 0}, weapon_sprite.rotation * DEG2RAD);
                }
            }
        }
    }

    MoveCreature(input_dir);

    if(is_stunned) {
        stun_timer.Update();
    }

    detect_timer.Update();
    action_timer.Update();

    if(!can_use_spell) {
        spell_timer.Update();
    }
    if(!can_mele) {
        mele_timer.Update();
    }

    raycast.position = {position.x, position.y};
    float rot = GetAngleFromTo(raycast.position, g_current_player->position);
    float dist = Vector2Distance(raycast.position, g_current_player->position);
    raycast.direction = Vector2Rotate({dist,0}, rot);

    CollisionResult result;
    is_obstructed = GetRayCollisionWithLevel(raycast, result, 0);
}

void CreatureEntity::Draw() {
    //TraceLog(LOG_INFO, "+++++++draw++++++");
    if(!is_on_screen) {
        return;
    }


    if(!is_obstructed) {
        DrawSprite(shadow_sprite);
        DrawSprite(weapon_sprite);
        DrawSprite(sprite);
    }

    if(g_game_settings.show_debug == true) {
        Color detect_color = GREEN;
        if(target_creature) {
            detect_color = ORANGE;
        }
        DrawCircleV( Vector2Add(position, centered_offset), collision_radius, detect_color);
        DrawCircleV(Vector2Add(position, centered_offset), 1, WHITE);
        DrawCircleV(Vector2Add(position, ground_point_offset), 1, BLUE); 
        Color line_color = WHITE;
        if(is_obstructed) {
            line_color = RED;
        }
        DrawLineV(raycast.position, {raycast.position.x + raycast.direction.x, raycast.position.y + raycast.direction.y}, line_color);
    }
}

void CreatureEntity::DrawUI() {

}


bool CreatureEntity::CanEquip(int item_id) {
    return true;
}


void CreatureEntity::Equip(int item_id) {
    TraceLog(LOG_INFO, "%i trying to equip item %i", uid, item_id);

    int _id = ITEM_ID_NONE;

    auto item_it = g_item_instances.find(item_id);
    if(item_it != g_item_instances.end()) {
        if(item_it->second.type == TYPE_WEAPON) {
            data->max_power += item_it->second.weapon_data.max_power;
            data->current_power = item_it->second.weapon_data.current_power;
            _id = item_it->second.sprite_id;
            LoadSpriteCentered(weapon_sprite, g_item_sprites[ _id ], position);
            weapon_sprite.center.x -= weapon_sprite.center.x - 3;
            current_primary_data = &item_it->second;
            TraceLog(LOG_INFO, "equiping primary weapon %i sprite_id %i", item_id, _id);
        }
        
        if(item_it->second.type >= TYPE_HEAD_ARMOR and item_it->second.type <= TYPE_HAND_ARMOR) {
            _id = item_it->second.sprite_id;
            data->defence += item_it->second.armor_data.defence;
            data->magic_defence += item_it->second.armor_data.magic_defence;
            TraceLog(LOG_INFO, "equiping armor %i sprite_id %i", item_id, _id);
        }
        
/*         for(int mod = 0; mod < item_it->second.char_mods.size(); mod++) {
            TraceLog(LOG_INFO, "character mod %i %s", item_it->second.char_mods[mod].mod_id, item_it->second.char_mods[mod].mod_name.c_str());
            if(item_it->second.char_mods[mod].health != 0){data->health += item_it->second.char_mods[mod].health;}
            if(item_it->second.char_mods[mod].speed != 0){data->current_speed += item_it->second.char_mods[mod].speed;}
            if(item_it->second.char_mods[mod].stamina != 0){data->max_stamina += item_it->second.char_mods[mod].stamina;}
            
        } */
    }
    TraceLog(LOG_INFO, "+++++++++++++");
}


bool CreatureEntity::CanUnEquip(int item_id) {
    return true;
}


void CreatureEntity::UnEquip(int item_id) {

    auto item_it = g_item_instances.find(item_id);

    if(item_it != g_item_instances.end()) {
        if(item_it->second.type == TYPE_WEAPON) {
            TraceLog(LOG_INFO, "%i unequiping primary weapon %i %i", uid, item_id, data->primary[0]);
            Texture2D t;
            LoadSpriteCentered(weapon_sprite, t, position);
            current_primary_data = nullptr;
                
            data->max_power -= item_it->second.weapon_data.max_power;
            data->current_power -= item_it->second.weapon_data.max_power;

            if(data->max_power < 0) {
                data->max_power = 0;
            }

            if(item_it->second.weapon_data.current_power > item_it->second.weapon_data.max_power) {
                item_it->second.weapon_data.current_power = item_it->second.weapon_data.max_power;
            }
            if(data->current_power < 0) {
                data->current_power = 0;
            }
        }

        if(item_it->second.type >= TYPE_HEAD_ARMOR and item_it->second.type <= TYPE_HAND_ARMOR) {
            TraceLog(LOG_INFO, "unequiping armor %i", item_id);
            data->defence -= item_it->second.armor_data.defence;
            data->magic_defence -= item_it->second.armor_data.magic_defence;
        }
        
/*         for(int mod = 0; mod < item_it->second.char_mods.size(); mod++) {
                TraceLog(LOG_INFO, "character mod %i %s", item_it->second.char_mods[mod].mod_id, item_it->second.char_mods[mod].mod_name.c_str());
                if(item_it->second.char_mods[mod].health != 0){data->health -= item_it->second.char_mods[mod].health;}
                if(item_it->second.char_mods[mod].speed != 0){data->current_speed -= item_it->second.char_mods[mod].speed;}
                if(item_it->second.char_mods[mod].stamina != 0){data->max_stamina -= item_it->second.char_mods[mod].stamina;}
        } */
    }
}


void CreatureEntity::OnSpellTimerTimeout() {
    can_use_spell = true;
    TraceLog(LOG_INFO, "can_use_spell");
}

void CreatureEntity::OnMeleTimerTimeout() {
    can_mele = true;
    TraceLog(LOG_INFO, "can_mele");
}

void CreatureEntity::OnStunTimerTimeout() {
    is_stunned = false;
    //sprite.modulate = WHITE;
    SetAmination(sprite, ANIM_IDLE);
    //TraceLog(LOG_INFO, "un stunned");
}

void CreatureEntity::OnHungerTimerTimeout() {

}


void CreatureEntity::OnDetectTimerTimeout() {
    EntityDetectResult result;
    DetectCreatures(*this, detect_range, result);
    detected_creatures.clear();
    //target_creature = nullptr;
    detected_creatures = result.detected_creatures;
    bool target_found = false;

    for(auto &creature : detected_creatures) {
        if(creature == g_current_player.get()) {
            target_found = true;
            if(target_creature == nullptr) {
                if(!is_obstructed) {

                    target_creature = g_current_player.get();
                    path_index = 0;
                    obstructed_count = 0;
                    target_path.clear();
                    target_path.push_back(target_creature->position);
                    SpawnCharacterMessage (position, "!", WHITE, 0.3f);
                }
            }
            else if (!is_obstructed) {
                path_index = 0;
                obstructed_count = 0;
                target_path.clear();
                target_path.push_back(target_creature->position);
            }
            else {
                obstructed_count++;
                target_path.push_back(target_creature->position);
            }

        }
    }
    if(!target_found or obstructed_count > 5) {
        target_creature = nullptr;
        obstructed_count = 0;
    }
    
    //TraceLog(LOG_INFO, "%s detected: %i\n", data->name.c_str(), target_path.size());
}

void CreatureEntity::OnActionTimerTimeout() {
    //TraceLog(LOG_INFO, "action time %s health %i", data->name.c_str(), data->health);

    if(target_creature) {
        target_path.push_back(target_creature->position);
        detect_state = DETECT_PLAYER;
    }
    else {
        detect_state = DETECT_NONE;
    }

    if(data->ai_data.hostility > AI_TERRITORIAL and detect_state == DETECT_PLAYER) {

        if(can_use_spell) {
            if(GetRandomValue(0, 60) < (data->ai_data.hostility * 10)) {
                should_use_spell = true;
                can_use_spell = false;
            }
        }

        if(can_mele) {
            if(GetRandomValue(0, 60) < (data->ai_data.hostility * 10)) {
                should_mele = true;
                can_mele = false;
            }
        }
    }
}

CreatureEntity::~CreatureEntity()
{
    TraceLog(LOG_INFO, "deleting creature !!!!!!!!!!!!!!!!!!!! %i", uid);
    for(int i = 0; i < data->primary.size(); i++) {
            g_item_instances.erase(data->primary[i]);
            TraceLog(LOG_INFO, "entity primary instance  #%i   erased %i", data->primary[i], g_item_instances.size());
    }
}


float CreatureEntity::GetYSort() {
    return position.y + ground_point_offset.y;
}


void CreatureEntity::TakeDamage(DamagePayload _payload) {
    //TraceLog(LOG_INFO, "%i: taking damage %i", uid, _payload.damage);

    int damage = CalculateDamage(_payload, g_character_data[uid]);

    is_stunned = true;
    //sprite.modulate = RED;
    SetAmination(sprite, ANIM_STUN);
    stun_timer.Start(0.5f, true);

    std::string damage_string = std::to_string(damage);

    velocity = _payload.knockback;

    data->health -= damage;
    if(data->health <= 0) {
        should_delete = true;
    }

    SpawnCharacterMessage (position, damage_string, DARKRED, 0.3f);
    //TraceLog(LOG_INFO, "knockback %0.2f  %0.2f", _payload.knockback.x, _payload.knockback.y);
}


Vector2 CreatureEntity::GetDirToPlayer() {

    if(abs(position.x - target_path[path_index].x) < 5 and abs(position.y - target_path[path_index].y) < 5 ) {
        path_index++;
    }

    if(path_index >= target_path.size()) {
        path_index = target_path.size() - 1;
    }

    float rot = GetAngleFromTo(position, target_path[path_index]);
    return Vector2Rotate({1, 0}, rot);
}

void CreatureEntity::MoveCreature(Vector2 _input_dir) {

    velocity = Vector2Lerp(velocity, _input_dir * data->base_speed, .15);

    if(_input_dir == Vector2{0,0}) {
        if(abs(velocity.x) < 4.0f) {
            velocity.x = {0.0};
        }
        if (abs(velocity.y) < 4.0f) {
            velocity.y = {0.0};
        }
    }

    if(position.x + _input_dir.x  < position.x){
        sprite.source.width = -sprite.size.x;
        swing_dir = -1;
    }
    else {
        sprite.source.width = sprite.size.x;
        swing_dir = 1;
    }

    Vector2 next_position = position;

    if(velocity.x != 0 or velocity.y != 0) {
        next_position = Vector2Add(next_position, velocity * GetFrameTime());
        if(!is_stunned) {
            SetAmination(sprite, ANIM_RUN);
        }

        CollisionResult result;
        result.collision_dir = {0,0};

        if(CollideAndSlide(this, result, next_position) == false) {
            position = next_position;
        }
        else {
            TraceLog(LOG_INFO, "COLLIDED, %0.0f %0.0f \n", result.collision_dir.x, result.collision_dir.y);
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
        if(!is_stunned) {
            SetAmination(sprite, ANIM_IDLE);
        }
    }

    sprite.position = position;
    weapon_sprite.position = position;
    shadow_sprite.position =  Vector2Add( position, {0, 3});
}