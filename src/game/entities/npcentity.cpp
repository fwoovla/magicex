#include "../../core/gamedefs.h"
#include<cmath>
#include <raymath.h>

#define DETECT_RANGE 3
//#define SPEED 25.0f

NpcEntity::NpcEntity(Vector2 _position, int _uid): CreatureEntity(_position, _uid) {


    d_area.size = {16,16};
    d_area.position = position;

    d_area.area_activated.Connect( [&](){OnStartDialogue();} );

    TraceLog(LOG_INFO, "NPC CREATED uid %i   creature id %i  name %s speed  %0.2f", _uid, data->creature_id, data->name.c_str(), data->base_speed);
}


void NpcEntity::Update() {
    CreatureEntity::Update();
    d_area.position = {position.x - 8, position.y - 8};
    d_area.Update();

}

void NpcEntity::DrawUI() {
    //CreatureEntity::DrawUI();
    d_area.Draw();
}

void NpcEntity::OnStartDialogue() {
    start_dialogue.EmitSignal();
}