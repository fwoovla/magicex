#pragma once
#include <raylib.h>
#include <string>

struct Label {
    std::string text;
    Vector2 position;
    Color default_color;
    int text_size;
};



void CreateLabel(Label &_label, Vector2 _position, int _text_size, Color _default_color, std::string _text);


void DrawLabelCentered(Label &_label);


void DrawLabel(Label &_label);

void DrawLabelWithBG(Label &_label, Color color);

void DrawLabelCenteredWithBG(Label &_label, Color color);
