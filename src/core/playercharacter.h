#pragma once 
#include "gamedefs.h"

enum ActionState {
    ACTION_IDLE,
    ACTION_CHARGE,
    ACTION_RETREAT,
};

enum DetectState {
    DETECT_NONE,
    DETECT_PLAYER
};

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
    virtual void OnHungerTimerTimeout() = 0;

    //int uid;
    Vector2 velocity;
    Sprite weapon_sprite;
    Sprite shadow_sprite;
    Vector2 aim_position;
    bool can_use_spell;
    bool can_mele;
    bool can_switch;
    bool can_reload;
    bool is_stunned;
    int stunned_frames;
    float weapon_end_rotation;
    int swing_dir;
    Timer spell_timer;
    Timer mele_timer;
    Timer stun_timer;
    Timer hunger_timer;
    float hunger_rate;

    ItemInstanceData *current_primary_data;
    CharacterData *data;

    AnimationState animation_state;
    WeaponState weapon_state;
    ActionState action_state;
    DetectState detect_state;

    int obstructed_count;


    RayCast raycast;
}; 



class TestDummyEntity : public CharacterEntity {
    public:
    TestDummyEntity(Vector2 _position, int uid);
    ~TestDummyEntity() override;
    void TakeDamage(DamagePayload _payload) override;
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
    void OnHungerTimerTimeout() override;
};


class CreatureEntity : public CharacterEntity {
    public:
    CreatureEntity(Vector2 _position, int uid);
    ~CreatureEntity() override;
    void TakeDamage(DamagePayload _payload) override;
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
    void OnHungerTimerTimeout() override;
    void OnDetectTimerTimeout();
    void OnActionTimerTimeout();

    Vector2 GetDirToPlayer();
    void MoveCreature(Vector2 _input_dir);

    float detect_range;
    float attack_range;
    
    CharacterEntity *target_creature;
    std::vector<CharacterEntity *> detected_creatures;
    std::vector<Vector2> target_path;
    int path_index;

    Timer detect_timer;
    Timer action_timer;

    bool should_use_spell;
    bool should_mele;

};


class PlayerCharacter : public CharacterEntity {
    public:
    PlayerCharacter(Vector2 _position, int uid);
    ~PlayerCharacter() override;
    void TakeDamage(DamagePayload _payload) override;
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
    void OnHungerTimerTimeout() override;
    
    void CheckInput();

    Sprite aim_sprite;
    
}; 

extern PlayerCharacter *g_current_player;
extern std::unordered_map< int, CharacterData> g_character_data;