#pragma once
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
    void EmitSignal();    

    std::vector<std::function<void()>> callbacks;
};

/* class AreaSignal {
    public:
    void Connect(std::function<void( TransitionArea a )> const& callback);
    void EmitSignal(TransitionArea a);    

    std::vector<std::function<void( TransitionArea a )>> callbacks;
}; */


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


Vector2 vClamp(Vector2 v, float maxLength);


float GetAngleFromTo(Vector2 from, Vector2 to);


float WrapAngle(float a);


float ShortestAngleDiffRad(float from, float to);


float RotateTowardsRad(float current, float target, float maxTurn, float dt);

void SetCursorPosition(Vector2 _pos);

