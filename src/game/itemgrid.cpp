#include "../core/gamedefs.h"

ItemGrid::ItemGrid(int c, int r, int s, Vector2 p) {
    rows = r;
    cols = c;
    grid_size = s;
    position = p;
}

ItemGrid::~ItemGrid() {

}

void ItemGrid::Update() {

    cell_hovered = false;


    for (int c = 0; c <= cols; c++) { 
        for(int r = 0; r <= rows; r++) {
            Vector2 cell_pos;
            cell_pos.x = position.x + (c * grid_size);
            cell_pos.y = position.y + (r * grid_size);

            if(CheckCollisionPointRec(g_input.screen_mouse_position * g_inv_scale, {cell_pos.x, cell_pos.y, (float)grid_size, (float)grid_size})) {
                //TraceLog(LOG_INFO, "%0.0f %0.0f", cell_pos.x, cell_pos.y);
                cell_hovered = true;
                hovered_cell = cell_pos;
            }
        }
    }
}

void ItemGrid::Draw() {
    for(int r = 0; r <= rows; r++) {
        DrawLine(position.x, position.y + (r * grid_size), position.x + (cols * grid_size) , position.y + (r * grid_size), BLACK);
    }
    for (int c = 0; c <= cols; c++) {
        DrawLine(position.x + (c * grid_size), position.y, position.x + (c * grid_size), position.y + (rows * grid_size), BLACK);
    }

    if(cell_hovered) {
        DrawRectangle(hovered_cell.x, hovered_cell.y, (float)grid_size, (float)grid_size, BLUE);
    }

}