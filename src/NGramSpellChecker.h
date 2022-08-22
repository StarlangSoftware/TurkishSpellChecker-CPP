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
    NGram<string> nGram = NGram<string>(1);
    bool rootNGram = false;
    double threshold = 0.0;
public:
    NGramSpellChecker(FsmMorphologicalAnalyzer fsm, NGram<string>& nGram, bool rootNGram);
    Word* checkAnalysisAndSetRoot(Sentence* sentence, int index);
    void setThreshold(double threshold);
    Sentence* spellCheck(Sentence* sentence) override;
};


#endif //SPELLCHECKER_NGRAMSPELLCHECKER_H
