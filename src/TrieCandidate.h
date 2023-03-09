//
// Created by Olcay Taner YILDIZ on 7.03.2023.
//

#ifndef SPELLCHECKER_TRIECANDIDATE_H
#define SPELLCHECKER_TRIECANDIDATE_H


#include "Candidate.h"

class TrieCandidate : public Candidate{
private:
    int currentIndex;
    double currentPenalty;
public:
    TrieCandidate(const string& _word, int _currentIndex, double _currentPenalty);
    int getCurrentIndex() const;
    double getCurrentPenalty() const;
    void nextIndex();
};


#endif //SPELLCHECKER_TRIECANDIDATE_H
