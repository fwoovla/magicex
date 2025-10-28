#pragma once
#include "gamedefs.h"

struct Area {
    public:
        ~Area() = default;

        Vector2 position;
        int size;

        Signal mouse_entered;
        Signal entity_entered;

};

extern std::vector<Area> g_game_areas;


void UpdateGameAreas();
void DrawGameAreas(Color color);