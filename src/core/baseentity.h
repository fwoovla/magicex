#pragma once


#include "gamedefs.h"
#include <raylib.h>
#include <raymath.h>
#include "sprite.h"

#define DRAW_LIST_SIZE 100

struct DamagePayload{
    Vector2 knockback;
    int attacker_id;
    int damage;
    //int payload_index;
};

class CharacterEntity;

class BaseEntity  {
    public:
    virtual ~BaseEntity() = default;
    virtual void Update() = 0;   
    virtual void Draw() = 0; 
    virtual void DrawUI() = 0;
    virtual float GetYSort() = 0;
    virtual void TakeDamage(DamagePayload _payload) = 0;

    //Sprite sprite;

    int uid;
    bool should_delete = false;
    Rectangle collision_rect;
    bool collided = false;
    Vector2 position;
    float rotation;
    Vector2 centered_offset;
    Vector2 ground_point_offset;
    float collision_radius;
    std::string identifier;

    bool is_persistant = false;
    bool is_obstructable = false;
    bool is_obstructed = false;
    bool y_sort = false;
    bool can_take_damage = false;
    bool is_on_screen = false;

    CharacterEntity *character_entity = nullptr;
};

class SpriteEntity : public BaseEntity {
    public:
    Sprite sprite;

    virtual ~SpriteEntity() = default;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void DrawUI() = 0;
    virtual float GetYSort() = 0;
};

class AnimatedSpriteEntity : public BaseEntity {
    public:
    AnimatedSprite sprite;

    virtual ~AnimatedSpriteEntity() = default;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void DrawUI() = 0;
    virtual float GetYSort() = 0;
};




/* inline void YSortEntities(LevelData &_level_data) {

} */


