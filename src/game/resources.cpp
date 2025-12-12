#include "../core/gamedefs.h"


void LoadResources() {
    TraceLog(LOG_INFO, "LOADING UNIT SPRITES");


    g_sprite_sheets[SPRITE_MAIN] = LoadTexture("assets/main_spritesheet.png");

    Image sheet_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    Vector2 sheet_pos = {20,1};
    ImageCrop(&sheet_image, {sheet_pos.x * 16, sheet_pos.y * 16, 16*4, 16*4});
    g_sprite_sheets[SPRITE_NERD] = LoadTextureFromImage(sheet_image);
    TraceLog(LOG_INFO, "SHEETS   SPRITE_NERD");

    sheet_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    sheet_pos = {20,1};
    ImageCrop(&sheet_image, {sheet_pos.x * 16, sheet_pos.y * 16, 16*4, 16*4});
    g_sprite_sheets[SPRITE_APPRENTICE] = LoadTextureFromImage(sheet_image);
    TraceLog(LOG_INFO, "SHEETS   SPRITE_APPRENTICE");


    
    TraceLog(LOG_INFO, "LOADING SPELL SPRITES");

    Image spell_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    Vector2 spell_pos = {20,15};

    ImageCrop(&spell_image, {spell_pos.x * 16, spell_pos.y * 16, 16, 16});
    g_spell_sprites[SPELL_ID_MAGICMISSLE] = LoadTextureFromImage(spell_image);
    TraceLog(LOG_INFO, "SPRITES   spell SPELL_ID_MAGICMISSLE");


//---------------------------------------------------------------------------------------

    spell_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    spell_pos = {21,15};

    ImageCrop(&spell_image, {spell_pos.x * 16, spell_pos.y * 16, 16, 16});
    g_spell_sprites[SPELL_ID_FIREBALL] = LoadTextureFromImage(spell_image);
    TraceLog(LOG_INFO, "SPRITES   spell SPELL_ID_FIREBALL");

//---------------------------------------------------------------------------------------
    spell_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    spell_pos = {22,15};

    ImageCrop(&spell_image, {spell_pos.x * 16, spell_pos.y * 16, 32, 16});
    g_spell_sprites[SPELL_ID_LIGHTNING] = LoadTextureFromImage(spell_image);
    TraceLog(LOG_INFO, "SPRITES   spell SPELL_ID_LIGHTNING");

//---------------------------------------------------------------------------------------

    TraceLog(LOG_INFO, "LOADING ITEM and ICON SPRITES");

    Image icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    Image item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    Vector2 icon_pos = {1,1};
    Vector2 item_pos = {10,1};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_DAGGER] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_DAGGER] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_DAGGER");

//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {2,1};
    item_pos = {11,1};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_SWORD] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_SWORD] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_SWORD");

//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {3,1};
    item_pos = {12,1};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_SPEAR] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_SPEAR] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_SPEAR");

//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {4,1};
    item_pos = {13,1};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_AXE] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_AXE] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_AXE");

//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {5,1};
    item_pos = {14,1};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_BOW] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_BOW] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_BOW");


//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {1,4};
    item_pos = {10,4};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_WAND] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_WAND] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_WAND");


    //---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {5,4};
    item_pos = {14,4};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_STAFF] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_STAFF] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_STAFF");









//---------------------------------------------------------------------------------------
     icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {1,5};
    item_pos = {10,5};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_MAGICMISSLE_WAND] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_MAGICMISSLE_WAND] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_MAGICMISSLE_WAND_1");


//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {2,5};
    item_pos = {11,5};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_FIREBALL_WAND] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_FIREBALL_WAND] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_FIREBALL_WAND_1");


//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {3,5};
    item_pos = {12,5};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_LIGHTNING_WAND] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_LIGHTNING_WAND] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_LIGHTNING_WAND_1");


//---------------------------------------------------------------------------------------



//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {5,5};
    item_pos = {14,5};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_MAGICMISSLE_STAFF] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_MAGICMISSLE_STAFF] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_MAGICMISSLE_STAFF_1");


//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {6,5};
    item_pos = {15,5};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_FIREBALL_STAFF] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_FIREBALL_STAFF] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_FIREBALL_STAFF_1");


//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {7,5};
    item_pos = {16,5};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_LIGHTNING_STAFF] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_LIGHTNING_STAFF] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_LIGHTNING_STAFF_1");

//---------------------------------------------------------------------------------------

 






//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {10,17};
    item_pos = {15,17};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_MUSHROOM] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_MUSHROOM] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_MUSHROOM");

//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {1,18};
    item_pos = {5,18};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_MUSHROOM_JUICE] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_MUSHROOM_JUICE] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_MUSHROOM_JUICE");









//---------------------------------------------------------------------------------------

    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {6,0};
    item_pos = {9,0};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_HELMET] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_HELMET] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_HELMET");

//---------------------------------------------------------------------------------------


    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {6,1};
    item_pos = {15,1};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_BOOTS] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_BOOTS] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_BOOTS");

//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {7,1};
    item_pos = {16,1};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_BODY] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_BODY] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_BODY");

//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {8,1};
    item_pos = {17,1};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_GLOVES] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_GLOVES] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_GLOVES");
 

//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {9,1};
    item_pos = {18,1};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_LEGGINGS] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_LEGGINGS] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_LEGGINGS");







//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {10,15};
    item_pos = {15,15};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_MAGICMMISSLE_SCROLL] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_MAGICMMISSLE_SCROLL] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_MAGICMMISSLE_SCROLL");

