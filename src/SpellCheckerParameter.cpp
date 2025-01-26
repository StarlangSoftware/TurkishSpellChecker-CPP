//
// Created by Olcay Taner YILDIZ on 23.11.2022.
//

#include "SpellCheckerParameter.h"

/**
 * Constructs a SpellCheckerParameter object with default values.
 *
 * The default threshold is 0.0, the De-Mi check is enabled, the root ngram is enabled and
 * the minimum word length is 4.
 */
SpellCheckerParameter::SpellCheckerParameter() = default;

/**
 * Sets the threshold value used in calculating the n-gram probabilities.
 *
 * @param _threshold the threshold for the spell checker
 */
void SpellCheckerParameter::setThreshold(double _threshold) {
    this->threshold = _threshold;
}

/**
 * Enables or disables De-Mi check for the spell checker.
 * @see SimpleSpellChecker#forcedDeDaSplitCheck(Word, Sentence)
 * @see SimpleSpellChecker#forcedQuestionSuffixSplitCheck(Word, Sentence) (Word, Sentence)
 *
 * @param _suffixCheck a boolean indicating whether the De-Mi check should be enabled (true) or disabled (false)
 */
void SpellCheckerParameter::setSuffixCheck(bool _suffixCheck) {
    this->suffixCheck = _suffixCheck;
}

/**
 * Enables or disables the root n-gram for the spell checker.
 *
 * @param _rootNGram a boolean indicating whether the root n-gram should be enabled (true) or disabled (false)
 */
void SpellCheckerParameter::setRootNGram(bool _rootNGram) {
    this->rootNGram = _rootNGram;
}

/**
 * Returns the threshold value used in calculating the n-gram probabilities.
 *
 * @return the threshold for the spell checker
 */
double SpellCheckerParameter::getThreshold() const{
    return threshold;
}

/**
 * Returns whether De-Mi check is enabled for the spell checker.
 * @see SimpleSpellChecker#forcedDeDaSplitCheck(Word, Sentence)
 * @see SimpleSpellChecker#forcedQuestionSuffixSplitCheck(Word, Sentence) (Word, Sentence)
 *
 * @return a boolean indicating whether De-Mi check is enabled for the spell checker
 */
bool SpellCheckerParameter::isSuffixCheck() const {
    return suffixCheck;
}

/**
 * Returns whether the root n-gram is enabled for the spell checker.
 *
 * @return a boolean indicating whether the root n-gram is enabled for the spell checker
 */
bool SpellCheckerParameter::isRootNGram() const {
    return rootNGram;
}

/**
 * Sets the minimum length of words viable for spell checking.
 *
 * @param _minWordLength the minimum word length for the spell checker
 */
void SpellCheckerParameter::setMinWordLength(int _minWordLength) {
    this->minWordLength = _minWordLength;
}

/**
 * Returns the minimum length of words viable for spell checking.
 *
 * @return the minimum word length for the spell checker
 */
int SpellCheckerParameter::getMinWordLength() const {
    return minWordLength;
}

/**
 * Sets the _domain name to the specified value.
 *
 * @param _domain the new _domain name to set for this object
 */
void SpellCheckerParameter::setDomain(const string& _domain) {
    this->domain = _domain;
}

/**
 * Returns the domain name
 *
 * @return the domain name
 */
string SpellCheckerParameter::getDomain() const{
    return domain;
}
