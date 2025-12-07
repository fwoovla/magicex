#include "../../core/gamedefs.h"


EnvironmentalEntity::EnvironmentalEntity(Vector2 _position, int _sprite_id, bool _fadeable) {
    position = _position;
    //sprite_id = _sprite_id;
    rotation = 0.0f;
    y_sort = true;
    

    LoadSprite(sprite, g_environment_sprites[_sprite_id], position);
    LoadSprite(shadow_sprite, g_shadow_sprites[_sprite_id], {position.x - (sprite.size.x/2), position.y});

    float x_offset = sprite.texture.width/2;
    float y_offset = sprite.texture.height;
    sprite.center = { x_offset, y_offset };

    collision_radius = 5;
    centered_offset = {0,0};
    collided = false;

    collision_rect = {
            position.x - sprite.center.x,
            position.y - sprite.center.y,
            sprite.size.x,
            sprite.size.y,
        };
    
    should_delete = false;
    is_persistant = false;
    is_obstructable = true;
    is_obstructed = false;

    fadeable = _fadeable;
}

EnvironmentalEntity::~EnvironmentalEntity() {

    TraceLog(LOG_INFO, "ENVIRONMENTAL ENTITY DESTROYED");
}

void EnvironmentalEntity::Update() {

}

void EnvironmentalEntity::Draw() {
    //DrawCircleV(sprite.position, 2, WHITE);
    if(fadeable) {
                    
        
                    
        if( CheckCollisionPointRec(Vector2Add(g_current_player->position, g_current_player->ground_point_offset), collision_rect ) ) {
            float alpha_value = Lerp((float)sprite.modulate.a, 30.0f, 0.5f );
            sprite.modulate.a = (unsigned char)alpha_value;
        }
        else {
            float alpha_value = Lerp((float)sprite.modulate.a, 255.0f, 0.5f );
            sprite.modulate.a = (unsigned char)alpha_value;
        }
    }
    if(g_game_settings.show_debug) {

        DrawRectangleRec(collision_rect, BLUE);
    }
    DrawSprite(sprite);
    DrawSprite(shadow_sprite);
}

void EnvironmentalEntity::DrawUI() {
}

float EnvironmentalEntity::GetYSort() {
    return position.y;
}