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
NGramSpellChecker::NGramSpellChecker(const FsmMorphologicalAnalyzer& fsm, const NGram<string>& nGram, bool rootNGram) : SimpleSpellChecker(fsm) {
    this->nGram = nGram;
    this->rootNGram = rootNGram;
}

void NGramSpellChecker::setThreshold(double _threshold) {
    this->threshold = _threshold;
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
    Candidate* bestCandidate;
    FsmParseList fsmParses;
    double previousProbability, nextProbability, bestProbability;
    vector<Candidate*> candidates;
    auto* result = new Sentence();
    root = checkAnalysisAndSetRootForWordAtIndex(sentence, 0);
    nextRoot = checkAnalysisAndSetRootForWordAtIndex(sentence, 1);
    for (int i = 0; i < sentence->wordCount(); i++) {
        Word* nextWord = nullptr;
        Word* previousWord = nullptr;
        Word* nextNextWord = nullptr;
        Word* previousPreviousWord = nullptr;
        word = sentence->getWord(i);
        if (i > 0){
            previousWord = sentence->getWord(i - 1);
        }
        if (i > 1){
            previousPreviousWord = sentence->getWord(i - 2);
        }
        if (i < sentence->wordCount() - 1){
            nextWord = sentence->getWord(i + 1);
        }
        if (i < sentence->wordCount() - 2){
            nextNextWord = sentence->getWord(i + 2);
        }
        if (forcedMisspellCheck(word, result)){
            previousRoot = checkAnalysisAndSetRootForWordAtIndex(result, result->wordCount() - 1);
            root = nextRoot;
            nextRoot = checkAnalysisAndSetRootForWordAtIndex(sentence, i + 2);
            continue;
        }
        if (forcedBackwardMergeCheck(word, result, previousWord)){
            previousRoot = checkAnalysisAndSetRootForWordAtIndex(result, result->wordCount() - 1);
            root = checkAnalysisAndSetRootForWordAtIndex(sentence, i + 1);
            nextRoot = checkAnalysisAndSetRootForWordAtIndex(sentence, i + 2);
            continue;
        }
        if (forcedForwardMergeCheck(word, result, nextWord)){
            i++;
            previousRoot = checkAnalysisAndSetRootForWordAtIndex(result, result->wordCount() - 1);
            root = checkAnalysisAndSetRootForWordAtIndex(sentence, i + 1);
            nextRoot = checkAnalysisAndSetRootForWordAtIndex(sentence, i + 2);
            continue;
        }
        if (forcedSplitCheck(word, result) || forcedShortcutCheck(word, result)){
            previousRoot = checkAnalysisAndSetRootForWordAtIndex(result, result->wordCount() - 1);
            root = nextRoot;
            nextRoot = checkAnalysisAndSetRootForWordAtIndex(sentence, i + 2);
            continue;
        }
        if (root == nullptr) {
            candidates = candidateList(word);
            vector<Candidate*> mergedCandidates = mergedCandidatesList(previousWord, word, nextWord);
            candidates.insert(candidates.end(), mergedCandidates.begin(), mergedCandidates.end());
            vector<Candidate*> splitCandidates = splitCandidatesList(word);
            candidates.insert(candidates.end(), splitCandidates.begin(), splitCandidates.end());
            bestCandidate = new Candidate(word->getName(), Operator::NO_CHANGE);
            bestRoot = word;
            bestProbability = this->threshold;
            for (Candidate* candidate : candidates) {
                if (candidate->getOperator() == Operator::SPELL_CHECK || candidate->getOperator() == Operator::MISSPELLED_REPLACE){
                    root = checkAnalysisAndSetRoot(candidate->getName());
                }
                if (candidate->getOperator() == Operator::BACKWARD_MERGE && previousWord != nullptr){
                    root = checkAnalysisAndSetRoot(previousWord->getName() + word->getName());
                    if (previousPreviousWord != nullptr){
                        previousRoot = checkAnalysisAndSetRoot(previousPreviousWord->getName());
                    }
                }
                if (candidate->getOperator() == Operator::FORWARD_MERGE && nextWord != nullptr){
                    root = checkAnalysisAndSetRoot(word->getName() + nextWord->getName());
                    if (nextNextWord != nullptr){
                        nextRoot = checkAnalysisAndSetRoot(nextNextWord->getName());
                    }
                }
                if (previousRoot != nullptr) {
                    if (candidate->getOperator() == Operator::SPLIT){
                        root = checkAnalysisAndSetRoot(Word::split(candidate->getName())[0]);
                    }
                    previousProbability = getProbability(previousRoot->getName(), root->getName());
                } else {
                    previousProbability = 0.0;
                }
                if (nextRoot != nullptr) {
                    if (candidate->getOperator() == Operator::SPLIT){
                        root = checkAnalysisAndSetRoot(Word::split(candidate->getName())[1]);
                    }
                    nextProbability = getProbability(root->getName(), nextRoot->getName());
                } else {
                    nextProbability = 0.0;
                }
                if (std::max(previousProbability, nextProbability) > bestProbability) {
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
            } else{
                if (bestCandidate->getOperator() == Operator::SPLIT){
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

Word *NGramSpellChecker::checkAnalysisAndSetRoot(const string& word){
    FsmParseList fsmParses = fsm.morphologicalAnalysis(word);
    if (fsmParses.size() != 0){
        if (rootNGram){
            return fsmParses.getParseWithLongestRootWord().getWord();
        } else {
            return new Word(word);
        }
    }
    return nullptr;
}

double NGramSpellChecker::getProbability(const string& word1, const string& word2) const{
    return nGram.getProbability({word1, word2});
}
