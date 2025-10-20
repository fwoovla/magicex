#include "../../core/gamedefs.h"

MapData g_current_map_data;
std::vector<std::vector<int>> g_current_ordered_layers;

TileLayer::TileLayer() {
    g_current_ordered_layers.clear();
/*     rows = 0;
    cols = 0; */

}

TileLayer::~TileLayer() {
    //delete level_tile_array;

}

void TileLayer::SetTiles(Texture2D &_tilesheet, int _map_index) {

    //set up tile sheet
    map_index = _map_index;
    g_current_map_data = g_maps_data[map_index];
    tilesheet = _tilesheet;
    int tile_size = g_current_map_data.tile_size;

    //tilesheet rows and cols
    int tsr = tilesheet.height / tile_size;
    int tsc = tilesheet.width / tile_size;
    //TraceLog(LOG_INFO, "tile sheet cols: %i   tile rows: %i", tsc, tsr);

    //get the source x and y for each tile

    for (int y = 0; y < tsr; y++) {
        for (int x = 0; x < tsc; x++) {
            tile_atlas.push_back( {(float)x,(float)y} );//  [y * cols + x] = {(float)x,(float)y};
            TraceLog(LOG_INFO, "new atlas tile : (%i, %i)  id: %i", x, y, tile_atlas.size() - 1);
        }
    }

    //sort data into ordered ids
    g_current_ordered_layers.clear();

    int order_index = 0;

    for (int l = 0; l < g_current_map_data.layers.size(); l++) {

        std::vector<int> new_layer;
        g_current_ordered_layers.push_back(new_layer);

        for (int y = 0; y < g_current_map_data.map_height; y++) {
            for (int x = 0; x < g_current_map_data.map_width; x++) {
                g_current_ordered_layers[l].push_back(-1);
            }
        }
    }

    for (int l = 0; l < g_current_map_data.layers.size(); l++) {

        TileLayerData this_layer = g_current_map_data.layers[l];
        
        int data_index = 0; 
        int order_index = 0;

        for (int y = 0; y < g_current_map_data.map_height; y++) {
            for (int x = 0; x < g_current_map_data.map_width; x++) {

                data_index = y * g_current_map_data.map_width + x;
                if( data_index < g_current_map_data.layers[l].tiles.size() ) {

                    Vector2 tile_pos = this_layer.tiles[data_index].pos;
                    order_index = tile_pos.y * g_current_map_data.map_width + tile_pos.x;
                        
                    g_current_ordered_layers[l][order_index] = this_layer.tiles[data_index].id;
                                        
                    //TraceLog(LOG_INFO, "layer %i index %i ", l, order_index);
                    //TraceLog(LOG_INFO, "tile id %i  tile pos: x: %0.0f,  y: %0.0f\n", base_layer.tiles[index].id, tile_pos.x, tile_pos.y);
                }
            }
        }
    }
    TraceLog(LOG_INFO, "map data w: %i   h: %i", g_current_map_data.map_width, g_current_map_data.map_height);
    TraceLog(LOG_INFO, "map tile size %i", tile_size);
    TraceLog(LOG_INFO, "layer count %i", g_current_ordered_layers.size());
    TraceLog(LOG_INFO, "collisiion layer index %i", g_current_map_data.collision_layer_index);

}

void TileLayer::Update() {

}

void TileLayer::Draw() {

    //limited to viewport
    int tile_size = g_current_map_data.tile_size;

    int x_offset = ((g_resolution.x * 0.5f) / g_camera.zoom) * g_current_map_data.inv_tile_size;
    int y_offset = ((g_resolution.y * 0.5f) / g_camera.zoom) * g_current_map_data.inv_tile_size;

    Vector2 center = {g_camera.target.x/tile_size, g_camera.target.y/tile_size};

    int x_min = center.x - x_offset;
    if(x_min < 0) {
        x_min = 0;
    }

    int x_max = center.x + (x_offset * 2) + 2;
    if(x_max > g_current_map_data.map_width) {
        x_max = g_current_map_data.map_width;
    } 

    int y_min = center.y - y_offset;
    if(y_min < 0) {
        y_min = 0;
    }

    int y_max = center.y + (y_offset * 2) + 2;
    if(y_max > g_current_map_data.map_height) {
        y_max = g_current_map_data.map_height;
    } 


    int tiles_drawn = 0;

    //invert layers for drawing
    for (int l = g_current_ordered_layers.size() - 1; l >= 0; l--) {
        if(l != g_current_map_data.collision_layer_index) {
            //TraceLog(LOG_INFO, "drawing layer %i  ", l);
            int index = 0;   
            for (int y = y_min; y < y_max; y++) {
                for (int x = x_min; x < x_max; x++) {

                    index = (y) * g_current_map_data.map_width + (x);

                    if(index >= g_current_ordered_layers[l].size()) {
                        break;
                    }
                    TraceLog(LOG_INFO, "drawing tile %i  ", index);
                    int tile_id = g_current_ordered_layers[l][index];

                    if(tile_id != -1) {
                        //TraceLog(LOG_INFO, "layer %i  tile id %i", l, tile_id);
                                
                        Vector2 atlas_pos = tile_atlas[ tile_id ];
                        Vector2 tile_pos = { (float)x, (float)y};// base_layer.tiles[index].pos;

                        Color color = WHITE;

                        if(g_game_settings.show_debug == true) {
                            if(l == g_current_map_data.collision_layer_index) {
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
    }
    //TraceLog(LOG_INFO, "\n\n");
}
