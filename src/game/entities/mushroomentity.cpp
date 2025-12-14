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

    c_area.area_activated.Connect( [&](){OnContainerOpened();} );
}

MushroomEntity::~MushroomEntity() {
    for(int i = 0; i < c_area.item_list.size(); i++) {
        g_item_instances.erase(c_area.item_list[i]);
        TraceLog(LOG_INFO, "instance erased %i", g_item_instances.size());
    }
    TraceLog(LOG_INFO, "MUSHROOM DESTROYED");    
}

void MushroomEntity::Update() {
    c_area.Update();
}

void MushroomEntity::Draw() {
    //DrawRectangle(c_area.position.x, c_area.position.y, c_area.size.x, c_area.size.y, RED);
    DrawSprite(sprite);
}

void MushroomEntity::DrawUI() {
    c_area.Draw();
}

void MushroomEntity::OnContainerOpened() {
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

void MushroomEntity::TakeDamage() {
    TraceLog(LOG_INFO, "MushroomEntity taking damage ");
}