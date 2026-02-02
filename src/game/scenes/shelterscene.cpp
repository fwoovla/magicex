#include "../../core/gamedefs.h"

#define MAX_ZOOM 2.4f
#define MIN_ZOOM 1.8f
#define ZOOM_STEP 0.20f


ShelterScene::ShelterScene() {

    g_input.world_mouse_position.x = 100;
    g_input.world_mouse_position.y = 100;


    //ClearSubLevelData();

    scene_id = SHELTER_SCENE;
    return_scene = NO_SCENE;
    character_menu_visible = false;
    module_menu_visible = false;
    dialogue_menu_visible = false;
    
    LoadLevelData(level_data);

    WaitTime(1);

    InstanceLevelObjects(level_data);


    for(int area_index = 0; area_index < level_data.game_areas.size(); area_index++) {
        if (auto* transition_area = dynamic_cast<TransitionArea*>(level_data.game_areas[area_index].get())) {
            transition_area->area_entered.Connect( [this](){OnTransitionAreaEntered();} );
            transition_area->area_activated.Connect( [this](){OnTransitionAreaActivated();} );

        }
    }
    for(int entity_index = 0; entity_index < level_data.entity_list.size(); entity_index++) {

        if (auto* container_entity = dynamic_cast<BaseContainerEntity*>(level_data.entity_list[entity_index].get())) {
            container_entity->open_container.Connect( [this](){OnContainerOpened();} );
        }

        if (auto* module_entity = dynamic_cast<ModuleEntity*>(level_data.entity_list[entity_index].get())) {
            module_entity->open_module.Connect( [this](){OnModuleUsed();} );
        }
        if (auto* npc_entity = dynamic_cast<NpcEntity*>(level_data.entity_list[entity_index].get())) {
            npc_entity->start_dialogue.Connect( [this](){OnStartDialogue();} );
        }
    }

    ui_layer = new GameUILayer();
    ui_layer->quit_pressed.Connect( [this](){OnQuitPressed();} );
    
    tile_layer = new TileLayer();

    character_menu = new CharacterMenu();

    module_menu = new ModuleMenu();

    dialogue_menu = new DialogueMenu();

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
    //TraceLog(LOG_INFO, "shelter update");
    if(show_map_menu == true) {
        map_menu->Update();
    }
    else {
        if(character_menu_visible) {
            character_menu->Update();
        }
        else if(module_menu_visible) {
            module_menu->Update();
        }
        else if(dialogue_menu_visible) {
            dialogue_menu->Update();
        }
        else {
            ui_layer->Update();
            for(int i = 0; i < level_data.game_areas.size(); i++) {
                level_data.game_areas[i]->Update();
            } 
            DL_Update(level_data.entity_list);
            DL_Update(level_data.spell_list);
            DL_Update(level_data.ui_entities);
            DL_Update(level_data.environment_entities);
            g_current_player->Update();
            HandleCamera();
        }



        if(g_input.keys_pressed[0] == KEY_E and !module_menu_visible and !dialogue_menu_visible) {
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
                                //TraceLog(LOG_INFO, "found item  %i", spi);
                                break;
                            }
                        }
                    }
                    if(spi != -1) {
                        TraceLog(LOG_INFO, "dropping item  %i", spi);
                        std::unique_ptr<GroundContainerEntity> new_container = std::make_unique<GroundContainerEntity>(pos, spi);
                        new_container->c_area.area_activated.Connect( [this](){OnContainerOpened();} );
                        new_container->identifier = "GroundContainerEntity";
                        new_container->c_area.identifier = "GroundContainerEntity";
                        new_container->c_area.position = pos;
                        new_container->c_area.item_list = character_menu->blank_list;
                        new_container->c_area.size = {8, 8};
                        new_container->iid = character_menu->default_iid;
                        new_container->is_persistant = true;
                        new_container->level_index = g_game_data.current_map_index;
                        DL_Add(level_data.entity_list, std::move(new_container));
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
        if(g_input.keys_pressed[0] == KEY_E and module_menu_visible) {
            module_menu_visible = false;
        }
        if(g_input.keys_pressed[0] == KEY_E and dialogue_menu_visible) {
            dialogue_menu_visible = false;
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

    DL_Draw(level_data.ui_entities);
    //LDTKDrawShadows(g_current_player->position);
    


    EndMode2D();
}


void ShelterScene::DrawUI() {

    

    if(show_map_menu == true) {
        map_menu->Draw();
    }
    else if (module_menu_visible) {
        module_menu->Draw();
    }
    else if(character_menu_visible) {
        character_menu->Draw();
    }
    else if(dialogue_menu_visible) {
        dialogue_menu->Draw();
    }
    else {
        for(int i = 0; i < level_data.game_areas.size(); i++) {
            level_data.game_areas[i]->Draw();
        }
        DL_DrawUI(level_data.entity_list);
        //DL_DrawUI(level_data.ui_entities);
        ui_layer->Draw();
        character_menu->DrawHotBarOnly();
    }
    //DrawCircleV(g_camera.target, 5, RED);
}


ShelterScene::~ShelterScene() {
    TraceLog(LOG_INFO, "SCENE DESTRUCTOR CALLED:  SHELTER return scene %i", return_scene);
    //SaveGame();
    delete ui_layer;
    delete character_menu;
    delete tile_layer;
    delete module_menu;
    delete dialogue_menu;
    
    ClearLevelData(level_data);
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
    //return_scene = GAME_SCENE;
    //TraceLog(LOG_INFO, "MAP SELECTED:  %i", g_game_data.current_map_index);
}


void ShelterScene::OnTransitionAreaEntered() {
    TraceLog(LOG_INFO, "TRANSITION ENTERED:  %i", g_game_data.current_map_index);
    //return_scene = GAME_SCENE;

}

void ShelterScene::OnTransitionAreaActivated() {
    TraceLog(LOG_INFO, "TRANSITION ACTIVATED:  %i", g_game_data.current_map_index);
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

    g_camera.target = Vector2Subtract( g_current_player->position, {x_offset_f, y_offset_f} );

}


void ShelterScene::OnContainerOpened() {
    if(character_menu_visible) {
        return;
    }
    //TraceLog(LOG_INFO, "OPENNING CONTAINER");

    character_menu->OpenWith(g_game_data.return_container);
    character_menu_visible = true;

}

void ShelterScene::OnModuleUsed() {
    if(module_menu_visible) {
        return;
    }
    //TraceLog(LOG_INFO, "OPENNING CONTAINER");

    module_menu->OpenModule();
    module_menu_visible = true;

}

void ShelterScene::OnStartDialogue() {
    TraceLog(LOG_INFO, "STARTING DIALOGUE");
    dialogue_menu-> Open();
    dialogue_menu_visible = true;
}