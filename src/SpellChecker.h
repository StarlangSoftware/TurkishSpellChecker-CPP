//
// Created by Olcay Taner Yıldız on 9.03.2019.
//

#ifndef SPELLCHECKER_SPELLCHECKER_H
#define SPELLCHECKER_SPELLCHECKER_H

#include <Sentence.h>

class SpellChecker {
    /**
     * The spellCheck method which takes a {@link Sentence} as an input.
     *
     * @param sentence {@link Sentence} type input.
     * @return Sentence result.
     */
    virtual Sentence* spellCheck(Sentence* sentence) = 0;
};


#endif //SPELLCHECKER_SPELLCHECKER_H
