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
    detect_range = 100.0f;
    target_creature = nullptr;
    weapon_state = WSTATE_IDLE;
    action_state = ACTION_IDLE;
    path_index = 0;

    detect_timer.timer_timeout.Connect( [&](){OnDetectTimerTimeout();} );
    detect_timer.Start((GetRandomValue(1, 20) * .1), false);

    action_timer.timer_timeout.Connect( [&](){OnActionTimerTimeout();} );
    action_timer.Start((GetRandomValue(1, 20) * .1), false);


    int _id = ITEM_ID_ERROR;
    auto item_it = g_item_instances.find(data->primary[0]);
    if(item_it != g_item_instances.end()) {
        Equip(data->primary[0]);
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


    Vector2 input_dir = {0,0};

    if(action_state == ACTION_IDLE) {
        //nothing right now
    }

    if(action_state == ACTION_CHARGE and target_creature) {
        //move to target
        if(abs(position.x - target_path[path_index].x) < 5 and abs(position.y - target_path[path_index].y) < 5 ) {
            path_index++;
        }

        if(path_index >= target_path.size()) {
            path_index = target_path.size() - 1;
        }
        //TraceLog(LOG_INFO, "difference %0.2f  %0.2f", abs(position.x - target_path[path_index].x), abs(position.y - target_path[path_index].y));

        float rot = GetAngleFromTo(position, target_path[path_index]);
        input_dir = Vector2Rotate({1, 0}, rot);
    }

    if(action_state == ACTION_RETREAT) {
        //move away from target
    }

    velocity = Vector2Lerp(velocity, input_dir * data->base_speed, .15);


    if(input_dir == Vector2{0,0}) {
        if(abs(velocity.x) < 4.0f) {
            velocity.x = {0.0};
        }
        if (abs(velocity.y) < 4.0f) {
            velocity.y = {0.0};
        }
    }

    if(position.x + input_dir.x  < position.x){
        sprite.source.width = -sprite.size.x;
        swing_dir = -1;
    }
    else {
        sprite.source.width = sprite.size.x;
        swing_dir = 1;
    }

    Vector2 previous_position = position;

    if(velocity.x != 0 or velocity.y != 0) {
        position = Vector2Add(position, velocity * GetFrameTime());
        SetAmination(sprite, ANIM_RUN);

        CollisionResult result;
        result.collision_dir = {0,0};

        if(CollideAndSlide(this, result, 2) == true) {
            
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

    detect_timer.Update();
    action_timer.Update();
}

void CreatureEntity::Draw() {
    //TraceLog(LOG_INFO, "+++++++draw++++++");
    DrawSprite(shadow_sprite);
    DrawSprite(weapon_sprite);

    if(target_creature) {
        sprite.modulate = RED;
    }
    else {
        sprite.modulate = WHITE;
    }

    DrawSprite(sprite);
    if(g_game_settings.show_debug == true) {
        DrawCircleV( Vector2Add(position, centered_offset), collision_radius, RED);
        DrawCircleV(Vector2Add(position, centered_offset), 1, WHITE);
        DrawCircleV(Vector2Add(position, ground_point_offset), 1, BLUE); 
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
            data->max_power += item_it->second.max_power;
            data->current_power = item_it->second.current_power;
            _id = item_it->second.sprite_id;
            LoadSpriteCentered(weapon_sprite, g_item_sprites[ _id ], position);
            weapon_sprite.center.x -= weapon_sprite.center.x - 3;
            current_primary_data = &item_it->second;
            TraceLog(LOG_INFO, "equiping primary weapon %i sprite_id %i", item_id, _id);
        }
        
        if(item_it->second.type >= TYPE_HEAD_ARMOR and item_it->second.type <= TYPE_HAND_ARMOR) {
            _id = item_it->second.sprite_id;
            data->defence += item_it->second.defence;
            data->magic_defence += item_it->second.magic_defence;
            TraceLog(LOG_INFO, "equiping armor %i sprite_id %i", item_id, _id);
        }
        
        for(int mod = 0; mod < item_it->second.char_mods.size(); mod++) {
            TraceLog(LOG_INFO, "character mod %i %s", item_it->second.char_mods[mod].mod_id, item_it->second.char_mods[mod].mod_name.c_str());
            if(item_it->second.char_mods[mod].health != -1000){data->health += item_it->second.char_mods[mod].health;}
            if(item_it->second.char_mods[mod].speed != -1000){data->current_speed += item_it->second.char_mods[mod].speed;}
            if(item_it->second.char_mods[mod].stamina != -1000){data->max_stamina += item_it->second.char_mods[mod].stamina;}
            
        }
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
                
            data->max_power -= item_it->second.max_power;
            data->current_power -= item_it->second.max_power;

            if(data->max_power < 0) {
                data->max_power = 0;
            }

            if(item_it->second.current_power > item_it->second.max_power) {
                item_it->second.current_power = item_it->second.max_power;
            }
            if(data->current_power < 0) {
                data->current_power = 0;
            }
        }

        if(item_it->second.type >= TYPE_HEAD_ARMOR and item_it->second.type <= TYPE_HAND_ARMOR) {
            TraceLog(LOG_INFO, "unequiping armor %i", item_id);
            data->defence -= item_it->second.defence;
            data->magic_defence -= item_it->second.magic_defence;
        }
        
        for(int mod = 0; mod < item_it->second.char_mods.size(); mod++) {
                TraceLog(LOG_INFO, "character mod %i %s", item_it->second.char_mods[mod].mod_id, item_it->second.char_mods[mod].mod_name.c_str());
                if(item_it->second.char_mods[mod].health != -1000){data->health -= item_it->second.char_mods[mod].health;}
                if(item_it->second.char_mods[mod].speed != -1000){data->current_speed -= item_it->second.char_mods[mod].speed;}
                if(item_it->second.char_mods[mod].stamina != -1000){data->max_stamina -= item_it->second.char_mods[mod].stamina;}
        }
    }
}


