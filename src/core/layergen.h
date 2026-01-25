#pragma once
#include "gamedefs.h"

void GenerateLowerTerrainLayer(LDTKLevel &_level, WorldGenTileSet &_tileset);

void GenerateUpperTerrainLayer(LDTKLevel &level, WorldGenTileSet &_tileset);

void GenerateStructuresLayer(LDTKLevel &level, WorldGenTileSet &current_tileset, WorldGenTileSet &structure_tileset);

void GenerateCollisionLayer(LDTKLevel &level, WorldGenTileSet &_tileset);

void GenerateEntitiesLayer(LDTKLevel &level, WorldGenTileSet &_tileset);