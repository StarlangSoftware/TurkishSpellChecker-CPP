//
// Created by Olcay Taner YILDIZ on 6.03.2023.
//
#include "Dictionary/Word.h"
#include "SpellCheckerTrie.h"

/**
 * A constructor of SpellCheckerTrie class which constructs a new SpellCheckerTrie with an empty root node
 */
SpellCheckerTrie::SpellCheckerTrie() {
    rootNode = new SpellCheckerTrieNode();
}

/**
 * Inserts a new word into the SpellCheckerTrie
 *
 * @param word The word to be inserted
 */
void SpellCheckerTrie::insert(const string& word) {
    SpellCheckerTrieNode* currentNode = rootNode;
    for (int i = 0; i < Word::size(word); i++) {
        string ch = Word::charAt(word, i);
        if (currentNode->getChild(ch) == nullptr) {
            currentNode->addChild(ch, new SpellCheckerTrieNode());
        }
        currentNode = currentNode->getChild(ch);
    }
    currentNode->setIsWord(true);
}

/**
 * Checks if a word is in the SpellCheckerTrie
 *
 * @param word The word to be searched for
 * @return true if the word is in the SpellCheckerTrie, false otherwise
 */
bool SpellCheckerTrie::search(const string &word) const {
    SpellCheckerTrieNode* node = getTrieNode(Word::toLowerCase(word));
    if (node == nullptr) {
        return false;
    } else {
        return node->isWord();
    }
}

/**
 * Checks if a given prefix exists in the SpellCheckerTrie
 *
 * @param prefix The prefix to be searched for
 * @return true if the prefix exists, false otherwise
 */
bool SpellCheckerTrie::startsWith(const string &prefix) const {
    if (getTrieNode(Word::toLowerCase(prefix)) == nullptr) {
        return false;
    } else {
        return true;
    }
}

/**
 * Returns the SpellCheckerTrieNode corresponding to the last character of a given word
 *
 * @param word The word to be searched for
 * @return The SpellCheckerTrieNode corresponding to the last character of the word
 */
SpellCheckerTrieNode *SpellCheckerTrie::getTrieNode(const string &word) const {
    SpellCheckerTrieNode* currentNode = rootNode;
    for (int i = 0; i < Word::size(word); i++) {
        string ch = Word::charAt(word, i);
        if (currentNode->getChild(ch) == nullptr) {
            return nullptr;
        }
        currentNode = currentNode->getChild(ch);
    }
    return currentNode;
}
