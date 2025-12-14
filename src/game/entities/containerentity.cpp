#include "../../core/gamedefs.h"

PermContainerEntity::PermContainerEntity(Vector2 _position, int _s_id, int _lt_id) {
    position = _position;
    sprite_id = _s_id;
    rotation = 0.0f;
    LoadSprite(sprite, g_container_sprites[sprite_id], position);
    collision_radius = 5;
    centered_offset = {0,0};
    collided = false;
    y_sort = true;

    c_area.area_activated.Connect( [&](){OnContainerOpened();} );
    
    should_delete = false;
    is_persistant = false;
    is_obstructable = true;
    is_obstructed = false;
    can_take_damage = true;
}

PermContainerEntity::~PermContainerEntity() {

    if(!is_persistant) {
        for(int i = 0; i < c_area.item_list.size(); i++) {
            g_item_instances.erase(c_area.item_list[i]);
            TraceLog(LOG_INFO, "pc instance  #%i   erased %i", c_area.item_list[i], g_item_instances.size());
        }
    }

    TraceLog(LOG_INFO, "CONTAINER DESTROYED");
}

void PermContainerEntity::Update() {

    bool is_on_screen = false;
    int tile_size = g_current_scene->level_data.precalc.tile_size;

    if(position.x > (g_viewport.x_min - 2) * tile_size and position.x < (g_viewport.x_max + 2) * tile_size) {
        if(position.y > (g_viewport.y_min - 2) * tile_size and position.y < (g_viewport.y_max + 2) * tile_size) {
            is_on_screen = true;
        }
    }

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

void PermContainerEntity::Draw() {
    if(!is_obstructed) {
        DrawSprite(sprite);
    }

    if(g_game_settings.show_debug) {
        Color color = WHITE;
        if(is_obstructed) {
            color = RED;
        }
        DrawLineV(raycast.position, {raycast.position.x + raycast.direction.x, raycast.position.y + raycast.direction.y}, color);
    }
}

void PermContainerEntity::DrawUI() {
    c_area.Draw();
}


void PermContainerEntity::OnContainerOpened() {
    TraceLog(LOG_INFO, "CONTAINER OPEN");
    g_game_data.return_container = this;
    open_container.EmitSignal();
}

bool PermContainerEntity::IsEmpty() {
    for(int item = 0; item < c_area.item_list.size(); item++) {
        if(c_area.item_list[item] != -1) {
            return false;
        }
    }
    return true;
}

float PermContainerEntity::GetYSort() {
    return position.y;
}

void PermContainerEntity::TakeDamage() {
    TraceLog(LOG_INFO, "container taking damage ");
}

//=====================================================

GroundContainerEntity::GroundContainerEntity(Vector2 _position, int _s_id) {
    position = _position;
    sprite_id = _s_id;
    rotation = 0.0f;
    LoadSprite(sprite, g_item_sprites[sprite_id], Vector2Add(position, {4,4}));
    ScaleSprite(sprite, {0.5f, 0.5f});
    collision_radius = 5;
    centered_offset = {0,0};
    collided = false;
    y_sort = true;

    c_area.area_activated.Connect( [&](){OnContainerOpened();} );
    c_area.list_changed.Connect( [&](){OnListChanged();} );

    should_delete = false;
    is_persistant = false;
    is_obstructable = true;
    is_obstructed = false;
    can_take_damage = false;
}

GroundContainerEntity::~GroundContainerEntity() {

    if(!is_persistant) {
        for(int i = 0; i < c_area.item_list.size(); i++) {
            g_item_instances.erase(c_area.item_list[i]);
            TraceLog(LOG_INFO, "gc instance  #%i   erased %i", c_area.item_list[i], g_item_instances.size());
        }
    }

    TraceLog(LOG_INFO, "GroundContainerEntity DESTROYED");
}

void GroundContainerEntity::Update() {
    bool is_on_screen = false;
    int tile_size = g_current_scene->level_data.precalc.tile_size;

    if(position.x > (g_viewport.x_min - 2) * tile_size and position.x < (g_viewport.x_max + 2) * tile_size) {
        if(position.y > (g_viewport.y_min - 2) * tile_size and position.y < (g_viewport.y_max + 2) * tile_size) {
            is_on_screen = true;
        }
    }

    if(!is_on_screen) {
        return;
    }

    c_area.Update();


    raycast.position = {position.x, position.y};
    float rot = GetAngleFromTo(raycast.position, g_current_player->position);
    float dist = Vector2Distance(raycast.position, g_current_player->position);
    raycast.direction = Vector2Rotate({dist,0}, rot);

    CollisionResult result;
    is_obstructed = GetRayCollisionWithLevel(raycast, result, 0);
}

void GroundContainerEntity::Draw() {
    if(!is_obstructed) {
        DrawSprite(sprite);
    }

    if(g_game_settings.show_debug) {
        Color color = WHITE;
        if(is_obstructed) {
            color = RED;
        }
        DrawLineV(raycast.position, {raycast.position.x + raycast.direction.x, raycast.position.y + raycast.direction.y}, color);
    }
}

void GroundContainerEntity::DrawUI() {
    c_area.Draw();
}

void GroundContainerEntity::OnContainerOpened() {
    TraceLog(LOG_INFO, "CONTAINER OPEN");
    g_game_data.return_container = this;
    open_container.EmitSignal();
}

void GroundContainerEntity::OnListChanged() {

}


bool GroundContainerEntity::IsEmpty() {
    for(int item = 0; item < c_area.item_list.size(); item++) {
        if(c_area.item_list[item] != -1) {
            return false;
        }
    }
    return true;
}

void GroundContainerEntity::SetSprite(int sprite_id) {
    LoadSprite(sprite, g_item_sprites[sprite_id], position);
    ScaleSprite(sprite, {0.5f, 0.5f});
}

float GroundContainerEntity::GetYSort() {
    return position.y;
}

void GroundContainerEntity::TakeDamage() {
    TraceLog(LOG_INFO, "container taking damage ");
}