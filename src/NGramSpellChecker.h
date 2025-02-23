//
// Created by Olcay Taner Yıldız on 9.03.2019.
//

#ifndef SPELLCHECKER_NGRAMSPELLCHECKER_H
#define SPELLCHECKER_NGRAMSPELLCHECKER_H

#include <FsmMorphologicalAnalyzer.h>
#include "NGram.h"
#include "SimpleSpellChecker.h"

class NGramSpellChecker : public SimpleSpellChecker {
private:
    NGram<string>* nGram = new NGram<string>(1);
    Word* checkAnalysisAndSetRootForWordAtIndex(Sentence* sentence, int index);
    Word* checkAnalysisAndSetRoot(const string& word);
    double getProbability(const string& word1, const string& word2) const;
public:
    NGramSpellChecker(const FsmMorphologicalAnalyzer& fsm, NGram<string>* nGram, const SpellCheckerParameter& _parameter);
    NGramSpellChecker(const FsmMorphologicalAnalyzer& fsm, NGram<string>* nGram);
    Sentence* spellCheck(Sentence* sentence) override;
};


#endif //SPELLCHECKER_NGRAMSPELLCHECKER_H
