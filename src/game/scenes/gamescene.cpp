#include "../../core/gamedefs.h"
/* 
#define MAX_ZOOM 3.8f
#define MIN_ZOOM 0.05f
#define ZOOM_STEP 0.05f */

GameScene::GameScene()
{

    scene_id = GAME_SCENE;
    return_scene = NO_SCENE;
    character_menu_visible = false;
    module_menu_visible = false;

    // ClearLevelData(level_data);
    LoadLevelData(level_data);
    InstanceLevelObjects(level_data);

    for (int area_index = 0; area_index < level_data.game_areas.size(); area_index++)
    {
        if (auto *transition_area = dynamic_cast<TransitionArea *>(level_data.game_areas[area_index].get()))
        {
            if (level_data.game_areas[area_index]->identifier == "LevelTransition")
            {
                transition_area->area_entered.Connect([this]()
                                                      { OnMapTransitionEntered(); });
                transition_area->area_activated.Connect([this]()
                                                        { OnMapTransitionActivated(); });
            }
            if (level_data.game_areas[area_index]->identifier == "ShelterTransition")
            {
                transition_area->area_entered.Connect([this]()
                                                      { OnShelterTransitionEntered(); });
                transition_area->area_activated.Connect([this]()
                                                        { OnShelterTransitionActivated(); });
            }
            if (level_data.game_areas[area_index]->identifier == "HouseTransition")
            {
                transition_area->area_entered.Connect([this]()
                                                      { OnHouseTransitionEntered(); });
                transition_area->area_activated.Connect([this]()
                                                        { OnHouseTransitionActivated(); });
            }
        }
    }
    for (int entity_index = 0; entity_index < level_data.entity_list.size(); entity_index++)
    {

        if (auto *container_entity = dynamic_cast<BaseContainerEntity *>(level_data.entity_list[entity_index].get())) {
            container_entity->open_container.Connect([this](){ OnContainerOpened(); });
        }

        if (auto *module_entity = dynamic_cast<ModuleEntity *>(level_data.entity_list[entity_index].get())) {
            module_entity->open_module.Connect([this](){ OnModuleUsed(); });
        }
        if (auto* npc_entity = dynamic_cast<NpcEntity*>(level_data.entity_list[entity_index].get())) {
            npc_entity->start_dialogue.Connect( [this](){OnStartDialogue();} );
        }
        if (auto* door_entity = dynamic_cast<DoorEntity*>(level_data.entity_list[entity_index].get())) {
            door_entity->open_door.Connect( [this](){OnDoorOpened();} );
        }
    }

    tile_layer = new TileLayer();

    ui_layer = new GameUILayer();
    ui_layer->quit_pressed.Connect([this]()
                                   { OnQuitPressed(); });

    character_menu = new CharacterMenu();
    character_menu->Open();

    module_menu = new ModuleMenu();

    dialogue_menu = new DialogueMenu();

    g_current_player->position = level_data.spawn_position;

    g_camera = {0};
    g_camera.target = Vector2Subtract(g_current_player->position, {200, 200});
    g_camera.offset = (Vector2){0, 0};
    g_camera.rotation = 0.0f;
    g_camera.zoom = 1.8f;
    g_world2screen = (g_scale * g_camera.zoom);
}

