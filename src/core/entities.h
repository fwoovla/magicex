#pragma once 
#include "gamedefs.h"
#include "baseentity.h"


class BaseContainerEntity : public SpriteEntity {
    public:

    ~BaseContainerEntity() = default;
    virtual void OnContainerOpened() = 0;
    virtual bool IsEmpty() = 0;

    std::string iid;
    int level_index;
    int loot_table_id;
    int sprite_id;
    bool is_open;
    
    ContainerArea c_area;

    Signal open_container;

};


class PermContainerEntity : public BaseContainerEntity {
    public:
    PermContainerEntity(Vector2 _position, int _s_id, int _lt_id);
    ~PermContainerEntity() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;
    void OnContainerOpened() override;
    bool IsEmpty() override;
};

class GroundContainerEntity : public BaseContainerEntity {
    public:
    GroundContainerEntity(Vector2 _position, int _s_id);
    ~GroundContainerEntity() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;
    void OnContainerOpened() override;
    bool IsEmpty() override;
    void SetSprite(int sprite_id);

    void OnListChanged();
};

class MushroomEntity : public BaseContainerEntity {
    public:
    MushroomEntity(Vector2 _position);
    ~MushroomEntity() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;
    void OnContainerOpened() override;
    bool IsEmpty() override;
};