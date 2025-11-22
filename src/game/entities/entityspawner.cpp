#include "../../core/gamedefs.h"


void SpawnSpell(SpellData _data, BaseScene &_scene, NewSpellPayload nsp) {

    if(_data.spell_id == SPELL_ID_MAGICMISSLE_1 or _data.spell_id == SPELL_ID_MAGICMISSLE_2 or _data.spell_id == SPELL_ID_MAGICMISSLE_3) {
        MagicMissle *mm = new MagicMissle(nsp.position, nsp.shooter_id, _data);
        //TraceLog(LOG_INFO, "magic missle spell cast"); 
        DL_Add( _scene.level_data.entity_list, mm);
    }
    if(_data.spell_id == SPELL_ID_FIREBALL_1 or _data.spell_id == SPELL_ID_FIREBALL_2 or _data.spell_id == SPELL_ID_FIREBALL_3) {
        FireBall *mm = new FireBall(nsp.position, nsp.shooter_id, _data);
        //TraceLog(LOG_INFO, "fireball spell cast"); 
        DL_Add( _scene.level_data.entity_list, mm);
    }
    if(_data.spell_id == SPELL_ID_LIGHTNING_1 or _data.spell_id == SPELL_ID_LIGHTNING_2 or _data.spell_id == SPELL_ID_LIGHTNING_3) {
        Lightning *mm = new Lightning(nsp.position, nsp.shooter_id, _data);
        //TraceLog(LOG_INFO, "lightning spell cast"); 
        DL_Add( _scene.level_data.entity_list, mm);
    }
    //TraceLog(LOG_INFO, "entity list size  %i", _scene.level_data.entity_list.size()); 
}