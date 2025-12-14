#include "../../core/gamedefs.h"


FireBall::FireBall(Vector2 _position, int _shooter_id, SpellData _data) {

    data = _data;
    should_delete = false;
    y_sort = false;


    position = _position;
    centered_offset = {0,0};
    collision_radius = data.radius;
    collided = false;
    collision_rect = { position.x - centered_offset.x , position.y - centered_offset.y, 16, 16 }; 
             
    
    
    shooter_id = _shooter_id;

    target_position = g_input.world_mouse_position;
    
    target_rotation = GetAngleFromTo(position, target_position);
    rotation = target_rotation * RAD2DEG;
    velocity = Vector2Rotate({data.speed, 0}, rotation * DEG2RAD );
    
    target_dist = Vector2Distance(position, target_position);
    dist_scale = target_dist/data.speed;
    //TraceLog(LOG_INFO, "scale %f", dist_scale);

    lifetime_timer.timer_timeout.Connect([&](){this->OnLifetimeTimeout();});
    lifetime_timer.Start(data.lifetime * dist_scale, true);

    sprite = {};
    LoadSpriteCentered(sprite, g_spell_sprites[data.spell_id], position);
    sprite.rotation = rotation;

}

FireBall::~FireBall() {

}

void FireBall::Update() {
    if(should_delete) {
        return;
    }

    Vector2 previous_position = position;
    float dt = GetFrameTime();

    position = Vector2Add(position, velocity * GetFrameTime());

    CollisionResult result;
    if(CollideWithTile(this, result)) {
        should_delete = true;
    }
    
    if(CollideWithEntity(this, result)) {
        should_delete = true;
        result.collider->TakeDamage();
    }

    sprite.position = position;

    lifetime_timer.Update(); 

}

void FireBall::Draw() {
    DrawSprite(sprite);

    if(g_game_settings.show_debug) {
        DrawCircleV(target_position, 4, DARKRED);
    }
}

void FireBall::DrawUI() {

}

void FireBall::OnLifetimeTimeout() {
    should_delete = true;
}

float FireBall::GetYSort() {
    return position.y;
}

void FireBall::TakeDamage() {
    TraceLog(LOG_INFO, "taking damage ");
}