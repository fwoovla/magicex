#include "../../core/gamedefs.h"


CharacterMessage::CharacterMessage(Vector2 _position, std::string _text, Color _color, float _delay_seconds) {
    position = _position;
    //sprite_id = _sprite_id;
    rotation = 0.0f;
    y_sort = false;

    collision_radius = 5;
    centered_offset = {0,0};
    collided = false;
   
    should_delete = false;
    is_persistant = false;
    is_obstructable = true;
    is_obstructed = false;
    can_take_damage = false;
    delay_seconds = _delay_seconds;
    current_seconds = 0.0f;

    CreateLabel(label, _position, FONTSIZE_14, _color, _text);
    TraceLog(LOG_INFO, "CharacterMessage ENTITY CREATED");
}

CharacterMessage::~CharacterMessage() {

    TraceLog(LOG_INFO, "CharacterMessage ENTITY DESTROYED");
}

void CharacterMessage::Update() {
    if(should_delete) {
        return;
    }
    current_seconds += GetFrameTime();
    if(current_seconds >= delay_seconds) {
        should_delete = true;
    }

    label.position.y -= 50 *GetFrameTime();
    //label.default_color.a -=  5 * GetFrameTime();
    
}

void CharacterMessage::Draw() {
    DrawLabelCentered(label);
    //DrawCircleV(sprite.position, 2, WHITE);

}

void CharacterMessage::DrawUI() {
    DrawLabelCentered(label);
}

float CharacterMessage::GetYSort() {
    return position.y;
}

void CharacterMessage::TakeDamage(DamagePayload _payload) {

}










//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++







int CalculateDamage(DamagePayload &damage_payload, CharacterData &character_data) {
    int damage = damage_payload.damage;
    //do stuff
    return damage;
}











void SpawnSpell(NewSpellPayload payload, SpellData *_data) {

    BaseScene *this_scene = nullptr;
    std::unique_ptr<BaseSpell> this_spell = nullptr;

    if(g_game_data.is_in_sub_map) {
        this_scene = g_sub_scene.get();
    }
    else {
        this_scene = g_current_scene.get();
    }

    if(this_scene) {
        if(payload.spell_type == SPELL_TYPE_BOLT) {
            this_spell = std::make_unique<BoltSpell>(payload, _data);
        }
        if(payload.spell_type == SPELL_TYPE_AOE) {
            this_spell = std::make_unique<AoeSpell>(payload, _data);
        }
        if(payload.spell_type == SPELL_TYPE_INSTANT) {
            this_spell = std::make_unique<InstantSpell>(payload, _data);
        }


        if(this_spell) {
            DL_Add( this_scene->level_data.spell_list, std::move(this_spell));
        }
    }
    
    TraceLog(LOG_INFO, "spell list size  %i", this_scene->level_data.spell_list.size()); 
}

/* 

    if(_data->spell_id >= SPELL_ID_MAGICMISSLE_1 and _data->spell_id <= SPELL_ID_MAGICMISSLE_4) {
        this_spell = std::make_unique<MagicMissle>(payload, _data);
    }
    if(_data->spell_id >= SPELL_ID_FIREBALL_1 and _data->spell_id <= SPELL_ID_FIREBALL_4) {
        this_spell = std::make_unique<FireBall>(payload, _data);
    }
    if(_data->spell_id >= SPELL_ID_LIGHTNING_1 and _data->spell_id <= SPELL_ID_LIGHTNING_4) {
        this_spell = std::make_unique<Lightning>(payload, _data);
    }
    if(_data->spell_id >= SPELL_ID_POISON_1 and _data->spell_id <= SPELL_ID_POISON_4) {
        this_spell = std::make_unique<Poison>(payload, _data);
    }

    if(this_scene and this_spell) {
        DL_Add( this_scene->level_data.spell_list, std::move(this_spell));
    } */













