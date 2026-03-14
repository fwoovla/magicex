#include "../../core/gamedefs.h"


AoeSpell::AoeSpell(NewSpellPayload payload, CasterData *_data){

    data = _data;
    should_delete = false;
    y_sort = false;


    position = payload.position;
    centered_offset = {0,0};
    collision_radius = data->coupler.radius;
    collided = false;
    collision_rect = { position.x - centered_offset.x , position.y - centered_offset.y, 16, 16 }; 
    is_on_screen = true;
    
    shooter_id = payload.shooter_id;

    target_position = payload.target_position;
    
    target_rotation = GetAngleFromTo(position, target_position);
    rotation = (target_rotation * RAD2DEG) + GetRandomValue(-payload.spread, payload.spread);
    velocity = Vector2Rotate({data->coupler.speed, 0}, rotation * DEG2RAD );
    
    target_dist = Vector2Distance(position, target_position);
    dist_scale = target_dist/data->coupler.speed;
    //TraceLog(LOG_INFO, "scale %f", dist_scale);

    lifetime_timer.timer_timeout.Connect([&](){this->OnLifetimeTimeout();});
    lifetime_timer.Start(1.0f * dist_scale, true);

    sprite = {};
    LoadSpriteCentered(sprite, g_spell_sprites[data->coupler.type], position);
    sprite.rotation = rotation;
    sprite.modulate = g_spell_effect_colors[data->igniter.effect];

}

AoeSpell::~AoeSpell() {

}

void AoeSpell::Update() {
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
    velocity.x += cosf(rad) * 25.0f;
    velocity.y += sinf(rad) * 25.0f;

    vClamp(velocity, 1.0);
    //TraceLog(LOG_INFO, "rotating: %f  ", rotation);
    velocity = Vector2ClampValue(velocity, -data->coupler.speed , data->coupler.speed);

    position = Vector2Add(position, velocity * GetFrameTime());

    CollisionResult result;

    if(CollideWithTile(this, result)) {
        should_delete = true;
    }

    if(CollideWithEntity(this, result)) {
        if(result.collider->uid != shooter_id) {
            TraceLog(LOG_INFO, "collider %s", result.collider->identifier.c_str());
            DamagePayload new_payload;
            new_payload.attacker_id = shooter_id;
            new_payload.damage = data->coupler.damage;
            new_payload.knockback = Vector2Rotate( {data->rod.knockback, 0}, rotation * DEG2RAD);
            result.collider->TakeDamage(new_payload);
    
            should_delete = true;
        }
        
    }
    sprite.position = position;
    sprite.rotation = rotation;

    lifetime_timer.Update(); 

}

void AoeSpell::Draw() {
    DrawSprite(sprite);

    if(g_game_settings.show_debug) {
        DrawCircleV(position, collision_radius, YELLOW);
        DrawCircleV(target_position, 4, DARKRED);
    }
}

void AoeSpell::DrawUI() {

}

void AoeSpell::OnLifetimeTimeout() {
    should_delete = true;
}

float AoeSpell::GetYSort() {
    return position.y;
}

void AoeSpell::TakeDamage(DamagePayload _payload) {
    TraceLog(LOG_INFO, "taking damage ");
}