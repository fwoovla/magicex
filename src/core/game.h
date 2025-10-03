#pragma once
#include <raylib.h>

class Game {
    public:
        void StartGame();
        void CleanUp();

        RenderTexture2D render_texture;

        SceneManager scene_manager;
};