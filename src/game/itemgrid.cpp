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
    hovered_cell = {-1,-1};


    for (int c = 0; c <= cols; c++) { 
        for(int r = 0; r <= rows; r++) {
            Vector2 cell_pos;

            cell_pos.x = position.x + (c * grid_size);
            cell_pos.y = position.y + (r * grid_size);

            if(CheckCollisionPointRec(g_input.screen_mouse_position * g_inv_scale, {cell_pos.x, cell_pos.y, (float)grid_size, (float)grid_size})) {
                //TraceLog(LOG_INFO, "%0.0f %0.0f", cell_pos.x, cell_pos.y);
                cell_hovered = true;
                hovered_cell = {(float)c,(float)r};
            }
        }
    }
}

void ItemGrid::Draw() {
    for(int r = 0; r <= rows; r++) {
        //DrawLine(position.x, position.y + (r * grid_size), position.x + (cols * grid_size) , position.y + (r * grid_size), BLACK);
        for (int c = 0; c <= cols; c++) {
            //DrawLine(position.x + (c * grid_size), position.y, position.x + (c * grid_size), position.y + (rows * grid_size), BLACK);
            if(hovered_cell == Vector2{(float)c,(float)r} ) {
                DrawRectangle(position.x + (c * grid_size), position.y  + (r * grid_size), (float)grid_size, (float)grid_size, DARKGRAY);

            }
            else if( r < rows and c < cols ) {
                //DrawRectangleLines();
                DrawRectangle(position.x + (c * grid_size), position.y  + (r * grid_size), (float)grid_size - 1, (float)grid_size - 1, BLACK);
            }

            
            if(r * cols + c < item_sprites.size()) {
                //TraceLog(LOG_INFO, "item sprite %i  %i", (int)(r*cols+c), item_sprites.size());
                DrawSprite(item_sprites[r*cols+c]);
            }
        }
    }
    //TraceLog(LOG_INFO, "-----------------\n");
}

void ItemGrid::SetItems(std::vector<int> &list) {
    item_sprites.clear();
    for(int i = 0; i < list.size(); i++) {
        Sprite sp;
        int x = i%(cols);
        int y = i/(cols);
        LoadSpriteCentered(sp, g_item_sprites[g_item_data[list[i]].sprite_id], {position.x + (x * grid_size) + (grid_size/2), position.y + (y * grid_size) + (grid_size/2) });
        ScaleSprite(sp, {2,2});
        item_sprites.push_back(sp);
        //TraceLog(LOG_INFO, "item sprite %i:  %i %i", i, x, y);
    }
}