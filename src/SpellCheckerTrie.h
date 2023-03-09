//
// Created by Olcay Taner YILDIZ on 6.03.2023.
//

#ifndef SPELLCHECKER_SPELLCHECKERTRIE_H
#define SPELLCHECKER_SPELLCHECKERTRIE_H


#include "SpellCheckerTrieNode.h"

class SpellCheckerTrie {
private:
    SpellCheckerTrieNode *rootNode = nullptr;
public:
    SpellCheckerTrie();

    void insert(const string &word);

    bool search(const string &word) const;

    bool startsWith(const string &prefix) const;

    SpellCheckerTrieNode *getTrieNode(const string &word) const;
};

#endif //SPELLCHECKER_SPELLCHECKERTRIE_H
