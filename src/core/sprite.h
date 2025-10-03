#pragma once
#include <raylib.h>
#include <raymath.h>

struct Sprite {
    Texture2D texture;
    Vector2 position;
    Vector2 center;
    Vector2 size;
    Rectangle source;
    Rectangle dest;
    Vector2 scale;
    float roataion;
    Color modulate;
    
};

struct AnimatedSprite : public Sprite {

    int max_frames;
    int frame;
    int y_index;
    float frame_delay;
    double last_frame_time;
    float frame_size;
};



inline void LoadSpriteCentered(Sprite &_sprite, Texture2D _texture, Vector2 _position) {
    _sprite.texture = _texture;
    _sprite.size = {(float)_texture.width, (float)_texture.height};
    _sprite.position = _position;
    _sprite.center = { _sprite.size.x/2, _sprite.size.y /2 };
    _sprite.source = {
        .x = 0,
        .y = 0,
        .width = _sprite.size.x,
        .height = _sprite.size.y,
    };
    _sprite.dest = {
        _sprite.position.x,
        _sprite.position.y,
        _sprite.size.x,
        _sprite.size.y  
    };
    _sprite.roataion = 0.0;
    _sprite.modulate = WHITE;
        
}


inline void LoadSpriteCentered(AnimatedSprite &_sprite, Texture2D _texture, Vector2 _position, int _max_frames, float _frame_size, float _frame_delay) {
    _sprite.texture = _texture;
    _sprite.size = {_frame_size, _frame_size};
    _sprite.position = _position;
    _sprite.center = { _sprite.size.x/2, _sprite.size.y /2 };
    _sprite.source = {
        .x = 0,
        .y = 0,
        .width = _sprite.size.x,
        .height = _sprite.size.y,
    };
    _sprite.dest = {
        _sprite.position.x,
        _sprite.position.y,
        _sprite.size.x,
        _sprite.size.y  
    };
    _sprite.roataion = 0.0;
    _sprite.modulate = WHITE;

    _sprite.max_frames = _max_frames;
    _sprite.frame_delay = _frame_delay;
    _sprite.last_frame_time = GetTime();
    _sprite.frame = 0;
    _sprite.y_index = 0;
        
}


inline void LoadSprite(Sprite &_sprite, Texture2D _texture, Vector2 _position) {
    _sprite.texture = _texture;
    _sprite.size = {(float)_texture.width, (float)_texture.height};
    _sprite.position = _position;
     _sprite.center = {0,0};
    _sprite.source = {
        .x = 0,
        .y = 0,
        .width = _sprite.size.x,
        .height = _sprite.size.y,
    };
    _sprite.dest = {
        _sprite.position.x,
        _sprite.position.y,
        _sprite.size.x,
        _sprite.size.y  
    };
    _sprite.roataion = 0.0;
    _sprite.modulate = WHITE;
}

inline void LoadSprite(AnimatedSprite &_sprite, Texture2D _texture, Vector2 _position, int _max_frames, float _frame_size, float _frame_delay) {
    _sprite.texture = _texture;
    _sprite.size = {_frame_size, _frame_size};
    _sprite.position = _position;
    _sprite.center = {0,0};
    _sprite.source = {
        .x = 0,
        .y = 0,
        .width = _sprite.size.x,
        .height = _sprite.size.y,
    };
    _sprite.dest = {
        _sprite.position.x,
        _sprite.position.y,
        _sprite.size.x,
        _sprite.size.y  
    };
    _sprite.roataion = 0.0;
    _sprite.modulate = WHITE;

    _sprite.max_frames = _max_frames;
    _sprite.frame_delay = _frame_delay;
    _sprite.last_frame_time = GetTime();
    _sprite.frame = 0;
    _sprite.y_index = 0;
}

inline void ScaleSprite(Sprite &_sprite, Vector2 _scale) {
    
    _sprite.scale = _scale;
    _sprite.center = { abs((_sprite.size.x * _sprite.scale.x)) * 0.5f, abs((_sprite.size.y * _sprite.scale.y)) * 0.5f };
    _sprite.source = {
        .x = 0,
        .y = 0,
        .width = _sprite.size.x,
        .height = _sprite.size.y
    };
    _sprite.dest = {
        _sprite.position.x,
        _sprite.position.y,
        _sprite.size.x * _sprite.scale.x,
        _sprite.size.y * _sprite.scale.y  
    };
}

inline void DrawSprite(Sprite &_sprite) {
    DrawTexturePro(_sprite.texture, _sprite.source, _sprite.dest, _sprite.center, _sprite.roataion, _sprite.modulate );       
}

inline void DrawSprite(AnimatedSprite &_sprite) {
    _sprite.dest.x = _sprite.position.x;
    _sprite.dest.y = _sprite.position.y;
    
    if(_sprite.last_frame_time + _sprite.frame_delay < GetTime()) {
        _sprite.frame++;
        _sprite.last_frame_time = GetTime();
 
        if (_sprite.frame >= _sprite.max_frames) {
            _sprite.frame = 0;
        }
         
        _sprite.source.x = (_sprite.frame * _sprite.size.x);
        _sprite.source.y = _sprite.y_index * _sprite.size.y;
    }
    
    DrawTexturePro(_sprite.texture, _sprite.source, _sprite.dest, _sprite.center, _sprite.roataion, _sprite.modulate );       
}

inline void SetAmination(AnimatedSprite &_sprite, int _y_index) {
    _sprite.y_index = _y_index;
}