#pragma once

#include "baseentity.h"
#include "gamedefs.h"
#include <vector>
#include <raylib.h>
#include <cmath>
#include <functional>

#define MAX_SIGNAL_CONNECTIONS 256


enum SIGNAL {
    END_GAME,
    TIMER_TIMEOUT,
    PLAY_PRESSED,
    SHOULD_DELETE,
    CAN_FIRE
};

class Signal {
    public:
    void Connect(std::function<void()> const& callback);
    void DisconnectAll();
    void EmitSignal();    

    std::vector<std::function<void()>> callbacks;
};


class Timer{
    public:
    void Start(double _wait_time, bool _one_shot);
    void Stop();
    void Update(); //false if finished
    bool IsDone();
    bool IsActive();
    double TimeRemaining();
    double TimeElapsed();
    double GetWaitTime();
    
    Signal timer_timeout;

    double wait_time; //in seconds
    private:
    double elapsed_time;
    bool active;
    bool finished;
    bool one_shot;    
};


struct  CollisionResult {
    Vector2 collision_dir;
    BaseEntity *collider;
    
};

struct  EntityDetectResult {
    //Vector2 collision_dir;
    std::vector<CharacterEntity*> detected_creatures;
};

struct  RayCast {
    Vector2 position;
    Vector2 direction;
};

struct Polygon {
    std::vector<Vector2> points;
    Color color;
};

Vector2 vClamp(Vector2 v, float maxLength);




bool IsOnScreen(Vector2 _position, Vector2 _size);


float GetAngleFromTo(Vector2 from, Vector2 to);


float WrapAngle(float a);


float ShortestAngleDiffRad(float from, float to);


float RotateTowardsRad(float current, float target, float maxTurn, float dt);

float RectCenterDistSq(const Rectangle& a, const Rectangle& b);


void SetCursorPosition(Vector2 _pos);



void DL_Add(std::vector<std::unique_ptr<BaseEntity>> &_draw_list, std::unique_ptr<BaseEntity> new_entity);

void DL_Draw(std::vector<std::unique_ptr<BaseEntity>> &_draw_list);

void DL_DrawUI(std::vector<std::unique_ptr<BaseEntity>> &_draw_list);

void DL_Update(std::vector<std::unique_ptr<BaseEntity>> &_draw_list);

void DL_Clear(std::vector<std::unique_ptr<BaseEntity>> &_draw_list);

bool GetRayCollisionWithLevel(RayCast &_ray, CollisionResult &result, int range);

bool CollideWithEntity(BaseEntity *checker, CollisionResult &collision_result);

bool CollideWithEntity(Vector2 c_pos, float c_radius, CollisionResult &collision_result);

void DetectCreatures(CharacterEntity &checker, float c_radius, EntityDetectResult &_result);