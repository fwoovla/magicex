#pragma once
#include "gamedefs.h"

void GenerateLowerTerrainLayer(LDTKLevel &_level, WorldGenTileSet &_tileset, Vector2 _map_size);

void GenerateUpperTerrainLayer(LDTKLevel &level, WorldGenTileSet &_tileset, Vector2 _map_size);

void GenerateStructuresLayer(LDTKLevel &level, WorldGenTileSet &_tileset, Vector2 _map_size);

void GenerateCollisionLayer(LDTKLevel &level, WorldGenTileSet &_tileset, Vector2 _map_size);