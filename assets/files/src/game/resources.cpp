#include "../core/gamedefs.h"


void LoadResources() {
    
    TraceLog(LOG_INFO, "LOADING UNIT SPRITES");
    
    g_main_sprite_sheet = LoadTexture("assets/main_spritesheet.png");

    Image sheet_image = LoadImageFromTexture(g_main_sprite_sheet);
    Vector2 sheet_pos = {20,1};
    ImageCrop(&sheet_image, {sheet_pos.x * 16, sheet_pos.y * 16, 16*4, 16*4});
    g_character_sprite_sheets[SPRITE_NERD] = LoadTextureFromImage(sheet_image);
    UnloadImage(sheet_image);
    TraceLog(LOG_INFO, "SHEETS   SPRITE_NERD");

    sheet_image = LoadImageFromTexture(g_main_sprite_sheet);
    sheet_pos = {20,6};
    ImageCrop(&sheet_image, {sheet_pos.x * 16, sheet_pos.y * 16, 16*4, 16*4});
    g_character_sprite_sheets[SPRITE_APPRENTICE] = LoadTextureFromImage(sheet_image);
    UnloadImage(sheet_image);
    TraceLog(LOG_INFO, "SHEETS   SPRITE_APPRENTICE");

    sheet_image = LoadImageFromTexture(g_main_sprite_sheet);
    sheet_pos = {25,6};
    ImageCrop(&sheet_image, {sheet_pos.x * 16, sheet_pos.y * 16, 16*4, 16*4});
    g_creature_sprite_sheets[SPRITE_TESTDUMMY] = LoadTextureFromImage(sheet_image);
    UnloadImage(sheet_image);
    TraceLog(LOG_INFO, "SHEETS   SPRITE_TESTDUMMY");
    
    sheet_image = LoadImageFromTexture(g_main_sprite_sheet);
    sheet_pos = {25,11};
    ImageCrop(&sheet_image, {sheet_pos.x * 16, sheet_pos.y * 16, 16*4, 16*4});
    g_creature_sprite_sheets[SPRITE_BUNNY] = LoadTextureFromImage(sheet_image);
    UnloadImage(sheet_image);
    TraceLog(LOG_INFO, "SHEETS   SPRITE_BUNNY");

    sheet_image = LoadImageFromTexture(g_main_sprite_sheet);
    sheet_pos = {30,6};
    ImageCrop(&sheet_image, {sheet_pos.x * 16, sheet_pos.y * 16, 16*4, 16*4});
    g_creature_sprite_sheets[SPRITE_SCAVENGER] = LoadTextureFromImage(sheet_image);
    UnloadImage(sheet_image);
    TraceLog(LOG_INFO, "SHEETS   SPRITE_SCAVENGER");

    sheet_image = LoadImageFromTexture(g_main_sprite_sheet);
    sheet_pos = {30,11};
    ImageCrop(&sheet_image, {sheet_pos.x * 16, sheet_pos.y * 16, 16*4, 16*4});
    g_creature_sprite_sheets[SPRITE_TRADER] = LoadTextureFromImage(sheet_image);
    UnloadImage(sheet_image);
    TraceLog(LOG_INFO, "SHEETS   SPRITE_TRADER");

    sheet_image = LoadImageFromTexture(g_main_sprite_sheet);
    sheet_pos = {35,6};
    ImageCrop(&sheet_image, {sheet_pos.x * 16, sheet_pos.y * 16, 16*4, 16*4});
    g_creature_sprite_sheets[SPRITE_SLIME] = LoadTextureFromImage(sheet_image);
    UnloadImage(sheet_image);
    TraceLog(LOG_INFO, "SHEETS   SPRITE_SLIME");




//---------------------------------------------------------------------------------------

    
    Image icon_image;
    Image item_image;
    Image item_part_image;
    Image icon_part_image;

    Vector2 icon_pos;
    Vector2 item_pos;
    Vector2 item_part_pos;
    Vector2 icon_part_pos;

    TraceLog(LOG_INFO, "LOADING SPELL SPRITES");

    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_pos = {20,15};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_spell_sprites[SPELL_TYPE_BOLT] = LoadTextureFromImage(item_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPELL_TYPE_BOLT loaded");

    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_pos = {20,16};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_spell_sprites[SPELL_TYPE_AOE] = LoadTextureFromImage(item_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPELL_TYPE_AOE loaded");

    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_pos = {20,17};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_spell_sprites[SPELL_TYPE_INSTANT] = LoadTextureFromImage(item_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPELL_TYPE_INSTANT loaded");



    TraceLog(LOG_INFO, "LOADING ITEM and ICON SPRITES");

    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {1,1};
    item_pos = {10,1};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_DAGGER] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_DAGGER] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_DAGGER");

//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {2,1};
    item_pos = {11,1};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_SWORD] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_SWORD] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_SWORD");

