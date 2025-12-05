#include "../../core/gamedefs.h"

#define MAX_ZOOM 2.4f
#define MIN_ZOOM 1.8f
#define ZOOM_STEP 0.20f


ShelterScene::ShelterScene() {

    for (auto& [key, value] : g_sub_scene_data) {
        DL_Clear(g_sub_scene_data[key]->entity_list);
        g_sub_scene_data[key]->level_transitions.clear();
        g_sub_scene_data[key]->container_data.clear();
        g_sub_scene_data[key]->game_areas.clear();
    }

    g_sub_scene_data.clear();

    scene_id = SHELTER_SCENE;
    return_scene = NO_SCENE;
    character_menu_visible = false;
    
    LoadLevelData(level_data);
    WaitTime(1);
    InstanceLevelObjects(level_data);
    

    for(int area_index = 0; area_index < level_data.game_areas.size(); area_index++) {
        if(level_data.game_areas[area_index]->identifier == "LevelTransition") {
            TransitionArea* t_area = dynamic_cast<TransitionArea*>(level_data.game_areas[area_index]);

            t_area->area_entered.Connect( [this](){OnTransitionAreaEntered();} );
            t_area->area_activated.Connect( [this](){OnTransitionAreaActivated();} );
        }
    }
    for(int entity_index = 0; entity_index < level_data.entity_list.size(); entity_index++) {
        std::string identifier = level_data.entity_list[entity_index]->identifier;
        if(identifier == "PermContainerEntity" or identifier == "GroundContainerEntity" or identifier == "Mushroom") {
            TraceLog(LOG_INFO, "container area identified  %s", level_data.entity_list[entity_index]->identifier.c_str());
            BaseContainerEntity* p_entity = dynamic_cast<BaseContainerEntity*>(level_data.entity_list[entity_index]);

/*             if(identifier == "PermContainerEntity" or identifier == "GroundContainerEntity") {
                p_entity->is_persistant = true;
            } */
            if(p_entity) {
                TraceLog(LOG_INFO, "container connected");
                p_entity->open_container.Connect( [this](){OnContainerOpened();} );
            }
        }
    }

    ui_layer = new ShelterUILayer();
    ui_layer->quit_pressed.Connect( [this](){OnQuitPressed();} );
    ui_layer->start_pressed.Connect( [this](){OnStartPressed();} );
    
    tile_layer = new TileLayer();

    character_menu = new CharacterMenu();
    //character_menu->Open();
    
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

    //SaveGame(level_data);

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
            if(character_menu_visible) {  //open

                character_menu->Open();
            }
            else { //closed
                if(character_menu->use_ground) { //was picked off ground
                    int spi = -1;
                    Vector2 pos = g_current_player->position;
                    for(int item = 0; item < character_menu->blank_list.size(); item++) {
                        if(character_menu->blank_list[item] != -1) {

                            //dfgg

                            auto item_it = g_item_instances.find(character_menu->blank_list[item]);
                            if(item_it != g_item_instances.end()) {
                                spi = item_it->second.item_id;
                            }

                            break;
                        }
                    }
                    if(spi != -1) {
                        GroundContainerEntity *new_container = new GroundContainerEntity(pos, spi);
                        DL_Add(level_data.entity_list, new_container);
                        new_container->c_area.area_activated.Connect( [this](){OnContainerOpened();} );
                        new_container->identifier = "GroundContainerEntity";
                        new_container->c_area.identifier = "GroundContainerEntity";
                        new_container->c_area.position = pos;
                        new_container->c_area.item_list = character_menu->blank_list;
                        new_container->c_area.size = {8, 8};
                        new_container->iid = character_menu->default_iid;
                        new_container->is_persistant = true;
                        new_container->level_index = g_game_data.current_map_index;
                    }
                }
                else { //was existing container
                    //if ground container
                    if(g_game_data.return_container != nullptr) {
                        if(g_game_data.return_container->identifier == "GroundContainerEntity" or g_game_data.return_container->identifier == "Mushroom") {
                            //if empty
                            if(g_game_data.return_container->IsEmpty()) {
                                g_game_data.return_container->should_delete = true;
                            }
                        }
                    }
                }
            }
        }
    }

    YSortEntities(level_data);
    return return_scene;
}

void ShelterScene::Draw() {
}


void ShelterScene::DrawScene() {

    BeginMode2D(g_camera);
    //tile_layer->Draw();
    LDTKDrawMap(g_current_player->position);
    
    
    for(auto e: level_data.draw_list) {
        e->Draw();
    }

    LDTKDrawShadows(g_current_player->position);

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
            DL_DrawUI(level_data.entity_list);
            ui_layer->Draw();
            character_menu->DrawHotBarOnly();
        }
    }
}


ShelterScene::~ShelterScene() {
    TraceLog(LOG_INFO, "SCENE DESTRUCTOR CALLED:  SHELTER return scene %i", return_scene);
    //SaveGame();
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
    //TraceLog(LOG_INFO, "TRANSITION ACTIVATED:  %i", g_game_data.current_map_index);
    if(return_scene != GAME_SCENE) {
        SaveGame(level_data);
        return_scene = GAME_SCENE;
    }
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
    //TraceLog(LOG_INFO, "OPENNING CONTAINER");

    character_menu->OpenWith(g_game_data.return_container);
    character_menu_visible = true;

}