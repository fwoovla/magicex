#pragma once

#include "gamedefs.h"

struct SpellData {
    int level;
    SpellID spell_id;
    int shooter_id;
    float damage;
    float lifetime;
    float radius;
    float speed;
    std::string spell_name;
};

extern std::unordered_map<int, SpellData> g_spell_data;

struct NewSpellPayload {
    Vector2 position;
    float rotation;
    int shooter_id;
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

    SpellData data;
    Timer lifetime_timer;
    Vector2 velocity;
    Vector2 target_position;

};

class MagicMissle : public BaseSpell {
    public:

    MagicMissle(Vector2 _position, int _shooter_id, SpellData _data);
    ~MagicMissle() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;
    float GetYSort() override;
    void OnLifetimeTimeout() override;

};

class FireBall : public BaseSpell {
    public:

    FireBall(Vector2 _position, int _shooter_id, SpellData _data);
    ~FireBall() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;
    float GetYSort() override;
    void OnLifetimeTimeout() override;

};

class Lightning : public BaseSpell {
    public:

    Lightning(Vector2 _position, int _shooter_id, SpellData _data);
    ~Lightning() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;
    float GetYSort() override;
    void OnLifetimeTimeout() override;

};