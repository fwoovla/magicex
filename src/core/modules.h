#pragma once 
#include "gamedefs.h"
#include "baseentity.h"

struct ModuleData {
    ModuleID module_id;
    std::string module_name;
    std::vector<int> recipies;
};

extern std::unordered_map<int, ModuleData> g_module_data;

class BaseModuleEntity : public SpriteEntity {
    public:

    ~BaseModuleEntity() = default;
    virtual void OnModuleUsed() = 0;
    

    std::string iid;
    int level_index;
    int loot_table_id;
    int module_id;
    bool is_open;
    
    ModuleArea m_area;

    Signal open_module;

    RayCast raycast;

};

class WorkbenchModuleEntity : public BaseModuleEntity {
    public:
    WorkbenchModuleEntity(Vector2 position, int _module_id);
    ~WorkbenchModuleEntity() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;
    void OnModuleUsed() override;
    float GetYSort() override;
};