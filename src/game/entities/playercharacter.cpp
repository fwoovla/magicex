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

    //data = _data;
    //data.name = unit_names[ GetRandomValue(0, 10)];
    position = _position;
    rotation = 0.0f;
    hovered = false;
    selected = false;
    velocity = {0,0};
    LoadSpriteCentered(sprite, g_sprite_sheets[g_player_data.sprite_sheet_id], position, 4, 24.0f, 0.10f);
    collision_radius = (sprite.size.x + 1) /2;
    collided = false;
    times_collided = 0;
    is_inside = false;
}

void PlayerCharacter::Update() {
    
    hovered = false;
    CheckInput();

    //move position
    collided = false;
    

    if(velocity.x != 0 or velocity.y != 0) {
        position = Vector2Add(position, velocity * GetFrameTime());
        SetAmination(sprite, RUN);

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

    DrawSprite(sprite);
    //DrawCircleV(position, 2, RED);

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
    velocity = input_dir * g_player_data.base_speed;
}

/*     if(CheckCollisionPointRec(g_input.screen_mouse_position, screen_rect )) {
        hovered = true;
        if(g_input.mouse_left) {
            selected = true;
        }
    }
    else if (g_input.mouse_left) {
        selected = false;
    } */

/*     if(g_input.selecting) {
        if(CheckCollisionRecs(g_input.selected_rect, world_rect)) {
            selected = true;
        }
        else {
            selected = false;
        }
    }

    if(selected) {
        if(g_input.mouse_right) {
            is_inside = false;
            times_collided = 0;
            target_position.x = (g_input.mouse_right_clicked_screen_position.x * g_screen2world) + (g_camera.target.x);
            target_position.y = (g_input.mouse_right_clicked_screen_position.y * g_screen2world) + (g_camera.target.y);
            float rad = GetAngleFromTo(position, target_position);
            velocity = Vector2Rotate( {UNIT_SPEED,0}, rad );
            //TraceLog(LOG_INFO, "--------\ntarget pisition.x %f  target pisition.y %f", target_position.x, target_position.y);
        }
    } */

PlayerCharacter::~PlayerCharacter()
{
    TraceLog(LOG_INFO, "deleting player!!!!!!!!!!!!!!!!!!!!");
    //UnloadTexture(sprite.texture);
}

