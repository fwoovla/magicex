#pragma once
#include "gamedefs.h"

enum DIALOGUE_TOPIC {
    TOPIC_NONE = -1,
    TOPIC_GENERAL,
    TOPIC_TRADE,
};


struct DialogueNode;

struct DialogueResponse {
    std::string text = "";
    std::string next_node = "";
};

struct DialogueNode {
    std::string node_id = "";
    std::string text = "";
    std::vector<DialogueResponse> options; 
};

struct DialogueTree {
    DIALOGUE_TOPIC topic;
    std::string start_node = "";
    //std::vector<DialogueNode> nodes;
    std::unordered_map<std::string, DialogueNode> nodes;
};


struct ActiveDialogue {
    DialogueTree *tree;
    DialogueNode *active_node;
};

extern ActiveDialogue g_active_dialogue;


extern std::unordered_map<DIALOGUE_TOPIC, DialogueTree> g_dialogues;

DIALOGUE_TOPIC StrToDialogueId(std::string id_str);

void LoadDialogue();