SCENE_ID GameScene::Update()
{
    if (can_delete_sub)
    {
        g_sub_scene.reset();
        can_delete_sub = false;
    }

    if (g_game_data.is_in_sub_map)
    {
        g_sub_scene->Update();
    }
    else if (module_menu_visible)
    {
        module_menu->Update();
    }
    else
    {
        if (character_menu_visible)
        {
            character_menu->Update();
        }
        else
        {
            // TraceLog(LOG_INFO, "GAME SCENE UPDATE");
            for (int i = 0; i < level_data.game_areas.size(); i++)
            {
                level_data.game_areas[i]->Update();
            }
            g_current_player->Update();
            DL_Update(level_data.entity_list);
            DL_Update(level_data.spell_list);
            DL_Update(level_data.ui_entities);

            // maybe divide envirinmental entities into groups for better preformance
            DL_Update(level_data.environment_entities);
            //----------------------- performance hit at > 2000 env entities onscreen
            //-----------------------

            ui_layer->Update();
            // HandleCamera();
            HandleCamera4();
        }

        if (g_input.keys_pressed[0] == KEY_E and !module_menu_visible and !dialogue_menu_visible)
        {
            character_menu_visible = !character_menu_visible;
            if (character_menu_visible)
            { // open

                character_menu->Open();
            }
            else
            { // closed
                if (character_menu->use_ground)
                { // was picked off ground
                    SpawnGroundContainer(g_current_player->position, character_menu->blank_list);
                }
                else
                { // was existing container
                    // if ground container
                    if (g_game_data.return_container != nullptr)
                    {
                        if (g_game_data.return_container->identifier == "GroundContainerEntity" or g_game_data.return_container->identifier == "Mushroom")
                        {
                            // if empty
                            if (g_game_data.return_container->IsEmpty())
                            {
                                g_game_data.return_container->should_delete = true;
                            }
                        }
                    }
                }
            }
        }
        if (g_input.keys_pressed[0] == KEY_E and module_menu_visible)
        {
            module_menu_visible = false;
        }
        if (g_input.keys_pressed[0] == KEY_E and dialogue_menu_visible)
        {
            dialogue_menu_visible = false;
        }
    }
    YSortEntities(level_data);
    return return_scene;
}

void GameScene::Draw()
{
}

void GameScene::DrawScene()
{

    if (g_game_data.is_in_sub_map)
    {
        // TraceLog(LOG_INFO, "SUB GAME SCENE DRAW");
        g_sub_scene->DrawScene();
    }
    else
    {
        // TraceLog(LOG_INFO, "GAME SCENE DRAW, %i", g_game_data.current_map_index);

        BeginMode2D(g_camera);
        g_debug_data.tiles_drawn = LDTKDrawMap(g_current_player->position);

        for (auto e : level_data.draw_list)
        {
            e->Draw();
        }
        g_debug_data.entities_drawn = level_data.draw_list.size();

        // LDTKDrawShadows(g_current_player->position);
        DL_Draw(level_data.ui_entities);
        EndMode2D();
    }
}

void GameScene::DrawUI()
{

    if (g_game_data.is_in_sub_map)
    {
        // TraceLog(LOG_INFO, "SUB GAME SCENE DRAW");
        g_sub_scene->DrawUI();
    }
    else if (module_menu_visible)
    {
        module_menu->Draw();
    }
    else if (character_menu_visible)
    {
        character_menu->Draw();
    }
    else if (dialogue_menu_visible)
    {
        dialogue_menu->Update();
    }
    else
    {
        for (int i = 0; i < level_data.game_areas.size(); i++)
        {
            level_data.game_areas[i]->Draw();
        }
        DL_DrawUI(level_data.entity_list);
        ui_layer->Draw();
        // character_menu->DrawHotBarOnly();
    }
}

GameScene::~GameScene()
{

    delete ui_layer;
    delete character_menu;
    delete tile_layer;
    delete module_menu;
    delete dialogue_menu;
    g_sub_scene.reset();

    TraceLog(LOG_INFO, "\n\n");
    std::vector<int> item_ids_to_delete;

    for (auto &item : g_item_instances)
    {
        std::string container_id = item.second.container_id;

        for (auto &sub_container : g_sub_temp_containers)
        {
            if (item.second.container_id == sub_container.first)
            {
                item_ids_to_delete.push_back(item.second.instance_id);
            }
        }
    }

    for (int instance_id : item_ids_to_delete)
    {
        TraceLog(LOG_INFO, "deleting sub item instance %i", instance_id);
        g_item_instances.erase(instance_id);
    }

    g_sub_temp_containers.clear();

    ClearLevelData(level_data);

    TraceLog(LOG_INFO, "SCENE DESTRUCTOR:  GAME");
    TraceLog(LOG_INFO, "\n\n");
}

void GameScene::OnQuitPressed()
{
    g_game_data.paused = true;
}

void GameScene::OnMapTransitionEntered()
{

    // TraceLog(LOG_INFO, "TRANSITION ACTIVATED:  %i", g_game_data.current_map_index);
    // return_scene = GAME_SCENE;
}

void GameScene::OnMapTransitionActivated()
{

    // TraceLog(LOG_INFO, "TRANSITION ACTIVATED:  %i", g_game_data.current_map_index);
    return_scene = GAME_SCENE;
}

