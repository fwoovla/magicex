#include "core/gamedefs.h"

GameSettings g_game_settings = {
    .window_size = {1280, 720},
    .resolution = {640*1.5, 360*1.5},
    .show_debug = false
    
};


int main(int argc, char *argv[]){


    InitWindow(g_game_settings.window_size.x, g_game_settings.window_size.y, "new window");
    Game game;

    game.StartGame();
    
    game.CleanUp();

    return 0;
}