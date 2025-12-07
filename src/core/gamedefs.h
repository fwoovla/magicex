#pragma once
#include <raylib.h>



inline const Color DARKRED = {.r = 131, .g = 12, .b = 0, .a = 255};

inline const Color TRANSDARKRED = {.r = 50, .g = 0, .b = 0, .a = 60};
inline const Color TRANSRED = {.r = 255, .g = 0, .b = 0, .a = 60};

inline const Color CYAN = {.r = 0, .g = 201, .b = 153, .a = 255};
inline const Color DARKYELLOW = {.r = 153, .g = 151, .b = 0, .a = 255};
inline const Color DARKERGRAY = {.r = 40, .g = 40, .b = 40, .a = 255};
inline const Color TRANSDARKERGRAY = {.r = 40, .g = 40, .b = 40, .a = 150};

inline const Color TRANSDARKGREEN = {.r = 0, .g = 50, .b = 0, .a = 60};
inline const Color TRANSGREEN = {.r = 0, .g = 255, .b = 0, .a = 60};

//inline const int TILE_SIZE = 32;
//inline const int INV_TILE_SIZE = 1.0f/TILE_SIZE;



struct GameSettings {
    Vector2 window_size;
    Vector2 resolution;
    bool show_debug;
};

struct DebugData {
    float tile_dt;
    int tiles_drawn;
};

extern DebugData g_debug_data;


struct PlayerInput {
    Vector2 screen_mouse_position;
    Vector2 world_mouse_position;
    Vector2 mouse_left_clicked_screen_position;
    Vector2 mouse_left_clicked_world_position;
    Vector2 mouse_right_clicked_screen_position;
    Vector2 mouse_right_clicked_world_position;

    bool mouse_left;
    bool mouse_left_down;
    bool mouse_right;
    bool mouse_right_down;
    bool mouse_middle;
    bool mouse_middle_down;
    float mouse_wheel;
    bool selecting;
    Rectangle selected_rect;

    bool key_up;
    bool key_down;
    bool key_left;
    bool key_right;

    bool key_sprint;

    bool key_use;

    bool key_switch_weapon;

    KeyboardKey keys_pressed[10];
};

enum SCENE_ID {
    NO_SCENE,
    SPLASH_SCENE,
    TITLE_SCENE,
    STAGING_SCENE,
    SHELTER_SCENE,
    GAME_SCENE,
    SUB_SCENE,
    END_SCENE
};


struct Viewport {
    int tile_size;
    int x_offset_i;
    int y_offset_i;
    int x_max;
    int x_min;
    int y_max;
    int y_min;
    float x_offset_f;
    float y_offset_f;
    float inv_tile_size;
    Vector2 center_i;
    Vector2 center_f;
    Vector2 screen_size;
    Vector2 resolution;


};

#include "sprite.h"

struct Cursor {
    Vector2 poisition;
    Sprite sprite;
};

extern Cursor g_cursor;

extern GameSettings g_game_settings;
//extern int *level_data_array;
//extern GameSettings g_game_settings;
extern float g_scale;
extern float g_inv_scale;
extern float g_world2screen;
extern float g_screen2world;
extern Vector2 g_resolution;
extern Vector2 g_screen_center;
extern int g_map_width;
extern int g_map_height;
extern Camera2D g_camera;
extern PlayerInput g_input;
extern Viewport g_viewport;

#include "ldtk.h"
#include "resources.h"
#include "itemgenerator.h"
#include "data.h"
#include "spells.h"
#include "utils.h"
#include "events.h"
#include "viewport.h"
#include "areas.h"
#include "input.h"
#include "button.h"
#include "label.h"
#include "baseentity.h"
#include "entities.h"
#include "modules.h"
#include "playercharacter.h"
#include "entityspawner.h"
//#include "units.h"
#include "tilelayer.h"
#include "uilayers.h"
#include "scenes.h"
#include "scenemanager.h"
#include "game.h"




