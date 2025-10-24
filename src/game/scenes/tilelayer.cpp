#include "../../core/gamedefs.h"


TileLayer::TileLayer() {


}

TileLayer::~TileLayer() {

}

void TileLayer::SetTiles(Texture2D &_tilesheet, int _map_index) {

}

void TileLayer::Update() {

}

void TileLayer::Draw() {

    double time = GetTime();

    g_debug_data.tiles_drawn =  LDTKDrawMap(g_current_player->position);

    g_debug_data.tile_dt = GetTime() - time;

    //TraceLog(LOG_INFO, "dt tile draw %f", g_debug_data.tile_dt);
    //TraceLog(LOG_INFO, "tiles drawn %i", g_debug_data.tiles_drawn);

}
