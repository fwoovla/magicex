#pragma once

#include "gamedefs.h"

#define MAX_SPRITES 1000

extern Texture2D g_main_sprite_sheet;

enum SpriteSheetID {
    SPRITE_NONE = -1,
    SPRITE_APPRENTICE,
    SPRITE_NERD,
    SPRITE_TESTDUMMY,
    SPRITE_BUNNY,
    SPRITE_SCAVENGER,
    SPRITE_TRADER,
    SPRITE_SLIME,
    SPRITE_MAIN,

    SPRITE_NUM_SPRITES
};

extern Texture2D g_character_sprite_sheets[MAX_SPRITES];

enum UISpriteID {
    UI_ID_CROSSHAIR,
    UI_ID_USE,
    UI_ID_AIM,
};

extern Texture2D g_ui_sprites[MAX_SPRITES];


enum CreatureID {
    CREATURE_NONE = -1,
    CREATURE_TESTDUMMY,
    CREATURE_BUNNY,
    CREATURE_SCAVENGER,
    CREATURE_TRADER,
    CREATURE_SLIME,
    CREATURE_NPC_1,
    CREATURE_NUM_CREATURES
};

extern Texture2D g_creature_sprite_sheets[MAX_SPRITES];

enum ITEM_TIER {
    TIER_1 = 0,
    TIER_2,
    TIER_3,
    TIER_4,
    TIER_5,
    TIER_MAX,
};

enum ItemID {
    ITEM_ID_NONE = -1,
    ITEM_ID_DAGGER,         
    ITEM_ID_SWORD,
    ITEM_ID_SPEAR,
    ITEM_ID_AXE,
    ITEM_ID_BOW,
    
    ITEM_ID_WAND_FRESH,
    ITEM_ID_WAND_AGED,
    ITEM_ID_CASTERBASE,

    ITEM_ID_IGNITER_1,
    ITEM_ID_IGNITER_2,
    ITEM_ID_IGNITER_3,
    ITEM_ID_IGNITER_4,

    ITEM_ID_COUPLER_1,
    ITEM_ID_COUPLER_2,
    ITEM_ID_COUPLER_3,
    ITEM_ID_COUPLER_4,

    ITEM_ID_ROD_1,
    ITEM_ID_ROD_2,
    ITEM_ID_ROD_3,
    ITEM_ID_ROD_4,


    ITEM_ID_MUSHROOM,
    ITEM_ID_MUSHROOM_JUICE,

    ITEM_ID_HELMET,
    ITEM_ID_BOOTS,
    ITEM_ID_BODY,
    ITEM_ID_GLOVES,
    ITEM_ID_LEGGINGS,

    ITEM_ID_APPLE,
    ITEM_ID_CHEESE,
    ITEM_ID_BREAD,
    ITEM_ID_MEAT,

    ITEM_ID_RING,

    ITEM_ID_STOVE_PLAN,
    ITEM_ID_MUSHROOMPRESS_PLAN,

    ITEM_ID_HAMMER,
    ITEM_ID_SAW,
    ITEM_ID_SHOVEL,
    ITEM_ID_TONGS,
    ITEM_ID_ANVIL,

    ITEM_ID_CHARCOAL,
    ITEM_ID_RESIN,
    ITEM_ID_BONE,
    ITEM_ID_ROPE,
    ITEM_ID_BUCKET,

    ITEM_ID_ERROR,
    ITEM_ID_MAX
};

extern Texture2D g_item_sprites[MAX_SPRITES];
extern Texture2D g_icon_sprites[MAX_SPRITES];
extern Texture2D g_icon_part_sprites[MAX_SPRITES];
extern Texture2D g_item_part_sprites[MAX_SPRITES];
extern Texture2D g_spell_icon_delivery[MAX_SPRITES];


enum RecipieID {
    RECIPIE_ID_NONE = -1,
    RECIPIE_ID_APPLE,
    RECIPIE_ID_WAND,
    RECIPIE_ID_STAFF,
    RECIPIE_ID_MUSHROOMJUICE
};


enum PlanID {
    PLAN_ID_NONE = -1,
    PLAN_ID_STOVE,
    PLAN_ID_MUSHROOMPRESS,
};

enum ItemModID {
    ITEMMOD_NONE = -1,
//
    ITEMMOD_FOODSTART,
    ITEMMOD_NUTRITIOUS,
    ITEMMOD_MOLDY,
    ITEMMOD_FOODEND,
//
    ITEMMOD_MAX
};


enum ModuleID {
    MODULE_ID_NONE = -1,
    MODULE_ID_WORKBENCH,
    MODULE_ID_STOVE,
    MODULE_ID_MUSHROOMPRESS,
    MODULE_ID_SPELLGENERATOR,
};

extern Texture2D g_module_sprites[MAX_SPRITES];

enum AIID {
    AI_FEARFULL,
    AI_PASSIVE,
    AI_TERRITORIAL,
    AI_AGGRESSIVE,
    AI_HOSTILE,
};



extern Texture2D g_spell_sprites[MAX_SPRITES];
extern Texture2D g_spell_sheet;



enum ContainerSprites {
    SPRITE_CONTAINER_CHEST,
    SPRITE_CONTAINER_BARREL,
    SPRITE_CONTAINER_NUM_SPRITES
};

extern Texture2D g_container_sprites[MAX_SPRITES];



enum DoorSprites {
    SPRITE_DOOR_1,
    SPRITE_DOOR_2,
    SPRITE_NUM_DOORS
};

extern Texture2D g_door_sprites[MAX_SPRITES];




enum PortraitSpriteID {
    PORTRAIT_BASE_CHAR,
    PORTRAIT_NUM_SPRITES
};

extern Texture2D g_portrait_sprites[MAX_SPRITES];

enum UIPanelID {
    PANEL_GRAY,
    PANEL_WOOD_LARGE,
    PANEL_NUM_SPRITES,
    PANEL_CHAR_SCREEN,
    PANEL_MODULE_SCREEN,
    PANEL_SPELLGEN_SCREEN
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



enum EnvironmentSpriteID {
    SPRITE_ENVIRO_ERROR,
    SPRITE_ENVIRO_TREE_START,
    SPRITE_ENVIRO_TREE1,
    SPRITE_ENVIRO_TREE2,
    SPRITE_ENVIRO_TREE_END,
    SPRITE_ENVIRO_GRASS_START,
    SPRITE_ENVIRO_GRASS1,
    SPRITE_ENVIRO_GRASS2,
    SPRITE_ENVIRO_GRASS_END,
    SPRITE_ENVIRO_DECCO_START,
    SPRITE_ENVIRO_DECCO_1,
    SPRITE_ENVIRO_DECCO_2,
    SPRITE_ENVIRO_DECCO_END,
    SPRITE_ENVIRO_NUM_SPRITES
};

extern Texture2D g_environment_sprites[MAX_SPRITES];

enum ShadowSpriteID {
    SPRITE_SHADOW_ERROR,
    SPRITE_SHADOW_TREE1 =SPRITE_ENVIRO_TREE1,
    SPRITE_SHADOW_TREE2,
    SPRITE_SHADOW_GRASS1 = SPRITE_ENVIRO_GRASS1,
    SPRITE_SHADOW_GRASS2,
    SPRITE_SHADOW_DECCO_1 = SPRITE_ENVIRO_DECCO_1,
    SPRITE_SHADOW_DECCO_2,
    SPRITE_SHADOW_CHAR1,
    SPRITE_SHADOW_NUM_SPRITES
};

extern Texture2D g_shadow_sprites[MAX_SPRITES];



void LoadResources();


void UnloadResources();
