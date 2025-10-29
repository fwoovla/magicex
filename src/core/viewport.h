#pragma once 
#include "gamedefs.h"

inline void CalculateViewport() {

    g_viewport.tile_size = g_ldtk_maps.default_grid_size ;
    g_viewport.inv_tile_size = 1.0f/(float)g_viewport.tile_size;
    g_viewport.x_offset_i = (g_resolution.x * 0.5f) / g_camera.zoom * g_viewport.inv_tile_size;
    g_viewport.y_offset_i = (g_resolution.y * 0.5f) / g_camera.zoom * g_viewport.inv_tile_size;
    g_viewport.x_offset_f = (g_resolution.x * 0.5f) / g_camera.zoom;
    g_viewport.y_offset_f = (g_resolution.y * 0.5f) / g_camera.zoom;

    g_viewport.center_i = Vector2Add(g_camera.target, {g_viewport.x_offset_f, g_viewport.y_offset_f} ) * g_viewport.inv_tile_size;
    g_viewport.center_f = Vector2Add(g_camera.target, {g_viewport.x_offset_f, g_viewport.y_offset_f} );


    LDTKLevel *this_level = &g_ldtk_maps.levels[g_game_data.current_map_index];

    int64_t map_width = this_level->px_wid * g_viewport.inv_tile_size;
    int64_t map_height = this_level->px_hei * g_viewport.inv_tile_size;
    
    g_viewport.x_min = g_viewport.center_i.x - (g_viewport.x_offset_i + 1);
    if(g_viewport.x_min < 0) {
        g_viewport.x_min = 0;
    }
        
    g_viewport.x_max = g_viewport.center_i.x + (g_viewport.x_offset_i + 2);
    if(g_viewport.x_max > map_width) {
        g_viewport.x_max = map_width;
    } 
            
    g_viewport.y_min = g_viewport.center_i.y - (g_viewport.y_offset_i + 1);
    if(g_viewport.y_min < 0) {
        g_viewport.y_min = 0;
    }
                
    g_viewport.y_max = g_viewport.center_i.y + (g_viewport.y_offset_i + 2);
    if(g_viewport.y_max > map_height) {
        g_viewport.y_max = map_height;
    }

    g_world2screen = g_camera.zoom * g_scale;
    g_screen2world = 1/g_world2screen;

}

