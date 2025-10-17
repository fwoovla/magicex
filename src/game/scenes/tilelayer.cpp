#include "../../core/gamedefs.h"

MapData g_game_map_data;
//std::vector<int> g_ordered_map_data;
std::vector<std::vector<int>> g_ordered_map_layers;

TileLayer::TileLayer() {
    g_ordered_map_layers.clear();
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
            tile_atlas.push_back( {(float)x,(float)y} );//  [y * cols + x] = {(float)x,(float)y};
            TraceLog(LOG_INFO, "new tile x: %i   y: %i  id: %i", x, y, tile_atlas.size() - 1);
        }
    }

    //print map data
    TraceLog(LOG_INFO, "map data w: %i   h: %i", g_game_map_data.map_width, g_game_map_data.map_height);

    TraceLog(LOG_INFO, "map tile size %i", tile_size);
    //TraceLog(LOG_INFO, "layer count %i", g_game_map_data.layers.size());
    //TraceLog(LOG_INFO, "0,0 id %i", g_game_map_data.layers[0].tiles[0]);
    

    g_ordered_map_layers.clear();

    int order_index = 0;

    for (int l = 0; l < g_game_map_data.layers.size(); l++) {

        std::vector<int> new_layer;
        g_ordered_map_layers.push_back(new_layer);

        for (int y = 0; y < g_game_map_data.map_height; y++) {
            for (int x = 0; x < g_game_map_data.map_width; x++) {
                //g_ordered_map_data.push_back(0);
                //g_ordered_map_layers[l].layer_name = "";

                g_ordered_map_layers[l].push_back(-1);
            }
        }
    }

    for (int l = 0; l < g_game_map_data.layers.size(); l++) {

        TileLayerData this_layer = g_game_map_data.layers[l];
        
        int data_index = 0;   
        int order_index = 0;

        for (int y = 0; y < g_game_map_data.map_height; y++) {
            for (int x = 0; x < g_game_map_data.map_width; x++) {

                data_index = y * g_game_map_data.map_width + x;
                if( data_index < g_game_map_data.layers[l].tiles.size() ) {
                    Vector2 tile_pos = this_layer.tiles[data_index].pos;
                    order_index = tile_pos.y * g_game_map_data.map_width + tile_pos.x;
                    
                    //g_ordered_map_data[order_index] = base_layer.tiles[data_index].id;
                    
                    g_ordered_map_layers[l][order_index] = this_layer.tiles[data_index].id;
                    
                    //g_map_map[tile_pos] = base_layer.tiles[index].id;
                    
                    TraceLog(LOG_INFO, "layer %i index %i ", l, order_index);
                    //TraceLog(LOG_INFO, "tile id %i  tile pos: x: %0.0f,  y: %0.0f\n", base_layer.tiles[index].id, tile_pos.x, tile_pos.y);
                }
            }
        }
    }
    TraceLog(LOG_INFO, "layer count %i", g_game_map_data.layers.size());

}

void TileLayer::Update() {

}

void TileLayer::Draw() {
    //limit to viewport pls?

    int tile_size = g_game_map_data.tile_size;

    int x_offset = ((g_resolution.x * 0.5f) / g_camera.zoom) * g_game_map_data.inv_tile_size;
    int y_offset = ((g_resolution.y * 0.5f) / g_camera.zoom) * g_game_map_data.inv_tile_size;

    Vector2 center = {g_camera.target.x/tile_size, g_camera.target.y/tile_size};
    //TraceLog(LOG_INFO, "drawing layers ");
    //TileLayerData base_layer = g_game_map_data.layers[0];
    
    
    int x_min = center.x - x_offset;
    if(x_min < 0) {
        x_min = 0;
    }

    int y_min = center.y - y_offset;
    if(y_min < 0) {
        y_min = 0;
    }

    int tiles_drawn = 0;

    //invert layers for drawing
    for (int l = g_ordered_map_layers.size() - 1; l >= 0; l--) {
        //TraceLog(LOG_INFO, "drawing layer %i  ", l);
        int index = 0;   
        for (int y = y_min; y < (center.y + (y_offset * 2)); y++) {
            for (int x = x_min; x < (center.x + (x_offset * 2)); x++) {

                index = (y) * g_game_map_data.map_width + (x);

                if(index >= g_ordered_map_layers[l].size()) {
                    break;
                }

                int tile_id = g_ordered_map_layers[l][index];

                if(tile_id != -1) {
                    //TraceLog(LOG_INFO, "layer %i  tile id %i", l, tile_id);
                            
                    Vector2 atlas_pos = tile_atlas[ tile_id ];
                    Vector2 tile_pos = { (float)x, (float)y};// base_layer.tiles[index].pos;

                    Color color = WHITE;

                    if(g_game_settings.show_debug == true) {
                        if(tile_id == 31) {
                            color = DARKGRAY;
                        }
                    }

                    DrawTexturePro(
                        tilesheet,
                        {atlas_pos.x * tile_size, atlas_pos.y * tile_size, (float)tile_size, (float)tile_size},
                        {(float)tile_pos.x*tile_size, (float)tile_pos.y*tile_size ,(float)tile_size, (float )tile_size},
                        {0,0},
                        0.0,
                        color
                    );
                    tiles_drawn++;
                }
            }
        }
    }
    //TraceLog(LOG_INFO, "\n\n");
}
