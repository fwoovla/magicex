#pragma once 
#include "gamedefs.h"

enum AnimationState {
    ANIM_IDLE = 0,
    ANIM_WALK = 1,
    ANIM_RUN = 2,
    ANIM_STUN = 3
};

enum WeaponState {
    WSTATE_IDLE = 0,
    WSTATE_MELE = 1,
    WSTATE_SPELL = 2,
};

class CharacterEntity : public AnimatedSpriteEntity {
    public:
    
    ~CharacterEntity() = default;
    virtual bool CanEquip(int item_id) = 0;
    virtual void Equip(int item_id) = 0;

    virtual bool CanUnEquip(int item_id) = 0;
    virtual void UnEquip(int item_id) = 0;

    virtual void OnSpellTimerTimeout() = 0;
    virtual void OnMeleTimerTimeout() = 0;
    virtual void OnStunTimerTimeout() = 0;

    int uid;
    Vector2 velocity;
    Sprite weapon_sprite;
    Sprite shadow_sprite;
    bool can_use_spell;
    bool can_mele;
    bool can_switch;
    bool is_stunned;
    int stunned_frames;
    float weapon_end_rotation;
    int swing_dir;
    Timer spell_timer;
    Timer mele_timer;
    Timer stun_timer;
    WeaponData current_weapon_data;

    AnimationState animation_state;
    WeaponState weapon_state;
}; 



class TestDummyEntity : public CharacterEntity {
    public:
    TestDummyEntity(Vector2 _position, int uid);
    ~TestDummyEntity() override;
    void TakeDamage() override;
    void Update() override;
    void Draw() override;
    void DrawUI()override;
    float GetYSort() override;
    
    bool CanEquip(int item_id) override;
    void Equip(int item_id) override;
    
    bool CanUnEquip(int item_id) override;
    void UnEquip(int item_id) override;
    
    void OnSpellTimerTimeout() override;
    void OnMeleTimerTimeout() override;
    void OnStunTimerTimeout() override;
};


class PlayerCharacter : public CharacterEntity {
    public:
    PlayerCharacter(Vector2 _position, int uid);
    ~PlayerCharacter() override;
    void TakeDamage() override;
    void Update() override;
    void Draw() override;
    void DrawUI()override;
    float GetYSort() override;
    
    bool CanEquip(int item_id) override;
    void Equip(int item_id) override;
    
    bool CanUnEquip(int item_id) override;
    void UnEquip(int item_id) override;
    
    void OnSpellTimerTimeout() override;
    void OnMeleTimerTimeout() override;
    void OnStunTimerTimeout() override;
    
    void CheckInput();
    
}; 

extern PlayerCharacter *g_current_player;
extern std::unordered_map< int, CharacterData> g_character_data;