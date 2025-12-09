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
    show_details = false;
}

ItemGrid::~ItemGrid() {
    //TraceLog(LOG_INFO, "DESTRUCTOR ITEMGRID");
    container_iid = "n/a";
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
                int instance_id = (*item_list)[r * cols + c];
                hovered_cell = {(float)c,(float)r};
                if(!can_select) {
                    shared_data->dest_cell = hovered_cell;
                    shared_data->dest_grid = this_grid;
                }
                if(instance_id != -1) {
                    cell_hovered = true;
                    if(hovered_cell == last_hovered_cell and can_select) {
                        hovered_time += GetFrameTime() * 1;
                        //TraceLog(LOG_INFO, "hover time %0.2f", hovered_time);
                        if(hovered_time > 0.5f) {
                            show_details = true;
                        }
                    }
                    else {
                        hovered_time = 0.0f;
                        show_details = false;
                    }
                    last_hovered_cell = hovered_cell;

                    if(can_select){
                        //ItemID item_id = ITEM_ID_ERROR;
                        std::string i_name = "no item found";
                        auto itter = g_item_instances.find(instance_id);
                        if(itter != g_item_instances.end()) {
                            //TraceLog(LOG_INFO, "item id %i  at %i %i", item_id, c, r);
                            i_name = itter->second.item_name;
                            //item_id = itter->second.item_id;

                        }
                        Color color = g_item_type_colors[itter->second.type];
                        CreateLabel(name_label, {g_input.screen_mouse_position.x*g_inv_scale, (g_input.screen_mouse_position.y+ 50)*g_inv_scale}, 20, color, i_name.c_str());

                        if(show_details == true) {
                            CreateLabel(details_label, {g_input.screen_mouse_position.x*g_inv_scale, (g_input.screen_mouse_position.y + 100)*g_inv_scale}, 18, WHITE, i_name.c_str());
                        }
                        //TraceLog(LOG_INFO, "item id %i  at %i %i", item_id, c, r);
                    }
                        
                    if(g_input.selecting and can_select) {
                        cell_selected = true;
                        selected_cell = hovered_cell;
                        shared_data->source_grid = this_grid;
                        shared_data->source_cell = selected_cell;
                        shared_data->item_id = instance_id;
                        selecting.EmitSignal();
                    }
                    if(g_input.mouse_right and can_select) {
                        //cell_selected = true;
                        selected_cell = hovered_cell;
                        shared_data->source_grid = this_grid;
                        shared_data->source_cell = selected_cell;
                        shared_data->item_id = instance_id;
                        pickup.EmitSignal();
                    }
                }
            }
        }
    }
    
    if(!g_input.selecting) {

        int source_index = selected_cell.y * cols + selected_cell.x;
        int dest_index = hovered_cell.y * cols + hovered_cell.x;

        if(cell_selected) {
            if(shared_data->dest_grid != this_grid) {
                //item dropped in another grid
                transfer_item.EmitSignal();
            }
            else {
                //item dropped in this grid
                TraceLog(LOG_INFO, "DROPPED IN CELL  hc %0.0f %0.0f   %i", hovered_cell.x, hovered_cell.y, (*item_list)[source_index]);
                if(hovered_cell == Vector2{-1,-1} or (*item_list)[dest_index] != -1) {
                    TraceLog(LOG_INFO, "DROPPED IN invalid CELL  hc %0.0f %0.0f   %i", hovered_cell.x, hovered_cell.y, (*item_list)[source_index]);
                    item_sprites[source_index].position = {position.x + (selected_cell.x * grid_size) + (grid_size/2), position.y + (selected_cell.y * grid_size) + (grid_size/2) };
                }
                else {
                    TraceLog(LOG_INFO, "DROPPED IN valid CELL  hc %0.0f %0.0f   %i", hovered_cell.x, hovered_cell.y, (*item_list)[source_index]);
                    (*item_list)[dest_index] = (*item_list)[source_index];

                    auto itter = g_item_instances.find(shared_data->item_id);
                    int _id = ITEM_ID_ERROR;
                    if(itter != g_item_instances.end()) {
                        _id = itter->second.icon_id;
                    }

                    //LoadSpriteCentered(item_sprites[dest_index], g_icon_sprites[ g_item_data[ (*item_list)[dest_index] ].id ], {position.x + (hovered_cell.x * grid_size) + (grid_size/2), position.y + (hovered_cell.y * grid_size) + (grid_size/2) });
                    LoadSpriteCentered(item_sprites[dest_index], g_icon_sprites[_id], {position.x + (hovered_cell.x * grid_size) + (grid_size/2), position.y + (hovered_cell.y * grid_size) + (grid_size/2) });
                    ScaleSprite(item_sprites[dest_index], {2,2});
                    
                    (*item_list)[source_index] = -1;
                    item_sprites[source_index].position = {position.x + (selected_cell.x * grid_size) + (grid_size/2), position.y + (selected_cell.y * grid_size) + (grid_size/2) };
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
                DrawRectangle(position.x + (c * grid_size), position.y  + (r * grid_size), (float)grid_size - 1, (float)grid_size - 1, DARKERGRAY);
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
        DrawLabelCentered(name_label);
        if(show_details) {
            DrawLabelCenteredWithBG(details_label, DARKERGRAY);
        }
    }
}


void ItemGrid::SetItems(std::vector<int> *list) {
    item_list = list;
    item_sprites.clear();
    int item_count = item_list->size();

    //TraceLog(LOG_INFO, "setting items --list size %i  %s", item_list->size(), container_iid.c_str());

    for(int i = 0; i < cols*rows; i++) {
        if(i < item_count) {
            //TraceLog(LOG_INFO, "setting  index %i: item id %i  ", i, (*item_list)[i]);
            if((*item_list)[i] != -1){

                Sprite sp;
                int x = i%(cols);
                int y = i/(cols);
                auto itter = g_item_instances.find((*item_list)[i]);
                int _id = ITEM_ID_ERROR;
                if(itter != g_item_instances.end()) {
                    _id = itter->second.icon_id;

                }
                LoadSpriteCentered(sp, g_icon_sprites[_id], {position.x + (x * grid_size) + (grid_size/2), position.y + (y * grid_size) + (grid_size/2) });
                //LoadSpriteCentered(sp, g_icon_sprites[g_item_data[ (*item_list)[i] ].id], {position.x + (x * grid_size) + (grid_size/2), position.y + (y * grid_size) + (grid_size/2) });
                ScaleSprite(sp, {2,2});
                item_sprites.push_back(sp);
                //TraceLog(LOG_INFO, "   item id %i  ", (*item_list)[i]);
            }
            else {
                Sprite bs;
                item_sprites.push_back(bs);
                //TraceLog(LOG_INFO, "empty list item");
            }
        }
        else {
            item_list->push_back(-1);
            Sprite bs;
            item_sprites.push_back(bs);
            //TraceLog(LOG_INFO, "pushing new empty item");
        }
    }
    //TraceLog(LOG_INFO, "items set\n");
}

bool ItemGrid::HasRoom() {
    int count = 0;

    for(int i = 0; i < item_list->size(); i++) {
        if((*item_list)[i] != -1) {
            count++;
        }
    }
    if(count >= cols * rows) {
        return false;
    }
    return true;
}



bool ItemGrid::CanAddItem(int item_id, Vector2 dest_cell) {
    int index = dest_cell.y * cols + dest_cell.x;
    if((*item_list)[index] != -1) {
        return false;
    }

    if(accepted_type != TYPE_ALL) {

        int _type = -1;// TYPE_ALL;

        auto itter = g_item_instances.find(item_id);
         TraceLog(LOG_INFO, "checking  item instance id  :%i", item_id);
        if(itter != g_item_instances.end()) {
            _type = itter->second.type;
            TraceLog(LOG_INFO, "checking     accepted:%i  type:%i", accepted_type, _type);
            if(accepted_type !=  _type) {
                TraceLog(LOG_INFO, "rejecting item      accepted:%i  type:%i", accepted_type, _type);
                return false;
            }
        }
    }
    return true;
}

void ItemGrid::AddItem(int item_id) {
    //TraceLog(LOG_INFO, "moving item to :%s", container_iid.c_str());
     int _id = ITEM_ID_ERROR;
    for(int i = 0; i < item_list->size(); i++) {
        if((*item_list)[i] == -1) {
            int x = i%(cols);
            int y = i/(cols);
            (*item_list)[i] = item_id;

            auto itter = g_item_instances.find(item_id);
            if(itter != g_item_instances.end()) {
                    _id = itter->second.icon_id;
                    LoadSpriteCentered(item_sprites[i], g_icon_sprites[_id], {position.x + (x * grid_size) + (grid_size/2), position.y + (y * grid_size) + (grid_size/2) });
                    ScaleSprite(item_sprites[i], {2,2});
                    break;
                }
        }
    }
}

void ItemGrid::AddItem(int item_id, Vector2 dest_cell) {
    int index = dest_cell.y * cols + dest_cell.x;
    (*item_list)[index] = item_id;

    int _id = ITEM_ID_ERROR;

    auto itter = g_item_instances.find(item_id);
    if(itter != g_item_instances.end()) {
        _id = itter->second.icon_id;
        itter->second.container_id = container_iid;
    }
    LoadSpriteCentered(item_sprites[index], g_icon_sprites[_id ], {position.x + (dest_cell.x * grid_size) + (grid_size/2), position.y + (dest_cell.y * grid_size) + (grid_size/2) });
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