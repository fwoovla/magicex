#pragma once
#include "gamedefs.h"

struct Area {
    public:
        ~Area() = default;

        Signal mouse_entered;
        Signal entity_entered;

};