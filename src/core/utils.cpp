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

void Signal::DisconnectAll() {
    callbacks.clear();
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



void DL_Add(std::vector<BaseEntity *> &_draw_list, BaseEntity *new_entity) {


    _draw_list.push_back(new_entity);
    //TraceLog(LOG_INFO, "ADDING DRAWABLE AT INDEX %i", _draw_list.size());

   //TraceLog(LOG_INFO, "ENTITY LIST SIZE %i", _draw_list.size());
}


void DL_Draw(std::vector<BaseEntity *> &_draw_list) {
    //TraceLog(LOG_INFO, "ENTITY LIST SIZE draw %i", _draw_list.size());
    for(int i = 0; i < _draw_list.size(); i++) {
        if(_draw_list[i] != nullptr){
            _draw_list[i]->Draw();
        }
    }
}


void DL_DrawUI(std::vector<BaseEntity *> &_draw_list) {
    //TraceLog(LOG_INFO, "ENTITY LIST SIZE draw %i", _draw_list.size());
    for(int i = 0; i < _draw_list.size(); i++) {
        if(_draw_list[i] != nullptr){
            _draw_list[i]->DrawUI();
        }
    }
}

void DL_Update(std::vector<BaseEntity *> &_draw_list) {
    //TraceLog(LOG_INFO, "ENTITY LIST SIZE update %i", _draw_list.size());

    for(int i = 0; i < _draw_list.size(); i++) {
        if(_draw_list[i] != nullptr){
            _draw_list[i]->Update();
            if(_draw_list[i]->should_delete) {
                //TraceLog(LOG_INFO, "DELETING ENTITY");
                delete _draw_list[i];
                _draw_list.erase(_draw_list.begin() + i);
                //_draw_list[i] = nullptr;
                --i;
            }
        }
    }
}

void DL_Clear(std::vector<BaseEntity *> &_draw_list) {

    for(int i = 0; i < _draw_list.size(); i++) {
        if(_draw_list[i] != nullptr){
            //TraceLog(LOG_INFO, "DELETING ENTITY");
             delete _draw_list[i];
            //_draw_list.erase(_draw_list.begin() + i);
                //_draw_list[i] = nullptr;
            //--i;
        }
    }
    
    _draw_list.clear();
    //TraceLog(LOG_INFO, "ENTITY LIST SIZE %i", _draw_list.size());
}

void DL_Sort(LevelData &_level_data) {

    _level_data.draw_list.clear();

    _level_data.draw_list.push_back(g_current_player);

    for (auto e : _level_data.environment_entities)
        _level_data.draw_list.push_back(e);


    for (auto e : _level_data.entity_list)
        _level_data.draw_list.push_back(e);

    //std::sort(level_data.draw_list.begin(), level_data.draw_list.end());

    std::sort(_level_data.draw_list.begin(), _level_data.draw_list.end(),
    [](BaseEntity* a, BaseEntity* b) {
        return a->GetYSort() < b->GetYSort();
    });
}


bool GetRayCollisionWithLevel(RayCast &_ray, CollisionResult &result, int range) {
    Vector2 end = Vector2Add(_ray.position, _ray.direction);
    Vector2 step = _ray.direction * 0.1;

    BaseScene *this_scene = g_current_scene.get();
    //TraceLog(LOG_INFO, "RAY CHECKING %i ", this_scene->level_data.precalc.map_index);

    if(g_game_data.is_in_sub_map) {
        this_scene = g_sub_scene.get();
    }

    
    LDTKLevel &this_level = g_ldtk_maps.levels[this_scene->level_data.precalc.map_index];

    if(this_scene->level_data.precalc.collision_layer_index == -1) {
        return false;
    }
    
    LDTKLayerInstance &col_layer = this_level.layer_instances[this_scene->level_data.precalc.collision_layer_index];

    int tile_size = this_scene->level_data.precalc.tile_size;
    float inv_tile_size = this_scene->level_data.precalc.inv_tile_size;
    int map_width = this_scene->level_data.precalc.map_width;

    //Vector2 mid = Vector2Add(_ray.position, _ray.direction * 0.5f);

    for (int i = 1; i <= 10; i++) {

        int ix = ( ((step.x * i) + _ray.position.x) * this_scene->level_data.precalc.inv_tile_size);
        int iy = ( ((step.y * i) + _ray.position.y) * this_scene->level_data.precalc.inv_tile_size);

        int index = iy * this_scene->level_data.precalc.map_width + ix;
        int value = col_layer.int_grid[index];

        //TraceLog(LOG_INFO, "RAY CHECKING %i %i %i  step %f %f", i, ix, iy, step.x, step.y);

        if(value == 1) {
            return true;
        }
    }
    return false;
}

bool CollideWithEntity(BaseEntity *checker, CollisionResult &collision_result) {
    bool collided = false;

    BaseScene *this_scene = g_current_scene.get();
    //TraceLog(LOG_INFO, "RAY CHECKING %i ", this_scene->level_data.precalc.map_index);

    if(g_game_data.is_in_sub_map) {
        this_scene = g_sub_scene.get();
    }

    
    for(auto entity : this_scene->level_data.entity_list) {
        if(entity->can_take_damage) {
            collided = CheckCollisionCircles( checker->position, checker->collision_radius, entity->position, entity->collision_radius);
            if(collided) {
                collision_result.collider = entity;
            }
        }
    }
    return collided;
}

bool CollideWithEntity(Vector2 c_pos, float c_radius, CollisionResult &collision_result) {

    bool collided = false;

    BaseScene *this_scene = g_current_scene.get();
    //TraceLog(LOG_INFO, "RAY CHECKING %i ", this_scene->level_data.precalc.map_index);

    if(g_game_data.is_in_sub_map) {
        this_scene = g_sub_scene.get();
    }

    for(auto entity : this_scene->level_data.entity_list) {
        if(entity->can_take_damage) {
            collided = CheckCollisionCircles( c_pos, c_radius, entity->position, entity->collision_radius);
            if(collided) {
                collision_result.collider = entity;
            }
        }
    }
    return collided;

}