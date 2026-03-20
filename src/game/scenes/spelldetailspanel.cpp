#include "../../core/gamedefs.h"




SpellDetailsPanel::SpellDetailsPanel(SpellData *_data, Vector2 position, float width, float height) {
    data = _data;

    panel_rect.x = position.x;
    panel_rect.y = position.y;
    panel_rect.width = width;
    panel_rect.height = height;
    LoadSprite(spell_icon, g_spell_icon_delivery[data->delivery_type], position);
    ScaleSprite(spell_icon, (Vector2){3,3});
    spell_icon.position.x += (spell_icon.dest.width/2) + 10;
    spell_icon.position.y += (spell_icon.dest.height/2) + 10;
    
    


    TraceLog(LOG_INFO, "SpellDetailsPanelL UI");
}

SpellDetailsPanel::~SpellDetailsPanel() {
    //UnloadSound(button_sound);
    TraceLog(LOG_INFO, "UI DESTRUCTOR:  SpellDetailsPanelL UI");
}

void SpellDetailsPanel::Draw() {
    DrawRectangleRec(panel_rect, DARKERGRAY);
    DrawSprite(spell_icon);
    TraceLog(LOG_INFO, "SpellDetailsPanelL UI drawing");

}

void SpellDetailsPanel::Update() {


}



/* void SpellDetailsPanel::OpenPanel(ItemInstanceData *_data) {

} */
