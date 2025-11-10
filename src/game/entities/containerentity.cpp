#include "../../core/gamedefs.h"

ContainerEntity::ContainerEntity(Vector2 _position, int _s_id, int _lt_id) {
    position = _position;
    sprite_id = _s_id;
    rotation = 0.0f;
    LoadSprite(sprite, g_container_sprites[sprite_id], position);
    collision_radius = 5;
    centered_offset = {0,0};
    collided = false;

    c_area.area_activated.Connect( [&](){OnContainerOpened();} );
    should_delete = false;
}

ContainerEntity::~ContainerEntity() {
    TraceLog(LOG_INFO, "CONTAINER DESTROYED");    
}

void ContainerEntity::Update() {

}

void ContainerEntity::Draw() {
    DrawSprite(sprite);
}

void ContainerEntity::OnContainerOpened() {
    TraceLog(LOG_INFO, "CONTAINER OPEN");
    open_container.EmitSignal();

}