#pragma once
#include "gamedefs.h"
#include "label.h"

/* class AreaSignal {
    public:
    void Connect(std::function<void( TransitionArea a )> const& callback);
    void EmitSignal(TransitionArea a);    

    std::vector<std::function<void( TransitionArea a )>> callbacks;
}; */

class BaseArea {
    public:
        virtual ~BaseArea() = default;
        virtual void Update() = 0;
        virtual void Draw() = 0;
        //virtual void Activate() = 0;

        std::string identifier;
        Vector2 position;
        Vector2 size;
        bool collided;
        bool hovered;
        bool in_range;

        float time_pressed;
        

        std::string payload_s;
        Vector2 payload_v;
        int payload_i;
        

        Signal mouse_entered;
        Signal entity_entered;


        Label label;
};


class TransitionArea : public BaseArea {

    public:
        TransitionArea(){};
        ~TransitionArea() override;
        void Update() override;
        void Draw() override;
        //void Activate() override;

        Signal area_entered;
        Signal area_activated;

        Label label;

};

class ContainerArea : public BaseArea {

    public:
        ContainerArea(){};
        ~ContainerArea() override;
        void Update() override;
        void Draw() override;
        //void Activate() override;

        Signal area_entered;
        Signal area_activated;
        Signal list_changed;
        Signal list_empty;

        bool is_empty;
        bool is_ground;
        int loot_table_id;
        std::vector<int> item_list;

        Label label;

};
