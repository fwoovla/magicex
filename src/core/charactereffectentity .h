#pragma once

#include "gamedefs.h"
#include "baseentity.h"

class BaseCharacterEffectEntity : public SpriteEntity {
    public:
    
    //
    ~BaseCharacterEffectEntity() = default;
    
    Sprite sprite;
};


class MovementEffectEntity : public BaseCharacterEffectEntity {

    MovementEffectEntity(CharacterEffectData &_data, Vector2 _position);
    ~MovementEffectEntity() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;
    float GetYSort() override;

    void OnLifetimeTimeout();

    CharacterEffectData data;
    Timer lifetime;

};