#pragma once 
#include "gamedefs.h"

class PlayerCharacter : public AnimatedSpriteEntity {
    public:
    PlayerCharacter(Vector2 _position);
    ~PlayerCharacter() override;
    void Update() override;
    void Draw() override;
    void CheckInput();


    Vector2 velocity;

}; 

extern PlayerCharacter *g_current_player;