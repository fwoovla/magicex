#include "../core/gamedefs.h"


void LoadResources() {
    TraceLog(LOG_INFO, "LOADING UNIT SPRITES");

    g_sprite_sheets[SPRITE_APPRENTICE] = LoadTexture("assets/bandit.png");
    g_sprite_sheets[SPRITE_NERD] = LoadTexture("assets/hero.png");
    g_sprite_sheets[SPRITE_CROSSHAIR] = LoadTexture("assets/crosshair.png");
    g_sprite_sheets[SPRITE_WAND] = LoadTexture("assets/wand.png");

    TraceLog(LOG_INFO, "LOADING PORTRAITS");
    g_portrait_sprites[PORTRAIT_BASE_CHAR] = LoadTexture("assets/portrait1.png");

    TraceLog(LOG_INFO, "LOADING UI");
    g_ui_panels[PANEL_GRAY] = LoadTexture("assets/graypanel1.png");
    g_ui_panels[PANEL_WOOD_LARGE] = LoadTexture("assets/largepanel1.png");

    TraceLog(LOG_INFO, "LOADING UI BACKGROUNDS");
    g_ui_backgrounds[BG_STAGING] = LoadTexture("assets/staging_bg2.png");
    g_ui_backgrounds[BG_SHELTER] = LoadTexture("assets/staging_bg1.png");
    g_ui_backgrounds[BG_HOUSE_TRANS] = LoadTexture("assets/house_trans.png");
    g_ui_backgrounds[BG_OUTSIDE_TRANS] = LoadTexture("assets/outside_trans.png");

/*     TraceLog(LOG_INFO, "LOADING SPRITESHEET");
    g_tile_sheets[TS_FOREST] = LoadTexture("assets/maps/ldtk/tilesheet.png"); */

/*     TraceLog(LOG_INFO, "LOADING LDTK TILESHEETS");
    g_ldtk_tilesheets[TS_FOREST] = LoadTexture("assets/maps/forrest_spritesheet.png");
    g_ldtk_tilesheets[TS_SHELTER] = LoadTexture("assets/maps/shelter_spritesheet.png"); */


}

void UnloadResources() {
    
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