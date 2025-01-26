//
// Created by Olcay Taner YILDIZ on 6.03.2023.
//

#include "SpellCheckerTrieNode.h"

/**
 * A constructor of SpellCheckerTrieNode class which constructs a new SpellCheckerTrieNode with an empty children HashMap
 */
SpellCheckerTrieNode::SpellCheckerTrieNode() = default;

/**
 * Returns the child SpellCheckerTrieNode with the given character as its value.
 * @param ch The character value of the child SpellCheckerTrieNode.
 * @return SpellCheckerTrieNode with the given character value.
 */
SpellCheckerTrieNode *SpellCheckerTrieNode::getChild(const string& ch) const{
    if (children.contains(ch)){
        return children.find(ch)->second;
    } else {
        return nullptr;
    }
}

/**
 * Adds a child TrieNode to the current TrieNode instance.
 *
 * @param ch the character key of the child node to be added.
 * @param child the TrieNode object to be added as a child.
 */
void SpellCheckerTrieNode::addChild(const string& ch, SpellCheckerTrieNode* child) {
    children.emplace(ch, child);
}

/**
 * Returns a string representation of the keys of all child TrieNodes of the current TrieNode instance.
 *
 * @return a string of characters representing the keys of all child TrieNodes.
 */
string SpellCheckerTrieNode::childrenToString() const{
    string result;
    for (auto & it : children){
        result += it.first;
    }
    return result;
}

/**
 * Returns whether the current SpellCheckerTrieNode represents the end of a word.
 * @return true if the current SpellCheckerTrieNode represents the end of a word, false otherwise.
 */
bool SpellCheckerTrieNode::isWord() const{
    return isEndOfWord;
}

/**
 * Sets whether the current SpellCheckerTrieNode represents the end of a word.
 * @param isWord true if the current SpellCheckerTrieNode represents the end of a word, false otherwise.
 */
void SpellCheckerTrieNode::setIsWord(bool isWord) {
    isEndOfWord = isWord;
}
