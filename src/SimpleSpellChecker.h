//
// Created by Olcay Taner Yıldız on 9.03.2019.
//

#ifndef SPELLCHECKER_SIMPLESPELLCHECKER_H
#define SPELLCHECKER_SIMPLESPELLCHECKER_H


#include <FsmMorphologicalAnalyzer.h>
#include <unordered_map>
#include "SpellChecker.h"
#include "SpellCheckerParameter.h"
#include "Candidate.h"

class SimpleSpellChecker : public SpellChecker {
protected:
    FsmMorphologicalAnalyzer fsm;
    SpellCheckerParameter parameter;
    bool forcedMisspellCheck(const Word* word, Sentence* result) const;
    bool forcedBackwardMergeCheck(const Word* word, Sentence* result, const Word* previousWord) const;
    bool forcedForwardMergeCheck(const Word* word, Sentence* result, const Word* nextWord) const;
    string getCorrectForm(const string& wordName, const unordered_map<string, string>& dictionary) const;
    bool forcedSplitCheck(const Word* word, Sentence* result) const;
    bool forcedShortcutCheck(const Word* word, Sentence* result) const;
    bool forcedDeDaSplitCheck(const Word* word, Sentence* result);
    bool forcedSuffixMergeCheck(const Word* word, Sentence* result, const Word* previousWord);
    bool forcedHyphenMergeCheck(const Word* word, Sentence* result, const Word* previousWord, const Word* nextWord);
    bool forcedQuestionSuffixSplitCheck(const Word* word, Sentence* result);
    bool forcedSuffixSplitCheck(const Word* word, Sentence* result);
    vector<Candidate*> mergedCandidatesList(const Word* previousWord, const Word* word, const Word* nextWord);
    vector<Candidate*> splitCandidatesList(const Word* word);
    void addSplitWords(const string& multiWord, Sentence* result) const;
    void loadDictionaries();
    virtual vector<Candidate*> candidateList(Word* word, Sentence* sentence);
    ifstream getInputStream(const string& fileName) const;
private:
    vector<Candidate*> generateCandidateList(const string& word) const;
    unordered_map<string, string> mergedWords;
    unordered_map<string, string> splitWords;
    static const vector<string> shortcuts;
    static const vector<string> conditionalShortcuts;
    static const vector<string> questionSuffixList;
    pair<string, string> getSplitPair(const Word* word) const;
public:
    explicit SimpleSpellChecker(const FsmMorphologicalAnalyzer& fsm);
    SimpleSpellChecker(const FsmMorphologicalAnalyzer& fsm, const SpellCheckerParameter& parameter);
    Sentence* spellCheck(Sentence* sentence) override;
};


#endif //SPELLCHECKER_SIMPLESPELLCHECKER_H
