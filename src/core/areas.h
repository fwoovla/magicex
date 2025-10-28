#pragma once
#include "gamedefs.h"

struct Area {
    public:
        ~Area() = default;

        std::string identifier;
        Vector2 position;
        int size;

        std::string payload_s;
        

        Signal mouse_entered;
        Signal entity_entered;

};

extern std::vector<Area> g_game_areas;


void UpdateGameAreas();
void DrawGameAreas(Color color);