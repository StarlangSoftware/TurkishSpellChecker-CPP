//
// Created by Olcay Taner YILDIZ on 7.03.2023.
//

#ifndef SPELLCHECKER_TRIEBASEDSPELLCHECKER_H
#define SPELLCHECKER_TRIEBASEDSPELLCHECKER_H

#include "NGramSpellChecker.h"
#include "SpellCheckerTrie.h"
#include "TrieCandidate.h"

class TrieBasedSpellChecker : public NGramSpellChecker{
private:
    vector<string> generatedWords;
    SpellCheckerTrie* trie;
    void prepareTrie();
    vector<TrieCandidate*> generateTrieCandidates(TrieCandidate* candidate);
    int searchCandidates(const vector<Candidate *>& results, TrieCandidate* candidate);
public:
    TrieBasedSpellChecker(const FsmMorphologicalAnalyzer& fsm, NGram<string>* nGram, const SpellCheckerParameter& _parameter);
protected:
    void loadTrieDictionaries();
    vector<Candidate*> candidateList(Word* word, Sentence* sentence) override;
};


#endif //SPELLCHECKER_TRIEBASEDSPELLCHECKER_H
