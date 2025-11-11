#include "core/gamedefs.h"

std::unordered_map<int, PlayerData> g_class_data;
std::unordered_map<int, ItemData> g_item_data;

std::vector<std::vector<int>> g_loot_tables;

GameData g_game_data;
DebugData g_debug_data;

LDTKMaps g_ldtk_maps;
std::unordered_map<int, TileSheetData> g_ldtk_tilesheets;
std::vector<LDTKTileset> g_ldtk_tilesets;

//LevelData g_level_data;

Texture2D g_tile_sheets[MAX_TILE_SHEETS];
Texture2D g_sprite_sheets[MAX_SPRITE_SHEETS];
Texture2D g_item_sprites[1000];
Texture2D g_container_sprites[MAX_SPRITE_SHEETS];
Texture2D g_portrait_sprites[MAX_PORTRAIT_SPRITES];
Texture2D g_ui_panels[MAX_UI_PANELS];
Texture2D g_ui_backgrounds[MAX_UI_BACKGROUNDS];
PlayerData g_player_data;

PlayerCharacter *g_current_player;

bool game_running;
float g_scale;
float g_inv_scale;
float g_world2screen;
float g_screen2world;

Vector2 g_resolution;
Vector2 g_screen_center;

//std::vector<BaseEntity *> active_entity_list;
//std::vector<std::unique_ptr<BaseEntity>> entity_draw_list;

int g_map_width;
int g_map_height;
Camera2D g_camera;
PlayerInput g_input;

Viewport g_viewport;

Cursor g_cursor;


void Game::StartGame() {
    g_game_data.save_available = false;
    g_game_data.paused = false;
    g_game_data.current_scene_id = NO_SCENE;
    g_game_data.current_map_index = -1;
    g_game_settings.show_debug = false;

    LoadResources();
    LoadGameData();

    g_player_data = g_class_data[0];
    g_current_player = nullptr;
    
    game_running = true;
    
    SetTextureFilter(scene_render_texture.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(ui_render_texture.texture, TEXTURE_FILTER_BILINEAR);
    
    float scaleX = (float)GetScreenWidth() / g_game_settings.resolution.x;
    float scaleY = (float)GetScreenHeight() / g_game_settings.resolution.y;
    
    g_scale = (scaleX < scaleY) ? scaleX : scaleY;
    g_inv_scale = 1/g_scale;
    g_resolution = g_game_settings.resolution;
    g_screen_center = { g_resolution.x/2, g_resolution.y/2 };
    
    int offsetX = (GetScreenWidth() - (g_resolution.x*g_scale)) / 2;
    int offsetY = (GetScreenHeight() - (g_resolution.y*g_scale)) / 2;

    g_world2screen = g_camera.zoom * g_scale;
    g_screen2world = 1/g_world2screen;
      
    scene_render_texture = LoadRenderTexture(g_resolution.x, g_resolution.y);
    ui_render_texture = LoadRenderTexture(g_resolution.x, g_resolution.y);
    //ui_render_texture = LoadRenderTexture(g_game_settings.window_size.x, g_game_settings.window_size.y);

    SetTargetFPS(60);

    scene_manager.Init();


    while(game_running) {

        scene_manager.UpdateScene();
        
        //draw scene
        BeginTextureMode(scene_render_texture);
        ClearBackground(BLANK);
        scene_manager.DrawScene();
        EndTextureMode();

        //ui
        BeginTextureMode(ui_render_texture);
        ClearBackground(BLANK);
        scene_manager.DrawUI();
        EndTextureMode();


        BeginDrawing();


        ClearBackground(BLANK);

        
        DrawTexturePro(
            scene_render_texture.texture,
            (Rectangle){ 0, 0, (float)scene_render_texture.texture.width, -(float)scene_render_texture.texture.height },
            (Rectangle){ (float)offsetX, (float)offsetY, g_resolution.x*g_scale, g_resolution.y*g_scale },
            (Vector2){0, 0}, 0.0f, WHITE
        );

        //DrawRectangleRec( (Rectangle){ 0, 0, (float)ui_render_texture.texture.width, -(float)ui_render_texture.texture.height }, WHITE);
        DrawTexturePro(
            ui_render_texture.texture,
            (Rectangle){ 0, 0, (float)ui_render_texture.texture.width, -(float)ui_render_texture.texture.height },
            (Rectangle){ (float)offsetX, (float)offsetY, g_resolution.x*g_scale, g_resolution.y*g_scale },
            (Vector2){0, 0}, 0.0f, WHITE
        );


        EndDrawing();

        if(g_game_settings.show_debug == true) {
            TraceLog(LOG_INFO, "dt %f", GetFrameTime());
            TraceLog(LOG_INFO, "dt tile draw %f", g_debug_data.tile_dt);
            TraceLog(LOG_INFO, "fps %i\n", GetFPS());
        }

        if(WindowShouldClose()) {
            game_running = false;
        }
    }
}

void Game::CleanUp() {
    scene_manager.CleanUp();
    UnloadResources();
    TraceLog(LOG_INFO, "cleaning up game.");
    if(IsWindowFullscreen()) {  
        ToggleFullscreen();
    } 
}