#include "../../core/gamedefs.h"

FadeTransition::FadeTransition(float _durration) {
    TraceLog(LOG_INFO, "  TRANSITION start");
    durration = _durration;

    alpha_value = 0;  
    alpha_step = 255/durration;
    phase = 0;

}


void FadeTransition::Update() {
    //TraceLog(LOG_INFO, "  TRANSITION update");
    alpha_value += (float)alpha_step * GetFrameTime();
    //TraceLog(LOG_INFO, "  TRANSITION alpha value %f    %f", alpha_value, alpha_step * GetFrameTime());
    if(alpha_value > 255) {
        phase++;
        alpha_step = -alpha_step;
        //TraceLog(LOG_INFO, "  TRANSITION mid");
        transition_midpoint.EmitSignal();
    }
    if(phase == 1 and alpha_value < 1) {
        //TraceLog(LOG_INFO, "  TRANSITION end");
        transition_ended.EmitSignal();
    }

}

void FadeTransition::Draw() {
    //DrawRectangle( 0,0, g_resolution.x, g_resolution.y, DARKERGRAY ); 
    //TraceLog(LOG_INFO, "  TRANSITION draw");
    //DrawLabelCentered(title_label);
    DrawRectangle( 0,0, g_game_settings.resolution.x, g_game_settings.resolution.y, {0, 0, 0, (unsigned char)alpha_value} );
}

FadeTransition::~FadeTransition() {
    TraceLog(LOG_INFO, "SCENE DESTRUCTOR:  TRANSITION");
}
