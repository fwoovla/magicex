#include "../../core/pathgen.h"


void ConnectStructuresWithPaths(WorldGenTileSet &_tileset) {
    for(int structure_index = 0; structure_index < _tileset.structure_positions.size()-1; structure_index++) {
        //Vector2 start_position = _tileset.structure_positions[structure_index];
        Vector2 start_position = _tileset.structure_positions[0];
        Vector2 end_position = _tileset.structure_positions[(structure_index +1)];

        std::vector<Vector2> new_path;
        Vector2 p = start_position;

        new_path.push_back(p);

        while(p.x != end_position.x or p.y != end_position.y) {

            //TraceLog(LOG_INFO, "worm pos %0.0f %0.0f", worm.position.x, worm.position.y);

            std::vector<Vector2> choices;

            if (p.x < end_position.x) {
                //choices.push_back({ p.x + 1, p.y });
                choices.push_back({ 1, 0 });
            } 
            if (p.x > end_position.x){  
                //choices.push_back({ p.x - 1, p.y });
                choices.push_back({ -1, 0 });
            } 
            if (p.y < end_position.y) {
                //choices.push_back({ p.x, p.y + 1 });
                choices.push_back({ 0, 1 });
            }
            if (p.y > end_position.y) {
                //choices.push_back({ p.x, p.y - 1 });
                choices.push_back({ 0, -1 });
            }

            int choice = rand() % choices.size();
            p = Vector2Add(p, choices[choice]);
            new_path.push_back(p);

            int index = p.y * _tileset.map_size.x + p.x;

            _tileset.upper_zone_grid[index] = ZONE_PATH;
            _tileset.collision_grid[index] = 0;

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
                _tileset.collision_grid[index_up] = 0;
            }
            if(_tileset.upper_zone_grid[index_right] != ZONE_PATH){
                _tileset.upper_zone_grid[index_right] = ZONE_PATH_SIDE;
                _tileset.collision_grid[index_right] = 0;
            }
            if(_tileset.upper_zone_grid[index_down] != ZONE_PATH){
                _tileset.upper_zone_grid[index_down] = ZONE_PATH_SIDE;
                _tileset.collision_grid[index_down] = 0;
            }
            if(_tileset.upper_zone_grid[index_left] != ZONE_PATH){
                _tileset.upper_zone_grid[index_left] = ZONE_PATH_SIDE;
                _tileset.collision_grid[index_left] = 0;
            }
            
        }
    }
}



