//
// Created by Olcay Taner Yıldız on 9.03.2019.
//

#include "NGramSpellChecker.h"

/**
 * A constructor of NGramSpellChecker class which takes a FsmMorphologicalAnalyzer and an NGram
 * as inputs. Then, calls its super class SimpleSpellChecker with given FsmMorphologicalAnalyzer and
 * assigns given NGram to the nGram variable.
 *
 * @param fsm   FsmMorphologicalAnalyzer type input.
 * @param nGram NGram type input.
 */
NGramSpellChecker::NGramSpellChecker(const FsmMorphologicalAnalyzer &fsm, NGram<string> *nGram,
                                     const SpellCheckerParameter &_parameter) : SimpleSpellChecker(fsm) {
    this->nGram = nGram;
    this->parameter = _parameter;
}

/**
 * A constructor of NGramSpellChecker class which takes an FsmMorphologicalAnalyzer and an NGram
 * as inputs. Then, calls its super class SimpleSpellChecker with given FsmMorphologicalAnalyzer and
 * assigns given NGram to the nGram variable.
 *
 * @param fsm   FsmMorphologicalAnalyzer type input.
 * @param nGram NGram type input.
 */
NGramSpellChecker::NGramSpellChecker(const FsmMorphologicalAnalyzer &fsm, NGram<string> *nGram) : SimpleSpellChecker(fsm){
    this->nGram = nGram;
}

/**
 * The spellCheck method takes a Sentence as an input and loops i times where i ranges from 0 to size of words in given sentence.
 * Then, it calls morphologicalAnalysis method with each word and assigns it to the FsmParseList, if the size of
 * FsmParseList is equal to the 0, it adds current word to the candidateList and assigns it to the candidates ArrayList.
 * <p>
 * Later on, it loops through candidates ArrayList and calls morphologicalAnalysis method with each word and
 * assigns it to the FsmParseList. Then, it gets the root from FsmParseList. For the first time, it defines a previousRoot
 * by calling getProbability method with root, and for the following times it calls getProbability method with previousRoot and root.
 * Then, it finds out the best probability and the corresponding candidate as best candidate and adds it to the result Sentence.
 * <p>
 * If the size of FsmParseList is not equal to 0, it directly adds the current word to the result Sentence and finds
 * the previousRoot directly from the FsmParseList.
 *
 * @param sentence Sentence type input.
 * @return Sentence result.
 */
