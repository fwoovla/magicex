#include "../../core/gamedefs.h"

#define MAX_ZOOM 2.4f
#define MIN_ZOOM 1.8f
#define ZOOM_STEP 0.20f

bool first_run = true;

ShelterScene::ShelterScene() {

    g_input.world_mouse_position.x = 100;
    g_input.world_mouse_position.y = 100;


    //ClearSubLevelData();

    scene_id = SHELTER_SCENE;
    return_scene = NO_SCENE;
    character_menu_visible = false;
    module_menu_visible = false;
    dialogue_menu_visible = false;
    spell_menu_visible = false;
    g_game_data.is_in_sub_map = false;
    
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
        if (auto* door_entity = dynamic_cast<DoorEntity*>(level_data.entity_list[entity_index].get())) {
            door_entity->open_door.Connect( [this](){OnDoorOpened();} );
        }
    }

    ui_layer = new GameUILayer();
    ui_layer->quit_pressed.Connect( [this](){OnQuitPressed();} );
    
    tile_layer = new TileLayer();

    character_menu = new CharacterMenu();

    module_menu = new ModuleMenu();

    dialogue_menu = new DialogueMenu();

    spell_menu = new SpellGenMeneu();

    show_map_menu = false;

    g_current_player->position = level_data.spawn_position;

    g_camera = { 0 };
    g_camera.target = g_current_player->position;
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
        else if(spell_menu_visible) {
            spell_menu->Update();
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

            if(!first_run) {
                HandleCamera4();
            }
            else { g_camera.target =   Vector2Subtract(g_current_player->position, {150, 150});}

        }



        if(g_input.keys_pressed[0] == KEY_E and !module_menu_visible and !dialogue_menu_visible and !spell_menu_visible) {
            character_menu_visible = !character_menu_visible;
            if(character_menu_visible) {  //open

                character_menu->Open();
            }
            else { //closed
                if(character_menu->use_ground) { //was picked off ground
                    SpawnGroundContainer(g_current_player->position, character_menu->blank_list);
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
        if(g_input.keys_pressed[0] == KEY_E and spell_menu_visible) {
            spell_menu_visible = false;
        }
    }

    YSortEntities(level_data);
    first_run = false;
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
    else if(spell_menu_visible) {
        spell_menu->Draw();
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


void ShelterScene::OnContainerOpened() {
    if(character_menu_visible) {
        return;
    }
    //TraceLog(LOG_INFO, "OPENNING CONTAINER");

    character_menu->OpenWith(g_game_data.return_container);
    character_menu_visible = true;

}

void ShelterScene::OnDoorOpened() {
    TraceLog(LOG_INFO, "DOOR OPENED %0.0f %0.0f", g_game_data.return_door_pos.x, g_game_data.return_door_pos.y);

    LDTKToggleColision(g_game_data.return_door_pos);

}

void ShelterScene::OnModuleUsed() {
    if(module_menu_visible) {
        return;
    }
    //TraceLog(LOG_INFO, "OPENNING CONTAINER");

    if(g_game_data.current_module_id != MODULE_ID_SPELLGENERATOR) {
        module_menu->OpenModule();
        module_menu_visible = true;
    }
    else {
        spell_menu->OpenModule(g_active_creature_data[g_current_player->uid].primary[0]);
        spell_menu_visible = true;
    }

}

void ShelterScene::OnStartDialogue() {
    TraceLog(LOG_INFO, "STARTING DIALOGUE");
    dialogue_menu-> OpenWith(g_game_data.return_npc);
    dialogue_menu_visible = true;
}