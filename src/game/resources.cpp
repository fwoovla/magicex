#include "../core/gamedefs.h"


void LoadResources() {
    TraceLog(LOG_INFO, "LOADING UNIT SPRITES");

    g_sprite_sheets[SPRITE_APPRENTICE] = LoadTexture("assets/bandit.png");
    g_sprite_sheets[SPRITE_NERD] = LoadTexture("assets/hero.png");
    g_sprite_sheets[SPRITE_CROSSHAIR] = LoadTexture("assets/crosshair.png");
    g_sprite_sheets[SPRITE_WAND] = LoadTexture("assets/wand.png");

    g_sprite_sheets[SPRITE_ITEMS] = LoadTexture("assets/spritesheet.png");

    TraceLog(LOG_INFO, "LOADING IMAGE SPRITES");
    Image image = LoadImageFromTexture(g_sprite_sheets[SPRITE_ITEMS]);
    Vector2 index_pos = {0,0};
    ImageCrop(&image, {index_pos.x * 16, index_pos.y * 16, 16, 16});
    g_item_sprites[SPRITE_ITEM_BALL] = LoadTextureFromImage(image);

    image = LoadImageFromTexture(g_sprite_sheets[SPRITE_ITEMS]);
    index_pos = {1,0};
    ImageCrop(&image, {index_pos.x * 16, index_pos.y * 16, 16, 16});
    g_item_sprites[SPRITE_ITEM_CABBAGE] = LoadTextureFromImage(image);

    TraceLog(LOG_INFO, "LOADING CONTAINER SPRITES");
    image = LoadImageFromTexture(g_sprite_sheets[SPRITE_ITEMS]);
    index_pos = {2,0};
    ImageCrop(&image, {index_pos.x * 16, index_pos.y * 16, 16, 16});
    g_container_sprites[SPRITE_CONTAINER_CHEST] = LoadTextureFromImage(image);

    image = LoadImageFromTexture(g_sprite_sheets[SPRITE_ITEMS]);
    index_pos = {3,0};
    ImageCrop(&image, {index_pos.x * 16, index_pos.y * 16, 16, 16});
    g_container_sprites[SPRITE_CONTAINER_BARREL] = LoadTextureFromImage(image);

    //UnloadTexture()

    TraceLog(LOG_INFO, "LOADING PORTRAITS");
    g_portrait_sprites[PORTRAIT_BASE_CHAR] = LoadTexture("assets/portrait1.png");

    TraceLog(LOG_INFO, "LOADING UI");
    g_ui_panels[PANEL_GRAY] = LoadTexture("assets/graypanel1.png");
    g_ui_panels[PANEL_WOOD_LARGE] = LoadTexture("assets/largepanel1.png");
    g_ui_panels[PANEL_CHAR_SCREEN] = LoadTexture("assets/characterpanelbg.png");

    TraceLog(LOG_INFO, "LOADING UI BACKGROUNDS");
    g_ui_backgrounds[BG_STAGING] = LoadTexture("assets/staging_bg2.png");
    g_ui_backgrounds[BG_SHELTER] = LoadTexture("assets/staging_bg1.png");
    g_ui_backgrounds[BG_HOUSE_TRANS] = LoadTexture("assets/house_trans.png");
    g_ui_backgrounds[BG_OUTSIDE_TRANS] = LoadTexture("assets/outside_trans.png");

}

void UnloadResources() {
    TraceLog(LOG_INFO, "UNLOADING SPRITE SHEETS");
    for(int i = 0; i < MAX_SPRITE_SHEETS; i++) {
        UnloadTexture(g_sprite_sheets[i]);
    }

    TraceLog(LOG_INFO, "UNLOADING ITEM SPRITES");
    for(int i = 0; i < MAX_SPRITE_SHEETS; i++) {
        UnloadTexture(g_item_sprites[i]);
    }

    TraceLog(LOG_INFO, "UNLOADING CONTAINER SPRITES");
    for(int i = 0; i < MAX_SPRITE_SHEETS; i++) {
        UnloadTexture(g_container_sprites[i]);
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