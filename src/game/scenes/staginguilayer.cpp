#include "../../core/gamedefs.h"

StagingUILayer::StagingUILayer() {

    show_weapons = false;
    show_spells = false;
    show_food = false;
        
    CreateLabel(title_label, {g_screen_center.x, 20 / g_scale}, 40/g_scale, BLACK, "staging");
    
    CreateButton(settings_button, {g_resolution.x - 80, 10}, {120/g_scale , 30/g_scale}, YELLOW, "settings");
    settings_button.default_color = DARKYELLOW;
    settings_button.text_size = FONTSIZE_30;
    

    CreateButton(quit_button, {g_resolution.x - 10, 10}, {30/g_scale , 30/g_scale}, RED, "X");
    quit_button.default_color = DARKRED;
    quit_button.text_size = FONTSIZE_30;
    

    Vector2 start_but_size = {200/g_scale , 100/g_scale};
    Vector2 start_but_pos = {g_screen_center.x, g_resolution.y -50};
    CreateButton(start_button, start_but_pos, start_but_size, GREEN, "go!");
    start_button.default_color = DARKGREEN;
    start_button.text_size = FONTSIZE_50;
    
    //=============character info===============
    cpo = {100, 100};
    character_bounding_rect = {
        10,
        10,
        180,
        g_resolution.y-20
    };

    LoadSpriteCentered(character_panel_sprite, g_ui_panels[PANEL_WOOD_LARGE], cpo);
    LoadSpriteCentered(character_sprite, g_character_sprite_sheets[SPRITE_NERD], {cpo.x, cpo.y - 20}, 4, 16.0f, 0.10f);
    ScaleSprite(character_sprite, {3,3});

    CreateLabel(character_label,{cpo.x, cpo.y+25}, FONTSIZE_30, RAYWHITE, g_class_data[SPRITE_NERD].class_name.c_str());
    CreateLabel(character_stat_label,{20, cpo.y+85}, FONTSIZE_30, RAYWHITE, "");
    
    
    //=======================select panel=====================
    spo = {g_screen_center.x , g_screen_center.y - (100/g_scale) };

    select_index = 0;
    is_selecting = true;

    LoadSpriteCentered(select_character_panel_sprite, g_ui_panels[PANEL_WOOD_LARGE], spo);
    LoadSpriteCentered(select_chatacter_sprite, g_character_sprite_sheets[0], {spo.x, spo.y - 20}, 4, 16.0f, 0.10f);
    ScaleSprite(select_chatacter_sprite, {3,3});

    CreateButton(select_chatacter_button, start_but_pos, start_but_size, GREEN, "select");
    select_chatacter_button.default_color = DARKGREEN;
    select_chatacter_button.text_size = FONTSIZE_30; 

    CreateLabel(select_character_label,{spo.x, spo.y+25}, FONTSIZE_30, RAYWHITE, g_class_data[SPRITE_NERD].class_name.c_str());

    CreateButton(select_character_left_button, {spo.x - 20 - select_character_panel_sprite.size.x/2 , spo.y}, {30/g_scale , 100/g_scale}, YELLOW, "<");
    select_character_left_button.default_color = DARKYELLOW;
    select_character_left_button.text_size = FONTSIZE_30; 

    CreateButton(select_character_right_button, {spo.x + 20 + select_character_panel_sprite.size.x/2 , spo.y}, {30/g_scale , 100/g_scale}, YELLOW, ">");
    select_character_right_button.default_color = DARKYELLOW;
    select_character_right_button.text_size = FONTSIZE_30; 

//=============================starting equipment=================================



    tw_offset = {g_screen_center.x, 50};
    ts_offset = {g_screen_center.x, 225};
    tf_offset = {g_screen_center.x, 400};

    inventory_grid = new ItemGrid(5, 6, 50, {tw_offset.x - 275, tw_offset.y + 50}, &shared_data);

    int choice_index = 0;
    std::vector<int> choices;

    choices.clear();
    CreateLabel(take_weapon_label,{tw_offset.x, tw_offset.y}, FONTSIZE_40, RAYWHITE, "TAKE A WEAPON");
    take_weapon_rect = {
        .x = tw_offset.x - 10,
        .y = tw_offset.y - 10,
        .width = 450,
        .height = 150
    };

    for(int index = 0; index < g_weapon_data.size(); index++) {
        if(g_weapon_data[index].weapon_id == ITEM_ID_WAND or g_weapon_data[index].weapon_id == ITEM_ID_STAFF) {
            choices.push_back(g_weapon_data[index].weapon_id);
        }
    }

    for(int i = 0; i <= 3; i++) {
        int choice =  choices[GetRandomValue(0, choices.size() - 1)];
        TraceLog(LOG_INFO, "w selected index: %i", choice);
        weapon_ids.push_back(g_weapon_data[choice].weapon_id);
        std::unique_ptr<Button> new_button = std::make_unique<Button>();
        
        CreateButton(*new_button, {tw_offset.x + 70 + (100 * i), tw_offset.y + 75}, {50, 50}, YELLOW, g_weapon_data[choice].weapon_name.c_str() );
        new_button->text_size = FONTSIZE_14;

        take_weapon_buttons.push_back(std::move(new_button));
    }

    choices.clear();
    CreateLabel(take_food_label,{tf_offset.x, tf_offset.y}, FONTSIZE_40, RAYWHITE, "TAKE SOME FOOD");
        take_food_rect = {
        .x = tf_offset.x - 10,
        .y = tf_offset.y - 10,
        .width = 450,
        .height = 150
    };
    for(auto food : g_food_data) {
        choices.push_back(food.food_id);
    }
    for(int i = 0; i <= 3; i++) {
        choice_index = GetRandomValue(0, choices.size() - 1);
        TraceLog(LOG_INFO, "f selected item: %i", choice_index);
        food_ids.push_back(g_food_data[choice_index].food_id);
        std::unique_ptr<Button> new_button = std::make_unique<Button>();
        
        CreateButton(*new_button, {tf_offset.x + 70 + (100 * i), tf_offset.y + 75}, {50, 50}, YELLOW, g_food_data[choice_index].food_name.c_str() );
        new_button->text_size = FONTSIZE_14;

        take_food_buttons.push_back(std::move(new_button));
    }
}

