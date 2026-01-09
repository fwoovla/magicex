#pragma once

#include "gamedefs.h"

struct SpellData {
    std::string spell_name;
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