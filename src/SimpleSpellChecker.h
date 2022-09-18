//
// Created by Olcay Taner Yıldız on 9.03.2019.
//

#ifndef SPELLCHECKER_SIMPLESPELLCHECKER_H
#define SPELLCHECKER_SIMPLESPELLCHECKER_H


#include <FsmMorphologicalAnalyzer.h>
#include <unordered_map>
#include "SpellChecker.h"
#include "Candidate.h"

class SimpleSpellChecker : public SpellChecker {
protected:
    FsmMorphologicalAnalyzer fsm;
    bool forcedMisspellCheck(Word* word, Sentence* result);
    bool forcedBackwardMergeCheck(Word* word, Sentence* result, Word* previousWord);
    bool forcedForwardMergeCheck(Word* word, Sentence* result, Word* nextWord);
    string getCorrectForm(const string& wordName, unordered_map<string, string> dictionary);
    bool forcedSplitCheck(Word* word, Sentence* result);
    bool forcedShortcutCheck(Word* word, Sentence* result, Word* previousWord);
    vector<Candidate*> mergedCandidatesList(Word* previousWord, Word* word, Word* nextWord);
    vector<Candidate*> splitCandidatesList(Word* word);
private:
    vector<Candidate*> generateCandidateList(string word);
    unordered_map<string, string> mergedWords;
    unordered_map<string, string> splitWords;
    static const vector<string> shortcuts;
    void loadDictionaries();
    pair<string, string> getSplitPair(Word* word);
public:
    explicit SimpleSpellChecker(const FsmMorphologicalAnalyzer& fsm);
    vector<Candidate*> candidateList(Word* word);
    Sentence* spellCheck(Sentence* sentence) override;
};


#endif //SPELLCHECKER_SIMPLESPELLCHECKER_H
