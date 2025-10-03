#pragma once 
#include "gamedefs.h"

class BaseUnit : public AnimatedSpriteEntity {
    public:
    BaseUnit(Vector2 _position, UnitData _data);
    ~BaseUnit() override;
    void Update() override;
    void Draw() override;
    void CheckInput();

    UnitData data;

    bool hovered;
    bool selected;
    bool collided;
    bool is_inside;

    int times_collided;
    int steps;
    
    float collision_radius;
    
    Vector2 target_position;
    Vector2 velocity;
    Vector2 target_velocity;
    Vector2 spread_velocity;
}; 

inline bool CheckCollisionWithUnits(BaseEntity *checker, CollisionResult &collision_result) {
    if(checker->should_delete) {
        return false;
    }
    BaseUnit *checker_unit =  dynamic_cast<BaseUnit *>(checker);

    for(int i = 0; i < active_unit_list.size(); i++) {

        BaseUnit *_unit =  dynamic_cast<BaseUnit *>(active_unit_list[i]);
        
        if(_unit->data.id == checker_unit->data.id) {continue;}
        if(_unit->should_delete) {continue;}
        if(CheckCollisionCircles( checker_unit->position, checker_unit->collision_radius, _unit->position, _unit->collision_radius ) ) { 
            collision_result.collider = _unit;
            collision_result.collision_dir = {0,0};
            return true;
        }
    }
    return false;
}