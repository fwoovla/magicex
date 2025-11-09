#pragma once 
#include "gamedefs.h"

class ContainerEntity : public SpriteEntity {
    public:
    ContainerEntity(Vector2 _position);
    ~ContainerEntity() override;
    void Update() override;
    void Draw() override;

};