StagingUILayer::~StagingUILayer() {
    //UnloadSound(button_sound);
    TraceLog(LOG_INFO, "UI DESTRUCTOR:  STAGING UI");
}

void StagingUILayer::Draw() {
    //DrawLabelCentered(title_label);

    DrawButton(settings_button);
    DrawButton(quit_button);
    
/*     DrawCircleV(tw_offset, 2, RED);
    DrawCircleV(ts_offset, 2, RED);
    DrawCircleV(tf_offset, 2, RED); */
    
    if(is_selecting) {
        DrawSelectPanel();
    }

    else {
        DrawRectangleRec(take_weapon_rect, TRANSDARKERGRAY);
        DrawRectangleRec(take_spell_rect, TRANSDARKERGRAY);
        DrawRectangleRec(take_food_rect, TRANSDARKERGRAY );

        DrawCharacterInfo();
        if(show_weapons) { 
            DrawLabel(take_weapon_label);
            for(int i = 0; i < take_weapon_buttons.size(); i++) {
                DrawButton(*take_weapon_buttons[i]);
            }
        }

/*         if(show_spells) {
            DrawLabel(take_spell_label);
            for(int i = 0; i < take_weapon_buttons.size(); i++) {
                DrawButton(*take_spell_buttons[i]);
            }
        } */

        if(show_food) {
            DrawLabel(take_food_label);
            for(int i = 0; i < take_weapon_buttons.size(); i++) {
                DrawButton(*take_food_buttons[i]);
            }
        }

        inventory_grid->DrawGrid();
        inventory_grid->DrawItems();
    }
}

void StagingUILayer::Update() {

    if(IsButtonHovered(settings_button, g_scale)){
        if(settings_button.already_hovered == false) {
            //PlaySound(button_sound);
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            TraceLog(LOG_INFO, "SETTINGS BUTTON PRESSED ");
            //play_pressed.EmitSignal();
            settings_pressed.EmitSignal();
        }        
    }

    if(IsButtonHovered(quit_button, g_scale)){
        if(quit_button.already_hovered == false) {
            //PlaySound(button_sound);
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            TraceLog(LOG_INFO, "QUIT BUTTON PRESSED ");
            //play_pressed.EmitSignal();
            quit_pressed.EmitSignal();
        }        
    }

    if(is_selecting) {
        UpdateSelectPanel();
        
    }
    else {
        UpdateCharacterInfo();
        
    }
}

