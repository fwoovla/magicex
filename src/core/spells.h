#pragma once

#include "gamedefs.h"
//#include "loottables.h"




enum SPELL_DELIVERY {
    DELIVERY_INSTANT,
    DELIVERY_PROJECTILE
};

enum SPELL_EFFECT {
    SPELL_EFFECT_NONE = 0,
    SPELL_EFFECT_FORCE,
    SPELL_EFFECT_FIRE,
    SPELL_EFFECT_LIGHTNING,
    SPELL_EFFECT_POISON,
    SPELL_EFFECT_COUNT,
};



struct SpellBuildData {

    int charge_points;
    int damage_points;

    bool is_lingering;
    int duration_points;

    bool is_exploding;
    int radius_points;

    SPELL_EFFECT SPELL_EFFECT_NONE;

    SPELL_DELIVERY DELIVERY_PROJECTILE;
};

struct SpellData {

    float chargetime;
    float damage;
    
    bool is_lingering;
    float durration;
    float tick_rate;
    float tick_damage;
    
    bool is_exploding;
    float radius;

    float speed;
    float knockback;
    
    SPELL_DELIVERY delivery_type;
    SPELL_EFFECT effect_type;

};





enum WANDWOOD_TYPE {
    WANDWOOD_FRESH,
    WANDWOOD_AGED
};

struct WandWood {
    std::string name;
    WANDWOOD_TYPE type;
    int bonus_points;
};

extern std::vector<WandWood> g_wand_woods;

struct WandProfile {
    std::string name;

    int charge_bonus = 0;
    int damage_bonus = 0;
    int duration_bonus = 0;
    int radius_bonus = 0;
};

extern std::vector<WandProfile> g_wand_profiles;

struct WandData{

    WandWood wood;
    WandProfile profile;

    int slot_count;
    int active_spell_index = 0;
    std::vector<SpellData> spells;
};


struct SpellEffectData {
    SPELL_EFFECT id;
    Color color;

};

struct SpellRules {

    std::vector<SpellEffectData> effects;
    std::vector<int> level_points;
    float chargetime_base;
    //float chargetime_min;
    float duration_base;
    float radius_base;
    float damage_base;
    float damage_per_point;
    float charge_per_point;
    float radius_per_point;
    float duration_per_point;
    int charge_cost;
    int damage_cost;
    int duration_cost;
    int radius_cost;

};


extern SpellRules g_spell_rules;






WANDWOOD_TYPE StrToWandWoodType(const std::string &s);
SPELL_EFFECT StrToSpellEffectId(const std::string& s);

void LoadSpellData(json &j);
void LoadWandData(json &j);
//SPELL_TYPE StrToSpellTypeId(const std::string& s);





enum SPELL_TYPE {
    SPELL_TYPE_NONE = -1,
    SPELL_TYPE_BOLT,
    SPELL_TYPE_AOE,
    SPELL_TYPE_INSTANT,
    SPELL_TYPE_COUNT,

};


/* 

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
 */
//extern std::vector<SpellData> g_spell_data;

struct NewSpellPayload {
    Vector2 position;
    Vector2 target_position;
    float rotation;
    int shooter_id;
    int spread;
    SPELL_DELIVERY spell_type;
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