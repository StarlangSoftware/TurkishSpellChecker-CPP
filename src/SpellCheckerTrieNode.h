//
// Created by Olcay Taner YILDIZ on 6.03.2023.
//

#ifndef SPELLCHECKER_SPELLCHECKERTRIENODE_H
#define SPELLCHECKER_SPELLCHECKERTRIENODE_H

#include<map>
#include<string>

using namespace std;

class SpellCheckerTrieNode {
private:
    map<string, SpellCheckerTrieNode *> children;
    bool isEndOfWord = false;
public:
    SpellCheckerTrieNode();

    SpellCheckerTrieNode *getChild(const string &ch) const;

    void addChild(const string &ch, SpellCheckerTrieNode *child);

    string childrenToString() const;

    bool isWord() const;

    void setIsWord(bool isWord);
};

#endif //SPELLCHECKER_SPELLCHECKERTRIENODE_H
