#include "../../core/gamedefs.h"

FadeTransition::FadeTransition(float _durration) {
    durration = _durration;

    alpha_value = 0;  
    alpha_step = 255/durration;
    phase = 0;
}


void FadeTransition::Update() {
    //TraceLog(LOG_INFO, "  TRANSITION update");
    alpha_value += (float)alpha_step * GetFrameTime();
    if(alpha_value > 255) {
        phase++;
        alpha_step = -alpha_step;
        transition_midpoint.EmitSignal();
    }
    if(phase == 1 and alpha_value < 1) {
        transition_ended.EmitSignal();
    }
}

void FadeTransition::Draw() {
    DrawRectangle( 0,0, g_game_settings.resolution.x, g_game_settings.resolution.y, {0, 0, 0, (unsigned char)alpha_value} );
}

FadeTransition::~FadeTransition() {
    TraceLog(LOG_INFO, "SCENE DESTRUCTOR:  TRANSITION");
}
