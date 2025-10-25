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
    LoadSpriteCentered(sprite, g_sprite_sheets[g_player_data.sprite_sheet_id], position, 4, 24.0f, 0.10f);
    collision_radius = 5;
    centered_offset = {0,5};
    collided = false;
}

void PlayerCharacter::Update() {
    //TraceLog(LOG_INFO, "player update");
    //hovered = false;
    CheckInput();

    //move position
    collided = false;
    
    Vector2 previous_position = position;

    if(velocity.x != 0 or velocity.y != 0) {
        position = Vector2Add(position, velocity * GetFrameTime());
        SetAmination(sprite, RUN);

        CollisionResult result;
        result.collision_dir = {0,0};

        if(CheckCollisionWithLevel(this, result, 2) == true) {
            TraceLog(LOG_INFO, "COLLIDED, %0.0f %0.0f \n", result.collision_dir.x, result.collision_dir.y);

            if(result.collision_dir.x != 0) {
                position.x = previous_position.x;
                velocity.x = 0.0f;
            }
            if(result.collision_dir.y != 0) {
                position.y = previous_position.y;
                velocity.y = 0.0f;
            }

            //position = previous_position;
            //velocity = {0,0};

        }

       if(velocity.x < -1) {
            sprite.source.width = -sprite.size.x;
        }
        else if(velocity.x > 1){
            sprite.source.width = sprite.size.x;
        }
    }
    else {
        SetAmination(sprite, IDLE);
    }
    sprite.position = position;
}

void PlayerCharacter::Draw() {
    //TraceLog(LOG_INFO, "player draw");

    DrawSprite(sprite);
    if(g_game_settings.show_debug == true) {
        DrawCircleV( Vector2Add(position, centered_offset), collision_radius, RED);
        DrawCircleV(Vector2Add(position, centered_offset), 1, WHITE);   
    }
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
        speed = speed + (speed * 1.2);
    }

    //velocity = input_dir * speed;
    //TraceLog(LOG_INFO, "player speed : %0.04f", speed);

    velocity = Vector2Lerp(velocity, input_dir * speed, .15);
    if(abs(velocity.x) < 4.0f) {
        velocity.x = {0.0};
    }
    if (abs(velocity.y) < 4.0f) {
        velocity.y = {0.0};
    }
}


PlayerCharacter::~PlayerCharacter()
{
    TraceLog(LOG_INFO, "deleting player!!!!!!!!!!!!!!!!!!!!");
    //UnloadTexture(sprite.texture);
}

