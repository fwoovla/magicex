#include "../core/gamedefs.h"

char level1_data[] = "assets/map4.png";

void SceneManager::UpdateScene()
{
    GetInputFromPlayer();
    if(g_current_scene == nullptr){
        return;
    }


    SetCursorPosition(g_input.screen_mouse_position / g_scale);
    //SetCursorPosition(g_input.screen_mouse_position);



    SCENE_ID new_scene_id = NO_SCENE;
        
    if(g_game_data.paused == false) {
        new_scene_id = g_current_scene->Update();
    }
    else {
        pause_menu->Update();
    }
    if(!is_transitioning) {
        switch (new_scene_id) {
            case NO_SCENE:
                break;

            default:
            TransitionSceneTo(new_scene_id);
        }
    }

    if(g_input.keys_pressed[0] == KEY_P) {
        g_game_data.paused = !g_game_data.paused;
    }

    if(g_input.keys_pressed[0] == KEY_TAB) {
        g_game_settings.show_debug = !g_game_settings.show_debug;
    }

    if(is_transitioning) {
        fade_transition->Update();
    }

    if(g_game_settings.show_debug) {
        debug_label.text = TextFormat( "player pos ff %0.2f %0.2f \n player cell %i %i \n cti %i %i \ncamera target %f %f \nfps %i", 
        g_current_player->position.x, g_current_player->position.y,
        (int)(g_current_player->position.x / 16), (int)(g_current_player->position.y / 16),
        (int)g_camera.target.x, (int)g_camera.target.y,
        g_camera.target.x, g_camera.target.y,
        GetFPS()
    );
    }


}


void SceneManager::Init() {
    g_current_scene = std::make_unique<SplashScreen>();

    g_game_data.current_scene_id = SPLASH_SCENE;

    is_transitioning = false;

    pause_menu = new PauseMenu();
    pause_menu->continue_pressed.Connect( [&](){OnPausePressed();} );
    pause_menu->save_pressed.Connect( [&](){OnSavePressed();} );
    pause_menu->back_to_menu_pressed.Connect( [&](){OnBackToMenuPressed();} );

    LoadSpriteCentered( g_cursor.sprite, g_ui_sprites[UI_ID_CROSSHAIR], {0,0} );
    CreateLabel(debug_label, {20 / g_scale}, 40/g_scale, RAYWHITE, "DEBUG");
}


void SceneManager::CleanUp() {
    //delete current_scene;
    //delete pause_menu;
    TraceLog(LOG_INFO, "cleaning up scene manager");
}


void SceneManager::DrawScene() {

    g_current_scene->DrawScene();

}

void SceneManager::DrawUI() {

    g_current_scene->DrawUI();

    if(g_game_data.paused == true) {
        pause_menu->Draw();
    }
    DrawSprite(g_cursor.sprite);
    //BeginMode2D(g_camera);
    //EndMode2D();

    if(is_transitioning) {
        fade_transition->Draw();
    }
    if(g_game_settings.show_debug) {
        DrawLabel(debug_label);
    }
}


void SceneManager::ChangeSceneTo(SCENE_ID new_scene_id) {
    
    
    g_current_scene.reset();
    g_game_data.current_map_index = g_game_data.next_map_index;

    switch (new_scene_id) {
        case SPLASH_SCENE:
            g_current_scene = std::make_unique<SplashScreen>();
            break;

        case TITLE_SCENE:
            g_current_scene = std::make_unique<TitleScene>();
            break;

        case STAGING_SCENE:
            g_current_scene = std::make_unique<StagingScene>();
            break;

        case SHELTER_SCENE:
            g_current_scene = std::make_unique<ShelterScene>();
            break;

        case GAME_SCENE:
            g_current_scene = std::make_unique<GameScene>();
            break;

        case END_SCENE:
            g_current_scene = std::make_unique<EndScene>();
            break;

        default:
            break;
    }
    g_game_data.current_scene_id = new_scene_id;
}


void SceneManager::TransitionSceneTo(SCENE_ID new_scene_id) {
    next_scene_id = new_scene_id;
    is_transitioning = true;
    fade_transition = new FadeTransition(1);
    fade_transition->transition_ended.Connect( [&](){OnTransitionEnded();} );
    fade_transition->transition_midpoint.Connect( [&](){OnTransitionMidpoint();} );
}



