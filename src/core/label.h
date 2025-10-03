#pragma once
#include <raylib.h>
#include <string>

struct Label {
    std::string text;
    Vector2 position;
    Color default_color;
    int text_size;
};

inline void CreateLabel(Label &_label, Vector2 _position, int _text_size, Color _default_color, std::string _text) {
    _label.default_color = _default_color;
    _label.position = _position;
    _label.text_size = _text_size;
    _label.text = _text;
}

inline void DrawLabelCentered(Label &_label) {
    //TraceLog(LOG_INFO, "text size %i", _label.text_size);
    int tw = MeasureText(_label.text.c_str(), _label.text_size);
    DrawText((TextFormat("%s",_label.text.c_str())), _label.position.x - (tw*0.5), _label.position.y - (_label.text_size *0.5f), _label.text_size, _label.default_color);
}

inline void DrawLabel(Label &_label) {
    //TraceLog(LOG_INFO, "text size %i", _label.text_size);
    //int tw = MeasureText(_label.text.c_str(), _label.text_size);
    DrawText((TextFormat("%s",_label.text.c_str())), _label.position.x, _label.position.y, _label.text_size, _label.default_color);
}