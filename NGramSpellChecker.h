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
    Sentence* spellCheck(Sentence* sentence);
};


#endif //SPELLCHECKER_NGRAMSPELLCHECKER_H