void SceneManager::OnPausePressed() {
    g_game_data.paused = !g_game_data.paused;
}

void SceneManager::OnSavePressed() {
    SaveGame(g_current_scene->level_data);
    TraceLog(LOG_INFO, "GAME SAVED ");
}

void SceneManager::OnBackToMenuPressed() {
    //SaveGame();
    if (g_current_player != nullptr) {
        delete g_current_player;
    }
    g_game_data.paused = false;
    g_game_data.next_map_index = g_game_data.shelter_map_index;
    g_game_data.is_in_sub_map = false;
    ChangeSceneTo(TITLE_SCENE);
}


void SceneManager::OnTransitionMidpoint() {
    //TraceLog(LOG_INFO, "trans midpoint ");
    ChangeSceneTo(next_scene_id);
}

void SceneManager::OnTransitionEnded() {
    delete fade_transition;
    is_transitioning = false;
    //TraceLog(LOG_INFO, "trans destroy ");
}



void InstanceLevelObjects(LevelData &level_data) {

     bool is_shelter = false;
    if(g_game_data.current_map_index ==  g_game_data.shelter_map_index) {
        is_shelter = true;
    }

    TraceLog(LOG_INFO, "instacing game objects");

    TraceLog(LOG_INFO, " ||||||||||||||||||||||||||   g_item_instance before new level size %i", g_item_instances.size());
    
/*     TraceLog(LOG_INFO, "G_ITEM_INSTANCES ");
    for (const auto& [key, value] : g_item_instances) {
        TraceLog(LOG_INFO, "----item uid %i  item id %i", key, value.item_id);
    } */

//transition areas
    for(int t_index = 0; t_index < level_data.level_transitions.size(); t_index++) {
        Vector2 t_posisition = level_data.level_transitions[t_index].position_i;

        TransitionArea *new_area = new TransitionArea();

        new_area->identifier = level_data.level_transitions[t_index].identifier;
        new_area->position = t_posisition;
        new_area->size = {level_data.level_transitions[t_index].size.x, level_data.level_transitions[t_index].size.y};
        new_area->payload_s = level_data.level_transitions[t_index].dest_string;
        
        if(level_data.level_transitions[t_index].identifier == "HouseTransition") {
            new_area->payload_v = level_data.level_transitions[t_index].return_position;
        }

        for(int level_index = 0; level_index < g_ldtk_maps.levels.size(); level_index++) {
            if(g_ldtk_maps.levels[level_index].identifier == new_area->payload_s) {
                new_area->payload_i = level_index;
            }
        }
        //TraceLog(LOG_INFO, "instacing transition %s", new_area->identifier.c_str());
        level_data.game_areas.push_back(new_area);
    }

//containers
    for(int c_index = 0; c_index < level_data.container_data.size(); c_index++) {
        Vector2 pos = level_data.container_data[c_index].position_i;
        
        if(level_data.container_data[c_index].identifier == "PermContainerEntity") {
            TraceLog(LOG_INFO, "instacing perm container");
            int spi = level_data.container_data[c_index].sprite_id;
            int lti = level_data.container_data[c_index].loot_table_id;
            PermContainerEntity *new_container = new PermContainerEntity(pos, spi, lti);
            DL_Add(level_data.entity_list, new_container);
            
            new_container->is_persistant = is_shelter;
            new_container->identifier = level_data.container_data[c_index].identifier;
            new_container->iid = level_data.container_data[c_index].iid;
            new_container->loot_table_id = level_data.container_data[c_index].loot_table_id;
            new_container->level_index = g_game_data.current_map_index;

            new_container->c_area.identifier = level_data.container_data[c_index].identifier;
            new_container->c_area.position = pos;
            new_container->c_area.loot_table_id = level_data.container_data[c_index].loot_table_id;
            new_container->c_area.size = {level_data.container_data[c_index].size.x, level_data.container_data[c_index].size.y};
            //GenerateContainerItemList(lti, new_container->c_area.item_list);

            if(g_game_data.using_saved_data and g_game_data.current_map_index == g_game_data.shelter_map_index) {
                for (auto& [key, value] : g_item_instances) {
                    //TraceLog(LOG_INFO, "checking item %i\n   --%s  |  %s", key, value.container_id.c_str(), new_container->iid.c_str());
                    if(key, value.container_id == new_container->iid) {
                        //TraceLog(LOG_INFO, "ITEM FOUND");
                        new_container->c_area.item_list.push_back(value.instance_id);
                    }
                    //std::cout << key << " = " << value << "\n";
                }

            }
            else {
                InstanceItemList(g_loot_tables[new_container->loot_table_id], new_container->c_area.item_list, new_container->iid);
            }
        }


        if(level_data.container_data[c_index].identifier == "GroundContainerEntity") {
            TraceLog(LOG_INFO, "instacing ground container");
            int spi = ITEM_ID_ERROR;// g_item_data[ level_data.container_data[c_index].item_list[0]].id;

            GroundContainerEntity *new_container = new GroundContainerEntity(pos, spi);
            DL_Add(level_data.entity_list, new_container);
            
            new_container->is_persistant = is_shelter;
            new_container->identifier = level_data.container_data[c_index].identifier;
            new_container->iid = level_data.container_data[c_index].iid;
            new_container->level_index = g_game_data.current_map_index;

            new_container->c_area.identifier = level_data.container_data[c_index].identifier;
            new_container->c_area.position = pos;
            new_container->c_area.size = {level_data.container_data[c_index].size.x, level_data.container_data[c_index].size.y};
            
            if(g_game_data.using_saved_data and g_game_data.current_map_index == g_game_data.shelter_map_index) {
                for (auto& [key, value] : g_item_instances) {
                    //TraceLog(LOG_INFO, "checking item %i\n   --%s  |  %s", key, value.container_id.c_str(), new_container->iid.c_str());
                    if(key, value.container_id == new_container->iid) {
                        //TraceLog(LOG_INFO, "ITEM FOUND");
                        new_container->c_area.item_list.push_back(value.instance_id);
                        new_container->SetSprite(value.item_id);
                    }
                }
            }
            else {
                InstanceItemList(level_data.container_data[c_index].item_list, new_container->c_area.item_list, new_container->iid);
            }
            //TraceLog(LOG_INFO, "instacing ground container %i %s", spi, new_container->c_area.identifier.c_str());
        }
    }

//mushroom zones
    for(int zone = 0; zone < level_data.mushroom_zones.size(); zone++) {
        Vector2 zone_pos = level_data.mushroom_zones[zone].position_i;

        int num_mushrooms = GetRandomValue(1, level_data.mushroom_zones[zone].max_mushrooms);

        int min_x = level_data.mushroom_zones[zone].position_i.x;
        int min_y = level_data.mushroom_zones[zone].position_i.y;
        int max_x = level_data.mushroom_zones[zone].position_i.x + level_data.mushroom_zones[zone].size.x;
        int max_y = level_data.mushroom_zones[zone].position_i.y + level_data.mushroom_zones[zone].size.y;

        for(int i = 0; i < num_mushrooms; i++) {
            int x = GetRandomValue(min_x, max_x);
            int y = GetRandomValue(min_y, max_y);
            Vector2 pos = {(float)x,(float)y};

            MushroomEntity *new_mushroom = new MushroomEntity(pos);

            new_mushroom->identifier = "Mushroom";
            new_mushroom->c_area.identifier = "Mushroom";
            new_mushroom->c_area.position = Vector2Add(pos, {4,4});
            new_mushroom->c_area.size = {8,8};

            std::vector<int> temp_list;
            temp_list.push_back(ITEM_ID_MUSHROOM);
            InstanceItemList(temp_list, new_mushroom->c_area.item_list, "none");

            DL_Add(level_data.entity_list, new_mushroom);
            TraceLog(LOG_INFO, "instacing mushroom");
        }
    }

    TraceLog(LOG_INFO, "|||||||||||||||||||||||   g_item_instance size %i", g_item_instances.size());
/*     TraceLog(LOG_INFO, "G_ITEM_INSTANCES ");
    for (const auto& [key, value] : g_item_instances) {
        TraceLog(LOG_INFO, "----item uid %i  item id %i", key, value.item_id);
    } */
}