//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {11,15};
    item_pos = {16,15};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_FIREBALL_SCROLL] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_FIREBALL_SCROLL] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_FIREBALL_SCROLL");

    //---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {12,15};
    item_pos = {17,15};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_LIGHTNING_SCROLL] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_LIGHTNING_SCROLL] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_LIGHTNING_SCROLL");







//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {10,20};
    item_pos = {17,20};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_APPLE] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_APPLE] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_APPLE");

//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {10,21};
    item_pos = {17,21};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_CHEESE] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_CHEESE] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_CHEESE");

//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {10,22};
    item_pos = {17,22};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_BREAD] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_BREAD] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_BREAD");

//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {10,23};
    item_pos = {17,23};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_MEAT] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_MEAT] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_MEAT");







//---------------------------------------------------------------------------------------

    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {1,15};
    item_pos = {5,15};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_RING] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_RING] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_RING");






//---------------------------------------------------------------------------------------

    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {20,20};
    item_pos = {25,20};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_STOVE_PLAN] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_STOVE_PLAN] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_STOVE_PLAN");

//---------------------------------------------------------------------------------------

    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {20,20};
    item_pos = {25,20};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_MUSHROOMPRESS_PLAN] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_MUSHROOMPRESS_PLAN] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_MUSHROOMPRESS_PLAN");





    //---------------------------------------------------------------------------------------

    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {25,1};
    item_pos = {30,1};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_HAMMER] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_HAMMER] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_HAMMER");


    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {25,2};
    item_pos = {30,2};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_SAW] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_SAW] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_SAW");


    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {25,3};
    item_pos = {30,3};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_SHOVEL] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_SHOVEL] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_SHOVEL");


    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {25,4};
    item_pos = {30,4};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_TONGS] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_TONGS] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_TONGS");


    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {25,5};
    item_pos = {30,5};
    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_ANVIL] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_ANVIL] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_ANVIL");

///--------------error
//---------------------------------------------------------------------------------------
    icon_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    icon_pos = {0,0};
    item_pos = {0,0};

    ImageCrop(&icon_image, {icon_pos.x * 16, icon_pos.y * 16, 16, 16});
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_icon_sprites[ITEM_ID_ERROR] = LoadTextureFromImage(icon_image);
    g_item_sprites[ITEM_ID_ERROR] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   ITEM_ID_ERROR");


//environmental sprites
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_pos = {43,1};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16 * 5, 16 * 6});
    g_environment_sprites[SPRITE_ENVIRO_TREE1] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   SPRITE_ENVIRO_TREE1");

    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_pos = {43,7};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16 * 5, 16 * 3});
    g_shadow_sprites[SPRITE_SHADOW_TREE1] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   SPRITE_SHADOW_TREE1");
//
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_pos = {40,1};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16 * 2, 16 * 8});
    g_environment_sprites[SPRITE_ENVIRO_TREE2] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   SPRITE_ENVIRO_TREE2");

    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_pos = {40,9};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16 * 3, 16 * 3});
    g_shadow_sprites[SPRITE_SHADOW_TREE2] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   SPRITE_SHADOW_TREE2");
//
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_pos = {35,1};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_environment_sprites[SPRITE_ENVIRO_GRASS1] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   SPRITE_ENVIRO_GRASS1");

    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_pos = {35,2};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_shadow_sprites[SPRITE_SHADOW_GRASS1] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   SPRITE_SHADOW_GRASS1");
//
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_pos = {36,1};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_environment_sprites[SPRITE_ENVIRO_GRASS2] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   SPRITE_ENVIRO_GRASS2");

    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_pos = {36,2};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_shadow_sprites[SPRITE_SHADOW_GRASS2] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   SPRITE_SHADOW_GRASS2");

//containers
    TraceLog(LOG_INFO, "LOADING CONTAINER SPRITES");
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_pos = {1,20};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_container_sprites[SPRITE_CONTAINER_CHEST] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   SPRITE_CONTAINER_CHEST");

    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_pos = {2,20};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_container_sprites[SPRITE_CONTAINER_BARREL] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   SPRITE_CONTAINER_BARREL");


//modules
    TraceLog(LOG_INFO, "LOADING MODULE SPRITES");
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_pos = {21,20};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_module_sprites[MODULE_ID_WORKBENCH] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "MODULES   MUDULE_ID_WORKBENCH");

    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_pos = {22,20};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_module_sprites[MODULE_ID_STOVE] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "MODULES   MUDULE_ID_STOVE");

    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_pos = {23,20};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 16, 16});
    g_module_sprites[MODULE_ID_MUSHROOMPRESS] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "MODULES   MODULE_ID_MUSHROOMPRESS");


//UI sprites
    TraceLog(LOG_INFO, "LOADING UI SPRITES");
    item_image = LoadImageFromTexture(g_sprite_sheets[SPRITE_MAIN]);
    item_pos = {1,22};
    ImageCrop(&item_image, {item_pos.x * 16, item_pos.y * 16, 32, 32});
    g_ui_sprites[UI_ID_CROSSHAIR] = LoadTextureFromImage(item_image);
    TraceLog(LOG_INFO, "SPRITES   UI_ID_CROSSHAIR");


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

    TraceLog(LOG_INFO, "UNLOADING ENVIRONMENT SPRITES");
    for(int i = 0; i < MAX_SPRITES; i++) {
        UnloadTexture(g_environment_sprites[i]);
    }

    TraceLog(LOG_INFO, "UNLOADING ENVIRONMENT SPRITES");
    for(int i = 0; i < MAX_SPRITES; i++) {
        UnloadTexture(g_shadow_sprites[i]);
    }

    TraceLog(LOG_INFO, "UNLOADING ENVIRONMENT SPRITES");
    for(int i = 0; i < MAX_SPRITES; i++) {
        UnloadTexture(g_module_sprites[i]);
    }
}