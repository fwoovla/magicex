#pragma once 
#include "gamedefs.h"



class CharacterEntity : public AnimatedSpriteEntity {
    public:
    
    ~CharacterEntity() = default;

    int uid;
    Vector2 velocity;
    Sprite weapon_sprite;
    bool can_shoot;
    bool can_switch;
    Timer shot_timer;
    WeaponData current_weapon_data;

}; 


class PlayerCharacter : public CharacterEntity {
    public:
    PlayerCharacter(Vector2 _position, int uid);
    ~PlayerCharacter() override;
    void Update() override;
    void Draw() override;
    void DrawUI()override;
    float GetYSort() override;
    void CheckInput();

    bool CanEquip(int item_id);
    void Equip(int item_id);

    bool CanUnEquip(int item_id);
    void UnEquip(int item_id);

    void OnShotTimerTimeout();


    //Vector2 velocity;

    //Sprite weapon_sprite;

    //Sprite crosshair_sprite;
    //bool can_switch;
    //bool can_shoot;

    //Timer shot_timer;

    //WeaponData current_weapon_data;

}; 

extern PlayerCharacter *g_current_player;
extern std::unordered_map< int, PlayerData> g_character_data;