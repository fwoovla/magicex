#include "../../core/gamedefs.h"


Poison::Poison(NewSpellPayload payload, SpellData *_data){

    data = _data;
    should_delete = false;
    y_sort = false;


    position = payload.position;
    centered_offset = {0,0};
    collision_radius = data->radius;
    collided = false;
    collision_rect = { position.x - centered_offset.x , position.y - centered_offset.y, 16, 16 }; 
    is_on_screen = true;    
    
    shooter_id =payload.shooter_id;

    target_position = payload.target_position;
    
    target_rotation = GetAngleFromTo(position, target_position);
    rotation = (target_rotation * RAD2DEG) + GetRandomValue(-payload.spread, payload.spread);
    velocity = Vector2Rotate({data->speed, 0}, rotation * DEG2RAD );
    
    target_dist = Vector2Distance(position, target_position);
    dist_scale = target_dist/data->speed;
    //TraceLog(LOG_INFO, "scale %f", dist_scale);

    lifetime_timer.timer_timeout.Connect([&](){this->OnLifetimeTimeout();});
    lifetime_timer.Start(data->lifetime * dist_scale, true);

    sprite = {};
    LoadSpriteCentered(sprite, g_spell_sprites[data->spell_id], position);
    sprite.rotation = rotation;


}

Poison::~Poison() {

}

void Poison::Update() {
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
        
        if(result.collider->uid != shooter_id) {
            TraceLog(LOG_INFO, "collider %s", result.collider->identifier.c_str());
            DamagePayload new_payload;
            new_payload.attacker_id = data->shooter_id;
            new_payload.damage = data->damage;
            new_payload.knockback = Vector2Rotate( {data->knockback, 0}, rotation * DEG2RAD);
            result.collider->TakeDamage(new_payload);
    
            should_delete = true;
        }
    }

    sprite.position = position;

    lifetime_timer.Update(); 
}

void Poison::Draw() {
    DrawSprite(sprite);

    if(g_game_settings.show_debug) {
        DrawCircleV(position, collision_radius, YELLOW);
        DrawCircleV(target_position, 4, DARKRED);
    }
}

void Poison::DrawUI() {

}

void Poison::OnLifetimeTimeout() {
    should_delete = true;
}

float Poison::GetYSort() {
    return position.y;
}

void Poison::TakeDamage(DamagePayload _payload) {
    TraceLog(LOG_INFO, "taking damage ");
}