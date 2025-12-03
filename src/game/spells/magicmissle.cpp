#include "../../core/gamedefs.h"


MagicMissle::MagicMissle(Vector2 _position, int _shooter_id, SpellData _data){

    data = _data;
    should_delete = false;



    position = _position;
    centered_offset = {0,0};
    collision_radius = data.radius;
    collided = false;
    collision_rect = { position.x - centered_offset.x , position.y - centered_offset.y, 16, 16 }; 
             
    LoadSpriteCentered(sprite, g_spell_sprites[SPELL_ID_MAGICMISSLE_1 + (data.level - 1)], position);
    sprite.rotation = target_rotation;
    
    shooter_id = _shooter_id;



    target_position = g_input.world_mouse_position;
    
    target_rotation = GetAngleFromTo(position, target_position);
    rotation = (target_rotation * RAD2DEG) + GetRandomValue(-50, 50);
    velocity = Vector2Rotate({data.speed, 0}, rotation * DEG2RAD );
    
    target_dist = Vector2Distance(position, target_position);
    dist_scale = target_dist/data.speed;
    //TraceLog(LOG_INFO, "scale %f", dist_scale);

    lifetime_timer.timer_timeout.Connect([&](){this->OnLifetimeTimeout();});
    lifetime_timer.Start(data.lifetime * dist_scale, true);

    //id = GetRandomValue(0, 10000);

}

MagicMissle::~MagicMissle() {

}

void MagicMissle::Update() {
    if(should_delete) {
        return;
    }

    Vector2 previous_position = position;
    float dt = GetFrameTime();





    target_rotation = GetAngleFromTo(position, target_position);
    //rotation = RotateTowardsRad(rotation * DEG2RAD, target_rotation, PI * 20, GetFrameTime()) * RAD2DEG;
    rotation = target_rotation * RAD2DEG;

    //velocity = Vector2Rotate(velocity, rotation * DEG2RAD);
    float rad = rotation * DEG2RAD;
    velocity.x += cosf(rad) * 15.0f;
    velocity.y += sinf(rad) * 15.0f;

    vClamp(velocity, 1.0);
    //TraceLog(LOG_INFO, "rotating: %f  ", rotation);
    velocity = Vector2ClampValue(velocity, -data.speed , data.speed);





    position = Vector2Add(position, velocity * GetFrameTime());

    CollisionResult result;

    //if(CollideWithTile(this, result)) {
    if(CollideAndSlide(this, result, 2)) {
        should_delete = true;
    }
    
    sprite.position = position;

    lifetime_timer.Update(); 

}

void MagicMissle::Draw() {
    DrawSprite(sprite);

    DrawCircleV(target_position, 4, RED);
}

void MagicMissle::DrawUI() {

}

void MagicMissle::OnLifetimeTimeout() {
    should_delete = true;
}

float MagicMissle::GetYSort() {
    return position.y;
}