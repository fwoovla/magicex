#pragma once

#include "basescene.h"
#include "gamedefs.h"

void SpawnSpell(BaseScene &_scene, NewSpellPayload payload, SpellData *_data);

void SpawnCharacterMessage(Vector2 _position, std::string _text, Color _color, float _delay_seconds);

void SpawnCreature(LevelData &level_data, Vector2 _position, int _creature_index);