#include "gamedefs.h"

void Timer::Start(double _wait_time, bool _one_shot) {

    active = true;
    finished = false;
    elapsed_time = 0.0;
    one_shot = _one_shot;
    wait_time = _wait_time;
}

void Timer::Stop() {
    active = false;
}

void Timer::Update() {
    if(!active){
        return;
    }
    //TraceLog(LOG_INFO,"TIMER");
    elapsed_time += GetFrameTime();
    if(elapsed_time >= wait_time) {

        active = false;
        finished = true;
        elapsed_time = 0.0;

        if(!one_shot) {
            //active = true;
            finished = false;
            active = true;
        }
        timer_timeout.EmitSignal();
    }
}

bool Timer::IsDone()
{
    return finished;
}

bool Timer::IsActive()
{
    return active;
}

double Timer::TimeRemaining()
{
    return wait_time - elapsed_time;
}

double Timer::TimeElapsed() {
    return elapsed_time;
}

double Timer::GetWaitTime() {
    return wait_time;;
}



void Signal::Connect(std::function<void()> const& callback) {
    callbacks.push_back(callback);
}

void Signal::EmitSignal() {
    for(int i = 0; i < callbacks.size(); i++) {
        callbacks[i]();
    }
}   



/* void AreaSignal::Connect(std::function<void(TransitionArea a)> const& callback) {
    callbacks.push_back(callback);
}

void AreaSignal::EmitSignal(TransitionArea a) {
    for(int i = 0; i < callbacks.size(); i++) {
        callbacks[i](a);
    }
}    */









Vector2 vClamp(Vector2 v, float maxLength) {
    float length = sqrtf(v.x * v.x + v.y * v.y);
    if (length > maxLength) {
        float scale = maxLength / length;
        v.x *= scale;
        v.y *= scale;
    }
    return v;
}

float GetAngleFromTo(Vector2 from, Vector2 to) {
    Vector2 dir = { to.x - from.x, to.y - from.y };
    float angle = atan2(dir.y, dir.x);
    return angle;
}

float WrapAngle(float a) {
    a = std::fmod(a + PI, 2*PI);
    if (a < 0) a += 2*PI;
    return a - PI;
}

float ShortestAngleDiffRad(float from, float to) {
    return WrapAngle(to - from);
}


 float RotateTowardsRad(float current, float target, float maxTurn, float dt) {
    float diff = ShortestAngleDiffRad(current, target);
    float maxStep = maxTurn * dt;
    if (std::fabs(diff) <= 1e-4f) return target;
    if (std::fabs(diff) <= maxStep) return target;
    return current + (diff > 0 ? 1 : -1) * maxStep;
}


void SetCursorPosition(Vector2 _pos) {
    g_cursor.poisition = _pos;
    g_cursor.sprite.position = _pos;
}