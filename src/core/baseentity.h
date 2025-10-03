#pragma once


#include "gamedefs.h"
//#include "units.h"
#include <raylib.h>
#include <raymath.h>
#include "sprite.h"


#define DRAW_LIST_SIZE 100

class BaseEntity  {
    public:
    virtual ~BaseEntity() = default;
    virtual void Update() = 0;   
    virtual void Draw() = 0; 

    Rectangle collision_rect;
    bool should_delete;
    bool collided;
    Vector2 position;
    float rotation;
    //int id;
    Vector2 centered_offset;
};

class SpriteEntity : public BaseEntity {
    public:
    Sprite sprite;


    virtual ~SpriteEntity() = default;
    virtual void Update() = 0;
    virtual void Draw() = 0;
};

class AnimatedSpriteEntity : public BaseEntity {
    public:
    AnimatedSprite sprite;

    virtual ~AnimatedSpriteEntity() = default;
    virtual void Update() = 0;
    virtual void Draw() = 0;
};



struct  CollisionResult {
    Vector2 collision_dir;
    BaseEntity *collider;
    
};

struct  RayCast {
    Vector2 position;
    Vector2 direction;
    
};

extern std::vector<BaseEntity *> active_unit_list;
extern std::vector<BaseEntity *> recruit_unit_list;

//extern BaseEntity *active_unit_list[DRAW_LIST_SIZE];
//extern BaseEntity *recruit_unit_list[DRAW_LIST_SIZE];

extern int* level_array_data;



//inline void DL_Add(BaseEntity *_draw_list[DRAW_LIST_SIZE], BaseEntity *new_entity) {
inline void DL_Add(std::vector<BaseEntity *> _draw_list, BaseEntity *new_entity) {


    _draw_list.push_back(new_entity);
    TraceLog(LOG_INFO, "ADDING DRAWABLE AT INDEX %i", _draw_list.size());

/*     for(int i = 0; i < DRAW_LIST_SIZE; i++) {
        if(_draw_list[i] == nullptr){
            _draw_list[i] = new_entity; 
            //TraceLog(LOG_INFO, "ADDING DRAWABLE AT INDEX %i", i);
            return;
        }
    } */
}


//inline void DL_Draw(BaseEntity *_draw_list[DRAW_LIST_SIZE]) {
inline void DL_Draw(std::vector<BaseEntity *> _draw_list) {

    for(int i = 0; i < _draw_list.size(); i++) {
        if(_draw_list[i] != nullptr){
            _draw_list[i]->Draw();
        }
    }

/*     for(int i = 0; i < DRAW_LIST_SIZE; i++) {
        if(_draw_list[i] != nullptr){
            _draw_list[i]->Draw();
        }
    } */
}

//inline void DL_Update(BaseEntity *_draw_list[DRAW_LIST_SIZE]) {
inline void DL_Update(std::vector<BaseEntity *> _draw_list) {

    for(int i = 0; i < _draw_list.size(); i++) {
        if(_draw_list[i] != nullptr){
            _draw_list[i]->Update();
            if(_draw_list[i]->should_delete) {

                TraceLog(LOG_INFO, "DELETING ENTITY");
                _draw_list.erase(_draw_list.begin() + i);
            }
        }
    }

/*     for(int i = 0; i < 100; i++) {
        if(_draw_list[i] != nullptr){
            _draw_list[i]->Update();
            if(_draw_list[i]->should_delete) {
                TraceLog(LOG_INFO, "DELETING ENTITY");
                delete _draw_list[i];
                _draw_list[i] = nullptr;
            }
        }
    } */
}

//inline void DL_Clear(BaseEntity *_draw_list[DRAW_LIST_SIZE]) {
inline void DL_Clear(std::vector<BaseEntity *> _draw_list) {
    _draw_list.clear();

/*     for(int i = 0; i < DRAW_LIST_SIZE; i++) {
        if(_draw_list[i] != nullptr){
            delete _draw_list[i];
            _draw_list[i] = nullptr;
        }
    } */
}



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

inline bool CheckCollisionWithLevel(BaseEntity *checker, CollisionResult &collision_result, int _range) {
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
}

inline bool GetRayCollisionWithLevel(RayCast &_ray, CollisionResult &result, int range) {
    Vector2 end = Vector2Add(_ray.position, _ray.direction);
    Vector2 step = _ray.direction * 0.1;
    //Vector2 mid = Vector2Add(_ray.position, _ray.direction * 0.5f);

    for (int i = 1; i <= 10; i++) {

        int ix = ( ((step.x * i) + _ray.position.x) * INV_TILE_SIZE);
        int iy = ( ((step.y * i) + _ray.position.y) * INV_TILE_SIZE);

        //TraceLog(LOG_INFO, "RAY CHECKING %i %i %i  step %f %f", i, ix, iy, step.x, step.y);

        if(level_array_data[(iy * g_map_width + ix)] == 1) {
            return true;
        }
    }
    return false;
}