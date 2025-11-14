#include "../../core/gamedefs.h"

PermContainerEntity::PermContainerEntity(Vector2 _position, int _s_id, int _lt_id) {
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

PermContainerEntity::~PermContainerEntity() {
    TraceLog(LOG_INFO, "CONTAINER DESTROYED");    
}

void PermContainerEntity::Update() {
    c_area.Update();
}

void PermContainerEntity::Draw() {
    DrawSprite(sprite);
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

    c_area.area_activated.Connect( [&](){OnContainerOpened();} );
    c_area.list_changed.Connect( [&](){OnListChanged();} );
    c_area.list_empty.Connect( [&](){OnListEmpty();} );
    should_delete = false;
}

GroundContainerEntity::~GroundContainerEntity() {
    TraceLog(LOG_INFO, "GroundContainerEntity DESTROYED");
}

void GroundContainerEntity::Update() {
    
    c_area.Update();
}

void GroundContainerEntity::Draw() {
    //TraceLog(LOG_INFO, "gCONTAINER draw");
    DrawSprite(sprite);
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
/*     int index = 0;
    for(int item = 0; item < c_area.item_list.size(); item++) {
        //TraceLog(LOG_INFO, "looking for item %i index %i", character_menu->blank_list[item], item);
        if(c_area.item_list[item] != -1) {
            //TraceLog(LOG_INFO, "found item at index %i", i);
            index = g_item_data[ c_area.item_list[item]].sprite_id;
            TraceLog(LOG_INFO, "found item id %i", index);
            break;
        }
    }
    sprite.texture = g_item_sprites[index]; */
}

void GroundContainerEntity::OnListEmpty() {
    should_delete = true;
}


bool GroundContainerEntity::IsEmpty() {
    for(int item = 0; item < c_area.item_list.size(); item++) {
        if(c_area.item_list[item] != -1) {
            return false;
        }
    }
    return true;
}