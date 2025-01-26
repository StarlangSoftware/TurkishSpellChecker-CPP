//
// Created by Olcay Taner YILDIZ on 7.03.2023.
//

#include "TrieCandidate.h"

/**
 * Constructs a TrieCandidate object.
 *
 * @param _word the candidate word
 * @param _currentIndex the current index of the candidate word
 * @param _currentPenalty the currentPenalty associated with the candidate word
 */
TrieCandidate::TrieCandidate(const string& _word, int _currentIndex, double _currentPenalty) : Candidate(_word, Operator::TRIE_BASED){
    this->currentIndex = _currentIndex;
    this->currentPenalty = _currentPenalty;
}

/**
 * Returns the current index of the candidate word.
 *
 * @return the current index of the candidate word
 */
int TrieCandidate::getCurrentIndex() const {
    return currentIndex;
}

/**
 * Returns the currentPenalty value associated with the candidate word.
 *
 * @return the currentPenalty value associated with the candidate word
 */
double TrieCandidate::getCurrentPenalty() const {
    return currentPenalty;
}

/**
 * Increments the current index of the candidate word by 1.
 */
void TrieCandidate::nextIndex() {
    currentIndex++;
}