//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {3,1};
    item_pos = {12,1};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_SPEAR] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_SPEAR] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_SPEAR");

//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {4,1};
    item_pos = {13,1};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_AXE] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_AXE] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_AXE");

//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {5,1};
    item_pos = {14,1};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_BOW] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_BOW] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_BOW");







//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_part_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_part_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {10,5};
    item_pos = {10,5};
    item_part_pos = {5,5};
    icon_part_pos = {1,5};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    ImageCrop(&item_part_image, {item_part_pos.x * 16, item_part_pos.y * 16, 16, 16});
    ImageCrop(&icon_part_image, {icon_part_pos.x * 16, icon_part_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_CASTERBASE] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_CASTERBASE] = LoadTextureFromImage(item_image);
    g_item_part_sprites[ITEM_ID_CASTERBASE] = LoadTextureFromImage(item_part_image);
    g_icon_part_sprites[ITEM_ID_CASTERBASE] = LoadTextureFromImage(icon_part_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    UnloadImage(item_part_image);
    UnloadImage(icon_part_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_CASTERBASE");
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_part_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_part_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {11,5};
    item_pos = {11,5};
    item_part_pos = {6,5};
    icon_part_pos = {2,5};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    ImageCrop(&item_part_image, {item_part_pos.x * 16, item_part_pos.y * 16, 16, 16});
    ImageCrop(&icon_part_image, {icon_part_pos.x * 16, icon_part_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_IGNITER_1] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_IGNITER_1] = LoadTextureFromImage(item_image);
    g_item_part_sprites[ITEM_ID_IGNITER_1] = LoadTextureFromImage(item_part_image);
    g_icon_part_sprites[ITEM_ID_IGNITER_1] = LoadTextureFromImage(icon_part_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    UnloadImage(item_part_image);
    UnloadImage(icon_part_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_IGNITER_1");


    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_part_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_part_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {11,6};
    item_pos = {11,6};
    item_part_pos = {6,6};
    icon_part_pos = {2,6};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    ImageCrop(&item_part_image, {item_part_pos.x * 16, item_part_pos.y * 16, 16, 16});
    ImageCrop(&icon_part_image, {icon_part_pos.x * 16, icon_part_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_IGNITER_2] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_IGNITER_2] = LoadTextureFromImage(item_image);
    g_item_part_sprites[ITEM_ID_IGNITER_2] = LoadTextureFromImage(item_part_image);
    g_icon_part_sprites[ITEM_ID_IGNITER_2] = LoadTextureFromImage(icon_part_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    UnloadImage(item_part_image);
    UnloadImage(icon_part_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_IGNITER_2");


    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_part_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_part_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {11,7};
    item_pos = {11,7};
    item_part_pos = {6,7};
    icon_part_pos = {2,7};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    ImageCrop(&item_part_image, {item_part_pos.x * 16, item_part_pos.y * 16, 16, 16});
    ImageCrop(&icon_part_image, {icon_part_pos.x * 16, icon_part_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_IGNITER_3] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_IGNITER_3] = LoadTextureFromImage(item_image);
    g_item_part_sprites[ITEM_ID_IGNITER_3] = LoadTextureFromImage(item_part_image);
    g_icon_part_sprites[ITEM_ID_IGNITER_3] = LoadTextureFromImage(icon_part_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    UnloadImage(item_part_image);
    UnloadImage(icon_part_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_IGNITER_3");

    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_part_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_part_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {11,8};
    item_pos = {11,8};
    item_part_pos = {6,8};
    icon_part_pos = {2,8};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    ImageCrop(&item_part_image, {item_part_pos.x * 16, item_part_pos.y * 16, 16, 16});
    ImageCrop(&icon_part_image, {icon_part_pos.x * 16, icon_part_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_IGNITER_4] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_IGNITER_4] = LoadTextureFromImage(item_image);
    g_item_part_sprites[ITEM_ID_IGNITER_4] = LoadTextureFromImage(item_part_image);
    g_icon_part_sprites[ITEM_ID_IGNITER_4] = LoadTextureFromImage(icon_part_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    UnloadImage(item_part_image);
    UnloadImage(icon_part_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_IGNITER_4");

//---------------------------------------------------------------------------------------









//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_part_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_part_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {12,5};
    item_pos = {12,5};
    item_part_pos = {7,5};
    icon_part_pos = {3,5};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    ImageCrop(&item_part_image, {item_part_pos.x * 16, item_part_pos.y * 16, 16, 16});
    ImageCrop(&icon_part_image, {icon_part_pos.x * 16, icon_part_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_COUPLER_1] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_COUPLER_1] = LoadTextureFromImage(item_image);
    g_item_part_sprites[ITEM_ID_COUPLER_1] = LoadTextureFromImage(item_part_image);
    g_icon_part_sprites[ITEM_ID_COUPLER_1] = LoadTextureFromImage(icon_part_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    UnloadImage(item_part_image);
    UnloadImage(icon_part_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_COUPLER_1");


    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_part_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_part_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {12,6};
    item_pos = {12,6};
    item_part_pos = {7,6};
    icon_part_pos = {3,6};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    ImageCrop(&item_part_image, {item_part_pos.x * 16, item_part_pos.y * 16, 16, 16});
    ImageCrop(&icon_part_image, {icon_part_pos.x * 16, icon_part_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_COUPLER_2] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_COUPLER_2] = LoadTextureFromImage(item_image);
    g_item_part_sprites[ITEM_ID_COUPLER_2] = LoadTextureFromImage(item_part_image);
    g_icon_part_sprites[ITEM_ID_COUPLER_2] = LoadTextureFromImage(icon_part_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    UnloadImage(item_part_image);
    UnloadImage(icon_part_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_COUPLER_2");


    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_part_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_part_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {12,7};
    item_pos = {12,7};
    item_part_pos = {7,7};
    icon_part_pos = {3,7};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    ImageCrop(&item_part_image, {item_part_pos.x * 16, item_part_pos.y * 16, 16, 16});
    ImageCrop(&icon_part_image, {icon_part_pos.x * 16, icon_part_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_COUPLER_3] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_COUPLER_3] = LoadTextureFromImage(item_image);
    g_item_part_sprites[ITEM_ID_COUPLER_3] = LoadTextureFromImage(item_part_image);
    g_icon_part_sprites[ITEM_ID_COUPLER_3] = LoadTextureFromImage(icon_part_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    UnloadImage(item_part_image);
    UnloadImage(icon_part_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_COUPLER_3");

    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_part_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_part_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {12,8};
    item_pos = {12,8};
    item_part_pos = {7,8};
    icon_part_pos = {3,8};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    ImageCrop(&item_part_image, {item_part_pos.x * 16, item_part_pos.y * 16, 16, 16});
    ImageCrop(&icon_part_image, {icon_part_pos.x * 16, icon_part_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_COUPLER_4] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_COUPLER_4] = LoadTextureFromImage(item_image);
    g_item_part_sprites[ITEM_ID_COUPLER_4] = LoadTextureFromImage(item_part_image);
    g_icon_part_sprites[ITEM_ID_COUPLER_4] = LoadTextureFromImage(icon_part_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    UnloadImage(item_part_image);
    UnloadImage(icon_part_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_COUPLER_4");

//---------------------------------------------------------------------------------------


    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_part_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_part_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {13,5};
    item_pos = {13,5};
    item_part_pos = {8,5};
    icon_part_pos = {4,5};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    ImageCrop(&item_part_image, {item_part_pos.x * 16, item_part_pos.y * 16, 16, 16});
    ImageCrop(&icon_part_image, {icon_part_pos.x * 16, icon_part_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_ROD_1] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_ROD_1] = LoadTextureFromImage(item_image);
    g_item_part_sprites[ITEM_ID_ROD_1] = LoadTextureFromImage(item_part_image);
    g_icon_part_sprites[ITEM_ID_ROD_1] = LoadTextureFromImage(icon_part_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    UnloadImage(item_part_image);
    UnloadImage(icon_part_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_ROD_1");


    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_part_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_part_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {13,6};
    item_pos = {13,6};
    item_part_pos = {8,6};
    icon_part_pos = {4,6};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    ImageCrop(&item_part_image, {item_part_pos.x * 16, item_part_pos.y * 16, 16, 16});
    ImageCrop(&icon_part_image, {icon_part_pos.x * 16, icon_part_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_ROD_2] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_ROD_2] = LoadTextureFromImage(item_image);
    g_item_part_sprites[ITEM_ID_ROD_2] = LoadTextureFromImage(item_part_image);
    g_icon_part_sprites[ITEM_ID_ROD_2] = LoadTextureFromImage(icon_part_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    UnloadImage(item_part_image);
    UnloadImage(icon_part_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_ROD_2");


    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_part_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_part_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {13,7};
    item_pos = {13,7};
    item_part_pos = {8,7};
    icon_part_pos = {4,7};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    ImageCrop(&item_part_image, {item_part_pos.x * 16, item_part_pos.y * 16, 16, 16});
    ImageCrop(&icon_part_image, {icon_part_pos.x * 16, icon_part_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_ROD_3] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_ROD_3] = LoadTextureFromImage(item_image);
    g_item_part_sprites[ITEM_ID_ROD_3] = LoadTextureFromImage(item_part_image);
    g_icon_part_sprites[ITEM_ID_ROD_3] = LoadTextureFromImage(icon_part_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    UnloadImage(item_part_image);
    UnloadImage(icon_part_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_ROD_3");

    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_part_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_part_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {13,8};
    item_pos = {13,8};
    item_part_pos = {8,8};
    icon_part_pos = {4,8};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    ImageCrop(&item_part_image, {item_part_pos.x * 16, item_part_pos.y * 16, 16, 16});
    ImageCrop(&icon_part_image, {icon_part_pos.x * 16, icon_part_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_ROD_4] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_ROD_4] = LoadTextureFromImage(item_image);
    g_item_part_sprites[ITEM_ID_ROD_4] = LoadTextureFromImage(item_part_image);
    g_icon_part_sprites[ITEM_ID_ROD_4] = LoadTextureFromImage(icon_part_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    UnloadImage(item_part_image);
    UnloadImage(icon_part_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_ROD_4");
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------




    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {10,17};
    item_pos = {15,17};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_MUSHROOM] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_MUSHROOM] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_MUSHROOM");

//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {1,18};
    item_pos = {5,18};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_MUSHROOM_JUICE] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_MUSHROOM_JUICE] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_MUSHROOM_JUICE");






//---------------------------------------------------------------------------------------

    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {6,0};
    item_pos = {9,0};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_HELMET] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_HELMET] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_HELMET");

//---------------------------------------------------------------------------------------


    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {6,1};
    item_pos = {15,1};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_BOOTS] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_BOOTS] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_BOOTS");

//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {7,1};
    item_pos = {16,1};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_BODY] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_BODY] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_BODY");

//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {8,1};
    item_pos = {17,1};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_GLOVES] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_GLOVES] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_GLOVES");
 

//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {9,1};
    item_pos = {18,1};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_LEGGINGS] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_LEGGINGS] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_LEGGINGS");




//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {10,20};
    item_pos = {17,20};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_APPLE] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_APPLE] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_APPLE");

//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {10,21};
    item_pos = {17,21};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_CHEESE] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_CHEESE] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_CHEESE");

//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {10,22};
    item_pos = {17,22};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_BREAD] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_BREAD] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_BREAD");

//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {10,23};
    item_pos = {17,23};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_MEAT] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_MEAT] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_MEAT");





//---------------------------------------------------------------------------------------

    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {1,15};
    item_pos = {5,15};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_RING] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_RING] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_RING");






//---------------------------------------------------------------------------------------

    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {20,20};
    item_pos = {25,20};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_STOVE_PLAN] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_STOVE_PLAN] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_STOVE_PLAN");

