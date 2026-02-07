#include "../core/gamedefs.h"

ItemGrid::ItemGrid(int c, int r, int s, Vector2 p, SharedItemData *sd) {
    shared_data = sd;
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
    grid_is_selectable = true;
    

    CreateLabel(details_label, {0,0}, FONTSIZE_24, WHITE, "");
}

ItemGrid::~ItemGrid() {
    //TraceLog(LOG_INFO, "DESTRUCTOR ITEMGRID");
    //container_iid = "n/a";
}

void ItemGrid::Update() {
    if (!item_list) {return;}

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
                    //TraceLog(LOG_INFO, "dest grid %i",shared_data->dest_grid); 
                }

                if(instance_id != -1) {
                    cell_hovered = true;

                    if(hovered_cell == last_hovered_cell) {
                        hovered_time += GetFrameTime() * 1;
                        if(hovered_time > 0.70f) {
                            show_details = true;
                        }
                    }
                    else {
                        hovered_time = 0.0f;
                        show_details = false;
                        details_label.text = "";
                    }

                    //TraceLog(LOG_INFO, "hc %0.0f %0.0f    lhc %0.0f  %0.0f", hovered_cell.x, hovered_cell.y, last_hovered_cell.x, last_hovered_cell.y);
                    if(can_select){
                        std::string i_name = "no item found";
                        Color color = DEFAULTITEMCOLOR;
                        auto itter = g_item_instances.find(instance_id);
                        if(itter != g_item_instances.end()) {
                            i_name = itter->second.item_name;
                            color = g_item_type_colors[itter->second.type];
                            //TraceLog(LOG_INFO, "item id %i instance id %i type %i ", itter->second.item_id, itter->second.instance_id, itter->second.type);
                            
                            if(show_details == true) {
                                shared_data->item_id = itter->second.instance_id;
                                shared_data->source_grid = this_grid;
                                selected_cell = hovered_cell;
                                shared_data->source_cell = selected_cell;
                                open_details.EmitSignal();

                            }
                            if(g_input.mouse_right and this_grid == GRID_INVENTORY and grid_is_selectable) {
                                if(itter->second.type == TYPE_FOOD) {
                                    shared_data->item_id = itter->second.instance_id;
                                    shared_data->source_grid = this_grid;
                                    selected_cell = hovered_cell;
                                    shared_data->source_cell = selected_cell;
                                    can_select = false;
                                    use_item.EmitSignal();
                                    return;
                                }
                            }
                        }
                        //TraceLog(LOG_INFO, "item id %i  at %i %i", item_id, c, r);
                    }
                        
                    if(g_input.selecting and can_select and grid_is_selectable) {
                        cell_selected = true;
                        selected_cell = hovered_cell;
                        shared_data->source_grid = this_grid;
                        shared_data->source_cell = selected_cell;
                        shared_data->item_id = instance_id;
                        selecting.EmitSignal();
                    }
                    if(g_input.mouse_right and can_select and grid_is_selectable) {
                        selected_cell = hovered_cell;
                        shared_data->source_grid = this_grid;
                        shared_data->source_cell = selected_cell;
                        shared_data->item_id = instance_id;

                        if(this_grid != GRID_INVENTORY) {
                            pickup.EmitSignal();
                        }
                        else {
                            putdown_or_equip.EmitSignal();
                        }
                    }
                }
            }
        }
    }

    if(shared_data->showing_details == true and shared_data->source_grid == this_grid) {
        if(hovered_cell == Vector2{-1, -1}) {
            close_details.EmitSignal();
        }
        else if(last_hovered_cell != hovered_cell) {
            close_details.EmitSignal();
        }
        
    }

    last_hovered_cell = hovered_cell;

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
    int hovered_id;
    int hovered_index;

    for(int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if(r * cols + c < item_sprites.size()) {
                int index = r*cols+c;
                //TraceLog(LOG_INFO, "item sprite %i  %i", (int)(r*cols+c), item_sprites.size());
                int item_id = (*item_list)[index];
                if(item_id != -1){
                    if(hovered_cell == (Vector2){(float)c,(float)r}){
                        hovered_id = item_id;
                        hovered_index = index;
                    }
                    if(selected_cell == (Vector2){(float)c,(float)r}) {
                        item_sprites[index].position = { g_input.screen_mouse_position.x*g_inv_scale, g_input.screen_mouse_position.y*g_inv_scale };
                    }
                    DrawSprite(item_sprites[index]);
                    for(int mod = 0; mod < g_item_instances[item_id].mod_slots; mod++) {
                        Color _color = LIGHTGRAY;
                        Vector2 corner = Vector2Add(item_sprites[index].position, {-(float)grid_size * 0.40f, -(float)grid_size * 0.40f});
                        DrawCircleV( Vector2Add( corner, {(float)mod * 8, 0} ), 2, _color );
                    }
                }
            }
        }
    }
}


