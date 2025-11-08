#include "../core/gamedefs.h"

ItemGrid::ItemGrid(int c, int r, int s, Vector2 p, SharedItemData *sd) : shared_data(sd) {
    rows = r;
    cols = c;
    grid_size = s;
    position = p;
    cell_selected = false;
    cell_hovered = false;
    selected_cell = {-1,-1};
    hovered_cell = {-1,-1};
    can_select = true;
}

ItemGrid::~ItemGrid() {
    TraceLog(LOG_INFO, "DESTRUCTOR ITEMGRID");
}

void ItemGrid::Update() {

    cell_hovered = false;
    hovered_cell = {-1,-1};

    for (int c = 0; c < cols; c++) { 
        for(int r = 0; r < rows; r++) {
            Vector2 cell_pos;
                
            cell_pos.x = position.x + (c * grid_size);
            cell_pos.y = position.y + (r * grid_size);
                
            if(CheckCollisionPointRec(g_input.screen_mouse_position * g_inv_scale, {cell_pos.x, cell_pos.y, (float)grid_size, (float)grid_size})) {
                int item_id = (*item_list)[r * cols + c];
                hovered_cell = {(float)c,(float)r};
                if(!can_select) {
                    shared_data->dest_cell = hovered_cell;
                    shared_data->dest_grid = this_grid;
                }
                if(item_id != -1) {
                    cell_hovered = true;

                    if(can_select){
                        std::string i_name = g_item_data[item_id].item_name;
                        CreateLabel(label, {g_input.screen_mouse_position.x*g_inv_scale, (g_input.screen_mouse_position.y+ 50)*g_inv_scale}, 20, WHITE, i_name.c_str());
                        //TraceLog(LOG_INFO, "item id %i  at %i %i", item_id, c, r);
                    }
                        
                    if(g_input.selecting and can_select) {
                        cell_selected = true;
                        selected_cell = hovered_cell;
                        //shared_data->source_list = *item_list;
                        shared_data->source_grid = this_grid;
                        shared_data->source_cell = selected_cell;
                        shared_data->item_id = item_id;
                        selecting.EmitSignal();
                    }
                }
            }
        }
    }

    if(!g_input.selecting) {

        if(cell_selected) {
            int index = selected_cell.y * cols + selected_cell.x;
            if(shared_data->dest_grid != this_grid) {
                //item dropped in another grid
                transfer_item.EmitSignal();
            }
            else {
                //item dropped in this grid
                if(hovered_cell == selected_cell) {
                    item_sprites[index].position = {position.x + (selected_cell.x * grid_size) + (grid_size/2), position.y + (selected_cell.y * grid_size) + (grid_size/2) };
                }
                else {
                    int new_index = hovered_cell.y * cols + hovered_cell.x;
                    (*item_list)[new_index] = (*item_list)[index];
                    LoadSpriteCentered(item_sprites[new_index], g_item_sprites[g_item_data[ (*item_list)[new_index] ].sprite_id], {position.x + (hovered_cell.x * grid_size) + (grid_size/2), position.y + (hovered_cell.y * grid_size) + (grid_size/2) });
                    ScaleSprite(item_sprites[new_index], {2,2});
                    
                    (*item_list)[index] = -1;
                    item_sprites[index].position = {position.x + (selected_cell.x * grid_size) + (grid_size/2), position.y + (selected_cell.y * grid_size) + (grid_size/2) };
                }
            }
            not_selecting.EmitSignal();
        }
        cell_selected = false;
        selected_cell = {-1,-1};
    }

}

void ItemGrid::DrawGrid() {
    for(int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
 
            if(hovered_cell == Vector2{(float)c,(float)r} ) {
                DrawRectangle(position.x + (c * grid_size), position.y  + (r * grid_size), (float)grid_size, (float)grid_size, DARKGRAY);
            }
            else if( r < rows and c < cols ) {
                DrawRectangle(position.x + (c * grid_size), position.y  + (r * grid_size), (float)grid_size - 1, (float)grid_size - 1, BLACK);
            }
        }
    }
    //TraceLog(LOG_INFO, "-----------------\n");
}

void ItemGrid::DrawItems() {
    for(int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if(r * cols + c < item_sprites.size()) {
                int index = r*cols+c;
                //TraceLog(LOG_INFO, "item sprite %i  %i", (int)(r*cols+c), item_sprites.size());
                int item_id = (*item_list)[index];
                if(item_id != -1){
                    if(selected_cell == (Vector2){(float)c,(float)r}) {
                        item_sprites[index].position = { g_input.screen_mouse_position.x*g_inv_scale, g_input.screen_mouse_position.y*g_inv_scale };
                    }
                    DrawSprite(item_sprites[index]);
                }
            }
        }
    }

    if(cell_hovered and can_select) {
        DrawLabelCentered(label);
    }
}


void ItemGrid::SetItems(std::vector<int> *list) {
    item_list = list;
    item_sprites.clear();
    int item_count = item_list->size();

    for(int i = 0; i < cols*rows; i++) {
        if(i < item_count) {
            if((*item_list)[i] != -1){

                Sprite sp;
                int x = i%(cols);
                int y = i/(cols);
                LoadSpriteCentered(sp, g_item_sprites[g_item_data[ (*item_list)[i] ].sprite_id], {position.x + (x * grid_size) + (grid_size/2), position.y + (y * grid_size) + (grid_size/2) });
                ScaleSprite(sp, {2,2});
                item_sprites.push_back(sp);
                TraceLog(LOG_INFO, "item sprite %i:  %i %i  %i", i, x, y, (*item_list)[i]);
            }
            else {
                Sprite bs;
                item_sprites.push_back(bs);
            }
        }
        else {
            item_list->push_back(-1);
            Sprite bs;
            item_sprites.push_back(bs);
        }
    }
}

bool ItemGrid::CanAddItem(Vector2 dest_cell) {
    int index = dest_cell.y * cols + dest_cell.x;
    return (*item_list)[index] == -1;
}

void ItemGrid::AddItem(int item_id, Vector2 dest_cell) {
    int index = dest_cell.y * cols + dest_cell.x;
    (*item_list)[index] = item_id;

    LoadSpriteCentered(item_sprites[index], g_item_sprites[g_item_data[ (*item_list)[index] ].sprite_id], {position.x + (hovered_cell.x * grid_size) + (grid_size/2), position.y + (hovered_cell.y * grid_size) + (grid_size/2) });
    ScaleSprite(item_sprites[index], {2,2});
}

bool ItemGrid::CanRemoveItem(Vector2 source_cell) {
    int index = source_cell.y * cols + source_cell.x;
    return (*item_list)[index] == -1;;
}

void ItemGrid::RemoveItem(Vector2 source_cell) {
    int index = source_cell.y * cols + source_cell.x;
    (*item_list)[index] = -1;

}