#include "../../core/pathgen.h"


void ConnectStructuresWithPaths(WorldGenTileSet &_tileset) {
    for(int structure_index = 0; structure_index < _tileset.structure_positions.size()-1; structure_index++) {
        Vector2 start_position = _tileset.structure_positions[structure_index];
        Vector2 end_position = _tileset.structure_positions[(structure_index +1)];

        std::vector<Vector2> new_path;
        Vector2 p = start_position;

        new_path.push_back(p);

        while(p.x != end_position.x or p.y != end_position.y) {

            //TraceLog(LOG_INFO, "worm pos %0.0f %0.0f", worm.position.x, worm.position.y);

            std::vector<Vector2> choices;

            if (p.x < end_position.x) choices.push_back({ p.x + 1, p.y });
            if (p.x > end_position.x) choices.push_back({ p.x - 1, p.y });
            if (p.y < end_position.y) choices.push_back({ p.x, p.y + 1 });
            if (p.y > end_position.y) choices.push_back({ p.x, p.y - 1 });

            p = choices[rand() % choices.size()];
            int index = p.y * _tileset.map_size.x + p.x;

            _tileset.upper_zone_grid[index] = ZONE_PATH;

            int index_right = p.y * _tileset.map_size.x + (p.x+1);
            if(p.x+1 >= _tileset.map_size.x) {
                index_right = p.y * _tileset.map_size.x + p.x;
            }

            int index_left = p.y * _tileset.map_size.x + (p.x-1);
            if(p.x-1 < 0) {
                index_left = p.y * _tileset.map_size.x + p.x;
            }
            int index_down = (p.y+1) * _tileset.map_size.x + p.x;
            if(p.y+1 >= _tileset.map_size.y) {
                index_down = p.y * _tileset.map_size.x + p.x;
            }
            int index_up = (p.y-1) * _tileset.map_size.x + p.x;
            if(p.y-1 < 0) {
                index_up = p.y * _tileset.map_size.x + p.x;
            }

            if(_tileset.upper_zone_grid[index_up] != ZONE_PATH){
                _tileset.upper_zone_grid[index_up] = ZONE_PATH_SIDE;
            }
            if(_tileset.upper_zone_grid[index_right] != ZONE_PATH){
                _tileset.upper_zone_grid[index_right] = ZONE_PATH_SIDE;
            }
            if(_tileset.upper_zone_grid[index_down] != ZONE_PATH){
                _tileset.upper_zone_grid[index_down] = ZONE_PATH_SIDE;
            }
            if(_tileset.upper_zone_grid[index_left] != ZONE_PATH){
                _tileset.upper_zone_grid[index_left] = ZONE_PATH_SIDE;
            }

            new_path.push_back(p);
        }
    }
}





void GenerateMapWormPaths(WorldGenTileSet &_tileset) {

    for(int len = 0; len < 50; len++) {

        int worm_index = 0;
        for(auto &worm : _tileset.path_worms) {

            //TraceLog(LOG_INFO, "worm pos %0.0f %0.0f", worm.position.x, worm.position.y);
            int x = worm.position.x;
            int y = worm.position.y;
            int index = y * _tileset.map_size.x + x;

            int index_right = y * _tileset.map_size.x + (x+1);
            if(x+1 >= _tileset.map_size.x) {
                index_right = y * _tileset.map_size.x + x;
            }

            int index_left = y * _tileset.map_size.x + (x-1);
            if(x-1 < 0) {
                index_left = y * _tileset.map_size.x + x;
            }
            int index_down = (y+1) * _tileset.map_size.x + x;
            if(y+1 >= _tileset.map_size.y) {
                index_down = y * _tileset.map_size.x + x;
            }
            int index_up = (y-1) * _tileset.map_size.x + x;
            if(y-1 < 0) {
                index_up = y * _tileset.map_size.x + x;
            }


            _tileset.paths[worm_index].push_back(worm.position);

            _tileset.upper_zone_grid[index] = ZONE_PATH;

            //temp_grid[index] = 1;
            if(_tileset.upper_zone_grid[index_up] != ZONE_PATH){
                _tileset.upper_zone_grid[index_up] = ZONE_PATH_SIDE;
            }
            if(_tileset.upper_zone_grid[index_right] != ZONE_PATH){
                _tileset.upper_zone_grid[index_right] = ZONE_PATH_SIDE;
            }
            if(_tileset.upper_zone_grid[index_down] != ZONE_PATH){
                _tileset.upper_zone_grid[index_down] = ZONE_PATH_SIDE;
            }
            if(_tileset.upper_zone_grid[index_left] != ZONE_PATH){
                _tileset.upper_zone_grid[index_left] = ZONE_PATH_SIDE;
            }


            if(GetRandomValue(0, 100) > 50 and (worm.position.x > 4 and worm.position.x < _tileset.map_size.x-4)) { //do x
                if(worm.locked_dir.x != 0) { //base locked
                    worm.position.x += worm.locked_dir.x * -1;
                    worm.last_locked_dir = {worm.locked_dir.x,0};
                    //TraceLog(LOG_INFO, "move x %0.0f", worm.locked_dir.x*-1);
                }
                else { //not base locked
                    if(GetRandomValue(0, 100) > 50) {

                        if(worm.last_locked_dir.x < 0.0f) {
                            worm.position.x += 1;
                            worm.last_locked_dir = {-1,0};
                            //TraceLog(LOG_INFO, "right");
                        }
                        else {
                            worm.position.x += -1;
                            worm.last_locked_dir = {1,0};
                            //TraceLog(LOG_INFO, "left");
                        }
                    }   
                    else {
                        if(worm.last_locked_dir.x > 0.0f) {
                            worm.position.x += -1;
                            worm.last_locked_dir = {1,0};
                            //TraceLog(LOG_INFO, "left");
                        }
                        else {
                            worm.position.x += 1;
                            worm.last_locked_dir = {-1,0};
                            //TraceLog(LOG_INFO, "right"); 
                        }
                    }    
                }
            }

            else if(worm.position.y > 4 and worm.position.y < _tileset.map_size.y-4) { //do y
                if(worm.locked_dir.y != 0) {
                    worm.position.y += worm.locked_dir.y * -1;
                    worm.last_locked_dir = {0,worm.locked_dir.y};
                    //TraceLog(LOG_INFO, "move y %0.0f", worm.locked_dir.y*-1);
                }
                else {
                    if(GetRandomValue(0, 100) > 50) {
                        if(worm.last_locked_dir.y < 0.0f) {
                            worm.position.y += 1;
                            worm.last_locked_dir = {0,-1};
                            //TraceLog(LOG_INFO, "down");
                        }
                        else {
                            worm.position.y += -1;
                            worm.last_locked_dir = {0,1};
                            //TraceLog(LOG_INFO, "up");
                        }
                    }  
                    else {
                        if(worm.last_locked_dir.y > 0.0f) {
                            worm.position.y += -1;
                            worm.last_locked_dir = {0,1};
                            //TraceLog(LOG_INFO, "up");
                        }
                        else {
                            worm.position.y += 1;
                            worm.last_locked_dir = {0,-1};
                            //TraceLog(LOG_INFO, "down");
                        }
                    }   
                }
            }         
        }
    }
}
