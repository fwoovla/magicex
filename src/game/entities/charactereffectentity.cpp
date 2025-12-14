#include "../../core/gamedefs.h"

MovementEffectEntity::MovementEffectEntity(CharacterEffectData &_data, Vector2 _position) {
    data = _data;
    position = _position;
    //sprite_id = _s_id;
    rotation = 0.0f;
    //LoadSprite(sprite, g_item_sprites[sprite_id], Vector2Add(position, {4,4}));
    //ScaleSprite(sprite, {0.5f, 0.5f});
    collision_radius = 5;
    centered_offset = {0,0};
    collided = false;
    y_sort = true;


    should_delete = false;
    is_persistant = false;
    is_obstructable = false;
    is_obstructed = false;
    can_take_damage = false;

    lifetime.timer_timeout.Connect( [&](){OnLifetimeTimeout();} );
    lifetime.Start(data.durration, true);
}

MovementEffectEntity::~MovementEffectEntity() {


    TraceLog(LOG_INFO, "CharacterEffectEntity DESTROYED");
}

void MovementEffectEntity::Update() {

}

void MovementEffectEntity::Draw() {
    DrawCircleV(position, 3, RED);

}

void MovementEffectEntity::DrawUI() {

}


float MovementEffectEntity::GetYSort() {
    return position.y;
}

void MovementEffectEntity::OnLifetimeTimeout() {
    should_delete = true;
}