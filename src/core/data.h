#pragma once

//#include <raylib.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include "json.hpp"

#include "gamedefs.h"

using json = nlohmann::json;



inline std::string unit_names[10] = {
    "blerfy",
    "toyf",
    "margolot",
    "dr farts",
    "muffin man",
    "doof",
    "scar",
    "diffny",
    "bammabi",
    "skort mc dort"
};


struct UnitData {
    int id;
    int health;
    int rank;
    int exp;
    int texture_id;
    int portrait_id;
    std::string name;
};

extern std::unordered_map<int, UnitData> units_data;


inline void LoadGameData() {
    TraceLog(LOG_INFO, "LOADING GAME DATA....data.json");

    std::ifstream file("assets/data.json");
    if (!file.is_open()) {
        TraceLog(LOG_INFO, "CANNOT OPEN FILE");
        return;
    }
    
    json j;
    file>>j;
    TraceLog(LOG_INFO, "==========LOADING GAME DATA FROM FILE================");


    TraceLog(LOG_INFO, "==========loading base unit data================");

    for(int i = 0; i < j["base_unit"].size(); i++) {
        int id = -1; //not set
        int health = j["base_unit"][i]["health"];
        int rank = 0;
        int exp = 0;
        int texture_id = j["base_unit"][i]["texture_id"];
        int portrait_id = j["base_unit"][i]["portrait_id"];

        UnitData this_unit = {
            .id = id,
            .health = health,
            .rank = rank,
            .exp = exp,
            .texture_id = texture_id,
            .portrait_id = portrait_id,
            .name = "not assigned"
        };

        units_data[i] = this_unit;
    }
 
    TraceLog(LOG_INFO, "==========end base unit data================");

    TraceLog(LOG_INFO, "==========DATA LOADED================");
}
