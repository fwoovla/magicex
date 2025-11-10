#include "../../core/gamedefs.h"

#define MAX_ZOOM 2.4f
#define MIN_ZOOM 1.8f
#define ZOOM_STEP 0.20f


ShelterScene::ShelterScene() {
    SaveGame();
    scene_id = GAME_SCENE;
    return_scene = NO_SCENE;
    character_menu_visible = false;

    //ClearLevelData(level_data);
    LoadLevelData(level_data);
    InstanceLevelObjects(level_data);

    for(int area_index = 0; area_index < level_data.game_areas.size(); area_index++) {
        if(level_data.game_areas[area_index]->identifier == "LevelTransition") {
            TransitionArea* t_area = dynamic_cast<TransitionArea*>(level_data.game_areas[area_index]);

            t_area->area_entered.Connect( [this](){OnTransitionAreaEntered();} );
            t_area->area_activated.Connect( [this](){OnTransitionAreaActivated();} );
        }
        if(level_data.game_areas[area_index]->identifier == "ContainerEntity") {
            TraceLog(LOG_INFO, "container area identified");
            ContainerArea* c_area = dynamic_cast<ContainerArea*>(level_data.game_areas[area_index]);
            c_area->area_activated.Connect( [this](){OnContainerOpened();} );
        }
    }

    ui_layer = new ShelterUILayer();
    ui_layer->quit_pressed.Connect( [this](){OnQuitPressed();} );
    ui_layer->start_pressed.Connect( [this](){OnStartPressed();} );
    
    tile_layer = new TileLayer();

    character_menu = new CharacterMenu();
    character_menu->Open();
    
    map_menu = new MapMenu();
    map_menu->map_selected.Connect( [this](){OnMapSelected();} );

    show_map_menu = false;

    g_current_player->position = level_data.spawn_position;

    g_camera = { 0 };
    g_camera.target = (Vector2){0,0};
    g_camera.offset = (Vector2){0,0};
    g_camera.rotation = 0.0f;
    g_camera.zoom = 2.4f; 
    g_world2screen = (g_scale * g_camera.zoom);

    HideCursor();
}


SCENE_ID ShelterScene::Update() {

    if(show_map_menu == true) {
        map_menu->Update();
    }
    else {
        if(character_menu_visible) {
            character_menu->Update();
        }
        else {
            ui_layer->Update();
            for(int i = 0; i < level_data.game_areas.size(); i++) {
                level_data.game_areas[i]->Update();
            } 
            DL_Update(level_data.entity_list);
            g_current_player->Update();
            HandleCamera();
        }

        if(g_input.keys_pressed[0] == KEY_E) {
            character_menu_visible = !character_menu_visible;
            if(character_menu_visible) {
                std::vector<int> list;
                list.push_back(1);

                character_menu->Open();
            }
        }
    }
    return return_scene;
}

void ShelterScene::Draw() {
}


void ShelterScene::DrawScene() {

    BeginMode2D(g_camera);
    tile_layer->Draw();

/*     for(int i = 0; i < g_level_data.game_areas.size(); i++) {
        g_level_data.game_areas[i]->Draw();
    } */
    DL_Draw(level_data.entity_list);

    g_current_player->Draw();
    EndMode2D();
}


void ShelterScene::DrawUI() {


    if(show_map_menu == true) {
        map_menu->Draw();
    }
    else {
        if(character_menu_visible) {
            character_menu->Draw();
        }
        else {
            for(int i = 0; i < level_data.game_areas.size(); i++) {
                level_data.game_areas[i]->Draw();
            }
            ui_layer->Draw();
            character_menu->DrawHotBarOnly();
        }
    }
}


ShelterScene::~ShelterScene() {
    ClearLevelData(level_data);
    delete ui_layer;
    delete character_menu;
    delete tile_layer;
    delete map_menu;

    TraceLog(LOG_INFO, "SCENE DESTRUCTOR:  SHELTER");
}

void ShelterScene::OnQuitPressed() {
    g_game_data.paused = true;
}

void ShelterScene::OnStartPressed() {
    show_map_menu = true;
    //return_scene = GAME_SCENE;
}

void ShelterScene::OnMapSelected() {
    return_scene = GAME_SCENE;
    TraceLog(LOG_INFO, "MAP SELECTED:  %i", g_game_data.current_map_index);
}


void ShelterScene::OnTransitionAreaEntered() {
    TraceLog(LOG_INFO, "TRANSITION ENTERED:  %i", g_game_data.current_map_index);
    //return_scene = GAME_SCENE;

}

void ShelterScene::OnTransitionAreaActivated() {
    TraceLog(LOG_INFO, "TRANSITION ACTIVATED:  %i", g_game_data.current_map_index);
    return_scene = GAME_SCENE;
}


void ShelterScene::HandleCamera() {

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

    g_camera.target = Vector2Subtract(g_current_player->position, {x_offset_f, y_offset_f} );
}


void ShelterScene::OnContainerOpened() {
    if(character_menu_visible) {
        return;
    }
    TraceLog(LOG_INFO, "CONTAINER OPEN:  %i", g_game_data.loot_table_id);
    character_menu->OpenWith(*g_game_data.loot_table);
    character_menu_visible = true;
}