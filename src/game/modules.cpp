#include "../core/gamedefs.h"

WorkbenchModuleEntity::WorkbenchModuleEntity(Vector2 _position, int _module_id) {

    module_id = _module_id;
    position = _position;
    rotation = 0.0f;
    LoadSprite(sprite, g_module_sprites[_module_id], position);
    collision_radius = 5;
    centered_offset = {0,0};
    collided = false;
    
    should_delete = false;

    m_area.area_activated.Connect( [&](){OnModuleUsed();} );
}

WorkbenchModuleEntity::~WorkbenchModuleEntity() {


    TraceLog(LOG_INFO, "MODULE DESTROYED");    
}

void WorkbenchModuleEntity::Update() {
    m_area.Update();
}

void WorkbenchModuleEntity::Draw() {
    //DrawRectangle(c_area.position.x, c_area.position.y, c_area.size.x, c_area.size.y, RED);
    DrawSprite(sprite);
}

void WorkbenchModuleEntity::DrawUI() {
    m_area.Draw();
}

void WorkbenchModuleEntity::OnModuleUsed() {
    g_game_data.current_module_id = module_id;
    open_module.EmitSignal();
}


float WorkbenchModuleEntity::GetYSort() {
    return position.y;
}

void WorkbenchModuleEntity::TakeDamage() {
    TraceLog(LOG_INFO, "taking damage ");
}