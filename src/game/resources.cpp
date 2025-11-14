#include "../core/gamedefs.h"


void LoadResources() {
    TraceLog(LOG_INFO, "LOADING UNIT SPRITES");

    g_sprite_sheets[SPRITE_APPRENTICE] = LoadTexture("assets/bandit.png");
    g_sprite_sheets[SPRITE_NERD] = LoadTexture("assets/hero.png");
    //g_sprite_sheets[SPRITE_CROSSHAIR] = LoadTexture("assets/crosshair.png");
    //g_sprite_sheets[SPRITE_WAND] = LoadTexture("assets/wand.png");



    g_sprite_sheets[SPRITE_MAIN] = LoadTexture("assets/main_spritesheet.png");
    TraceLog(LOG_INFO, "LOADING ITEM and ICON SPRITES");

    Image icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    Image item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    Vector2 icon_pos = {1,1};
    Vector2 item_pos = {10,1};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_DAGGER] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_DAGGER] = LoadTextureFromImage(item_image);

//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {1,2};
    item_pos = {10,2};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_SWORD] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_SWORD] = LoadTextureFromImage(item_image);

//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {1,3};
    item_pos = {10,3};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_SPEAR] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_SPEAR] = LoadTextureFromImage(item_image);
//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {1,4};
    item_pos = {10,4};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_AXE] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_AXE] = LoadTextureFromImage(item_image);
//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {1,5};
    item_pos = {10,5};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_BOW] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_BOW] = LoadTextureFromImage(item_image);
//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {1,6};
    item_pos = {10,6};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_WAND] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_WAND] = LoadTextureFromImage(item_image);
//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {5,1};
    item_pos = {15,1};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_LEATHERBOOTS] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_LEATHERBOOTS] = LoadTextureFromImage(item_image);
//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {5,2};
    item_pos = {15,2};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_LEATHERVEST] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_LEATHERVEST] = LoadTextureFromImage(item_image);
//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {5,3};
    item_pos = {15,3};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_LEATHERGLOVES] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_LEATHERGLOVES] = LoadTextureFromImage(item_image);
//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {5,5};
    item_pos = {15,5};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_WIZARDHAT] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_WIZARDHAT] = LoadTextureFromImage(item_image);
//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {1,10};
    item_pos = {10,10};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_SCROLL] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_SCROLL] = LoadTextureFromImage(item_image);
//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {5,10};
    item_pos = {15,10};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_APPLE] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_APPLE] = LoadTextureFromImage(item_image);
//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {5,11};
    item_pos = {15,11};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_CHEESE] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_CHEESE] = LoadTextureFromImage(item_image);
//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {5,12};
    item_pos = {15,12};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_BREAD] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_BREAD] = LoadTextureFromImage(item_image);
//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {5,13};
    item_pos = {15,13};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_MEAT] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_MEAT] = LoadTextureFromImage(item_image);
//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {1,15};
    item_pos = {10,15};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_RING] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_RING] = LoadTextureFromImage(item_image);


//containers
    TraceLog(LOG_INFO, "LOADING CONTAINER SPRITES");
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_pos = {1,20};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_container_sprites[SPRITE_CONTAINER_CHEST] = LoadTextureFromImage(item_image);

    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_pos = {2,20};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_container_sprites[SPRITE_CONTAINER_BARREL] = LoadTextureFromImage(item_image);

//UI sprites
    TraceLog(LOG_INFO, "LOADING UI SPRITES");
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_pos = {1,22};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 32, 32});
    g_ui_sprites[UI_ID_CROSSHAIR] = LoadTextureFromImage(item_image);

    //UnloadTexture()

    TraceLog(LOG_INFO, "LOADING PORTRAITS");
    g_portrait_sprites[PORTRAIT_BASE_CHAR] = LoadTexture("assets/portrait1.png");

    TraceLog(LOG_INFO, "LOADING UI PANELS");
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
    for(int i = 0; i < MAX_SPRITES; i++) {
        UnloadTexture(g_sprite_sheets[i]);
    }

    TraceLog(LOG_INFO, "UNLOADING ITEM SPRITES");
    for(int i = 0; i < MAX_SPRITES; i++) {
        UnloadTexture(g_item_sprites[i]);
    }

    TraceLog(LOG_INFO, "UNLOADING ICON SPRITES");
    for(int i = 0; i < MAX_SPRITES; i++) {
        UnloadTexture(g_icon_sprites[i]);
    }

    TraceLog(LOG_INFO, "UNLOADING CONTAINER SPRITES");
    for(int i = 0; i < MAX_SPRITES; i++) {
        UnloadTexture(g_container_sprites[i]);
    }

    TraceLog(LOG_INFO, "UNLOADING UI SPRITES");
    for(int i = 0; i < MAX_SPRITES; i++) {
        UnloadTexture(g_ui_sprites[i]);
    }

    TraceLog(LOG_INFO, "UNLOADING PORTRAITS");
    for(int i = 0; i < MAX_SPRITES; i++) {
        UnloadTexture(g_portrait_sprites[i]);
    }

    TraceLog(LOG_INFO, "UNLOADING UI");
    for(int i = 0; i < MAX_SPRITES; i++) {
        UnloadTexture(g_ui_panels[i]);
    }

    TraceLog(LOG_INFO, "UNLOADING UI BACKGROUNDS");
    for(int i = 0; i < MAX_SPRITES; i++) {
        UnloadTexture(g_ui_backgrounds[i]);
    }
}