void ItemGrid::SetItems(std::vector<int> *list) {
    item_list = list;
    item_sprites.clear();
    int item_count = item_list->size();

    TraceLog(LOG_INFO, "setting items --list size %i  %s", item_list->size(), container_iid.c_str());

    for(int i = 0; i < cols*rows; i++) {
        if(i < item_count) {
            //TraceLog(LOG_INFO, "setting  index %i: item id %i  ", i, (*item_list)[i]);
            if((*item_list)[i] != -1){

                Sprite sp;
                int x = i%(cols);
                int y = i/(cols);
                int _id = ITEM_ID_ERROR;
                
                auto itter = g_item_instances.find((*item_list)[i]);
                if(itter != g_item_instances.end()) {
                    _id = itter->second.icon_id;

                }
                LoadSpriteCentered(sp, g_icon_sprites[_id], {position.x + (x * grid_size) + (grid_size/2), position.y + (y * grid_size) + (grid_size/2) });
                //LoadSpriteCentered(sp, g_icon_sprites[g_item_data[ (*item_list)[i] ].id], {position.x + (x * grid_size) + (grid_size/2), position.y + (y * grid_size) + (grid_size/2) });
                ScaleSprite(sp, {2,2});
                item_sprites.push_back(sp);
                //TraceLog(LOG_INFO, "item grid %i   item id %i  ", this_grid, (*item_list)[i]);
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

    //equipment slot
    if(accepted_type != TYPE_ALL) {
        if((*item_list)[0] != -1) {
            return false;
        }
    }

    //inventory or ground
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
                    itter->second.container_id = container_iid;
                    LoadSpriteCentered(item_sprites[i], g_icon_sprites[_id], {position.x + (x * grid_size) + (grid_size/2), position.y + (y * grid_size) + (grid_size/2) });
                    ScaleSprite(item_sprites[i], {2,2});
                    break;
                }
        }
    }
}

void ItemGrid::AddItem(int item_id, Vector2 dest_cell) {
    int index = dest_cell.y * cols + dest_cell.x;
    int _id = ITEM_ID_ERROR;

    (*item_list)[index] = item_id;

    auto itter = g_item_instances.find(item_id);
    if(itter != g_item_instances.end()) {
        _id = itter->second.icon_id;
        itter->second.container_id = container_iid;
    }
    LoadSpriteCentered(item_sprites[index], g_icon_sprites[_id ], {position.x + (dest_cell.x * grid_size) + (grid_size/2), position.y + (dest_cell.y * grid_size) + (grid_size/2) });
    ScaleSprite(item_sprites[index], {2,2});
}

bool ItemGrid::CanRemoveItem(Vector2 source_cell) {
    return true;
}

void ItemGrid::RemoveItem(Vector2 source_cell) {
    int index = source_cell.y * cols + source_cell.x;
    (*item_list)[index] = -1;

}


std::string ItemGrid::CreateDetails(ItemInstanceData &item_data) {
    std::string details;
    auto itter = g_item_instances.find(item_data.instance_id);
    if(itter != g_item_instances.end()) {
        for(int mod = 0; mod < itter->second.mod_slots; mod++) {
            details += "--empty slot--\n";
        }
        if(itter->second.mod_slots > 0) {
            details += "\n";
        }
    
        if(item_data.type == TYPE_WEAPON) {
            details += std::to_string(itter->second.weapon_data.damage) + "  damage\n";
            std::string cool = TextFormat("%0.2f", itter->second.weapon_data.cooldown);
            details += "cooldown: " + cool + "\n";

            std::string knock = TextFormat("%0.2f", itter->second.weapon_data.knockback);
            details += "knockback: " + knock + "\n";

            std::string recoil = TextFormat("%0.2f", itter->second.weapon_data.recoil);
            details += "recoil: " + recoil + "\n";

            if(itter->second.weapon_data.max_power > 0){
                std::string mpower = TextFormat("%0.2f", itter->second.weapon_data.max_power);
                std::string cpower = TextFormat("%0.2f", itter->second.weapon_data.current_power);
                details += cpower + "/" +  mpower +"\n";
            }
        }

        if(item_data.type >= TYPE_HEAD_ARMOR and item_data.type <= TYPE_HAND_ARMOR) {
            if(itter->second.armor_data.magic_defence > 0) {
                details += "defence: " + std::to_string(itter->second.armor_data.defence) + "\n";
                details += "magic defence: " + std::to_string(itter->second.armor_data.magic_defence) + "\n";
                
            }
        }
        if(item_data.type ==  TYPE_CONSUMEABLE) {
        }
        if(item_data.type ==  TYPE_FOOD) {
            std::string sat = TextFormat("%0.2f", itter->second.food_data.saturation);
            details += "saturation: " + sat + "\n";
            details += "  [ RMB ] to eat\n";
        }
        if(item_data.type ==  TYPE_PLAN) {
            details += "use with " +  ModuleIdToStr(  g_plan_data[item_data.item_id - ITEM_ID_STOVE_PLAN].module_id) + "\n";
        }
        if(item_data.type ==  TYPE_RESOURCE) {
            if(item_data.item_id == ITEM_ID_MUSHROOM_JUICE) {
                details += "'R'- Recharge power\n";
            }
        }
        details += "$" + std::to_string( itter->second.value);
    }
    return details;
}