//
// Created by Olcay Taner Yıldız on 9.03.2019.
//

#include "NGramSpellChecker.h"

/**
 * A constructor of {@link NGramSpellChecker} class which takes a {@link FsmMorphologicalAnalyzer} and an {@link NGram}
 * as inputs. Then, calls its super class {@link SimpleSpellChecker} with given {@link FsmMorphologicalAnalyzer} and
 * assigns given {@link NGram} to the nGram variable.
 *
 * @param fsm   {@link FsmMorphologicalAnalyzer} type input.
 * @param nGram {@link NGram} type input.
 */
NGramSpellChecker::NGramSpellChecker(FsmMorphologicalAnalyzer fsm, NGram<string>& nGram, bool rootNGram) : SimpleSpellChecker(fsm) {
    this->nGram = nGram;
    this->rootNGram = rootNGram;
}

void NGramSpellChecker::setThreshold(double threshold) {
    this->threshold = threshold;
}

/**
 * The spellCheck method takes a {@link Sentence} as an input and loops i times where i ranges from 0 to size of words in given sentence.
 * Then, it calls morphologicalAnalysis method with each word and assigns it to the {@link FsmParseList}, if the size of
 * {@link FsmParseList} is equal to the 0, it adds current word to the candidateList and assigns it to the candidates {@link ArrayList}.
 * <p>
 * Later on, it loops through candidates {@link ArrayList} and calls morphologicalAnalysis method with each word and
 * assigns it to the {@link FsmParseList}. Then, it gets the root from {@link FsmParseList}. For the first time, it defines a previousRoot
 * by calling getProbability method with root, and for the following times it calls getProbability method with previousRoot and root.
 * Then, it finds out the best probability and the corresponding candidate as best candidate and adds it to the result {@link Sentence}.
 * <p>
 * If the size of {@link FsmParseList} is not equal to 0, it directly adds the current word to the result {@link Sentence} and finds
 * the previousRoot directly from the {@link FsmParseList}.
 *
 * @param sentence {@link Sentence} type input.
 * @return Sentence result.
 */
Sentence *NGramSpellChecker::spellCheck(Sentence *sentence) {
    Word* word, *bestRoot, *previousRoot = nullptr, *root, *nextRoot;
    string bestCandidate;
    FsmParseList fsmParses;
    double previousProbability, nextProbability, bestProbability;
    vector<string> candidates;
    auto* result = new Sentence();
    root = checkAnalysisAndSetRoot(sentence, 0);
    nextRoot = checkAnalysisAndSetRoot(sentence, 1);
    for (int i = 0; i < sentence->wordCount(); i++) {
        word = sentence->getWord(i);
        if (root == nullptr) {
            candidates = candidateList(word);
            bestCandidate = word->getName();
            bestRoot = word;
            bestProbability = this->threshold;
            for (const string &candidate : candidates) {
                fsmParses = fsm.morphologicalAnalysis(candidate);
                if (rootNGram){
                    root = fsmParses.getParseWithLongestRootWord().getWord();
                } else {
                    root = new Word(candidate);
                }
                if (previousRoot != nullptr) {
                    previousProbability = nGram.getProbability({previousRoot->getName(), root->getName()});
                } else {
                    previousProbability = 0.0;
                }
                if (nextRoot != nullptr) {
                    nextProbability = nGram.getProbability({root->getName(), nextRoot->getName()});
                } else {
                    nextProbability = 0.0;
                }
                if (std::max(previousProbability, nextProbability) > bestProbability) {
                    bestCandidate = candidate;
                    bestRoot = root;
                    bestProbability = std::max(previousProbability, nextProbability);
                }
            }
            root = bestRoot;
            result->addWord(new Word(bestCandidate));
        } else {
            result->addWord(word);
        }
        previousRoot = root;
        root = nextRoot;
        nextRoot = checkAnalysisAndSetRoot(sentence, i + 2);
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
Word *NGramSpellChecker::checkAnalysisAndSetRoot(Sentence *sentence, int index) {
    if (index < sentence->wordCount()){
        FsmParseList fsmParses = fsm.morphologicalAnalysis(sentence->getWord(index)->getName());
        if (fsmParses.size() != 0){
            if (rootNGram){
                return fsmParses.getParseWithLongestRootWord().getWord();
            } else {
                return sentence->getWord(index);
            }
        }
    }
    return nullptr;
}
