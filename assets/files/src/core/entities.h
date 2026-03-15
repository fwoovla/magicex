#pragma once 
#include "gamedefs.h"
#include "baseentity.h"


class CharacterMessage : public BaseEntity {
    public:
    CharacterMessage(Vector2 _position, std::string _text, Color _color, float _delay_seconds);
    ~CharacterMessage() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;
    float GetYSort() override;
    void TakeDamage(DamagePayload _payload) override;
    
    Label label;
    Color color;
    float delay_seconds;
    float current_seconds;


};

class EnvironmentalEntity : public SpriteEntity {
    public:
    Sprite sprite;

    EnvironmentalEntity(Vector2 _position, int _sprite_id, bool _fadeable);
    ~EnvironmentalEntity() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;
    float GetYSort() override;
    void TakeDamage(DamagePayload _payload) override;

    Sprite shadow_sprite;
    bool fadeable = false;
};




class DoorEntity : public SpriteEntity {
    public:
    DoorEntity(Vector2 _position);
    ~DoorEntity();

    void Update() override;
    void Draw() override;
    void DrawUI() override;
    float GetYSort() override;
    void TakeDamage(DamagePayload _payload) override;
    
    void OnEntityActivated();
    bool IsLocked();

    std::string iid;
    int level_index;
    bool is_locked;
    bool is_open;
    
    DoorArea d_area;

    Signal open_door;

    RayCast raycast;

};



class BaseContainerEntity : public SpriteEntity {
    public:

    ~BaseContainerEntity() = default;
    virtual void OnEntityActivated() = 0;
    virtual bool IsEmpty() = 0;

    std::string iid;
    int level_index;
    int loot_table_id;
    int sprite_id;
    bool is_open;
    
    ContainerArea c_area;

    Signal open_container;

    RayCast raycast;

};


class PermContainerEntity : public BaseContainerEntity {
    public:
    PermContainerEntity(Vector2 _position, int _s_id, int _lt_id);
    ~PermContainerEntity() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;
    void OnEntityActivated() override;
    bool IsEmpty() override;
    float GetYSort() override;
    void TakeDamage(DamagePayload _payload) override;
};

class GroundContainerEntity : public BaseContainerEntity {
    public:
    GroundContainerEntity(Vector2 _position, int _s_id);
    ~GroundContainerEntity() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;
    void OnEntityActivated() override;
    bool IsEmpty() override;
    float GetYSort() override;
    void SetSprite(int sprite_id);
    void TakeDamage(DamagePayload _payload) override;

    void OnListChanged();
};

class MushroomEntity : public BaseContainerEntity {
    public:
    MushroomEntity(Vector2 _position);
    ~MushroomEntity() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;
    void OnEntityActivated() override;
    bool IsEmpty() override;
    float GetYSort() override;
    void TakeDamage(DamagePayload _payload) override;
};

