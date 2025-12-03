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

PlayerCharacter::PlayerCharacter(Vector2 _position): AnimatedSpriteEntity() {

    position = _position;
    rotation = 0.0f;
    velocity = {0,0};
    LoadSpriteCentered(sprite, g_sprite_sheets[g_player_data.sprite_sheet_id], position, 4, 16.0f, 0.10f);

    int _id = ITEM_ID_ERROR;
    auto item_it = g_item_instances.find(g_player_data.primary[0]);
    if(item_it != g_item_instances.end()) {
        _id = item_it->second.item_id;
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

    float speed = g_player_data.base_speed;

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
        TraceLog(LOG_INFO, "switching primary weapon");
        int temp_primary = g_player_data.primary[0];
        g_player_data.primary[0] = g_player_data.secondary[0];
        g_player_data.secondary[0] = temp_primary;
        Equip(g_player_data.primary[0]);
        can_switch = false;
    }
    if(!g_input.key_switch_weapon) {
        can_switch = true;
    }

    if(g_input.mouse_left_down and can_shoot) {

        int item_id = -1;
        auto item_it = g_item_instances.find(g_player_data.primary[0]);
        if(item_it != g_item_instances.end()) {
            item_id = item_it->second.item_id;
        }

        if(item_id == -1) {
            return;
        }

        auto weapon_it = g_weapon_data.find(item_id);
        if(weapon_it != g_weapon_data.end()) {
            shot_timer.Start(weapon_it->second.cooldown, true);
            can_shoot = false;

            auto spell_it = g_spell_data.find(weapon_it->second.spell_id);
            if(spell_it != g_spell_data.end()) {

                SpawnSpell(spell_it->second , *g_current_scene, {.position = position,.rotation = weapon_sprite.rotation,.shooter_id = 0});
            }
        }
        
    }
}


bool PlayerCharacter::CanEquip(int item_id) {
    return true;
}


void PlayerCharacter::Equip(int item_id) {
    if(g_item_data[item_id].type == TYPE_WEAPON) {
        if(g_player_data.primary[0] == item_id) {

            //current_weapon_data = g_weapon_data[g_player_data.primary[0]];
            if(item_id != -1) {
                TraceLog(LOG_INFO, "equiping primary weapon %i", item_id);

                int _id = ITEM_ID_ERROR;
                auto item_it = g_item_instances.find(item_id);
                if(item_it != g_item_instances.end()) {
                    _id = item_it->second.item_id;
                }
                
                LoadSpriteCentered(weapon_sprite, g_item_sprites[ _id ], position);
            }
            else {
                TraceLog(LOG_INFO, "nothing to equip %i", item_id);
                Texture2D t;
                LoadSpriteCentered(weapon_sprite, t, position);
            }
        }
    }
}



bool PlayerCharacter::CanUnEquip(int item_id) {
    return true;
}


void PlayerCharacter::UnEquip(int item_id) {
    if(g_item_data[item_id].type == TYPE_WEAPON) {
        if(g_player_data.primary[0] == item_id) {
            TraceLog(LOG_INFO, "unequiping primary weapon %i", item_id);
            Texture2D t;
            LoadSpriteCentered(weapon_sprite, t, position);
        }
    }
}


void PlayerCharacter::OnShotTimerTimeout() {
    can_shoot = true;
}


PlayerCharacter::~PlayerCharacter()
{
    TraceLog(LOG_INFO, "deleting player!!!!!!!!!!!!!!!!!!!!");
}

float PlayerCharacter::GetYSort() {
    return position.y + ground_point_offset.y;
}