//---------------------------------------------------------------------------------------

    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {20,20};
    item_pos = {25,20};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_MUSHROOMPRESS_PLAN] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_MUSHROOMPRESS_PLAN] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_MUSHROOMPRESS_PLAN");





    //---------------------------------------------------------------------------------------

    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {25,1};
    item_pos = {30,1};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_HAMMER] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_HAMMER] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_HAMMER");


    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {25,2};
    item_pos = {30,2};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_SAW] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_SAW] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_SAW");


    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {25,3};
    item_pos = {30,3};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_SHOVEL] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_SHOVEL] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_SHOVEL");


    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {25,4};
    item_pos = {30,4};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_TONGS] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_TONGS] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_TONGS");


    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {25,5};
    item_pos = {30,5};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_ANVIL] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_ANVIL] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_ANVIL");


    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {26,1};
    item_pos = {31,1};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_CHARCOAL] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_CHARCOAL] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_CHARCOAL");


    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {26,2};
    item_pos = {31,2};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_RESIN] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_RESIN] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_RESIN");


    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {26,3};
    item_pos = {31,3};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_BONE] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_BONE] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_BONE");


    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {26,4};
    item_pos = {31,4};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_ROPE] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_ROPE] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_ROPE");


    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {26,5};
    item_pos = {31,5};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_BUCKET] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_BUCKET] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_BUCKET");

