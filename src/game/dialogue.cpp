#include "../core/gamedefs.h"


void LoadDialogue() {
    std::ifstream cfile("assets/dialogue.json");
    if (!cfile.is_open()) {
        TraceLog(LOG_INFO, "CANNOT OPEN DIALOGUE FILE");
        return;
    }



    json root;
    cfile>>root;

    for (auto& [dialogue_id, dlg_json] : root["dialogues"].items()) {
        DialogueTree tree;
        tree.start_node = dlg_json["start"];

        for (auto& [node_id, node_json] : dlg_json["nodes"].items()) {
            DialogueNode node;
            node.node_id = node_id;
            node.text = node_json["text"];

            if (node_json.contains("action")){}

            if (node_json.contains("choices")) {
                for (auto& c : node_json["choices"]) {
                    DialogueResponse choice;
                    choice.text = c["text"];
                    choice.next_node = c["next"];
                    node.options.push_back(choice);
                    TraceLog(LOG_INFO, "DIALOGUE option #%s ", choice.next_node.c_str());
                }
            }

            tree.nodes[node_id] = node;
            TraceLog(LOG_INFO, "DIALOGUE node #%s added %s", node_id.c_str(), node.text.c_str());
        }
        DIALOGUE_TOPIC id = StrToDialogueId(dialogue_id);
        g_dialogues[id] = tree;
        TraceLog(LOG_INFO, "DIALOGUE tree #%i added %s", id, tree.start_node.c_str());
    }


    cfile.close();

}



DIALOGUE_TOPIC StrToDialogueId(std::string id_str) {

    static const std::unordered_map<std::string, DIALOGUE_TOPIC> lookup_table = {
        {"RATOPIC_GENERALRITY_COMMON",       DIALOGUE_TOPIC::TOPIC_GENERAL},
        {"TOPIC_TRADE",     DIALOGUE_TOPIC::TOPIC_TRADE},
    };

    if (auto it = lookup_table.find(id_str); it != lookup_table.end()) {
        //TraceLog(LOG_INFO, "Spell ID found %i", it->second);
        return it->second;
    }
    //TraceLog(LOG_INFO, "Spell ID not found ");
    return DIALOGUE_TOPIC::TOPIC_NONE;
}


