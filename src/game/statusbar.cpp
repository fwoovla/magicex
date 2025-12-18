#include "../core/gamedefs.h"


void CreateStatusBar (StatusBar &bar, Vector2 _position, float _max_value, float _width, float _height, Color _color) {
    bar.position = _position;
    bar.max_value = _max_value;
    bar.current_value = _max_value;
    bar.bar_color = _color;
    bar.width = _width;
    bar.height = _height;
    bar.unit_ratio =  bar.width / bar.max_value;
}

void DrawStatusBar(StatusBar &bar) {
    Rectangle max_rect = {
        .x = bar.position.x,
        .y = bar.position.y,
        .width = bar.width,
        .height = bar.height
    };

    Rectangle status_rect = {
        .x = bar.position.x,
        .y = bar.position.y,
        .width = bar.current_value * bar.unit_ratio,
        .height = bar.height
    };

    DrawRectangleLines(max_rect.x, max_rect.y, max_rect.width, max_rect.height, bar.bar_color);
    DrawRectangleRec(status_rect, bar.bar_color);

}

void UpdateStatusBar(StatusBar &bar) {

}