#pragma once

#include "gamedefs.h"

#define MAX_SPRITE_SHEETS 10
#define MAX_PORTRAIT_SPRITES 10
#define MAX_UI_PANELS 10
#define MAX_UI_BACKGROUNDS 10
#define MAX_TILE_SHEETS 10


enum UnitSpriteID {
    SPRITE_APPRENTICE,
    SPRITE_NERD,
    SPRITE_NUM_SPRITES
};
extern Texture2D g_sprite_sheets[MAX_SPRITE_SHEETS];

enum PortraitSpriteID {
    PORTRAIT_BASE_CHAR,
    PORTRAIT_NUM_SPRITES
};

extern Texture2D g_portrait_sprites[MAX_PORTRAIT_SPRITES];

enum UIPanelID {
    PANEL_GRAY,
    PANEL_WOOD_LARGE,
    PANEL_NUM_SPRITES
};

extern Texture2D g_ui_panels[MAX_UI_PANELS];

enum UIBackgroundID {
    BG_SPLASH,
    BG_TITLE,
    BG_STAGING,
    BG_SHELTER,
    BG_NUM_SPRITES
};

extern Texture2D g_ui_backgrounds[MAX_UI_BACKGROUNDS];


enum TileSheetID {
    TS_FOREST,
    TS_SHELTER,
    TS_NUM_SHEETS
};

extern Texture2D g_tile_sheets[MAX_TILE_SHEETS];

inline void LoadResources() {
    TraceLog(LOG_INFO, "LOADING UNIT SPRITES");

    g_sprite_sheets[SPRITE_APPRENTICE] = LoadTexture("assets/bandit.png");
    g_sprite_sheets[SPRITE_NERD] = LoadTexture("assets/hero.png");

    TraceLog(LOG_INFO, "LOADING PORTRAITS");
    g_portrait_sprites[PORTRAIT_BASE_CHAR] = LoadTexture("assets/portrait1.png");

    TraceLog(LOG_INFO, "LOADING UI");
    g_ui_panels[PANEL_GRAY] = LoadTexture("assets/graypanel1.png");
    g_ui_panels[PANEL_WOOD_LARGE] = LoadTexture("assets/largepanel1.png");

    TraceLog(LOG_INFO, "LOADING UI BACKGROUNDS");
    g_ui_backgrounds[BG_STAGING] = LoadTexture("assets/staging_bg2.png");
    g_ui_backgrounds[BG_SHELTER] = LoadTexture("assets/staging_bg1.png");

    TraceLog(LOG_INFO, "LOADING TILESHEETS");
    g_tile_sheets[TS_FOREST] = LoadTexture("assets/maps/forrest_spritesheet.png");
    g_tile_sheets[TS_SHELTER] = LoadTexture("assets/maps/shelter_spritesheet.png");

/*     TraceLog(LOG_INFO, "LOADING LDTK TILESHEETS");
    g_ldtk_tilesheets[TS_FOREST] = LoadTexture("assets/maps/forrest_spritesheet.png");
    g_ldtk_tilesheets[TS_SHELTER] = LoadTexture("assets/maps/shelter_spritesheet.png"); */


}

inline void UnloadResources() {
    
    TraceLog(LOG_INFO, "UNLOADING SPRITE SHEETS");
    for(int i = 0; i < MAX_SPRITE_SHEETS; i++) {
        UnloadTexture(g_sprite_sheets[i]);
    }
    TraceLog(LOG_INFO, "UNLOADING PORTRAITS");
    for(int i = 0; i < MAX_PORTRAIT_SPRITES; i++) {
        UnloadTexture(g_portrait_sprites[i]);
    }

    TraceLog(LOG_INFO, "UNLOADING UI");
    for(int i = 0; i < MAX_UI_PANELS; i++) {
        UnloadTexture(g_ui_panels[i]);
    }

    TraceLog(LOG_INFO, "UNLOADING UI BACKGROUNDS");
    for(int i = 0; i < MAX_UI_BACKGROUNDS; i++) {
        UnloadTexture(g_ui_backgrounds[i]);
    }
}