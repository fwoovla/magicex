/* #include "../../core/gamedefs.h"
#include<cmath>
#include <raymath.h>

#define DETECT_RANGE 3
#define UNIT_SPEED 15.0f

enum AnimationState {
    IDLE = 0,
    WALK = 1,
    RUN = 2,
};

BaseUnit::BaseUnit(Vector2 _position, UnitData _data): AnimatedSpriteEntity() {

    data = _data;
    data.name = unit_names[ GetRandomValue(0, 10)];
    data.id = GetRandomValue(0, 100000);
    position = _position;
    rotation = 0.0f;
    hovered = false;
    selected = false;
    target_position = {9999, 9999};
    velocity = {0,0};
    spread_velocity = {0,0};
    target_velocity = {0,0};
    LoadSprite(sprite, g_unit_sprites[data.texture_id], position, 4, 24.0f, 0.10f);
    collision_radius = (sprite.size.x + 1) /2;
    collided = false;
    times_collided = 0;
    is_inside = false;
}

void BaseUnit::Update() {
    
    hovered = false;
    CheckInput();

    //move position
    collided = false;
    
    if(target_position.x != 9999) {
        steps++;
        Vector2 previous_position = position;

        if(is_inside and ( abs(position.x - target_position.x) < 10 or abs(position.y - target_position.y) < 10)) {
                target_position = {9999,9999};
                steps = 0;
                SetAmination(sprite, IDLE);
                //TraceLog(LOG_INFO, "%i STOPPED CLOSE ENOOUGH", data.id);
        }
        

        if( abs(position.x - target_position.x) > 1 or abs(position.y - target_position.y) > 1) {
            SetAmination(sprite, RUN);
            
            position = Vector2Add(previous_position, velocity * GetFrameTime());

            CollisionResult result;
            if(CheckCollisionWithUnits(this, result) == true) {
                //TraceLog(LOG_INFO, "%i COLIDED", data.id);
                collided = true;
                times_collided +=1;
                if(times_collided % 500 == 100) { //find a new direction
                    is_inside = true;
                    //TraceLog(LOG_INFO, "%i is inside", data.id);
                    float rot = GetAngleFromTo(position, target_position);
                    float new_rot = rot + (GetRandomValue(-(int)(PI*0.5f *100), (int)(PI*0.5f *100) )) /100;
                    spread_velocity = Vector2Rotate( {UNIT_SPEED,0}, new_rot );
                }
            }
            else if(is_inside){
                times_collided = 0;
                is_inside = false;
                //TraceLog(LOG_INFO, "%i is NOT inside", data.id);
                if(target_position.x != 9999) {
                    float rot = GetAngleFromTo(position, target_position);
                    velocity = Vector2Rotate( {UNIT_SPEED,0}, rot );
                }
            }

            if(collided and !is_inside) {
                position = previous_position;
            }
            else if (collided and is_inside) {
                //TraceLog(LOG_INFO, "is inside  %i times", times_collided);
                position = Vector2Add(previous_position, spread_velocity * GetFrameTime());

                if(steps % 100 == 0) {  //correct velocity once in a while
                    float rot = GetAngleFromTo(position, target_position);
                    velocity = Vector2Rotate( {UNIT_SPEED,0}, rot );
                }

            }          
        }
        else {
            target_position = {9999,9999};
            steps = 0;
            SetAmination(sprite, IDLE);
            //TraceLog(LOG_INFO, "%i STOPPED on target", data.id);
        }
    }
    //sync sprite and collision positions
    sprite.position = position;
}

void BaseUnit::Draw() {

    if(g_game_settings.show_debug) {
        Rectangle rect = {.x = position.x, .y = position.y, .width = sprite.size.x, .height = sprite.size.y,};
        
        DrawRectangleRec(rect, BLUE);

        if (target_position.x != 9999) {
            DrawRectangleV(Vector2Add(target_position, {-1,-1}), {10, 10}, RED);
        }
    }

    if(hovered) {
        DrawCircleLines(position.x + (sprite.size.x * 0.5f), position.y + (sprite.size.y*0.5f),collision_radius -1, WHITE );
        //sprite.modulate = RED;
    }
    else {
        sprite.modulate = WHITE;
    }

    if(selected) {
        DrawCircle(position.x + (sprite.size.x * 0.5f), position.y + (sprite.size.y*0.5f),collision_radius -2, CYAN );
    }

    DrawCircleLines(position.x + (sprite.size.x * 0.5f), position.y + (sprite.size.y*0.5f), collision_radius, ORANGE);

    DrawSprite(sprite);

}

void BaseUnit::CheckInput() {

    Rectangle screen_rect = {
        .x = (position.x - g_camera.target.x) * g_world2screen,
        .y = (position.y - g_camera.target.y) * g_world2screen,
        .width = sprite.size.x * g_world2screen,
        .height = sprite.size.y * g_world2screen,
    };

    Rectangle world_rect = {position.x, position.y, sprite.size.x, sprite.size.y};

    if(CheckCollisionPointRec(g_input.screen_mouse_position, screen_rect )) {
        hovered = true;
        if(g_input.mouse_left) {
            selected = true;
        }
    }
    else if (g_input.mouse_left) {
        selected = false;
    }

    if(g_input.selecting) {
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
    }
}

BaseUnit::~BaseUnit()
{
    //UnloadTexture(sprite.texture);
}

 */