Sentence *NGramSpellChecker::spellCheck(Sentence *sentence) {
    Word *word, *bestRoot, *previousRoot = nullptr, *root, *nextRoot;
    Candidate *bestCandidate;
    double previousProbability, nextProbability, bestProbability;
    vector<Candidate *> candidates;
    auto *result = new Sentence();
    root = checkAnalysisAndSetRootForWordAtIndex(sentence, 0);
    nextRoot = checkAnalysisAndSetRootForWordAtIndex(sentence, 1);
    for (int i = 0; i < sentence->wordCount(); i++) {
        Word *nextWord = nullptr;
        Word *previousWord = nullptr;
        Word *nextNextWord = nullptr;
        Word *previousPreviousWord = nullptr;
        word = sentence->getWord(i);
        if (i > 0) {
            previousWord = sentence->getWord(i - 1);
        }
        if (i > 1) {
            previousPreviousWord = sentence->getWord(i - 2);
        }
        if (i < sentence->wordCount() - 1) {
            nextWord = sentence->getWord(i + 1);
        }
        if (i < sentence->wordCount() - 2) {
            nextNextWord = sentence->getWord(i + 2);
        }
        if (forcedMisspellCheck(word, result)) {
            previousRoot = checkAnalysisAndSetRootForWordAtIndex(result, result->wordCount() - 1);
            root = nextRoot;
            nextRoot = checkAnalysisAndSetRootForWordAtIndex(sentence, i + 2);
            continue;
        }
        if (forcedBackwardMergeCheck(word, result, previousWord) ||
            forcedSuffixMergeCheck(word, result, previousWord)) {
            previousRoot = checkAnalysisAndSetRootForWordAtIndex(result, result->wordCount() - 1);
            root = checkAnalysisAndSetRootForWordAtIndex(sentence, i + 1);
            nextRoot = checkAnalysisAndSetRootForWordAtIndex(sentence, i + 2);
            continue;
        }
        if (forcedForwardMergeCheck(word, result, nextWord) ||
            forcedHyphenMergeCheck(word, result, previousWord, nextWord)) {
            i++;
            previousRoot = checkAnalysisAndSetRootForWordAtIndex(result, result->wordCount() - 1);
            root = checkAnalysisAndSetRootForWordAtIndex(sentence, i + 1);
            nextRoot = checkAnalysisAndSetRootForWordAtIndex(sentence, i + 2);
            continue;
        }
        if (forcedSplitCheck(word, result) || forcedShortcutCheck(word, result)) {
            previousRoot = checkAnalysisAndSetRootForWordAtIndex(result, result->wordCount() - 1);
            root = nextRoot;
            nextRoot = checkAnalysisAndSetRootForWordAtIndex(sentence, i + 2);
            continue;
        }
        if (parameter.isSuffixCheck()) {
            if (forcedDeDaSplitCheck(word, result) || forcedQuestionSuffixSplitCheck(word, result) || forcedSuffixSplitCheck(word, result)) {
                previousRoot = checkAnalysisAndSetRootForWordAtIndex(result, result->wordCount() - 1);
                root = nextRoot;
                nextRoot = checkAnalysisAndSetRootForWordAtIndex(sentence, i + 2);
                continue;
            }
        }
        if (root == nullptr ||
            (Word::size(word->getName()) <= parameter.getMinWordLength() &&
             fsm.morphologicalAnalysis(word->getName()).size() == 0)) {
            candidates.clear();
            if (root == nullptr) {
                candidates = candidateList(word, sentence);
                vector<Candidate *> splitCandidates = splitCandidatesList(word);
                candidates.insert(candidates.end(), splitCandidates.begin(), splitCandidates.end());
            }
            vector<Candidate *> mergedCandidates = mergedCandidatesList(previousWord, word, nextWord);
            candidates.insert(candidates.end(), mergedCandidates.begin(), mergedCandidates.end());
            bestCandidate = new Candidate(word->getName(), Operator::NO_CHANGE);
            bestRoot = word;
            bestProbability = parameter.getThreshold();
            for (Candidate *candidate: candidates) {
                if (candidate->getOperator() == Operator::SPELL_CHECK ||
                    candidate->getOperator() == Operator::MISSPELLED_REPLACE ||
                    candidate->getOperator() == Operator::CONTEXT_BASED ||
                    candidate->getOperator() == Operator::TRIE_BASED) {
                    root = checkAnalysisAndSetRoot(candidate->getName());
                }
                if (candidate->getOperator() == Operator::BACKWARD_MERGE && previousWord != nullptr) {
                    root = checkAnalysisAndSetRoot(previousWord->getName() + word->getName());
                    if (previousPreviousWord != nullptr) {
                        previousRoot = checkAnalysisAndSetRoot(previousPreviousWord->getName());
                    }
                }
                if (candidate->getOperator() == Operator::FORWARD_MERGE && nextWord != nullptr) {
                    root = checkAnalysisAndSetRoot(word->getName() + nextWord->getName());
                    if (nextNextWord != nullptr) {
                        nextRoot = checkAnalysisAndSetRoot(nextNextWord->getName());
                    }
                }
                if (previousRoot != nullptr) {
                    if (candidate->getOperator() == Operator::SPLIT) {
                        root = checkAnalysisAndSetRoot(Word::split(candidate->getName())[0]);
                    }
                    previousProbability = getProbability(previousRoot->getName(), root->getName());
                } else {
                    previousProbability = 0.0;
                }
                if (nextRoot != nullptr) {
                    if (candidate->getOperator() == Operator::SPLIT) {
                        root = checkAnalysisAndSetRoot(Word::split(candidate->getName())[1]);
                    }
                    nextProbability = getProbability(root->getName(), nextRoot->getName());
                } else {
                    nextProbability = 0.0;
                }
                if (std::max(previousProbability, nextProbability) > bestProbability || candidates.size() == 1) {
                    bestCandidate = candidate;
                    bestRoot = root;
                    bestProbability = std::max(previousProbability, nextProbability);
                }
            }
            if (bestCandidate->getOperator() == Operator::FORWARD_MERGE) {
                i++;
            }
            if (bestCandidate->getOperator() == Operator::BACKWARD_MERGE) {
                result->replaceWord(i - 1, new Word(bestCandidate->getName()));
            } else {
                if (bestCandidate->getOperator() == Operator::SPLIT) {
                    addSplitWords(bestCandidate->getName(), result);
                } else {
                    result->addWord(new Word(bestCandidate->getName()));
                }
            }
            root = bestRoot;
        } else {
            result->addWord(word);
        }
        previousRoot = root;
        root = nextRoot;
        nextRoot = checkAnalysisAndSetRootForWordAtIndex(sentence, i + 2);
    }
    return result;
}