void SpawnCharacterMessage(Vector2 _position, std::string _text, Color _color, float _delay_seconds) {

    if(g_game_data.is_in_sub_map) {
        std::unique_ptr<CharacterMessage> new_msg = std::make_unique<CharacterMessage>( _position, _text, _color, _delay_seconds);
        DL_Add( g_sub_scene->level_data.ui_entities, std::move(new_msg));
    }
    else {
        std::unique_ptr<CharacterMessage> new_msg = std::make_unique<CharacterMessage>( _position, _text, _color, _delay_seconds);
        DL_Add( g_current_scene->level_data.ui_entities, std::move(new_msg));
    }
}












void SpawnCreature(LevelData &level_data, Vector2 _position, int _creature_index) {

    std::unique_ptr<CharacterEntity> new_creature;
    int uid = GetRandomValue(1000, 1000000);

    g_active_creature_data[uid] = level_data.creature_data[_creature_index];


//load from loot tables
    LootTableID lt_id = g_active_creature_data[uid].loot_table_id;
    g_active_creature_data[uid].inventory = GenerateItemsFromLootTable(lt_id, "", 1);
//

    if(g_active_creature_data[uid].is_npc) {
        new_creature = std::make_unique<NpcEntity>(g_active_creature_data[uid].spawn_position, uid);
    }
    else {
        new_creature = std::make_unique<CreatureEntity>(g_active_creature_data[uid].spawn_position, uid);
    }
    new_creature->identifier = g_active_creature_data[uid].name;
    DL_Add(level_data.entity_list, std::move(new_creature));
}








void SpawnGroundContainer(Vector2 _position, std::vector<int> item_list) {
    //TraceLog(LOG_INFO, "looking through creatures inventory  (size) %i", item_list.size());
    int sprite_id = -1;
    Vector2 pos = g_current_player->position;
    for(int item = 0; item < item_list.size(); item++) {
        //TraceLog(LOG_INFO, "index %i  id %i", item, item_list[item]);
        if(item_list[item] != -1) {
            auto item_it = g_item_instances.find(item_list[item]);
            if(item_it != g_item_instances.end()) {
                sprite_id = item_it->second.item_id;
                break;
            }
        }
    }

    if(sprite_id == -1) {
        TraceLog(LOG_INFO, "could not spawn ground container");
        return;
    }

    //TraceLog(LOG_INFO, "dropping item(s) on ground %i", sprite_id);
    std::unique_ptr<GroundContainerEntity> new_container = std::make_unique<GroundContainerEntity>(_position, sprite_id);

    new_container->identifier = "GroundContainerEntity";
    new_container->c_area.identifier = "GroundContainerEntity";
    new_container->c_area.position = _position;
    new_container->c_area.item_list = item_list;
    new_container->c_area.size = {8, 8};
    new_container->iid = "ground_" + std::to_string(GetRandomValue(1000, 1000000));
    new_container->is_persistant =(g_game_data.current_scene_id == SHELTER_SCENE);
    new_container->level_index = g_game_data.current_map_index;

    for(int item : item_list) {
        if(item != -1) {
            g_item_instances[item].container_id = new_container->iid;
        }
    }

    BaseScene *this_scene = nullptr;

    if(g_game_data.is_in_sub_map) {
        this_scene = g_sub_scene.get();
    }
    else {
        this_scene = g_current_scene.get();
    }

    new_container->c_area.area_activated.Connect( [this_scene](){this_scene->OnContainerOpened();} );

    DL_Add(this_scene->level_data.entity_list, std::move(new_container));

}



void SpawnDoor(LevelData &level_data, Vector2 _position) {


    std::unique_ptr<DoorEntity> new_door = std::make_unique<DoorEntity>(_position);


    new_door->identifier = "DoorEntity";
    new_door->d_area.identifier = "DoorEntity";
    new_door->d_area.position = _position;
    new_door->d_area.size = {16, 16};
    new_door->iid = "door_" + std::to_string(GetRandomValue(1000, 1000000));
    new_door->is_persistant =false;
    new_door->level_index = g_game_data.current_map_index;


    DL_Add(level_data.entity_list, std::move(new_door));

}
   