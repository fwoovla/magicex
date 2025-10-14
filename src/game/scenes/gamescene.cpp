#include "../../core/gamedefs.h"

#define MAX_ZOOM 2.0f
#define MIN_ZOOM 0.5f
#define ZOOM_STEP 0.05f

std::vector<BaseEntity *> active_entity_list;
int *level_data_array;
int g_map_width;
int g_map_height;
Camera2D g_camera;
PlayerInput g_input;


GameScene::GameScene(char map_path[]) {
    scene_id = GAME_SCENE;
    return_scene = NO_SCENE;
    character_menu_visible = false;

    tile_layer = new TileLayer();
    

    ui_layer = new GameUILayer();
    character_menu = new CharacterMenu();

    Image level_image = LoadImage(map_path);
    level_data_array = new int[level_image.width * level_image.height];
    g_map_width = level_image.width;
    g_map_height = level_image.height;
    
    Color *colors = LoadImageColors(level_image);

    for(int x = 0; x < g_map_width; x++) {
        for(int y = 0; y < g_map_height; y++) {

            if(colors[y * g_map_width + x].r == 0 and colors[y * g_map_width + x].g == 0 and colors[y * g_map_width + x].b == 0) {
                level_data_array[y * g_map_width + x] = 0;
            }

            else if (colors[y * g_map_width + x].r == 0 and colors[y * g_map_width + x].g == 255 and colors[y * g_map_width + x].b == 0) {
                level_data_array[y * g_map_width + x] = 2;
            }
        }      
    }

    tile_layer->SetTiles( g_tile_sheets[TS_FOREST], {16,16});

    //ground_tiles = LoadTexture("assets/tiles.png");

    DL_Add(active_entity_list, new PlayerCharacter({100,100}) );

    g_camera = { 0 };
    g_camera.target = (Vector2){0,0};
    g_camera.offset = (Vector2){0,0};
    g_camera.rotation = 0.0f;
    g_camera.zoom = 1.0f; 
    g_world2screen = (g_scale * g_camera.zoom);
    
    UnloadImage(level_image);
}


SCENE_ID GameScene::Update() {
    //GetInputFromPlayer();
    if(character_menu_visible) {
        character_menu->Update();
    }

    ui_layer->Update();
    DL_Update(active_entity_list);
    HandleCamera();
    
    if(g_input.keys_pressed[0] == KEY_SPACE) {
        return_scene = END_SCENE;
    }
    if (IsKeyPressed(g_input.keys_pressed[0] ==  KEY_ENTER)) {
        g_game_settings.show_debug = !g_game_settings.show_debug;
    }
    
    
    if(g_input.keys_pressed[0] == KEY_TAB) {
        g_game_settings.show_debug = !g_game_settings.show_debug;
    }

    if(g_input.keys_pressed[0] == KEY_E) {
        character_menu_visible = !character_menu_visible;
    }

    return return_scene;
}

void GameScene::Draw() {

    DrawRectangle( 0,0, g_resolution.x, g_resolution.y, DARKERGRAY ); 
    BeginMode2D(g_camera);
    DrawLevel();
    //TraceLog(LOG_INFO, "ENTITY LIST SIZE  %i", active_entity_list.size());
    DL_Draw(active_entity_list);
    
    
    if(g_input.selecting) {
        DrawRectangleLines(g_input.selected_rect.x, g_input.selected_rect.y, g_input.selected_rect.width, g_input.selected_rect.height, RAYWHITE);
    }
    
    EndMode2D();
    
    ui_layer->Draw();
    if(character_menu_visible) {
        character_menu->Draw();
    }
}

void GameScene::DrawLevel() {

    tile_layer->Draw();

/*     for(int x = 0; x < g_map_width; x++) {
        for(int y = 0; y < g_map_height; y++) {
            /* DrawTexturePro(
                        ground_tiles,
                        {0.0, (float)level_data_array[y*g_map_width+x]*TILE_SIZE ,TILE_SIZE, TILE_SIZE},
                        {(float)x*TILE_SIZE, (float)y*TILE_SIZE ,TILE_SIZE, TILE_SIZE},
                        {0,0},
                        0.0,
                        WHITE
                    ); 
        }
    } */
}

void GameScene::HandleCamera() {

    Vector2 worldPosBeforeZoom = GetScreenToWorld2D(g_input.world_mouse_position, g_camera);

    g_camera.zoom += g_input.mouse_wheel * ZOOM_STEP;
    if(g_camera.zoom < MIN_ZOOM) {
        g_camera.zoom = MIN_ZOOM;
    }
    if(g_camera.zoom > MAX_ZOOM) {
        g_camera.zoom = MAX_ZOOM;
    }

    Vector2 worldPosAfterZoom = GetScreenToWorld2D(g_input.world_mouse_position, g_camera);

    // Adjust camera target to keep mouse position stable
    Vector2 delta = Vector2Subtract(worldPosBeforeZoom, worldPosAfterZoom);
    g_camera.target = Vector2Add(g_camera.target, delta);

    if (g_input.mouse_middle_down) {
            Vector2 delta = GetMouseDelta() * 0.5f;
            delta = Vector2Scale(delta, -1.0f / g_camera.zoom);
            g_camera.target = Vector2Add(g_camera.target, delta);
    }

    if(g_camera.target.x < 0) {g_camera.target.x = 0;}
    if(g_camera.target.x > 1000) {g_camera.target.x = 1000;}
    if(g_camera.target.y < 0) {g_camera.target.y = 0;}
    if(g_camera.target.y > 1000) {g_camera.target.y = 1000;}

    g_world2screen = g_camera.zoom * g_scale;
    g_screen2world = 1/g_world2screen;
}


GameScene::~GameScene() {
    delete ui_layer;
    delete tile_layer;
    delete level_data_array;
    //UnloadTexture(ground_tiles);
    DL_Clear(active_entity_list);
    TraceLog(LOG_INFO, "SCENE DESTRUCTOR:  GAME");
}