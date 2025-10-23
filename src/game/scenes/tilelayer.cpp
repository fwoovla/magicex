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

    LDTKDrawMap(g_current_player->position);

}
