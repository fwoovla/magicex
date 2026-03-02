#pragma once
#include "gamedefs.h"

void GenerateDeccoLayer(LDTKLevel &_level, WorldGenTileSet &current_tileset, WorldGenTileSet &structure_tileset);

void GenerateLowerTerrainLayer(LDTKLevel &_level, WorldGenTileSet &_tileset);

void GenerateUpperTerrainLayer(LDTKLevel &level, WorldGenTileSet &_tileset);

void GenerateStructuresLayer(LDTKLevel &level, WorldGenTileSet &current_tileset, WorldGenTileSet &structure_tileset);

void GenerateCollisionLayer(LDTKLevel &level, WorldGenTileSet &_tileset);

void GenerateEntitiesLayer(LDTKLevel &level, WorldGenTileSet &_tileset);

void PlaceEntities(LDTKLevel &level, WorldGenTileSet &_tileset);

void PlaceCreatureEntities(LDTKLevel &level, WorldGenTileSet &_tileset);


void GenerateTerrainZones(LDTKLevel &level, WorldGenTileSet &_tileset);

void ShapeHills(LDTKLevel &level, WorldGenTileSet &_tileset);

void GenerateDebugLayer(LDTKLevel &_level, WorldGenTileSet &_tileset);