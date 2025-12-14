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

struct  RayCast {
    Vector2 position;
    Vector2 direction;
};

struct Polygon {
    std::vector<Vector2> points;
    Color color;
};

Vector2 vClamp(Vector2 v, float maxLength);


float GetAngleFromTo(Vector2 from, Vector2 to);


float WrapAngle(float a);


float ShortestAngleDiffRad(float from, float to);


float RotateTowardsRad(float current, float target, float maxTurn, float dt);

void SetCursorPosition(Vector2 _pos);



void DL_Add(std::vector<BaseEntity *> &_draw_list, BaseEntity *new_entity);

void DL_Draw(std::vector<BaseEntity *> &_draw_list);

void DL_DrawUI(std::vector<BaseEntity *> &_draw_list);

void DL_Update(std::vector<BaseEntity *> &_draw_list);

void DL_Clear(std::vector<BaseEntity *> &_draw_list);

bool GetRayCollisionWithLevel(RayCast &_ray, CollisionResult &result, int range);

bool CollideWithEntity(BaseEntity *checker, CollisionResult &collision_result);

bool CollideWithEntity(Vector2 c_pos, float c_radius, CollisionResult &collision_result);