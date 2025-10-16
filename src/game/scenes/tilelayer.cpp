#include "../../core/gamedefs.h"

MapData g_game_map_data;
std::vector<int> g_ordered_map__data;

TileLayer::TileLayer() {
    g_ordered_map__data.clear();
/*     rows = 0;
    cols = 0; */

}

TileLayer::~TileLayer() {
    //delete level_tile_array;

}

void TileLayer::SetTiles(Texture2D &_tilesheet, int _map_index) {

    //set up tile sheet
    map_index = _map_index;
    g_game_map_data = g_map_data[map_index];
    tilesheet = _tilesheet;
    int tile_size = g_game_map_data.tile_size;

    //tilesheet rows and cols
    int tsr = tilesheet.height / tile_size;
    int tsc = tilesheet.width / tile_size;
    
    //TraceLog(LOG_INFO, "tile sheet cols: %i   tile rows: %i", tsc, tsr);
    //get the source x and y for each tile
    
    //int index = 0;
    for (int y = 0; y < tsr; y++) {
        for (int x = 0; x < tsc; x++) {
            //index = y * cols + x;
            tile_lookup.push_back( {(float)x,(float)y} );//  [y * cols + x] = {(float)x,(float)y};
            TraceLog(LOG_INFO, "new tile x: %i   y: %i  id: %i", x, y, tile_lookup.size() - 1);
        }
    }

    //print map data
    TraceLog(LOG_INFO, "map data w: %i   h: %i", g_game_map_data.map_width, g_game_map_data.map_height);

    TraceLog(LOG_INFO, "map tile size %i", tile_size);
    TraceLog(LOG_INFO, "base layer name %s", g_game_map_data.layers[0].layer_name.c_str());
    TraceLog(LOG_INFO, "0,0 id %i", g_game_map_data.layers[0].tiles[0]);
    

    g_ordered_map__data.clear();
    for (int y = 0; y < g_game_map_data.map_height; y++) {
        for (int x = 0; x < g_game_map_data.map_width; x++) {
            g_ordered_map__data.push_back(0);
        }
    }

    TileLayerData base_layer = g_game_map_data.layers[0];
    
    int data_index = 0;   
    int order_index = 0;
    for (int y = 0; y < g_game_map_data.map_height; y++) {
        for (int x = 0; x < g_game_map_data.map_width; x++) {
            data_index = y * g_game_map_data.map_width + x;
            Vector2 tile_pos = base_layer.tiles[data_index].pos;
            order_index = tile_pos.y * g_game_map_data.map_width + tile_pos.x;

            g_ordered_map__data[order_index] = base_layer.tiles[data_index].id;
            //g_map_map[tile_pos] = base_layer.tiles[index].id;

            //TraceLog(LOG_INFO, "tile index %i ", index);
            //TraceLog(LOG_INFO, "tile id %i  tile pos: x: %0.0f,  y: %0.0f\n", base_layer.tiles[index].id, tile_pos.x, tile_pos.y);
        }
    }
}

void TileLayer::Update() {

}

void TileLayer::Draw() {
    //limit to viewport pls?

    int tile_size = g_game_map_data.tile_size;


    TileLayerData base_layer = g_game_map_data.layers[0];
    
    int index = 0;   
    for (int y = 0; y < g_game_map_data.map_height; y++) {
        for (int x = 0; x < g_game_map_data.map_width; x++) {
            index = y * g_game_map_data.map_width + x;
            //TraceLog(LOG_INFO, "tile index %i ", index);
            //TraceLog(LOG_INFO, "tile id %i  atlas: x: %0.0f,  y: %0.0f", base_layer.tiles[index].id, base_layer.tiles[index].pos.x, base_layer.tiles[index].pos.y);
            int tile_id = base_layer.tiles[index].id;
            Vector2 atlas_pos = tile_lookup[ base_layer.tiles[index].id ];
            Vector2 tile_pos = base_layer.tiles[index].pos;

            DrawTexturePro(
                tilesheet,
                {atlas_pos.x * tile_size, atlas_pos.y * tile_size, (float)tile_size, (float)tile_size},
                {(float)tile_pos.x*tile_size, (float)tile_pos.y*tile_size ,(float)tile_size, (float )tile_size},
                {0,0},
                0.0,
                WHITE
            );
        }
    }
}