///--------------error
//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    icon_pos = {0,0};
    item_pos = {0,0};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_ERROR] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_ERROR] = LoadTextureFromImage(item_image);
    UnloadImage(icon_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_ERROR");


//environmental sprites
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_pos = {43,1};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16 * 5, 16 * 6});
    g_environment_sprites[SPRITE_ENVIRO_TREE1] = LoadTextureFromImage(item_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   SPRITE_ENVIRO_TREE1");

    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_pos = {43,7};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16 * 5, 16 * 3});
    g_shadow_sprites[SPRITE_SHADOW_TREE1] = LoadTextureFromImage(item_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   SPRITE_SHADOW_TREE1");
//
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_pos = {40,1};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16 * 2, 16 * 8});
    g_environment_sprites[SPRITE_ENVIRO_TREE2] = LoadTextureFromImage(item_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   SPRITE_ENVIRO_TREE2");

    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_pos = {40,9};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16 * 3, 16 * 3});
    g_shadow_sprites[SPRITE_SHADOW_TREE2] = LoadTextureFromImage(item_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   SPRITE_SHADOW_TREE2");
//
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_pos = {35,1};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_environment_sprites[SPRITE_ENVIRO_GRASS1] = LoadTextureFromImage(item_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   SPRITE_ENVIRO_GRASS1");

    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_pos = {35,2};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_shadow_sprites[SPRITE_SHADOW_GRASS1] = LoadTextureFromImage(item_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   SPRITE_SHADOW_GRASS1");
//
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_pos = {36,1};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_environment_sprites[SPRITE_ENVIRO_GRASS2] = LoadTextureFromImage(item_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   SPRITE_ENVIRO_GRASS2");

    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_pos = {36,2};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_shadow_sprites[SPRITE_SHADOW_GRASS2] = LoadTextureFromImage(item_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   SPRITE_SHADOW_GRASS2");




    /* 
    item_pos = {40,1};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16 * 2, 16 * 8});
    g_environment_sprites[SPRITE_ENVIRO_DECCO_1] = LoadTextureFromImage(item_image);
    */
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_pos = {43,10};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16*4, 16*4});
    g_environment_sprites[SPRITE_ENVIRO_DECCO_1] = LoadTextureFromImage(item_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   SPRITE_ENVIRO_DECCO_1");

    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_pos = {43,14};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16*4, 16});
    g_shadow_sprites[SPRITE_SHADOW_DECCO_1] = LoadTextureFromImage(item_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   SPRITE_SHADOW_DECCO_1");


    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_pos = {45,15};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16*2, 16*4});
    g_environment_sprites[SPRITE_ENVIRO_DECCO_2] = LoadTextureFromImage(item_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   SPRITE_ENVIRO_DECCO_2");

    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_pos = {45,19};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16*2, 16*2});
    g_shadow_sprites[SPRITE_SHADOW_DECCO_2] = LoadTextureFromImage(item_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   SPRITE_SHADOW_DECCO_2");


    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_pos = {24,6};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_shadow_sprites[SPRITE_SHADOW_CHAR1] = LoadTextureFromImage(item_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   SPRITE_SHADOW_CHAR1");

//containers
    TraceLog(LOG_INFO, "LOADING CONTAINER SPRITES");
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_pos = {1,20};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_container_sprites[SPRITE_CONTAINER_CHEST] = LoadTextureFromImage(item_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   SPRITE_CONTAINER_CHEST");

    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_pos = {2,20};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_container_sprites[SPRITE_CONTAINER_BARREL] = LoadTextureFromImage(item_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   SPRITE_CONTAINER_BARREL");


//doors


    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_pos = {30,23};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_door_sprites[SPRITE_DOOR_1] = LoadTextureFromImage(item_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   SPRITE_DOOR_1");


//modules
    TraceLog(LOG_INFO, "LOADING MODULE SPRITES");
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_pos = {21,20};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_module_sprites[MODULE_ID_WORKBENCH] = LoadTextureFromImage(item_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "MODULES   MUDULE_ID_WORKBENCH");

    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_pos = {22,20};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_module_sprites[MODULE_ID_STOVE] = LoadTextureFromImage(item_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "MODULES   MUDULE_ID_STOVE");

    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_pos = {23,20};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_module_sprites[MODULE_ID_MUSHROOMPRESS] = LoadTextureFromImage(item_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "MODULES   MODULE_ID_MUSHROOMPRESS");


//UI sprites
    TraceLog(LOG_INFO, "LOADING UI SPRITES");
    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_pos = {1,22};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 32, 32});
    g_ui_sprites[UI_ID_CROSSHAIR] = LoadTextureFromImage(item_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   UI_ID_CROSSHAIR");

    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_pos = {3,22};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 32, 32});
    g_ui_sprites[UI_ID_USE] = LoadTextureFromImage(item_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   UI_ID_USE");

    item_image = LoadImageFromTexture(g_main_sprite_sheet);
    item_pos = {5,22};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 32, 32});
    g_ui_sprites[UI_ID_AIM] = LoadTextureFromImage(item_image);
    UnloadImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   UI_ID_AIM");




    TraceLog(LOG_INFO, "LOADING PORTRAITS");
    g_portrait_sprites[PORTRAIT_BASE_CHAR] = LoadTexture("assets/portrait1.png");

    TraceLog(LOG_INFO, "LOADING UI PANELS");
    g_ui_panels[PANEL_GRAY] = LoadTexture("assets/graypanel1.png");
    g_ui_panels[PANEL_WOOD_LARGE] = LoadTexture("assets/largepanel1.png");
    g_ui_panels[PANEL_CHAR_SCREEN] = LoadTexture("assets/characterpanelbg.png");
    g_ui_panels[PANEL_MODULE_SCREEN] = LoadTexture("assets/modulepanelbg.png");

    TraceLog(LOG_INFO, "LOADING UI BACKGROUNDS");
    g_ui_backgrounds[BG_TITLE] = LoadTexture("assets/titleimage.png");
    g_ui_backgrounds[BG_STAGING] = LoadTexture("assets/staging_bg2.png");
    g_ui_backgrounds[BG_SHELTER] = LoadTexture("assets/staging_bg1.png");
    g_ui_backgrounds[BG_HOUSE_TRANS] = LoadTexture("assets/house_trans.png");
    g_ui_backgrounds[BG_OUTSIDE_TRANS] = LoadTexture("assets/outside_trans.png");

}

