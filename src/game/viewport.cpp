#include "../core/gamedefs.h"

#define MAX_ZOOM 2.4f
#define MIN_ZOOM 1.5f
#define ZOOM_STEP 0.20f


float max_dist_sqr = 150*150;
float min_dist_sqr = 50*50;

void CalculateViewport() {

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




void HandleCamera1() {

    Vector2 worldPosBeforeZoom = GetScreenToWorld2D(g_input.world_mouse_position, g_camera);

    g_camera.zoom += g_input.mouse_wheel * ZOOM_STEP;
    if(g_camera.zoom < MIN_ZOOM) {
        g_camera.zoom = MIN_ZOOM;
    }
    if(g_camera.zoom > MAX_ZOOM) {
        g_camera.zoom = MAX_ZOOM;
    }

    CalculateViewport();

    float x_offset_f = g_viewport.x_offset_f;
    float y_offset_f = g_viewport.y_offset_f;

    g_camera.target = Vector2Subtract( g_current_player->position, {x_offset_f, y_offset_f} );

}

void HandleCamera2() {
Vector2 worldPosBeforeZoom = GetScreenToWorld2D(g_input.world_mouse_position, g_camera);

    g_camera.zoom += g_input.mouse_wheel * ZOOM_STEP;
    if (g_camera.zoom < MIN_ZOOM)
    {
        g_camera.zoom = MIN_ZOOM;
    }
    if (g_camera.zoom > MAX_ZOOM)
    {
        g_camera.zoom = MAX_ZOOM;
    }

    CalculateViewport();

    float x_offset_f = g_viewport.x_offset_f;
    float y_offset_f = g_viewport.y_offset_f;

    g_camera.target = Vector2Subtract(g_current_player->position, {x_offset_f, y_offset_f});

    if (g_current_player->position.x - x_offset_f < 0)
    {
        float x_dif = x_offset_f - g_current_player->position.x;
        // TraceLog(LOG_INFO, "x_dif %0.2f   %0.2f, %0.2f", x_dif, g_camera.target.x, g_camera.target.x);
        g_camera.target.x = g_camera.target.x + x_dif;
    }
    else if (g_current_player->position.x + x_offset_f > g_ldtk_maps.levels[g_game_data.current_map_index].px_wid)
    {
        float x_dif = (x_offset_f + g_current_player->position.x) - g_ldtk_maps.levels[g_game_data.current_map_index].px_wid;
        // TraceLog(LOG_INFO, "x_dif %0.2f   %0.2f, %0.2f", x_dif, g_camera.target.x, g_camera.target.y);
        g_camera.target.x = g_camera.target.x - x_dif;
    }

    if (g_current_player->position.y - y_offset_f < 0)
    {
        float y_dif = y_offset_f - g_current_player->position.y;
        // TraceLog(LOG_INFO, "y_dif %0.2f   %0.2f, %0.2f", y_dif, g_camera.target.y, g_camera.target.y);
        g_camera.target.y = g_camera.target.y + y_dif;
    }
    else if (g_current_player->position.y + y_offset_f > g_ldtk_maps.levels[g_game_data.current_map_index].px_hei)
    {
        float y_dif = (y_offset_f + g_current_player->position.y) - g_ldtk_maps.levels[g_game_data.current_map_index].px_hei;
        // TraceLog(LOG_INFO, "y_dif %0.2f   %0.2f, %0.2f", y_dif, g_camera.target.y, g_camera.target.y);
        g_camera.target.y = g_camera.target.y - y_dif;
    }
}


//bounded 
void HandleCamera3() {

    g_camera.zoom += g_input.mouse_wheel * ZOOM_STEP;
    if (g_camera.zoom < MIN_ZOOM)
    {
        g_camera.zoom = MIN_ZOOM;
    }
    if (g_camera.zoom > MAX_ZOOM)
    {
        g_camera.zoom = MAX_ZOOM;
    }

    CalculateViewport();

    Vector2 mouse_offset = Vector2Subtract(g_input.world_mouse_position, g_current_player->position);

    float half_w = g_viewport.x_offset_f;
    float half_h = g_viewport.y_offset_f;
    float level_w = g_ldtk_maps.levels[g_game_data.current_map_index].px_wid;
    float level_h = g_ldtk_maps.levels[g_game_data.current_map_index].px_hei;


    //TraceLog(LOG_INFO, "mouse offset  x %0.2f  y %0.2f     half w %0.2f   half h %0.2f", mouse_offset.x, mouse_offset.y, half_w, half_h);
    
    Vector2 new_camera_target = Vector2Add(g_camera.target, {mouse_offset.x, mouse_offset.y});

    
    new_camera_target.x = std::clamp(new_camera_target.x, 0.0f, level_w - (half_w*2));
    new_camera_target.y = std::clamp(new_camera_target.y, 0.0f, level_h - (half_h*2));

    g_camera.target.x = Lerp(g_camera.target.x, new_camera_target.x, .01);
    g_camera.target.y = Lerp(g_camera.target.y, new_camera_target.y, .01);
}






void HandleCamera4() {

    g_camera.zoom += g_input.mouse_wheel * ZOOM_STEP;

    if (g_camera.zoom < MIN_ZOOM){g_camera.zoom = MIN_ZOOM;}
    if (g_camera.zoom > MAX_ZOOM){g_camera.zoom = MAX_ZOOM;}

    CalculateViewport();

    float half_w = g_viewport.x_offset_f;
    float half_h = g_viewport.y_offset_f;
    float level_w = g_ldtk_maps.levels[g_game_data.current_map_index].px_wid;
    float level_h = g_ldtk_maps.levels[g_game_data.current_map_index].px_hei;
    
    
    Vector2 new_camera_target = Vector2Subtract(g_current_player->position, {half_w, half_h});
    
    Vector2 mouse_offset = Vector2Subtract(g_input.world_mouse_position, g_current_player->position);
    //TraceLog(LOG_INFO, "mouse_offset  x %0.2f  y %0.2f     half w %0.2f   half h %0.2f", mouse_offset.x, mouse_offset.y, half_w, half_h);

    mouse_offset.x = mouse_offset.x * 0.5f;
    mouse_offset.y = mouse_offset.y * 0.5f;

    float dist = Vector2DistanceSqr(g_input.world_mouse_position, g_current_player->position);

    if(dist < min_dist_sqr) {
        mouse_offset.x = 0.0f;
        mouse_offset.y = 0.0f;
    }


    if(mouse_offset.x < -115) {mouse_offset.x = -115;}
    else if(mouse_offset.x > 115) {mouse_offset.x = 115;}

    if(mouse_offset.y < -70) {mouse_offset.y = -70;}
    else if(mouse_offset.y > 70) {mouse_offset.y = 70;} 
    
    new_camera_target = Vector2Add(new_camera_target, mouse_offset);

    
    new_camera_target.x = std::clamp(new_camera_target.x, 0.0f, level_w - (half_w*2));
    new_camera_target.y = std::clamp(new_camera_target.y, 0.0f, level_h - (half_h*2));
    
    g_camera.target.x = Lerp(g_camera.target.x, new_camera_target.x, 0.04f);
    g_camera.target.y = Lerp(g_camera.target.y, new_camera_target.y, 0.04f);

    //TraceLog(LOG_INFO, "dist %0.02f, max dist %0.02f mouse_offset  x %0.2f  y %0.2f     half w %0.2f   half h %0.2f\n\n", dist, max_dist_sqr, mouse_offset.x, mouse_offset.y, half_w, half_h);
}