//
// Created by Olcay Taner Yıldız on 9.03.2019.
//

#ifndef SPELLCHECKER_SIMPLESPELLCHECKER_H
#define SPELLCHECKER_SIMPLESPELLCHECKER_H


#include <FsmMorphologicalAnalyzer.h>
#include "SpellChecker.h"

class SimpleSpellChecker : public SpellChecker {
protected:
    FsmMorphologicalAnalyzer fsm;
    vector<string> candidateList(Word* word);
private:
    vector<string> generateCandidateList(string word);
public:
    explicit SimpleSpellChecker(const FsmMorphologicalAnalyzer& fsm);
    Sentence* spellCheck(Sentence* sentence) override;
};


#endif //SPELLCHECKER_SIMPLESPELLCHECKER_H
