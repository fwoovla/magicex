#pragma once 
#include "gamedefs.h"
#include "baseentity.h"


class ContainerEntity : public SpriteEntity {
    public:
    ContainerEntity(Vector2 _position, int _s_id, int _lt_id);
    ~ContainerEntity() override;
    void Update() override;
    void Draw() override;

    void OnContainerOpened();

    int loot_table_id;
    int sprite_id;
    bool is_open;
    
    ContainerArea c_area;

    Signal open_container;

};