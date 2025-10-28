#pragma once
#include "gamedefs.h"

class TileLayer {

    public:
    TileLayer();
    ~TileLayer();
    void Update();
    void Draw();

    bool CheckCollisionWithLevel(BaseEntity *checker, CollisionResult &collision_result, int _range);


    int map_index;



};

bool CheckCollisionWithLevel(BaseEntity *checker, CollisionResult &collision_result, int _range);