void UnloadResources() {
    
    UnloadTexture(g_main_sprite_sheet);

    TraceLog(LOG_INFO, "UNLOADING CHARACTER SHEETS");
    for(int i = 0; i < MAX_SPRITES; i++) {
        UnloadTexture(g_character_sprite_sheets[i]);
    }

    TraceLog(LOG_INFO, "UNLOADING CREATURE SHEETS");
    for(int i = 0; i < MAX_SPRITES; i++) {
        UnloadTexture(g_creature_sprite_sheets[i]);
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

    TraceLog(LOG_INFO, "UNLOADING ENVIRONMENT SPRITES");
    for(int i = 0; i < MAX_SPRITES; i++) {
        UnloadTexture(g_environment_sprites[i]);
    }

    TraceLog(LOG_INFO, "UNLOADING SHADOW SPRITES");
    for(int i = 0; i < MAX_SPRITES; i++) {
        UnloadTexture(g_shadow_sprites[i]);
    }

    TraceLog(LOG_INFO, "UNLOADING MODULE SPRITES");
    for(int i = 0; i < MAX_SPRITES; i++) {
        UnloadTexture(g_module_sprites[i]);
    }

    TraceLog(LOG_INFO, "UNLOADING DOOR SPRITES");
    for(int i = 0; i < MAX_SPRITES; i++) {
        UnloadTexture(g_door_sprites[i]);
    }
}