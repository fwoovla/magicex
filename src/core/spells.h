#pragma once

#include "gamedefs.h"



enum SpellID {
    SPELL_ID_NONE = -1,
    SPELL_ID_FORCE_1,
    SPELL_ID_FORCE_2,
    SPELL_ID_FORCE_3,
    SPELL_ID_FORCE_4,

    SPELL_ID_FIRE_1,
    SPELL_ID_FIRE_2,
    SPELL_ID_FIRE_3,
    SPELL_ID_FIRE_4,

    SPELL_ID_LIGHTNING_1,
    SPELL_ID_LIGHTNING_2,
    SPELL_ID_LIGHTNING_3,
    SPELL_ID_LIGHTNING_4,

    SPELL_ID_POISON_1,
    SPELL_ID_POISON_2,
    SPELL_ID_POISON_3,
    SPELL_ID_POISON_4,

};


enum SPELL_TYPE {
    SPELL_TYPE_NONE = -1,
    SPELL_TYPE_BOLT,
    SPELL_TYPE_AOE,
    SPELL_TYPE_INSTANT,

};

enum SPELL_EFFECT {
    SPELL_EFFECT_NONE = -1,
    SPELL_EFFECT_FORCE,
    SPELL_EFFECT_FIRE,
    SPELL_EFFECT_LIGHTNING,
    SPELL_EFFECT_POISON,

};

struct SpellData {
    std::string spell_name;
    SPELL_EFFECT spell_effect;
    SpellID spell_id;
    int shooter_id;
    float damage;
    float lifetime;
    float radius;
    float speed;
    //float pps;
    float knockback;
};

extern std::vector<SpellData> g_spell_data;

struct NewSpellPayload {
    Vector2 position;
    Vector2 target_position;
    float rotation;
    int shooter_id;
    int spread;
    SPELL_TYPE spell_type;
    //SpellData *spell_data;
};








class BaseSpell : public AnimatedSpriteEntity {

    public:
    virtual ~BaseSpell() = default;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void DrawUI() = 0;
    virtual void OnLifetimeTimeout() = 0;

    int shooter_id;
    float target_dist;
    float target_rotation;
    float dist_scale;

    SpellData *data;
    Timer lifetime_timer;
    Vector2 velocity;
    Vector2 target_position;

};

class Poison : public BaseSpell {
    public:

    Poison(NewSpellPayload payload, SpellData *_data);
    ~Poison() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;
    float GetYSort() override;
    void OnLifetimeTimeout() override;
    void TakeDamage(DamagePayload _payload) override;

};

class MagicMissle : public BaseSpell {
    public:

    MagicMissle(NewSpellPayload payload, SpellData *_data);
    ~MagicMissle() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;
    float GetYSort() override;
    void OnLifetimeTimeout() override;
    void TakeDamage(DamagePayload _payload) override;

};

class FireBall : public BaseSpell {
    public:

    FireBall(NewSpellPayload payload, SpellData *_data);
    ~FireBall() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;
    float GetYSort() override;
    void OnLifetimeTimeout() override;
    void TakeDamage(DamagePayload _payload) override;

};

class Lightning : public BaseSpell {
    public:

    Lightning(NewSpellPayload payload, SpellData *_data);
    ~Lightning() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;
    float GetYSort() override;
    void OnLifetimeTimeout() override;
    void TakeDamage(DamagePayload _payload) override;

};






class BoltSpell : public BaseSpell {
    public:

    BoltSpell(NewSpellPayload payload, SpellData *_data);
    ~BoltSpell() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;
    float GetYSort() override;
    void OnLifetimeTimeout() override;
    void TakeDamage(DamagePayload _payload) override;

};

class AoeSpell : public BaseSpell {
    public:

    AoeSpell(NewSpellPayload payload, SpellData *_data);
    ~AoeSpell() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;
    float GetYSort() override;
    void OnLifetimeTimeout() override;
    void TakeDamage(DamagePayload _payload) override;

};

class InstantSpell : public BaseSpell {
    public:

    InstantSpell(NewSpellPayload payload, SpellData *_data);
    ~InstantSpell() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;
    float GetYSort() override;
    void OnLifetimeTimeout() override;
    void TakeDamage(DamagePayload _payload) override;

};