#pragma once

#include "gamedefs.h"

#define MAX_UNIT_SPRITES 10
#define MAX_PORTRAIT_SPRITES 10

enum UnitSpriteID {
    SPRITE_BASE_CHAR,
    SPRITE_NUM_SPRITES
};
extern Texture2D g_unit_sprites[MAX_UNIT_SPRITES];

enum PortraitSpriteID {
    PORTRAIT_BASE_CHAR,
    PORTRAIT_NUM_SPRITES
};
extern Texture2D g_portrait_sprites[MAX_PORTRAIT_SPRITES];

inline void LoadResources() {
    TraceLog(LOG_INFO, "LOADING UNIT SPRITES");

    g_unit_sprites[SPRITE_BASE_CHAR] = LoadTexture("assets/bandit.png");

    TraceLog(LOG_INFO, "LOADING PORTRAITS");
    g_portrait_sprites[SPRITE_BASE_CHAR] = LoadTexture("assets/portrait1.png");

}

inline void UnloadResources() {
    TraceLog(LOG_INFO, "UNLOADING UNIT SPRITES");
    for(int i = 0; i < MAX_UNIT_SPRITES; i++) {
        UnloadTexture(g_unit_sprites[i]);
    }
    TraceLog(LOG_INFO, "UNLOADING PORTRAITS");
    for(int i = 0; i < MAX_PORTRAIT_SPRITES; i++) {
        UnloadTexture(g_portrait_sprites[i]);
    }
}