/**
 * Checks the morphological analysis of the given word in the given index. If there is no misspelling, it returns
 * the longest root word of the possible analyses.
 * @param sentence Sentence to be analyzed.
 * @param index Index of the word
 * @return If the word is misspelled, null; otherwise the longest root word of the possible analyses.
 */
Word *NGramSpellChecker::checkAnalysisAndSetRootForWordAtIndex(Sentence *sentence, int index) {
    if (index < sentence->wordCount()) {
        string wordName = sentence->getWord(index)->getName();
        if ((regex_match(wordName, regex(".*\\d+.*")) && regex_match(wordName, regex(".*[a-zA-ZçöğüşıÇÖĞÜŞİ]+.*")) &&
             wordName.find('\'') == string::npos) || Word::size(wordName) < parameter.getMinWordLength()) {
            return sentence->getWord(index);
        }
        FsmParseList fsmParses = fsm.morphologicalAnalysis(sentence->getWord(index)->getName());
        if (fsmParses.size() != 0) {
            if (this->parameter.isRootNGram()) {
                return fsmParses.getParseWithLongestRootWord().getWord();
            }
            return sentence->getWord(index);
        } else {
            string upperCaseWordName = Word::toCapital(wordName);
            FsmParseList upperCaseFsmParses = fsm.morphologicalAnalysis(upperCaseWordName);
            if (upperCaseFsmParses.size() != 0) {
                if (parameter.isRootNGram()) {
                    return upperCaseFsmParses.getParseWithLongestRootWord().getWord();
                }
                return sentence->getWord(index);
            }
        }
    }
    return nullptr;
}

/**
 * Checks the morphological analysis of the given word. If there is no misspelling, it returns
 * the longest root word of the possible analysis.
 *
 * @param word Word to be analyzed.
 * @return If the word is misspelled, null; otherwise the longest root word of the possible analysis.
 */
Word *NGramSpellChecker::checkAnalysisAndSetRoot(const string &word) {
    FsmParseList fsmParsesOfWord = fsm.morphologicalAnalysis(word);
    if (fsmParsesOfWord.size() != 0) {
        if (this->parameter.isRootNGram()) {
            return fsmParsesOfWord.getParseWithLongestRootWord().getWord();
        } else {
            return new Word(word);
        }
    }
    FsmParseList fsmParsesOfCapitalizedWord = fsm.morphologicalAnalysis(Word::toCapital(word));
    if (fsmParsesOfCapitalizedWord.size() != 0) {
        if (this->parameter.isRootNGram()) {
            return fsmParsesOfCapitalizedWord.getParseWithLongestRootWord().getWord();
        } else {
            return new Word(word);
        }
    }
    return nullptr;
}

/**
 * Returns the bi-gram probability P(word2 | word1) for the given bigram consisting of two words.
 * @param word1 First word in bi-gram
 * @param word2 Second word in bi-gram
 * @return Bi-gram probability P(word2 | word1)
 */
double NGramSpellChecker::getProbability(const string &word1, const string &word2) const {
    return nGram->getProbability({word1, word2});
}

