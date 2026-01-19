#pragma once
#include "gamedefs.h"

void GenerateLowerTerrainLayer(LDTKLevel &_level, WorldGenTileSet &_tileset);

void GenerateUpperTerrainLayer(LDTKLevel &level, WorldGenTileSet &_tileset);

void GenerateStructuresLayer(LDTKLevel &level, WorldGenTileSet &_tileset);

void GenerateCollisionLayer(LDTKLevel &level, WorldGenTileSet &_tileset);

void GenerateEntitiesLayer(LDTKLevel &level, WorldGenTileSet &_tileset);