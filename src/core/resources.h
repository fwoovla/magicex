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
    SPRITE_CROSSHAIR,
    SPRITE_WAND,
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

void LoadResources();


void UnloadResources();
