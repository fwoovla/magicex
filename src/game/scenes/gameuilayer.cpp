#include "../../core/gamedefs.h"

GameUILayer::GameUILayer() {

    CreateLabel(title_label, {g_screen_center.x, 20 / g_scale}, 40/g_scale, RAYWHITE, "...");

    CreateLabel(debug_input_smp, {20, 20 / g_scale}, 20/g_scale, RED, "smp...");
    CreateLabel(debug_input_wmp, {20, 40 / g_scale}, 20/g_scale, GREEN, "wmp...");

    CreateLabel(debug_input_mw, {20, 60 / g_scale}, 20/g_scale, RAYWHITE, "mw...");

    CreateLabel(debug_input_mm, {20, 80 / g_scale}, 20/g_scale, RAYWHITE, "mm...");
    CreateLabel(debug_input_mmd, {20, 100 / g_scale}, 20/g_scale, RAYWHITE, "mmd...");

    CreateLabel(debug_input_mbl, {20, 120 / g_scale}, 20/g_scale, RAYWHITE, "mbl...");
    CreateLabel(debug_input_mbld, {20, 140 / g_scale}, 20/g_scale, RAYWHITE, "mbld...");
    CreateLabel(debug_input_mblscp, {20, 180 / g_scale}, 20/g_scale, RAYWHITE, "mblscp...");
    CreateLabel(debug_input_mblwcp, {20, 200 / g_scale}, 20/g_scale, RAYWHITE, "mblwcp...");


    CreateLabel(debug_input_mbr, {20, 220 / g_scale}, 20/g_scale, RAYWHITE, "mbr...");
    CreateLabel(debug_input_mbrd, {20, 240 / g_scale}, 20/g_scale, RAYWHITE, "mbrd...");
    CreateLabel(debug_input_mbrscp, {20, 260 / g_scale}, 20/g_scale, RAYWHITE, "mbrscp...");
    CreateLabel(debug_input_mbrwcp, {20, 280 / g_scale}, 20/g_scale, RAYWHITE, "mbrwcp...");

    CreateLabel(debug_scale, {20, 300 / g_scale}, 20/g_scale, RAYWHITE, "scale...");
    CreateLabel(debug_zoom_level, {20, 320 / g_scale}, 20/g_scale, RAYWHITE, "zoom...");
    CreateLabel(debug_camera_target, {20, 340 / g_scale}, 20/g_scale, RAYWHITE, "camera target...");
    CreateLabel(debug_w2s, {20, 360 / g_scale}, 20/g_scale, RAYWHITE, "w2s...");
    CreateLabel(debug_selected_rect, {20, 380 / g_scale}, 20/g_scale, RAYWHITE, "selected rect...");

}

GameUILayer::~GameUILayer() {
    //UnloadSound(button_sound);
    TraceLog(LOG_INFO, "UI DESTRUCTOR:  GAME UI");
}

void GameUILayer::Draw() {
    if(!g_game_settings.show_debug){
        return;
    }
    DrawLabelCentered(title_label);

    DrawLabel(debug_input_smp);
    DrawLabel(debug_input_wmp);
    DrawLabel(debug_input_mw);

    DrawLabel(debug_input_mm);
    DrawLabel(debug_input_mmd);

    DrawLabel(debug_input_mbl);
    DrawLabel(debug_input_mbld);
    DrawLabel(debug_input_mblscp);
    DrawLabel(debug_input_mblwcp);

    DrawLabel(debug_input_mbr);
    DrawLabel(debug_input_mbrd);
    DrawLabel(debug_input_mbrscp);
    DrawLabel(debug_input_mbrwcp);

    DrawLabel(debug_scale);
    DrawLabel(debug_zoom_level);
    DrawLabel(debug_camera_target);
    DrawLabel(debug_w2s);
    DrawLabel(debug_selected_rect);

}

void GameUILayer::Update() {

    if(!g_game_settings.show_debug){
        return;
    }

    debug_input_smp.text = TextFormat("smp %0.2f,%0.2f", g_input.screen_mouse_position.x, g_input.screen_mouse_position.y);
    debug_input_wmp.text = TextFormat("wmp %0.2f,%0.2f", g_input.world_mouse_position.x, g_input.world_mouse_position.y);

    debug_input_mw.text = TextFormat("mw %0.1f", g_input.mouse_wheel);

    debug_input_mm.text = TextFormat("mm %i", g_input.mouse_middle);
    debug_input_mmd.text = TextFormat("mmd %i", g_input.mouse_middle_down);

    debug_input_mbl.text = TextFormat("mbl %i", g_input.mouse_left);
    debug_input_mbld.text = TextFormat("mbld %i", g_input.mouse_left_down);
    debug_input_mblscp.text = TextFormat("mblscp %0.2f,%0.2f", g_input.mouse_left_clicked_screen_position.x, g_input.mouse_left_clicked_screen_position.y);
    debug_input_mblwcp.text = TextFormat("mblscp %0.2f,%0.2f", g_input.mouse_left_clicked_world_position.x, g_input.mouse_left_clicked_world_position.y);

    debug_input_mbr.text = TextFormat("mbr %i", g_input.mouse_right);
    debug_input_mbrd.text = TextFormat("mbrd %i", g_input.mouse_right_down);
    debug_input_mbrscp.text = TextFormat("mbrscp %0.2f,%0.2f", g_input.mouse_right_clicked_screen_position.x, g_input.mouse_right_clicked_screen_position.y);
    debug_input_mbrwcp.text = TextFormat("mbrwcp %0.2f,%0.2f", g_input.mouse_right_clicked_world_position.x, g_input.mouse_right_clicked_world_position.y);

    debug_scale.text = TextFormat("scale %0.2f", g_scale);
    debug_zoom_level.text = TextFormat("zoom %0.2f", g_camera.zoom);
    debug_camera_target.text = TextFormat("camera target %0.2f %0.2f", g_camera.target.x, g_camera.target.y);
    debug_w2s.text = TextFormat("w2s %0.4f", g_world2screen);
    debug_selected_rect.text = TextFormat("selected rect {%0.2f, %0.2f, %0.2f, %0.2f}", g_input.selected_rect.x, g_input.selected_rect.y, g_input.selected_rect.width, g_input.selected_rect.height);

}