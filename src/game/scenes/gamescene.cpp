#include "../../core/gamedefs.h"

#define MAX_ZOOM 1.8f
#define MIN_ZOOM 1.5f
#define ZOOM_STEP 0.05f


GameScene::GameScene() {

    scene_id = GAME_SCENE;
    return_scene = NO_SCENE;
    character_menu_visible = false;

    sub_scene = nullptr;


    //ClearLevelData(level_data);
    LoadLevelData(level_data);
    InstanceLevelObjects(level_data);


    for(int area_index = 0; area_index < level_data.game_areas.size(); area_index++) {
        if(level_data.game_areas[area_index]->identifier == "LevelTransition") {
            TransitionArea* t_area = dynamic_cast<TransitionArea*>(level_data.game_areas[area_index]);
            //TraceLog(LOG_INFO, "+ connect map");
            t_area->area_entered.Connect( [this](){OnMapTransitionEntered();} );
            t_area->area_activated.Connect( [this](){OnMapTransitionActivated();} );
        }

        if(level_data.game_areas[area_index]->identifier == "ShelterTransition") {
            TransitionArea* t_area = dynamic_cast<TransitionArea*>(level_data.game_areas[area_index]);
            //TransitionArea* t_area = dynamic_cast<TransitionArea*>( g_level_data.game_areas[area_index]);
            //TraceLog(LOG_INFO, "+ connect shelter");
            t_area->area_entered.Connect( [this](){OnShelterTransitionEntered();} );
            t_area->area_activated.Connect( [this](){OnShelterTransitionActivated();} );
        }
        if(level_data.game_areas[area_index]->identifier == "HouseTransition") {
            TransitionArea* t_area = dynamic_cast<TransitionArea*>(level_data.game_areas[area_index]);
            //TraceLog(LOG_INFO, "+ connect house");

            t_area->area_entered.Connect( [this](){OnHouseTransitionEntered();} );
            t_area->area_activated.Connect( [this](){OnHouseTransitionActivated();} );
        }

    }
    for(int entity_index = 0; entity_index < level_data.entity_list.size(); entity_index++) {
        if(level_data.entity_list[entity_index]->identifier == "PermContainerEntity" or 
            level_data.entity_list[entity_index]->identifier == "GroundContainerEntity" or 
            level_data.entity_list[entity_index]->identifier == "Mushroom") {
             TraceLog(LOG_INFO, "container area identified  %s", level_data.entity_list[entity_index]->identifier.c_str());
            BaseContainerEntity* p_entity = dynamic_cast<BaseContainerEntity*>(level_data.entity_list[entity_index]);
            if(p_entity) {
                TraceLog(LOG_INFO, "container connected");
                p_entity->open_container.Connect( [this](){OnContainerOpened();} );
            }
        }
    }

    tile_layer = new TileLayer();
    
    ui_layer = new GameUILayer();
    ui_layer->quit_pressed.Connect( [this](){OnQuitPressed();} );


    character_menu = new CharacterMenu();
    character_menu->Open();

    g_current_player->position = level_data.spawn_position;

    g_camera = { 0 };
    g_camera.target = (Vector2){0,0};
    g_camera.offset = (Vector2){0,0};
    g_camera.rotation = 0.0f;
    g_camera.zoom = 1.4f; 
    g_world2screen = (g_scale * g_camera.zoom);
}


