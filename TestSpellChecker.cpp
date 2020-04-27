//
// Created by Olcay Taner Yıldız on 2019-03-10.
//

#include <FsmMorphologicalAnalyzer.h>
#include "SpellChecker.h"
#include "SimpleSpellChecker.h"

int main(){
    FsmMorphologicalAnalyzer fsm;
    fsm = FsmMorphologicalAnalyzer();
    SimpleSpellChecker spellChecker = SimpleSpellChecker(fsm);
    Sentence* s1 = new Sentence("cekwt");
    Sentence* s2 = spellChecker.spellCheck(s1);
}