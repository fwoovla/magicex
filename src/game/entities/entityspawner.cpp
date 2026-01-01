#include "../../core/gamedefs.h"


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











int CalculateDamage(DamagePayload &damage_payload, CharacterData &character_data) {
    int damage = damage_payload.damage;
    //do stuff
    return damage;
}



void SpawnSpell(BaseScene &_scene, NewSpellPayload payload, SpellData *_data) {

    BaseScene *this_scene = nullptr;
    BaseSpell *this_spell = nullptr;

    if(g_game_data.is_in_sub_map) {
        this_scene = g_sub_scene.get();
    }
    else {
        this_scene = g_current_scene.get();
    }

    if(_data->spell_id == SPELL_ID_MAGICMISSLE) {
        this_spell = new MagicMissle(payload, _data);
    }
    if(_data->spell_id == SPELL_ID_FIREBALL) {
        this_spell = new FireBall(payload, _data);
    }
    if(_data->spell_id == SPELL_ID_LIGHTNING) {
        this_spell = new Lightning(payload, _data);
    }

    if(this_scene and this_spell) {
        DL_Add( this_scene->level_data.spell_list, this_spell);
    }

    TraceLog(LOG_INFO, "entity list size  %i", _scene.level_data.entity_list.size()); 
}

void SpawnCharacterMessage(Vector2 _position, std::string _text, Color _color, float _delay_seconds) {

    if(g_game_data.is_in_sub_map) {
        CharacterMessage *new_msg = new CharacterMessage( _position, _text, _color, _delay_seconds);
        DL_Add( g_sub_scene->level_data.ui_entities, new_msg);
    }
    else {
        CharacterMessage *new_msg = new CharacterMessage( _position, _text, _color, _delay_seconds);
        DL_Add( g_current_scene->level_data.ui_entities, new_msg);
    }
}


void SpawnCreature(LevelData &level_data, Vector2 _position, int _creature_index) {

    CharacterEntity  *new_creature;
    int uid = GetRandomValue(1000, 1000000);

    g_character_data[uid] = level_data.creature_data[_creature_index];

    ItemInstanceData *this_data;

    for(int item : g_character_data[uid].primary) {
        if(item != -1) {
            this_data = InstanceRandomCharacterItem((ItemID)item, uid, 2);
            g_character_data[uid].primary[0] = this_data->instance_id;
        }
    }

    TraceLog(LOG_INFO, "new creature  %s  uid %i  creature id %i   sprite id %i", g_character_data[uid].name.c_str(), uid, g_character_data[uid].creature_id, g_character_data[uid].sprite_sheet_id);

    new_creature = new CreatureEntity(g_character_data[uid].spawn_position, uid);
    new_creature->identifier = g_character_data[uid].name;
    DL_Add(level_data.entity_list, new_creature);

}