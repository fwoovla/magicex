#include "../../core/gamedefs.h"


FireBall::FireBall(Vector2 _position, float _rotation, int _shooter_id, SpellData _data) {

    data = _data;
    should_delete = false;

    lifetime_timer.timer_timeout.Connect([&](){this->OnLifetimeTimeout();});
    lifetime_timer.Start(data.lifetime, true);

    position = _position;
    rotation = _rotation;
    centered_offset = {0,0};
    collision_radius = 5;
    collided = false;


    velocity = {100, 0};
    velocity = Vector2Rotate(velocity, rotation * DEG2RAD);

    collided = false;

    shooter_id = _shooter_id;
    
    centered_offset = {0, 0};
    collision_rect = { position.x - centered_offset.x , position.y - centered_offset.y, 16, 16 }; 

    LoadSpriteCentered(sprite, g_spell_sprites[SPELL_ID_FIREBALL_1 + (data.level - 1)], position);
    sprite.roataion = rotation;

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
    
    sprite.position = position;

    lifetime_timer.Update(); 

}

void FireBall::Draw() {
    DrawSprite(sprite);
}

void FireBall::DrawUI() {

}

void FireBall::OnLifetimeTimeout() {
    should_delete = true;
}