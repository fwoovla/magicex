#include "../../core/pathgen.h"


void ConnectStructuresWithPaths(WorldGenTileSet &_tileset) {

    std::vector<Vector2> positions;

    for(Vector2 position : _tileset.wg_plan.structure_positions){ positions.push_back(position);}
    //for(Vector2 position : _tileset.exit_positions){ positions.push_back(position);}

    for(int position_index = 0; position_index < positions.size()-1; position_index++) {
        //Vector2 start_position = _tileset.structure_positions[structure_index];
        //Vector2 start_position = _tileset.structure_positions[0];
        //Vector2 end_position = _tileset.structure_positions[(structure_index +1)];

        Vector2 end_position = positions[0];
        Vector2 start_position = positions[(position_index +1)];

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

            if (p.x < 0) {p.x = 0;}
            if (p.x > _tileset.map_size.x-1) {p.x = _tileset.map_size.x-1;}
            if (p.y < 0) {p.y = 0;}
            if (p.y > _tileset.map_size.y-1) {p.y = _tileset.map_size.y-1;}

            new_path.push_back(p);

            int index = p.y * _tileset.map_size.x + p.x;


            _tileset.upper_zone_grid[index] = ZONE_PATH;
            _tileset.collision_grid[index] = 0;

            int index_right = p.y * _tileset.map_size.x + (p.x+1);
            if(p.x+1 >= _tileset.map_size.x) {
                index_right = index;
            }

            int index_left = p.y * _tileset.map_size.x + (p.x-1);
            if(p.x-1 < 0) {
                index_left = index;
            }

            int index_down = (p.y+1) * _tileset.map_size.x + p.x;
            if(p.y+1 >= _tileset.map_size.y) {
                index_down = index;
            }

            int index_up = (p.y-1) * _tileset.map_size.x + p.x;
            if(p.y-1 < 0) {
                index_up = index;
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



            if(GetRandomValue(0, 1000) > 985 ) {
                WorldGenDeccoEntityData new_decco_entity;
                new_decco_entity.decco_name = "DeccoEntity_2";
                new_decco_entity.position = {p.x+ GetRandomValue(-1,1), p.y+ GetRandomValue(-1,1)};
                _tileset.entity_decco_data.push_back(new_decco_entity);
            }
        }
    }
}



void BuildRoads(WorldGenTileSet &_tileset) {

    for(int position_index = 0; position_index < _tileset.wg_plan.exit_positions.size(); position_index++) {
    //for(Vector2 exit_pos :_tileset.exit_positions) {

        Vector2 dir = {0,0};

        if(_tileset.wg_plan.exit_positions[position_index].y == _tileset.wg_plan.road_midpoint.y) {
            if(_tileset.wg_plan.exit_positions[position_index].x < _tileset.wg_plan.road_midpoint.x) {
                dir.x = 1;
            }
            else {
                dir.x = -1;
            }
        }
        else {
            if(_tileset.wg_plan.exit_positions[position_index].y < _tileset.wg_plan.road_midpoint.y) {
                dir.y = 1;
            }
            else {
                dir.y = -1; 
            }
        }


        Vector2 end_position = _tileset.wg_plan.road_midpoint;
        Vector2 start_position = _tileset.wg_plan.exit_positions[(position_index)];

        std::vector<Vector2> new_path;
        Vector2 p = start_position;


        new_path.push_back(p);


        while(p.x != end_position.x or p.y != end_position.y) {

            p = Vector2Add(p, dir);

            if (p.x < 0) {p.x = 0;}
            if (p.x > _tileset.map_size.x-1) {p.x = _tileset.map_size.x-1;}
            if (p.y < 0) {p.y = 0;}
            if (p.y > _tileset.map_size.y-1) {p.y = _tileset.map_size.y-1;}

            new_path.push_back(p);

            //int index = p.y * _tileset.map_size.x + p.x;

            for(int y = 0; y < 2; y++) {
                for (int x = 0; x < 2; x++) {
                    int _x = p.x+x;
                    int _y = p.y+y;
                    
                    if ((p.x+x) < 1) {_x = 1;}
                    if ((p.x+x) > _tileset.map_size.x-1) {_x = _tileset.map_size.x-1;}
                    if ((p.y+y) < 1) {_y = 1;}
                    if ((p.y+y) > _tileset.map_size.y-1) {_y = _tileset.map_size.y-1;}
                    
                    int _index = _y * _tileset.map_size.x + _x;
                    _tileset.upper_zone_grid[_index] = ZONE_ROAD;
                    _tileset.collision_grid[_index] = 0;
                    //sTraceLog(LOG_INFO, "road pos %i %i", _x, _y);
                }
            }
        }
 


    }
}