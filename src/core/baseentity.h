#pragma once


#include "gamedefs.h"
#include <raylib.h>
#include <raymath.h>
#include "sprite.h"


#define DRAW_LIST_SIZE 100

class BaseEntity  {
    public:
    virtual ~BaseEntity() = default;
    virtual void Update() = 0;   
    virtual void Draw() = 0; 
    virtual void DrawUI() = 0;
    virtual float GetYSort() = 0;

    //Sprite sprite;

    bool should_delete;
    Rectangle collision_rect;
    bool collided;
    Vector2 position;
    float rotation;
    Vector2 centered_offset;
    Vector2 ground_point_offset;
    float collision_radius;
    std::string identifier;

    bool is_persistant;
    bool is_obstructable;
    bool is_obstructed;
    bool y_sort;
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