void CreatureEntity::OnSpellTimerTimeout() {
    can_use_spell = true;
    //TraceLog(LOG_INFO, "can_use_spell");
}

void CreatureEntity::OnMeleTimerTimeout() {
    can_mele = true;
    //TraceLog(LOG_INFO, "can_use_spell");
}

void CreatureEntity::OnStunTimerTimeout() {
    is_stunned = false;
    sprite.modulate = WHITE;
    SetAmination(sprite, ANIM_IDLE);
    //TraceLog(LOG_INFO, "un stunned");
}

void CreatureEntity::OnHungerTimerTimeout() {

}


void CreatureEntity::OnDetectTimerTimeout() {
    EntityDetectResult result;
    DetectCreatures(*this, 100.0f, result);
    detected_creatures.clear();
    //target_creature = nullptr;
    detected_creatures = result.detected_creatures;
    bool target_found = false;

    for(auto &creature : detected_creatures) {
        if(creature == g_current_player) {
            target_found = true;
            if(target_creature == nullptr) {
                target_path.clear();
                path_index = 0;
                target_creature = g_current_player;
                target_path.push_back(target_creature->position);
                SpawnCharacterMessage (position, "!", WHITE, 0.3f);
            }
            else {
                target_path.push_back(target_creature->position);
            }
        }
    }
    if(!target_found) {
        target_creature = nullptr;
    }
    
    //TraceLog(LOG_INFO, "%s detected: %i\n", data->name.c_str(), target_path.size());
}

void CreatureEntity::OnActionTimerTimeout() {
    //TraceLog(LOG_INFO, "action time %s health %i", data->name.c_str(), data->health);

    if(target_creature) {
        target_path.push_back(target_creature->position);
        if(data->health > 2) {
            action_state = ACTION_CHARGE;
        }
        else {
            action_state = ACTION_RETREAT;
        }
    }
    else {
        action_state = ACTION_IDLE;
    }
}

CreatureEntity::~CreatureEntity()
{
    TraceLog(LOG_INFO, "deleting entity!!!!!!!!!!!!!!!!!!!! %i", uid);
}


float CreatureEntity::GetYSort() {
    return position.y + ground_point_offset.y;
}


void CreatureEntity::TakeDamage(DamagePayload _payload) {
    //TraceLog(LOG_INFO, "%i: taking damage %i", uid, _payload.damage);

    is_stunned = true;
    sprite.modulate = RED;
    SetAmination(sprite, ANIM_STUN);
    stun_timer.Start(0.5f, true);

    std::string damage_string = std::to_string(_payload.damage);

    velocity = _payload.knockback;

    
    SpawnCharacterMessage (position, damage_string, DARKRED, 0.3f);
    //TraceLog(LOG_INFO, "knockback %0.2f  %0.2f", _payload.knockback.x, _payload.knockback.y);

}