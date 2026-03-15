#pragma once

#include "gamedefs.h"
//#include "loottables.h"

enum SPELL_TYPE {
    SPELL_TYPE_NONE = -1,
    SPELL_TYPE_BOLT,
    SPELL_TYPE_AOE,
    SPELL_TYPE_INSTANT,
    SPELL_TYPE_COUNT,

};

enum SPELL_EFFECT {
    SPELL_EFFECT_NONE = 0,
    SPELL_EFFECT_FORCE,
    SPELL_EFFECT_FIRE,
    SPELL_EFFECT_LIGHTNING,
    SPELL_EFFECT_POISON,
    SPELL_EFFECT_COUNT,

};


enum CASTER_PART {
    CASTER_PART_NONE = -1,
    CASTER_PART_BASE,
    CASTER_PART_IGNITER,
    CASTER_PART_COPLER,
    CASTER_PART_ROD
};


struct CasterBaseData {
    std::string name;
    ItemID id;
    float accuracy;
    ITEM_TIER tier;
};
extern std::vector<CasterBaseData> g_casterbase_data;


struct IgniterData {
    std::string name;
    ItemID id;
    SPELL_EFFECT effect;
    float recoil;
    float max_power;
    ITEM_TIER tier;
};
extern std::vector<IgniterData> g_igniter_data;


struct CouplerData {
    std::string name;
    ItemID id;
    SPELL_TYPE type;
    float radius;
    float cooldown;
    float pps;
    float damage;
    float speed;
    ITEM_TIER tier;
};
extern std::vector<CouplerData> g_coupler_data;


struct RodData {
    std::string name;
    ItemID id;
    float knockback;
    float damage;
    ITEM_TIER tier;
};
extern std::vector<RodData> g_rod_data;


struct CasterData {
    std::string caster_name;
    
    CasterBaseData base;
    IgniterData igniter;
    CouplerData coupler;
    RodData rod;
    ITEM_TIER tier;
    
    //int spell_sprite_id;
};

//extern std::vector<SpellData> g_spell_data;

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

    CasterData *data;
    Timer lifetime_timer;
    Vector2 velocity;
    Vector2 target_position;

};



class BoltSpell : public BaseSpell {
    public:

    BoltSpell(NewSpellPayload payload, CasterData *_data);
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

    AoeSpell(NewSpellPayload payload, CasterData *_data);
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

    InstantSpell(NewSpellPayload payload, CasterData *_data);
    ~InstantSpell() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;
    float GetYSort() override;
    void OnLifetimeTimeout() override;
    void TakeDamage(DamagePayload _payload) override;

};