SCENE_ID GameScene::Update() {
    //TraceLog(LOG_INFO, " UPDATE");
    if(can_delete_sub) {
        delete sub_scene;
        sub_scene = nullptr;
        can_delete_sub = false;
    }

    if(g_game_data.is_in_sub_map) {
        sub_scene->Update();
    }
    else {
        if(character_menu_visible) {
            character_menu->Update();
        }
        else {
            //TraceLog(LOG_INFO, "GAME SCENE UPDATE");
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
                        GroundContainerEntity *new_container = new GroundContainerEntity(pos, spi);
                        DL_Add(level_data.entity_list, new_container);
                        new_container->c_area.area_activated.Connect( [this](){OnContainerOpened();} );
                        new_container->identifier = "GroundContainerEntity";
                        new_container->c_area.identifier = "GroundContainerEntity";
                        new_container->c_area.position = pos;
                        new_container->c_area.item_list = character_menu->blank_list;
                        new_container->c_area.size = {8, 8};    
                        //level_data.game_areas.push_back(&new_container->c_area);
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

    return return_scene;
}

void GameScene::Draw() {
}

void GameScene::DrawScene() {

    if(g_game_data.is_in_sub_map) {
        //TraceLog(LOG_INFO, "SUB GAME SCENE DRAW");
        sub_scene->DrawScene();
    }
    else {
        BeginMode2D(g_camera);
        tile_layer->Draw();
        DL_Draw(level_data.entity_list);
        g_current_player->Draw();
        LDTKDrawShadows(g_current_player->position);
        LDTKDrawForegroundLayer(g_current_player->position);
        EndMode2D();
    }    
}

void GameScene::DrawUI() {

    if(g_game_data.is_in_sub_map) {
        //TraceLog(LOG_INFO, "SUB GAME SCENE DRAW");
        sub_scene->DrawUI();
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

void GameScene::HandleCamera() {

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

    if(g_current_player->position.x - x_offset_f < 0) {
        float x_dif = x_offset_f - g_current_player->position.x;
        //TraceLog(LOG_INFO, "x_dif %0.2f   %0.2f, %0.2f", x_dif, g_camera.target.x, g_camera.target.x);
        g_camera.target.x = g_camera.target.x + x_dif;
        //g_camera.target.x = (int)g_camera.target.x;
    }
    else if(g_current_player->position.x + x_offset_f > g_ldtk_maps.levels[g_game_data.current_map_index].px_wid) {
        float x_dif = (x_offset_f + g_current_player->position.x) - g_ldtk_maps.levels[g_game_data.current_map_index].px_wid;
        //TraceLog(LOG_INFO, "x_dif %0.2f   %0.2f, %0.2f", x_dif, g_camera.target.x, g_camera.target.y);
         g_camera.target.x = g_camera.target.x - x_dif;
        //g_camera.target.x = (int)g_camera.target.x;
    }

    if(g_current_player->position.y - y_offset_f < 0) {
        float y_dif = y_offset_f - g_current_player->position.y;
        //TraceLog(LOG_INFO, "y_dif %0.2f   %0.2f, %0.2f", y_dif, g_camera.target.y, g_camera.target.y);
        g_camera.target.y = (int)g_camera.target.y + y_dif;
        //g_camera.target.y = (int)g_camera.target.y;
    }
    else if(g_current_player->position.y + y_offset_f > g_ldtk_maps.levels[g_game_data.current_map_index].px_hei) {
        float y_dif = (y_offset_f + g_current_player->position.y) - g_ldtk_maps.levels[g_game_data.current_map_index].px_hei;
        //TraceLog(LOG_INFO, "y_dif %0.2f   %0.2f, %0.2f", y_dif, g_camera.target.y, g_camera.target.y);
        g_camera.target.y = g_camera.target.y - y_dif;
        //g_camera.target.y = (int)g_camera.target.y;

    }

    
}


GameScene::~GameScene() {

    ClearLevelData(level_data);
    delete ui_layer;
    delete character_menu;
    delete tile_layer;
    //DL_Clear(entity_draw_list);
    
    TraceLog(LOG_INFO, "SCENE DESTRUCTOR:  GAME");
}

void GameScene::OnQuitPressed() {
    g_game_data.paused = true;
}


void GameScene::OnMapTransitionEntered() {

    //TraceLog(LOG_INFO, "TRANSITION ACTIVATED:  %i", g_game_data.current_map_index);
    //return_scene = GAME_SCENE;

}

void GameScene::OnMapTransitionActivated() {

    //TraceLog(LOG_INFO, "TRANSITION ACTIVATED:  %i", g_game_data.current_map_index);
    return_scene = GAME_SCENE;

}

void GameScene::OnShelterTransitionEntered() {

    //TraceLog(LOG_INFO, "TRANSITION ACTIVATED:  %i", g_game_data.current_map_index);
    //return_scene = SHELTER_SCENE;

}

void GameScene::OnShelterTransitionActivated() {

    //TraceLog(LOG_INFO, "TRANSITION ACTIVATED:  %i", g_game_data.current_map_index);
    return_scene = SHELTER_SCENE;

}

void GameScene::OnHouseTransitionEntered() {

    //TraceLog(LOG_INFO, "SUB MAP TRANSITION ACTIVATED:  %i", g_game_data.sub_map_index);

}

void GameScene::OnHouseTransitionActivated() {

    //TraceLog(LOG_INFO, "SUB MAP TRANSITION ACTIVATED:  %i", g_game_data.sub_map_index);

    g_game_data.is_in_sub_map = true;
    sub_scene = new SubScene();
    sub_scene->sub_scene_exited.Connect( [this](){OnSubSceneExited();} );

}

void GameScene::OnSubSceneExited() {
    TraceLog(LOG_INFO, "SUB MAP EXITED %i", g_game_data.current_map_index);
    g_game_data.is_in_sub_map = false;

    g_current_player->position = g_game_data.sub_return_position;
    TraceLog(LOG_INFO, "+ reset player position");

    g_camera.zoom = 1.0f; 
    
    character_menu->Open();

    can_delete_sub = true;
}


void GameScene::OnContainerOpened() {
    if(character_menu_visible) {
        return;
    }
    TraceLog(LOG_INFO, "OPENNING CONTAINER");

    character_menu->OpenWith(g_game_data.return_container);
    character_menu_visible = true;
}