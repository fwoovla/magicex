#pragma once

#include "gamedefs.h"

#define MAX_SPRITES 1000



enum SpriteSheetID {
    SPRITE_APPRENTICE,
    SPRITE_NERD,
    SPRITE_CROSSHAIR,
    SPRITE_WAND,
    SPRITE_MAIN,
    SPRITE_NUM_SPRITES
};

extern Texture2D g_sprite_sheets[MAX_SPRITES];

enum UISpriteID {
    UI_ID_CROSSHAIR,
};

extern Texture2D g_ui_sprites[MAX_SPRITES];

enum ItemID {
    ITEM_ID_NONE = -1,
//0
    ITEM_ID_DAGGER,         
    ITEM_ID_SWORD,
    ITEM_ID_SPEAR,
    ITEM_ID_AXE,
    ITEM_ID_BOW,
//5
    ITEM_ID_WAND,           
    ITEM_ID_LEATHERBOOTS,
    ITEM_ID_LEATHERVEST,
    ITEM_ID_LEATHERGLOVES,
    ITEM_ID_WIZARDHAT,
//10
    ITEM_ID_SCROLL,         
    ITEM_ID_APPLE,
    ITEM_ID_CHEESE,
    ITEM_ID_BREAD,
    ITEM_ID_MEAT,
//15
    ITEM_ID_RING,
    ITEM_ID_MAX
};

extern Texture2D g_item_sprites[MAX_SPRITES];
extern Texture2D g_icon_sprites[MAX_SPRITES];


enum ContainerSprites {
    SPRITE_CONTAINER_CHEST,
    SPRITE_CONTAINER_BARREL,
    SPRITE_CONTAINER_NUM_SPRITES
};

extern Texture2D g_container_sprites[MAX_SPRITES];

enum PortraitSpriteID {
    PORTRAIT_BASE_CHAR,
    PORTRAIT_NUM_SPRITES
};

extern Texture2D g_portrait_sprites[MAX_SPRITES];

enum UIPanelID {
    PANEL_GRAY,
    PANEL_WOOD_LARGE,
    PANEL_NUM_SPRITES,
    PANEL_CHAR_SCREEN
};

extern Texture2D g_ui_panels[MAX_SPRITES];

enum UIBackgroundID {
    BG_SPLASH,
    BG_TITLE,
    BG_STAGING,
    BG_SHELTER,
    BG_HOUSE_TRANS,
    BG_OUTSIDE_TRANS,
    BG_NUM_SPRITES
};

extern Texture2D g_ui_backgrounds[MAX_SPRITES];


enum TileSheetID {
    TS_FOREST,
    TS_SHELTER,
    TS_NUM_SHEETS
};

extern Texture2D g_tile_sheets[MAX_SPRITES];

void LoadResources();


void UnloadResources();
