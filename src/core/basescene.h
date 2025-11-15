#pragma once
#include "gamedefs.h"


class BaseScene  {
    public:
    virtual ~BaseScene(){};
    virtual SCENE_ID Update() = 0;
    virtual void Draw() = 0;    
    virtual void DrawScene() = 0;
    virtual void DrawUI() = 0;
    
    LevelData level_data;
    SCENE_ID scene_id;
    SCENE_ID return_scene;
        
};

