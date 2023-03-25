//
// Created by Olcay Taner YILDIZ on 7.03.2023.
//

#ifndef SPELLCHECKER_CONTEXTBASEDSPELLCHECKER_H
#define SPELLCHECKER_CONTEXTBASEDSPELLCHECKER_H


#include "NGramSpellChecker.h"

class ContextBasedSpellChecker : public NGramSpellChecker{
private:
    map<string, vector<string>> contextList;
    int damerauLevenshteinDistance(const string& first, const string& second);
public:
    ContextBasedSpellChecker(const FsmMorphologicalAnalyzer& fsm, NGram<string>* nGram, const SpellCheckerParameter& _parameter);
    ContextBasedSpellChecker(const FsmMorphologicalAnalyzer& fsm, NGram<string>* nGram);
protected:
    void loadContextDictionaries();
    vector<Candidate*> candidateList(Word* word, Sentence* sentence) override;
};


#endif //SPELLCHECKER_CONTEXTBASEDSPELLCHECKER_H
