#pragma once
#include "gamedefs.h"

class TileLayer {

    public:
    TileLayer();
    ~TileLayer();
    void Update();
    void Draw();

    //bool CollideAndSlide(BaseEntity *checker, CollisionResult &collision_result, int _range);


    int map_index;

};

bool CollideAndSlide(BaseEntity *checker, CollisionResult &collision_result, int _range);

bool CollideWithTile(BaseEntity *checker, CollisionResult &collision_result);

