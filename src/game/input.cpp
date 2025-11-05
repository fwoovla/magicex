#include "../core/gamedefs.h"


void GetInputFromPlayer() {

    //float screen2world = 1/g_world2screen;
    //g_world2screen = g_camera.zoom * g_scale;

    Vector2 m_pos = GetMousePosition();
    
    g_input.screen_mouse_position =  m_pos;

//this took way too long!!!!!
    g_input.world_mouse_position.x = (m_pos.x * g_screen2world) + (g_camera.target.x);
    g_input.world_mouse_position.y = (m_pos.y * g_screen2world) + (g_camera.target.y);
//----
    
    g_input.mouse_left = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    if(g_input.mouse_left) {
        g_input.mouse_left_clicked_screen_position = g_input.screen_mouse_position;
        g_input.mouse_left_clicked_world_position = g_input.world_mouse_position;
    }
    
    g_input.mouse_left_down = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    if(!g_input.mouse_left_down) {
        g_input.selecting = false;
        g_input.selected_rect = {0,0,0,0};
    }


    g_input.mouse_right = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);

    if(g_input.mouse_right) {
        g_input.mouse_right_clicked_screen_position = g_input.screen_mouse_position;
        g_input.mouse_right_clicked_world_position = g_input.world_mouse_position;
    }
    g_input.mouse_right_down = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);
    
    g_input.mouse_middle = IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE);
    g_input.mouse_middle_down = IsMouseButtonDown(MOUSE_BUTTON_MIDDLE);

    g_input.mouse_wheel = GetMouseWheelMove();
    
    
    memset(g_input.keys_pressed, KEY_NULL, KEY_BUFFER_SIZE);

    int key = GetKeyPressed();
    while(key != 0) {
        //TraceLog(LOG_INFO, "next key %i", key);
        for(int i = 0; i < KEY_BUFFER_SIZE; i++) {
            if(g_input.keys_pressed[i] == 0) {
                g_input.keys_pressed[i] = (KeyboardKey)key;
                //TraceLog(LOG_INFO, "indexing key %i", i);
                break;
            }
        }
        key = GetKeyPressed();
        
    } 

    g_input.key_up = IsKeyDown(KEY_W);
    g_input.key_down = IsKeyDown(KEY_S);
    g_input.key_left = IsKeyDown(KEY_A);
    g_input.key_right = IsKeyDown(KEY_D);

    g_input.key_sprint = IsKeyDown(KEY_LEFT_SHIFT);

    g_input.key_use = IsKeyDown(KEY_SPACE);


    if(g_input.selecting) {
        
        float _x = 0.0f;
        float _y = 0.0f;
        float _width = 0.0f;
        float _height = 0.0f;

        if(g_input.world_mouse_position.x > g_input.mouse_left_clicked_world_position.x) {
            _x = g_input.mouse_left_clicked_world_position.x;
            _width = g_input.world_mouse_position.x - g_input.mouse_left_clicked_world_position.x;
        }
          else {
            _x = g_input.world_mouse_position.x;
            _width = g_input.mouse_left_clicked_world_position.x - g_input.world_mouse_position.x;
        }

        if(g_input.world_mouse_position.y > g_input.mouse_left_clicked_world_position.y) {
            _y = g_input.mouse_left_clicked_world_position.y;
            _height = g_input.world_mouse_position.y - g_input.mouse_left_clicked_world_position.y;
        }
        else {
            _y = g_input.world_mouse_position.y;
            _height = g_input.mouse_left_clicked_world_position.y - g_input.world_mouse_position.y;
        }

        g_input.selected_rect = {
            .x = _x,
            .y = _y,
            .width = _width,
            .height = _height
        };
    }

    if(g_input.mouse_left and !g_input.selecting) {
        g_input.selecting = true;
    }
}