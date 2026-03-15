#include "gamedefs.h"

#define RAY_STEP_RESOLUTION 20
#define RAY_STEP 1/RAY_STEP_RESOLUTION



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

bool Timer::IsDone() {
    return finished;
}

bool Timer::IsActive() {
    return active;
}

double Timer::TimeRemaining() {
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




bool IsOnScreen(Vector2 _position, Vector2 _size) {
    int tile_size = g_current_scene->level_data.precalc.tile_size;
    float inv_tile = g_current_scene->level_data.precalc.inv_tile_size;

if(_position.x > (g_viewport.x_min - (_size.x * inv_tile) ) * tile_size and _position.x < (g_viewport.x_max + (_size.x * inv_tile)) * tile_size) {
        if(_position.y > (g_viewport.y_min - (_size.y * inv_tile)) * tile_size and _position.y < (g_viewport.y_max + (_size.y * inv_tile)) * tile_size) {

            return true;
        }
    }
    return false;
}




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


float RectCenterDistSq(const Rectangle& a, const Rectangle& b)
{
    float ax = a.x + a.width  * 0.5f;
    float ay = a.y + a.height * 0.5f;
    float bx = b.x + b.width  * 0.5f;
    float by = b.y + b.height * 0.5f;

    float dx = ax - bx;
    float dy = ay - by;

    return dx*dx + dy*dy;
}


void SetCursorPosition(Vector2 _pos) {
    g_cursor.poisition = _pos;
    g_cursor.sprite.position = _pos;
}


void DL_Add(std::vector<std::unique_ptr<BaseEntity>> &_draw_list, std::unique_ptr<BaseEntity> new_entity) {

    _draw_list.push_back(std::move(new_entity));
    //TraceLog(LOG_INFO, "ADDING DRAWABLE AT INDEX %i", _draw_list.size());

   //TraceLog(LOG_INFO, "ENTITY LIST SIZE %i", _draw_list.size());
}


void DL_Draw(std::vector<std::unique_ptr<BaseEntity>> &_draw_list) {
    //TraceLog(LOG_INFO, "ENTITY LIST SIZE draw %i", _draw_list.size());
    for(int i = 0; i < _draw_list.size(); i++) {
        if(_draw_list[i] != nullptr){
            _draw_list[i]->Draw();
        }
    }
}


void DL_DrawUI(std::vector<std::unique_ptr<BaseEntity>> &_draw_list) {
    //TraceLog(LOG_INFO, "ENTITY LIST SIZE draw %i", _draw_list.size());
    for(int i = 0; i < _draw_list.size(); i++) {
        if(_draw_list[i] != nullptr){
            _draw_list[i]->DrawUI();
        }
    }
}

void DL_Update(std::vector<std::unique_ptr<BaseEntity>> &_draw_list) {
    //TraceLog(LOG_INFO, "ENTITY LIST SIZE update %i", _draw_list.size());

    for(int i = 0; i < _draw_list.size(); i++) {
        _draw_list[i]->Update();
        if(_draw_list[i]->should_delete) {
            if(_draw_list[i] != nullptr){
                _draw_list.erase(_draw_list.begin() + i);
                --i;
            }
        }      
    }
}


void DL_Clear(std::vector<BaseEntity *> &_draw_list) {


}

void DL_Sort(LevelData &_level_data) {
/* 
    _level_data.draw_list.clear();

    _level_data.draw_list.push_back(g_current_player.get());

    for (auto &e : _level_data.environment_entities)
        _level_data.draw_list.push_back(e.get());


    for (auto &e : _level_data.entity_list)
        _level_data.draw_list.push_back(e.get());

    std::sort(_level_data.draw_list.begin(), _level_data.draw_list.end(),
    [](BaseEntity* a, BaseEntity* b) {
        return a->GetYSort() < b->GetYSort();
    }); */
}


bool GetRayCollisionWithLevel(RayCast &_ray, CollisionResult &result, int range) {
    Vector2 end = Vector2Add(_ray.position, _ray.direction);
    Vector2 step = _ray.direction * RAY_STEP;

    BaseScene *this_scene = g_current_scene.get();
    //TraceLog(LOG_INFO, "RAY CHECKING %i ", this_scene->level_data.precalc.map_index);

    if(g_game_data.is_in_sub_map) {
        this_scene = g_sub_scene.get();
    }

    
    
    if(this_scene->level_data.precalc.collision_layer_index == -1) {
        return false;
    }
    
    LDTKLevel &this_level = g_ldtk_maps.levels[this_scene->level_data.precalc.map_index];
    
    LDTKLayerInstance &col_layer = this_level.layer_instances[this_scene->level_data.precalc.collision_layer_index];

    int tile_size = this_scene->level_data.precalc.tile_size;
    float inv_tile_size = this_scene->level_data.precalc.inv_tile_size;
    int map_width = this_scene->level_data.precalc.map_width;

    //Vector2 mid = Vector2Add(_ray.position, _ray.direction * 0.5f);

    for (int i = 1; i <= RAY_STEP_RESOLUTION; i++) {

        int ix = ( ((step.x * i) + _ray.position.x) * this_scene->level_data.precalc.inv_tile_size);
        int iy = ( ((step.y * i) + _ray.position.y) * this_scene->level_data.precalc.inv_tile_size);

        int index = iy * this_scene->level_data.precalc.map_width + ix;
        if(index < col_layer.int_grid.size()-1) {
        
            int value = col_layer.int_grid[index];
            
            //TraceLog(LOG_INFO, "RAY CHECKING %i %i %i  step %f %f", i, ix, iy, step.x, step.y);
            
            if(value == 1) {
                return true;
            }
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

    for(auto &entity : this_scene->level_data.entity_list) {
        if(entity->can_take_damage and entity->is_on_screen) {
            collided = CheckCollisionCircles( checker->position, checker->collision_radius, entity->position, entity->collision_radius);
            if(collided) {
                collision_result.collider = entity.get();
                return collided;
            }
        }
    }
    return collided;
}

bool CollideWithEntity(Vector2 c_pos, float c_radius, CollisionResult &collision_result) {

    bool collided = false;

    BaseScene *this_scene = g_current_scene.get();

    if(g_game_data.is_in_sub_map) {
        this_scene = g_sub_scene.get();
    }

    for(auto &entity : this_scene->level_data.entity_list) {
        if(entity->can_take_damage and entity->is_on_screen) {
            //TraceLog(LOG_INFO, "CHECKING COLLISION WITH? %s ", entity->identifier.c_str());
            //TraceLog(LOG_INFO, "entity position? %0.2f  %0.2f      radius %0.2f", entity->position.x, entity->position.y, entity->collision_radius);
            collided = CheckCollisionCircles( c_pos, c_radius, entity->position, entity->collision_radius);
            if(collided) {
                collision_result.collider = entity.get();
                return collided;
            }
        }
    }
    //TraceLog(LOG_INFO, "\n");
    return collided;

}


void DetectCreatures(CharacterEntity &checker, float c_radius, EntityDetectResult &_result) {

    bool collided = false;

    BaseScene *this_scene = g_current_scene.get();

    if(g_game_data.is_in_sub_map) {
        this_scene = g_sub_scene.get();
    }

    for(auto &entity : this_scene->level_data.draw_list) {
        collided = false;
        if(entity->can_take_damage and entity->character_entity and &checker != entity->character_entity) {
            collided = CheckCollisionCircles( checker.position, c_radius, entity->position, entity->collision_radius);
            if(collided) {
                _result.detected_creatures.push_back(entity->character_entity);
            }
        }
    }
    if(CheckCollisionCircles( checker.position, c_radius, g_current_player->position, g_current_player->collision_radius)) {
        _result.detected_creatures.push_back(g_current_player.get());
    }
    //TraceLog(LOG_INFO, "\n");
}