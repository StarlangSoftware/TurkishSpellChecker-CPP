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
public:
    NGramSpellChecker(FsmMorphologicalAnalyzer fsm, NGram<string>& nGram);
    Word* checkAnalysisAndSetRoot(Sentence* sentence, int index);
    Sentence* spellCheck(Sentence* sentence) override;
};


#endif //SPELLCHECKER_NGRAMSPELLCHECKER_H
