#include "../../core/gamedefs.h"

#define MAX_ZOOM 2.0f
#define MIN_ZOOM 0.80f
#define ZOOM_STEP 0.20f



SubScene::SubScene() {
    scene_id = SUB_SCENE;
    return_scene = NO_SCENE;
    character_menu_visible = false;


    //ClearLevelData();
    LoadLevelData(level_data);
    InstanceLevelObjects(level_data);

    for(int area_index = 0; area_index < level_data.game_areas.size(); area_index++) {
        if(level_data.game_areas[area_index]->identifier == "LevelTransition") {
            TransitionArea* t_area = dynamic_cast<TransitionArea*>(level_data.game_areas[area_index]);
            //TraceLog(LOG_INFO, "+ connect map");
            t_area->area_entered.Connect( [this](){OnMapTransitionEntered();} );
            t_area->area_activated.Connect( [this](){OnMapTransitionActivated();} );
        }
        if(level_data.game_areas[area_index]->identifier == "ContainerEntity") {
            TraceLog(LOG_INFO, "container area identified");
            ContainerArea* c_area = dynamic_cast<ContainerArea*>(level_data.game_areas[area_index]);
            c_area->area_activated.Connect( [this](){OnContainerOpened();} );
        }
    }

    ui_layer = new GameUILayer();
    ui_layer->quit_pressed.Connect( [this](){OnQuitPressed();} );
    
    tile_layer = new TileLayer();

    character_menu = new CharacterMenu();
    character_menu->Open();
    
    g_current_player->position = level_data.spawn_position;

    g_camera = { 0 };
    g_camera.target = (Vector2){0,0};
    g_camera.offset = (Vector2){0,0};
    g_camera.rotation = 0.0f;
    g_camera.zoom = 2.4f; 
    g_world2screen = (g_scale * g_camera.zoom);
}


SCENE_ID SubScene::Update() {
    //TraceLog(LOG_INFO, "SUB SCENE UPDATE, %i", g_game_data.sub_map_index);

    if(character_menu_visible) {
        character_menu->Update();
    }
    else {
        for(int i = 0; i < level_data.game_areas.size(); i++) {
            level_data.game_areas[i]->Update();
        }
        DL_Update(level_data.entity_list);
        ui_layer->Update(); 
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


    return return_scene;
}

void SubScene::Draw() {
}


void SubScene::DrawScene() {
    //TraceLog(LOG_INFO, "SUB SCENE DRAW");

    BeginMode2D(g_camera);
    tile_layer->Draw();
    DL_Draw(level_data.entity_list);
    g_current_player->Draw();

    EndMode2D();

}


void SubScene::DrawUI() {
    //TraceLog(LOG_INFO, "SUB SCENE DRAW");

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


SubScene::~SubScene() {
    delete ui_layer;
    delete tile_layer;
    delete character_menu;

    TraceLog(LOG_INFO, "SCENE DESTRUCTOR:  SUB SCENE");
}

void SubScene::OnQuitPressed() {
    g_game_data.paused = true;
}


void SubScene::OnMapTransitionEntered() {


    TraceLog(LOG_INFO, "SUB TRANSITION ACTIVATED:  %i", g_game_data.sub_map_index);
    //sub_scene_exited.EmitSignal();
    //return_scene = GAME_SCENE;

}

void SubScene::OnMapTransitionActivated() {


    TraceLog(LOG_INFO, "SUB TRANSITION ACTIVATED:  %i", g_game_data.sub_map_index);
    sub_scene_exited.EmitSignal();
}


void SubScene::HandleCamera() {

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


void SubScene::OnContainerOpened() {
    if(character_menu_visible) {
        return;
    }
    TraceLog(LOG_INFO, "CONTAINER OPEN:  %i", g_game_data.loot_table_id);
    character_menu->OpenWith(*g_game_data.loot_table);
    character_menu_visible = true;
}