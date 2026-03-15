#pragma once
#include <raylib.h>

class Game {
    public:
        void StartGame();
        void CleanUp();

        RenderTexture2D scene_render_texture;
        RenderTexture2D ui_render_texture;

        SceneManager scene_manager;
};