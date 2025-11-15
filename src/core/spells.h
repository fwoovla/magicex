#pragma once

#include "gamedefs.h"

struct SpellData {
    int level;
    SpellID spell_id;
    int shooter_id;
    float damage;
    float lifetime;
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

    SpellData data;
    Timer lifetime_timer;
    Vector2 velocity;
    int shooter_id;

};

class MagicMissle : public BaseSpell {
    public:

    MagicMissle(Vector2 _position, float _rotation, int _shooter_id, SpellData _data);
    ~MagicMissle() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;
    void OnLifetimeTimeout() override;

};

class FireBall : public BaseSpell {
    public:

    FireBall(Vector2 _position, float _rotation, int _shooter_id, SpellData _data);
    ~FireBall() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;
    void OnLifetimeTimeout() override;

};

class Lightning : public BaseSpell {
    public:

    Lightning(Vector2 _position, float _rotation, int _shooter_id, SpellData _data);
    ~Lightning() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;
    void OnLifetimeTimeout() override;

};