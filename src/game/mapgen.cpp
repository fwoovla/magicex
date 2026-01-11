#include "../core/gamedefs.h"


void GenerateWorldGenTilesets(std::string _path) {

    TraceLog(LOG_INFO, "++++++++++++++++++++++++++++++++ LOADING MAPGEN DATA....  (%s) ++++++++++++++++++++++++++\n", _path.c_str());

    std::ifstream mfile(_path);
    if (!mfile.is_open()) {
        TraceLog(LOG_INFO, "-CANNOT OPEN FILE");
        return;
    }
    
    json j;
    mfile>>j;
    TraceLog(LOG_INFO, "+FOUND MAP FILE");

    if(j.contains("defs")) {

        if(j["defs"].contains("tilesets")) {
            TraceLog(LOG_INFO, "+++TILESETS FOUND");
            int num_tilesets = 0;

            for(int i = 0; i < j["defs"]["tilesets"].size(); i++) {
                num_tilesets++;
                int uid = GetRandomValue(100, 1000);

                std::filesystem::path pathObj(j["defs"]["tilesets"][i]["relPath"]);
                std::string texture_path = pathObj.filename().string();

                if(std::filesystem::exists( "assets/maps/ldtk/" + texture_path)) {
                    TileSheetData this_sheet = {
                        .uid = uid,
                        .texture = LoadTexture(("assets/maps/ldtk/" + texture_path).c_str())
                    };

                    g_ldtk_tilesheets[this_sheet.uid] = this_sheet;
                    
                    TraceLog(LOG_INFO, "==========TILESETS TEXTURE LOADED %s", texture_path.c_str());

                    WorldGenTileSet this_tileset;

                    this_tileset.c_wid = j["defs"]["tilesets"][i]["__cWid"];
                    this_tileset.uid = uid;
                    this_tileset.rel_path = j["defs"]["tilesets"][i]["relPath"];
                    this_tileset.px_wid = j["defs"]["tilesets"][i]["pxWid"];
                    this_tileset.px_hei = j["defs"]["tilesets"][i]["pxHei"];
                    this_tileset.tile_grid_size = j["defs"]["tilesets"][i]["tileGridSize"] ;

                    for(int tag = 0; tag < j["defs"]["tilesets"][i]["enumTags"].size(); tag++) {
                        LDTKEnumTag new_tag;
                        new_tag.value_string = j["defs"]["tilesets"][i]["enumTags"][tag]["enumValueId"];
                        TraceLog(LOG_INFO, "====enum tag           _____________ %s", new_tag.value_string.c_str());
                        for(int tid = 0; tid <  j["defs"]["tilesets"][i]["enumTags"][tag]["tileIds"].size(); tid++) {
                            int id = j["defs"]["tilesets"][i]["enumTags"][tag]["tileIds"][tid];
                            new_tag.tile_ids.push_back(id);
                            TraceLog(LOG_INFO, "====tile id         _____________ %i", id);
                        }
                        this_tileset.tile_tags.push_back(new_tag);
                    }

                    g_worldgen_tilesets.push_back(this_tileset);

                }
            }
        }
    }


    if(j.contains("levels")) {
        TraceLog(LOG_INFO, "(%i) LEVELS FOUND", j["levels"].size());
        for(int level = 0; level < j["levels"].size(); level++) {
            std::string level_name = j["levels"][level]["identifier"];
            TraceLog(LOG_INFO, "LEVEL FOUND %s", level_name.c_str());
            

            for(int layer = 0; layer < j["levels"][level]["layerInstances"].size(); layer++) {
                TraceLog(LOG_INFO, "                          ---------");
                
                LDTKLayerInstance this_layer;
                
                this_layer.identifier = j["levels"][level]["layerInstances"][layer]["__identifier"];
                this_layer.type = j["levels"][level]["layerInstances"][layer]["__type"];
                
                TraceLog(LOG_INFO, "--------------------------NEW LAYER------------------------------");
                TraceLog(LOG_INFO, "--------------------------%s------------------------------", this_layer.identifier.c_str());

                if(this_layer.type == "Tiles") {
                    TraceLog(LOG_INFO, "++++++--------------------------------GRID TILES FOUND %s", this_layer.identifier.c_str());
                    this_layer.grid_size = j["levels"][level]["layerInstances"][layer]["__gridSize"];
                    this_layer.tileset_def_uid = j["levels"][level]["layerInstances"][layer]["__tilesetDefUid"];
                    this_layer.c_hei = j["levels"][level]["layerInstances"][layer]["__cHei"];
                    this_layer.c_wid = j["levels"][level]["layerInstances"][layer]["__cWid"];

                    //go through tile grid
                    //  match tile enum 
                }
            }



        }
    }

/* if(j.contains("defs")) {
        TraceLog(LOG_INFO, "++DEFS FOUND");
        if(j["defs"].contains("tilesets")) {
            TraceLog(LOG_INFO, "+++TILESETS FOUND");
            int num_tilesets = 0;
            for(int i = 0; i < j["defs"]["tilesets"].size(); i++) {
                num_tilesets++;

                std::filesystem::path pathObj(j["defs"]["tilesets"][i]["relPath"]);
                std::string texture_path = pathObj.filename().string();

                if(std::filesystem::exists( "assets/maps/ldtk/" + texture_path)) {
                    TileSheetData this_sheet = {
                        .uid = j["defs"]["tilesets"][i]["uid"],
                        .texture = LoadTexture(("assets/maps/ldtk/" + texture_path).c_str())
                    };

                    g_ldtk_tilesheets[this_sheet.uid] = this_sheet;
                    
                    TraceLog(LOG_INFO, "==========TILESETS TEXTURE LOADED %s", texture_path.c_str());
                }

                LDTKTileset this_tileset = {
                    .c_wid = mj["defs"]["tilesets"][i]["__cWid"],
                    .uid = mj["defs"]["tilesets"][i]["uid"],
                    .rel_path = mj["defs"]["tilesets"][i]["relPath"],
                    .px_wid = mj["defs"]["tilesets"][i]["pxWid"],
                    .px_hei = mj["defs"]["tilesets"][i]["pxHei"],
                    .tile_grid_size = mj["defs"]["tilesets"][i]["tileGridSize"]  
                    
                };

                for(int tag = 0; tag < mj["defs"]["tilesets"][i]["enumTags"].size(); tag++) {
                    LDTKEnumTag new_tag;
                    new_tag.value_string = mj["defs"]["tilesets"][i]["enumTags"][tag]["enumValueId"];
                    TraceLog(LOG_INFO, "====enum tag           ______________----------- %s", new_tag.value_string.c_str());
                    for(int tid = 0; tid <  mj["defs"]["tilesets"][i]["enumTags"][tag]["tileIds"].size(); tid++) {
                        int id = mj["defs"]["tilesets"][i]["enumTags"][tag]["tileIds"][tid];
                        new_tag.tile_ids.push_back(id);
                        TraceLog(LOG_INFO, "====enum tag    tileid       ______________----------- %i", id);

                    }
                    this_tileset.enum_tags.push_back(new_tag);
                }
                g_ldtk_maps.tilesets.push_back(this_tileset);
            }

            TraceLog(LOG_INFO, "==========TILESETS LOADED %i", g_ldtk_maps.tilesets.size());
        }
    } */
}

void GenerateMap(LDTKLevel &new_level) {

}