#include "../core/gamedefs.h"

ModuleEntity::ModuleEntity(Vector2 _position, int _module_id) {

    module_id = _module_id;
    position = _position;
    rotation = 0.0f;
    LoadSprite(sprite, g_module_sprites[_module_id], position);
    collision_radius = 5;
    centered_offset = {0,0};
    collided = false;
    can_take_damage = false;
    
    should_delete = false;

    m_area.area_activated.Connect( [&](){OnModuleUsed();} );
}

ModuleEntity::~ModuleEntity() {


    TraceLog(LOG_INFO, "MODULE DESTROYED");    
}

void ModuleEntity::Update() {
    is_on_screen = IsOnScreen(position, sprite.size);
    if(!is_on_screen) {
        return;
    }
    m_area.Update();
}

void ModuleEntity::Draw() {
    if(!is_on_screen) {
        return;
    }
    DrawSprite(sprite);
    if(g_game_settings.show_debug) {
        DrawRectangle(m_area.position.x, m_area.position.y, m_area.size.x, m_area.size.y, YELLOW);
    }
}

void ModuleEntity::DrawUI() {
    if(!is_on_screen) {
        return;
    }
    m_area.Draw();
}

void ModuleEntity::OnModuleUsed() {
    g_game_data.current_module_id = module_id;
    open_module.EmitSignal();
}


float ModuleEntity::GetYSort() {
    return position.y;
}

void ModuleEntity::TakeDamage(DamagePayload _payload) {
    TraceLog(LOG_INFO, "taking damage ");
}