#include "../../core/gamedefs.h"

DoorEntity::DoorEntity(Vector2 _position) {

    position = _position;
    rotation = 0.0f;
    LoadSprite(sprite, g_door_sprites[SPRITE_DOOR_1], position);
    collision_radius = 5;
    centered_offset = {0,0};
    collided = false;
    y_sort = true;
    can_take_damage = false;
    
    should_delete = false;

    is_open = false;
    is_persistant = false;
    is_obstructable = false;
    is_obstructed = false;
    can_take_damage = false;
    is_on_screen = false;


    d_area.area_activated.Connect( [&](){OnEntityActivated();} );
}

DoorEntity::~DoorEntity() {

    TraceLog(LOG_INFO, "DOOR DESTROYED");    
}

void DoorEntity::Update() {

    is_on_screen = IsOnScreen(position, sprite.size);

    if(!is_on_screen) {
        return;
    }

    d_area.Update();
}

void DoorEntity::Draw() {
    if(!is_on_screen) {
        return;
    }
    //DrawRectangle(c_area.position.x, c_area.position.y, c_area.size.x, c_area.size.y, RED);
    if(!is_open) {
        DrawSprite(sprite);
    }
}

void DoorEntity::DrawUI() {
    d_area.Draw();
}

void DoorEntity::OnEntityActivated() {
    is_open = !is_open;
    g_game_data.return_door_pos = position;
    open_door.EmitSignal();

}

bool DoorEntity::IsLocked() {

    return is_locked;
}

float DoorEntity::GetYSort() {
    return position.y;
}

void DoorEntity::TakeDamage(DamagePayload _payload) {
    TraceLog(LOG_INFO, "DoorEntity taking damage ");
}