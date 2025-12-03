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



//extern std::vector<BaseEntity *> active_entity_list;
//extern std::vector<std::unique_ptr<BaseEntity>> entity_draw_list;


//extern int* level_array_data;


void DL_Add(std::vector<BaseEntity *> &_draw_list, BaseEntity *new_entity);



//inline void DL_Draw(BaseEntity *_draw_list[DRAW_LIST_SIZE]) {
void DL_Draw(std::vector<BaseEntity *> &_draw_list);



void DL_Update(std::vector<BaseEntity *> &_draw_list);


void DL_Clear(std::vector<BaseEntity *> &_draw_list);




/* inline bool CheckCollisionWithEntities(BaseEntity *checker, CollisionResult &collision_result) {
    if(checker->should_delete) {
        return false;
    }

    for(int i = 0; i < DRAW_LIST_SIZE; i++) {
        if(entity_list[i] != nullptr) {
            if(entity_list[i]->id == checker->id) {continue;}
            if(entity_list[i]->should_delete) {continue;}
            //TraceLog(LOG_INFO, "checking entities at index %i", i); 

            if(CheckCollisionRecs( checker->collision_rect, entity_list[i]->collision_rect)) {   
                //TraceLog(LOG_INFO, "COLLIDED"); 
                collision_result.collider = entity_list[i];
                //TraceLog(LOG_INFO, "COLLIDED %i", i);
                return true;
            } 
        }
    }
    return false;
} */


/* inline bool CheckCollisionWithBullets(BaseEntity *checker, CollisionResult &collision_result) {
    if(checker->should_delete or checker->should_delete) {
        return false;
    }
     for(int i = 0; i < DRAW_LIST_SIZE; i++) {
        if(bullet_list[i] != nullptr) {
            if(bullet_list[i]->id == checker->id) {continue;}
            if(bullet_list[i]->should_delete) {continue;}
            //TraceLog(LOG_INFO, "checking bullets at index %i", i); 

            if(CheckCollisionRecs( checker->collision_rect, bullet_list[i]->collision_rect)) {    
                collision_result.collider = bullet_list[i];
                //TraceLog(LOG_INFO, "COLLIDED %i", collision_result.collider->id);
                return true; 
            } 
        }
    } 
    return false;
} */

/* inline bool CheckCollisionWithLevel(BaseEntity *checker, CollisionResult &collision_result, int _range) {
    //================TILE COLLISION=========================
    for(int x = -1; x <  _range; x++) {
        for(int y = -1; y < _range; y++) {

            int ix = (checker->collision_rect.x * INV_TILE_SIZE) + x;
            int iy = (checker->collision_rect.y * INV_TILE_SIZE) + y;
            if(level_array_data[(iy * g_map_width + ix)] == 1) {

                if(CheckCollisionRecs( checker->collision_rect, {(float)ix * TILE_SIZE, (float)iy * TILE_SIZE, TILE_SIZE, TILE_SIZE} )) {
                    return true;
                }
            }
        }
    }

    return false;
} */

//bool GetRayCollisionWithLevel(RayCast &_ray, CollisionResult &result, int range);