void StagingUILayer::UpdateSelectPanel() {
    select_character_label.text = g_class_data[select_index].class_name;
    character_label.text = g_class_data[select_index].class_name;

    if(IsButtonHovered(select_chatacter_button, g_scale)){
        if(select_chatacter_button.already_hovered == false) {
            //PlaySound(button_sound);
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            TraceLog(LOG_INFO, "CHARACTER SELECTED UI");
            is_selecting = false;

            show_weapons = true;
            show_spells = true;
            show_food = true;

            start_button.position.x = character_sprite.position.x;
            //play_pressed.EmitSignal();
            character_selected.EmitSignal();
        }        
    }

    if(IsButtonHovered(select_character_left_button, g_scale)){
        if(select_character_left_button.already_hovered == false) {
            //PlaySound(button_sound);
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            TraceLog(LOG_INFO, "PREVIOUS CHARACTER");
            select_index--;
            if(select_index  < 0) {
                select_index = g_class_data.size() - 1;
            }
            //TraceLog(LOG_INFO, "index: %i", select_index);
            LoadSpriteCentered(select_chatacter_sprite, g_character_sprite_sheets[select_index], {spo.x, spo.y - 20}, 4, 16.0f, 0.10f);
            ScaleSprite(select_chatacter_sprite, {3,3});

            LoadSpriteCentered(character_sprite, g_character_sprite_sheets[select_index], {cpo.x, cpo.y - 20}, 4, 16.0f, 0.10f);
            ScaleSprite(character_sprite, {3,3});
            //play_pressed.EmitSignal();
            character_left_pressed.EmitSignal();
        }        
    }

    if(IsButtonHovered(select_character_right_button, g_scale)){
        if(select_character_right_button.already_hovered == false) {
            //PlaySound(button_sound);
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            TraceLog(LOG_INFO, "NEXT CHARACTER");
            select_index++;
            if(select_index  > g_class_data.size() - 1) {
                select_index = 0;
            }
            //TraceLog(LOG_INFO, "index: %i", select_index);
            LoadSpriteCentered(select_chatacter_sprite, g_character_sprite_sheets[select_index], {spo.x, spo.y - 20}, 4, 16.0f, 0.10f);
            ScaleSprite(select_chatacter_sprite, {3,3});

            LoadSpriteCentered(character_sprite, g_character_sprite_sheets[select_index], {cpo.x, cpo.y - 20}, 4, 16.0f, 0.10f);
            ScaleSprite(character_sprite, {3,3});
            //play_pressed.EmitSignal();
            character_right_pressed.EmitSignal();
        }        
    }
}

void StagingUILayer::UpdateCharacterInfo() {
    if(IsButtonHovered(start_button, g_scale)){
        if(start_button.already_hovered == false) {
                //PlaySound(button_sound);
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            TraceLog(LOG_INFO, "PLAY BUTTON PRESSED ");
            //play_pressed.EmitSignal();
            play_pressed.EmitSignal();
        }        
    }

    if(show_weapons) {

        for(int i = 0; i <  take_weapon_buttons.size(); i++)
        {
            if(IsButtonHovered(*take_weapon_buttons[i], g_scale)){
                if(take_weapon_buttons[i]->already_hovered == false) {
                    //PlaySound(button_sound);
                }
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    show_weapons = false;
                    ItemSelected(weapon_ids[i]);
                }        
            }
        }
    }

    if(show_food) {
        for(int i = 0; i <  take_food_buttons.size(); i++) {
            if(IsButtonHovered(*take_food_buttons[i], g_scale)){
                if(take_food_buttons[i]->already_hovered == false) {
                    //PlaySound(button_sound);
                }
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    show_food = false;
                    ItemSelected(food_ids[i]);
                }        
            }
        }
    } 
        
    character_stat_label.text = TextFormat("health: %i\nspeed: %0.2f\nexp: %i", g_class_data[select_index].health, g_class_data[select_index].base_speed, g_class_data[select_index].exp);
}

void StagingUILayer::DrawSelectPanel() {
    DrawSprite(select_character_panel_sprite);
    DrawSprite(select_chatacter_sprite);
    DrawButton(select_chatacter_button);
    //DrawLabelCentered(select_character_label);
    DrawButton(select_character_right_button);
    DrawButton(select_character_left_button);
}

void StagingUILayer::DrawCharacterInfo() {
    DrawRectangleRec(character_bounding_rect, DARKERGRAY);
    DrawRectangleLines( character_bounding_rect.x, character_bounding_rect.y,character_bounding_rect.width,character_bounding_rect.height, RAYWHITE);
    DrawSprite(character_panel_sprite);
    DrawSprite(character_sprite);
    //DrawLabelCentered(character_label);
    DrawLabel(character_stat_label);
    DrawButton(start_button);
}


void StagingUILayer::SetPlayer(int uid) {
    inventory_grid->SetItems(&g_character_data[uid].inventory);
}

void StagingUILayer::ItemSelected(int item_id) {
    TraceLog(LOG_INFO, "selected item: %i", item_id);
    ItemInstanceData * inst = InstanceCharacterItem((ItemID) item_id, g_current_player->uid);
    TraceLog(LOG_INFO, "selected item type: %i", inst->type);
    if(inst->type == TYPE_WEAPON) {
        GenerateWeapon( *inst, 0, true);
    }
    SetPlayer(g_current_player->uid);
}


void StagingUILayer::SpellSelected(int spell_id) {
/*     TraceLog(LOG_INFO, "selected spell: %i", spell_id);

    //create scroll
    for(auto inst_id : g_current_player->data->inventory) {
        auto i_itter =  g_item_instances.find(inst_id);
        if(i_itter != g_item_instances.end()) {
            if(i_itter->second.type == TYPE_WEAPON) {
                //found weapon to add spell to 
                AddSpellToItem( i_itter->second, (SpellID)spell_id);
            }
        }

    }
    //ItemInstanceData *new_instance =  InstanceCharacterItem(ITEM_ID_SCROLL, g_current_player->uid);
    //GenerateScroll(*new_instance, (SpellID)spell_id, "");
    SetPlayer(g_current_player->uid); */
}