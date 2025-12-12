#include "../core/gamedefs.h"

void CreateLabel(Label &_label, Vector2 _position, int _text_size, Color _default_color, std::string _text) {
    _label.default_color = _default_color;
    _label.position = _position;
    _label.text_size = _text_size;
    _label.text = _text;
}

void DrawLabelCentered(Label &_label) {
    int tw = MeasureText(_label.text.c_str(), _label.text_size);
    DrawTextEx(g_font, _label.text.c_str(), { (float)(_label.position.x - (tw*0.5) ), (float)(_label.position.y - (_label.text_size *0.5f) ) }, _label.text_size, 0, _label.default_color);
    //DrawText((TextFormat("%s",_label.text.c_str())), _label.position.x - (tw*0.5), _label.position.y - (_label.text_size *0.5f), _label.text_size, _label.default_color);
}

void DrawLabel(Label &_label) {
    DrawTextEx(g_font, _label.text.c_str(), _label.position, _label.text_size, 0, _label.default_color);
    //DrawText((TextFormat("%s",_label.text.c_str())), _label.position.x, _label.position.y, _label.text_size, _label.default_color);
}


void DrawLabelWithBG(Label &_label, Color color) {
    int tw = MeasureText(_label.text.c_str(),  _label.text_size * 0.6);
    int lines = 1;
    for (char c : _label.text) {
        if (c == '\n') lines++;
    }

    DrawRectangle( _label.position.x, _label.position.y - (_label.text_size *0.5f), tw + 10, ((_label.text_size + 2) * lines), color );
    DrawTextEx(g_font, _label.text.c_str(), _label.position, _label.text_size, 0, _label.default_color);
    //TraceLog(LOG_INFO, "text size %i",tw);
    
}

void DrawLabelCenteredWithBG(Label &_label, Color color) {
    int tw = MeasureText(_label.text.c_str(), _label.text_size * 0.6);
    int lines = 1;
    for (char c : _label.text) {
        if (c == '\n') lines++;
    }

    DrawRectangle( _label.position.x - 5 - (tw*0.5), _label.position.y - 5 - (_label.text_size *0.5f), tw + 10, ((_label.text_size + 2) * lines), color );
    DrawTextEx(g_font, _label.text.c_str(), { (float)(_label.position.x - (tw*0.5) ), (float)(_label.position.y - (_label.text_size *0.5f) ) }, _label.text_size, 0, _label.default_color);
    //DrawText((TextFormat("%s",_label.text.c_str())), _label.position.x - (tw*0.5), _label.position.y - (_label.text_size *0.5f), _label.text_size, _label.default_color);
}