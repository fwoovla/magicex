#include "../../core/gamedefs.h"

#define MAX_ZOOM 2.0f
#define MIN_ZOOM 0.80f
#define ZOOM_STEP 0.20f



SubScene::SubScene(SubSceneState &sub_state, bool is_new) {
    scene_id = SUB_SCENE;
    return_scene = NO_SCENE;
    character_menu_visible = false;
    module_menu_visible = false;


    //ClearLevelData();
    if(is_new) {
        LoadLevelData(level_data);
        InstanceLevelObjects(level_data);
    }
    else {
        LoadSubSceneState( sub_state, level_data);
        LoadLevelData(level_data);
        InstanceLevelObjects(level_data);
    }

    TraceLog(LOG_INFO, "SPAWN POSITION, %0.02f %0.02f", level_data.spawn_position.x, level_data.spawn_position.y);


    for(int area_index = 0; area_index < level_data.game_areas.size(); area_index++) {
        if (auto* transition_area = dynamic_cast<TransitionArea*>(level_data.game_areas[area_index].get())) {
            if(level_data.game_areas[area_index]->identifier == "LevelTransition") {
                transition_area->area_entered.Connect( [this](){OnMapTransitionEntered();} );
                transition_area->area_activated.Connect( [this](){OnMapTransitionActivated();} );

            }
        }
    }
    for(int entity_index = 0; entity_index < level_data.entity_list.size(); entity_index++) {

        if (auto* container_entity = dynamic_cast<BaseContainerEntity*>(level_data.entity_list[entity_index].get())) {
            container_entity->open_container.Connect( [this](){OnContainerOpened();} );
        }

        if (auto* module_entity = dynamic_cast<ModuleEntity*>(level_data.entity_list[entity_index].get())) {
            module_entity->open_module.Connect( [this](){OnModuleUsed();} );
        }

    }



    ui_layer = new GameUILayer();
    ui_layer->quit_pressed.Connect( [this](){OnQuitPressed();} );
    
    tile_layer = new TileLayer();

    character_menu = new CharacterMenu();
    character_menu->Open();

    module_menu = new ModuleMenu();
    
    g_current_player->position = level_data.spawn_position;

    g_camera = { 0 };
    g_camera.target = (Vector2){0,0};
    g_camera.offset = (Vector2){0,0};
    g_camera.rotation = 0.0f;
    g_camera.zoom = 2.4f; 
    g_world2screen = (g_scale * g_camera.zoom);
}


SCENE_ID SubScene::Update() {
    //TraceLog(LOG_INFO, "SUB SCENE UPDATE, %i", g_game_data.current_map_index);

    if(character_menu_visible) {
        character_menu->Update();
    }
    else if(module_menu_visible) {
            module_menu->Update();
    }
    else {
        for(int i = 0; i < level_data.game_areas.size(); i++) {
            level_data.game_areas[i]->Update();
        }
        DL_Update(level_data.entity_list);
        DL_Update(level_data.spell_list);
        ui_layer->Update();
        DL_Update(level_data.ui_entities);
        DL_Update(level_data.environment_entities);
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
                            spi = g_item_data[ character_menu->blank_list[item]].id;
                            break;
                        }
                    }
                    if(spi != -1) {
                        std::unique_ptr<GroundContainerEntity> new_container = std::make_unique<GroundContainerEntity>(pos, spi);
                        DL_Add(level_data.entity_list, std::move(new_container));
                        new_container->c_area.area_activated.Connect( [this](){OnContainerOpened();} );
                        new_container->identifier = "GroundContainerEntity";
                        new_container->c_area.identifier = "GroundContainerEntity";
                        new_container->c_area.position = pos;
                        new_container->c_area.item_list = character_menu->blank_list;
                        new_container->c_area.size = {8, 8};    
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
    YSortEntities(level_data);
    return return_scene;
}

void SubScene::Draw() {
}


void SubScene::DrawScene() {
    //TraceLog(LOG_INFO, "SUB SCENE DRAW, %i", g_game_data.current_map_index);
    BeginMode2D(g_camera);
    LDTKDrawMap(g_current_player->position);

    for(auto e: level_data.draw_list) {
        e->Draw();
    }

    LDTKDrawShadows(g_current_player->position);
    DL_Draw(level_data.ui_entities);
    EndMode2D();
}


void SubScene::DrawUI() {
    if(character_menu_visible) {
        character_menu->Draw();
    }
    else if (module_menu_visible) {
        module_menu->Draw();
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


SubScene::~SubScene() {
    sub_scene_exited.DisconnectAll();

    delete ui_layer;
    delete tile_layer;
    delete character_menu;
    delete module_menu;

    SubSceneState state;
    SaveSubSceneState(level_data, state);

    
    for(int entity_index = 0; entity_index < level_data.entity_list.size(); entity_index++) {
        if (auto* container_entity = dynamic_cast<BaseContainerEntity*>(level_data.entity_list[entity_index].get())) {
            if(container_entity->is_persistant) {

                json container = {
                    {"iid", container_entity->iid},
                    {"is_persistant", container_entity->is_persistant},
                    {"identifier", container_entity->identifier},
                    {"loot_table_id", container_entity->loot_table_id},
                    {"position_x", container_entity->position.x},
                    {"position_y", container_entity->position.y},
                    {"size_x", container_entity->c_area.size.x},
                    {"size_y", container_entity->c_area.size.y},
                    {"sprite_id", container_entity->sprite_id},
                    {"level_index", container_entity->level_index},
                };

                ContainerData container_data = container.get<ContainerData>();
                g_sub_temp_containers[container_data.iid] = container_data;
            }
        }
    }



    g_sub_scene_state[g_game_data.sub_map_uid] = std::make_unique<SubSceneState>(state);

    ClearLevelData(level_data);

    TraceLog(LOG_INFO, "SCENE DESTRUCTOR:  SUB SCENE");
}

void SubScene::OnQuitPressed() {
    g_game_data.paused = true;
}

void SubScene::OnMapTransitionEntered() {


    //TraceLog(LOG_INFO, "SUB TRANSITION ACTIVATED:  %i", g_game_data.sub_map_index);
}

void SubScene::OnMapTransitionActivated() {
    TraceLog(LOG_INFO, "MAP TRANSITION ACTIVATED:  %i", g_game_data.sub_map_index);
    
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
    TraceLog(LOG_INFO, "OPENNING CONTAINER");

    character_menu->OpenWith(g_game_data.return_container);
    character_menu_visible = true;
}

void SubScene::OnModuleUsed() {
    if(module_menu_visible) {
        return;
    }
    //TraceLog(LOG_INFO, "OPENNING CONTAINER");

    module_menu->OpenModule();
    module_menu_visible = true;

}