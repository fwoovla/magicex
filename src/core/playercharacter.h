#pragma once 
#include "gamedefs.h"

class PlayerCharacter : public AnimatedSpriteEntity {
    public:
    PlayerCharacter(Vector2 _position);
    ~PlayerCharacter() override;
    void Update() override;
    void Draw() override;
    void DrawUI()override;
    void CheckInput();

    bool CanEquip(int item_id);
    void Equip(int item_id);

    bool CanUnEquip(int item_id);
    void UnEquip(int item_id);


    Vector2 velocity;

    Sprite weapon_sprite;

    //Sprite crosshair_sprite;
    bool can_switch;

}; 

extern PlayerCharacter *g_current_player;