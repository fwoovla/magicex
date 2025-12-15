#include "../../core/gamedefs.h"


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