#include "core/gamedefs.h"

std::unordered_map<int, PlayerData> g_class_data;

GameData g_game_data;
std::vector<MapData> g_map_data;

Texture2D g_tile_sheets[MAX_TILE_SHEETS];
Texture2D g_sprite_sheets[MAX_SPRITE_SHEETS];
Texture2D g_portrait_sprites[MAX_PORTRAIT_SPRITES];
Texture2D g_ui_panels[MAX_UI_PANELS];
Texture2D g_ui_backgrounds[MAX_UI_BACKGROUNDS];
PlayerData g_player_data;

PlayerCharacter *g_current_player;

bool game_running;
float g_scale;
float g_world2screen;
float g_screen2world;

Vector2 g_resolution;
Vector2 g_screen_center;


void Game::StartGame() {

    LoadResources();
    LoadGameData();

    g_player_data = g_class_data[0];
    g_current_player = nullptr;
    
    game_running = true;
    
    SetTextureFilter(render_texture.texture, TEXTURE_FILTER_BILINEAR);
    
    float scaleX = (float)GetScreenWidth() / g_game_settings.resolution.x;
    float scaleY = (float)GetScreenHeight() / g_game_settings.resolution.y;
    
    g_scale = (scaleX < scaleY) ? scaleX : scaleY;
    g_resolution = g_game_settings.resolution;
    g_screen_center = { g_resolution.x/2, g_resolution.y/2 };
    
    int offsetX = (GetScreenWidth() - (g_resolution.x*g_scale)) / 2;
    int offsetY = (GetScreenHeight() - (g_resolution.y*g_scale)) / 2;
      
    render_texture = LoadRenderTexture(g_resolution.x, g_resolution.y);

    scene_manager.Init();

    while(game_running) {

        scene_manager.UpdateScene();
        
        //draw everything to the render texture
        BeginTextureMode(render_texture);
        scene_manager.DrawScene();
        
        EndTextureMode();

        //do render stuff
        BeginDrawing();
        ClearBackground(BLACK);

        
        DrawTexturePro(
            render_texture.texture,
            (Rectangle){ 0, 0, (float)render_texture.texture.width, -(float)render_texture.texture.height },
            (Rectangle){ (float)offsetX, (float)offsetY, g_resolution.x*g_scale, g_resolution.y*g_scale },
            (Vector2){0, 0}, 0.0f, WHITE
        );
        EndDrawing();

        if(WindowShouldClose()) {
            game_running = false;
        }
    }
}

void Game::CleanUp() {
    scene_manager.CleanUp();
    UnloadResources();
    TraceLog(LOG_INFO, "cleaning up game.");
}