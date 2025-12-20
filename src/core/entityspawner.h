#pragma once

#include "basescene.h"
#include "gamedefs.h"

void SpawnSpell(BaseScene &_scene, NewSpellPayload payload, SpellData *_data);

void SpawnCharacterMessage(BaseScene &_scene, Vector2 _position, std::string _text, Color _color, float _delay_seconds);