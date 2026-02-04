#include "../core/gamedefs.h"


void LoadDialogue() {
    std::ifstream cfile("assets/dialogue.json");
    if (!cfile.is_open()) {
        TraceLog(LOG_INFO, "CANNOT OPEN DIALOGUE FILE");
        return;
    }




    


    cfile.close();

}