void GameScene::OnShelterTransitionEntered()
{

    // TraceLog(LOG_INFO, "TRANSITION ACTIVATED:  %i", g_game_data.current_map_index);
    // return_scene = SHELTER_SCENE;
}

void GameScene::OnShelterTransitionActivated()
{

    // TraceLog(LOG_INFO, "TRANSITION ACTIVATED:  %i", g_game_data.current_map_index);
    g_game_data.using_saved_data = true;
    return_scene = SHELTER_SCENE;
}

void GameScene::OnHouseTransitionEntered()
{

    // TraceLog(LOG_INFO, "SUB MAP TRANSITION ACTIVATED:  %i", g_game_data.sub_map_index);
}

void GameScene::OnHouseTransitionActivated()
{

    TraceLog(LOG_INFO, "\n\n");
    TraceLog(LOG_INFO, "SUB MAP TRANSITION ACTIVATED:  ss map index %i    ss uid %s", g_game_data.sub_map_index, g_game_data.sub_map_uid.c_str());
    TraceLog(LOG_INFO, "+ player reset position %0.0f, %0.0f", g_game_data.sub_return_position.x, g_game_data.sub_return_position.y);
    g_game_data.is_in_sub_map = true;
    g_game_data.using_saved_data = false;

    auto it = g_sub_scene_state.find(g_game_data.sub_map_uid);

    if (it != g_sub_scene_state.end())
    {
        TraceLog(LOG_INFO, "SUB MAP INSTANCE FOUND:  uid %s", g_game_data.sub_map_uid.c_str());

        g_game_data.using_saved_data = true;

        g_sub_scene = std::make_unique<SubScene>(*g_sub_scene_state[g_game_data.sub_map_uid].get(), false);

        g_sub_scene->sub_scene_exited.Connect([this]()
                                              { OnSubSceneExited(); });
    }

    else
    {
        TraceLog(LOG_INFO, "NEW SUB MAP INSTANCE :  uid %s", g_game_data.sub_map_uid.c_str());

        g_sub_scene_state[g_game_data.sub_map_uid] = std::make_unique<SubSceneState>();

        g_sub_scene = std::make_unique<SubScene>(*g_sub_scene_state[g_game_data.sub_map_uid].get(), true);

        g_sub_scene->sub_scene_exited.Connect([this]()
                                              { OnSubSceneExited(); });
    }

    // TraceLog(LOG_INFO, "SUB MAP TRANSITION ACTIVATED:  ss instances %i", g_sub_scene_state.size());
    TraceLog(LOG_INFO, "\n\n");
}

void GameScene::OnSubSceneExited()
{
    TraceLog(LOG_INFO, "\n\n");
    TraceLog(LOG_INFO, "SUB MAP EXITED %i", g_game_data.current_map_index);

    // g_sub_scene_data[g_game_data.sub_map_uid] = &g_sub_scene->level_data;

    g_current_player->position = g_game_data.sub_return_position;

    g_camera.zoom = 1.4f;

    character_menu->Open();

    can_delete_sub = true;
    g_game_data.is_in_sub_map = false;
    g_game_data.using_saved_data = false;

    TraceLog(LOG_INFO, "+ reset player position %0.0f, %0.0f", g_game_data.sub_return_position.x, g_game_data.sub_return_position.y);
    TraceLog(LOG_INFO, "\n\n");
}

void GameScene::OnContainerOpened()
{
    if (character_menu_visible)
    {
        return;
    }
    TraceLog(LOG_INFO, "OPENNING CONTAINER");

    character_menu->OpenWith(g_game_data.return_container);
    character_menu_visible = true;
}


void GameScene::OnDoorOpened() {
    TraceLog(LOG_INFO, "DOOR OPENED");
}




void GameScene::OnModuleUsed()
{
    if (module_menu_visible)
    {
        return;
    }
    // TraceLog(LOG_INFO, "OPENNING CONTAINER");

    module_menu->OpenModule();
    module_menu_visible = true;
}

void GameScene::OnStartDialogue()
{
    TraceLog(LOG_INFO, "STARTING DIALOGUE");
    dialogue_menu->Open();
    dialogue_menu_visible = true;
}