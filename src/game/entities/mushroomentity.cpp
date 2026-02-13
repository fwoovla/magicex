#include "../../core/gamedefs.h"

MushroomEntity::MushroomEntity(Vector2 _position) {

    position = _position;
    rotation = 0.0f;
    LoadSprite(sprite, g_item_sprites[ITEM_ID_MUSHROOM], position);
    collision_radius = 5;
    centered_offset = {0,0};
    collided = false;
    y_sort = true;
    can_take_damage = false;
    
    should_delete = false;


    is_persistant = false;
    is_obstructable = true;
    is_obstructed = false;
    can_take_damage = false;
    is_on_screen = false;


    c_area.area_activated.Connect( [&](){OnEntityActivated();} );
}

MushroomEntity::~MushroomEntity() {
    for(int i = 0; i < c_area.item_list.size(); i++) {
        g_item_instances.erase(c_area.item_list[i]);
        TraceLog(LOG_INFO, "instance erased %i", g_item_instances.size());
    }
    //TraceLog(LOG_INFO, "MUSHROOM DESTROYED");    
}

void MushroomEntity::Update() {

    is_on_screen = IsOnScreen(position, sprite.size);

    if(!is_on_screen) {
        return;
    }


    c_area.Update();

    raycast.position = {position.x + (sprite.size.x * 0.5f), position.y + (sprite.size.y * 0.5f)};
    float rot = GetAngleFromTo(raycast.position, g_current_player->position);
    float dist = Vector2Distance(raycast.position, g_current_player->position);
    raycast.direction = Vector2Rotate({dist,0}, rot);

    CollisionResult result;
    is_obstructed = GetRayCollisionWithLevel(raycast, result, 0);
}

void MushroomEntity::Draw() {
    if(!is_on_screen) {
        return;
    }
    if(!is_obstructed) {
        DrawSprite(sprite);
    }

        if(g_game_settings.show_debug) {
        Color color = WHITE;
        if(is_obstructed) {
            color = RED;
        }
        DrawRectangle(c_area.position.x, c_area.position.y, c_area.size.x, c_area.size.y, DARKBLUE);
        DrawLineV(raycast.position, {raycast.position.x + raycast.direction.x, raycast.position.y + raycast.direction.y}, color);
    }
}

void MushroomEntity::DrawUI() {
    c_area.Draw();
}

void MushroomEntity::OnEntityActivated() {
    g_game_data.return_container = this;
    open_container.EmitSignal();

}

bool MushroomEntity::IsEmpty() {
    for(int item = 0; item < c_area.item_list.size(); item++) {
        if(c_area.item_list[item] != -1) {
            return false;
        }
    }
    return true;
}

float MushroomEntity::GetYSort() {
    return position.y;
}

void MushroomEntity::TakeDamage(DamagePayload _payload) {
    TraceLog(LOG_INFO, "MushroomEntity taking damage ");
}