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










void SpawnSpell(BaseScene &_scene, NewSpellPayload payload, SpellData *_data) {

    if(_data->spell_id == SPELL_ID_MAGICMISSLE) {
        MagicMissle *mm = new MagicMissle(payload, _data);
        TraceLog(LOG_INFO, "magic missle spell cast"); 
        DL_Add( _scene.level_data.spell_list, mm);
    }
    if(_data->spell_id == SPELL_ID_FIREBALL) {
        FireBall *mm = new FireBall(payload, _data);
        TraceLog(LOG_INFO, "fireball spell cast"); 
        DL_Add( _scene.level_data.spell_list, mm);
    }
    if(_data->spell_id == SPELL_ID_LIGHTNING) {
        Lightning *mm = new Lightning(payload, _data);
        //MagicMissle *mm = new MagicMissle(nsp.position, nsp.shooter_id, _data);
        TraceLog(LOG_INFO, "lightning spell cast"); 
        DL_Add( _scene.level_data.spell_list, mm);
    }
    TraceLog(LOG_INFO, "entity list size  %i", _scene.level_data.entity_list.size()); 
}

void SpawnCharacterMessage(BaseScene &_scene, Vector2 _position, std::string _text, Color _color, float _delay_seconds) {

    CharacterMessage *new_msg = new CharacterMessage( _position, _text, _color, _delay_seconds);
    DL_Add( _scene.level_data.ui_entities, new_msg);

}
