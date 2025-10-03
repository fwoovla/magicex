#include "utils.h"

/* Timer::Timer(double _wait_time, bool _autostart, bool _one_shot) {
    wait_time = _wait_time;
    active = false;
    finished = false;
    elapsed_time = 0.0;
    one_shot = _one_shot;
    autostart = _autostart;


    if(autostart) {
        Start();
    }
} */

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


