//
// Created by Olcay Taner Yıldız on 9.03.2019.
//

#include "SimpleSpellChecker.h"
#include "TurkishLanguage.h"

/**
 * The generateCandidateList method takes a String as an input. Firstly, it creates a String consists of lowercase Turkish letters
 * and an {@link vector} candidates. Then, it loops i times where i ranges from 0 to the length of given word. It gets substring
 * from 0 to ith index and concatenates it with substring from i+1 to the last index as a new String called deleted. Then, adds
 * this String to the candidates {@link vector}. Secondly, it loops j times where j ranges from 0 to length of
 * lowercase letters String and adds the jth character of this String between substring of given word from 0 to ith index
 * and the substring from i+1 to the last index, then adds it to the candidates {@link vector}. Thirdly, it loops j
 * times where j ranges from 0 to length of lowercase letters String and adds the jth character of this String between
 * substring of given word from 0 to ith index and the substring from i to the last index, then adds it to the candidates {@link vector}.
 *
 * @param word String input.
 * @return vector candidates.
 */
vector<string> SimpleSpellChecker::generateCandidateList(string word) {
    string s = TurkishLanguage::LOWERCASE_LETTERS;
    vector<string> candidates;
    for (int i = 0; i < Word::size(word); i++) {
        string deleted = Word::substring(word, 0, i - 1) + Word::substring(word, i + 1, Word::size(word));
        candidates.push_back(deleted);
        for (int j = 0; j < Word::size(s); j++) {
            string replaced = Word::substring(word, 0, i - 1) + Word::charAt(s, j) + Word::substring(word, i + 1, Word::size(word));
            candidates.push_back(replaced);
        }
        for (int j = 0; j < s.length(); j++) {
            string added = Word::substring(word, 0, i - 1) + Word::charAt(s, j) + Word::substring(word, i, Word::size(word));
            candidates.push_back(added);
        }
    }
    return candidates;
}

/**
 * The candidateList method takes a {@link Word} as an input and creates a candidates {@link vector} by calling generateCandidateList
 * method with given word. Then, it loop i times where i ranges from 0 to size of candidates {@link vector} and creates a
 * {@link FsmParseList} by calling morphologicalAnalysis with each item of candidates {@link vector}. If the size of
 * {@link FsmParseList} is 0, it then removes the ith item.
 *
 * @param word Word input.
 * @return candidates {@link vector}.
 */
vector<string> SimpleSpellChecker::candidateList(Word *word) {
    vector<string> candidates;
    candidates = generateCandidateList(word->getName());
    for (int i = 0; i < candidates.size(); i++) {
        FsmParseList fsmParseList = fsm.morphologicalAnalysis(candidates.at(i));
        if (fsmParseList.size() == 0) {
            candidates.erase(candidates.begin() + i);
            i--;
        }
    }
    return candidates;
}

/**
 * A constructor of {@link SimpleSpellChecker} class which takes a {@link FsmMorphologicalAnalyzer} as an input and
 * assigns it to the fsm variable.
 *
 * @param fsm {@link FsmMorphologicalAnalyzer} type input.
 */
SimpleSpellChecker::SimpleSpellChecker(FsmMorphologicalAnalyzer& fsm) {
    this->fsm = fsm;
}

/**
 * The spellCheck method takes a {@link Sentence} as an input and loops i times where i ranges from 0 to size of words in given sentence.
 * Then, it calls morphologicalAnalysis method with each word and assigns it to the {@link FsmParseList}, if the size of
 * {@link FsmParseList} is equal to the 0, it adds current word to the candidateList and assigns it to the candidates {@link ArrayList}.
 * if the size of candidates greater than 0, it generates a random number and selects an item from candidates {@link ArrayList} with
 * this random number and assign it as newWord. If the size of candidates is not greater than 0, it directly assigns the
 * current word as newWord. At the end, it adds the newWord to the result {@link Sentence}.
 *
 * @param sentence {@link Sentence} type input.
 * @return Sentence result.
 */
Sentence *SimpleSpellChecker::spellCheck(Sentence *sentence) {
    Word* word;
    Word* newWord;
    unsigned long randomCandidate;
    vector<string> candidates;
    auto* result = new Sentence();
    for (int i = 0; i < sentence->wordCount(); i++) {
        word = sentence->getWord(i);
        FsmParseList fsmParseList = fsm.morphologicalAnalysis(word->getName());
        if (fsmParseList.size() == 0) {
            candidates = candidateList(word);
            if (!candidates.empty()) {
                randomCandidate = random() % candidates.size();
                newWord = new Word(candidates.at(randomCandidate));
            } else {
                newWord = word;
            }
        } else {
            newWord = word;
        }
        result->addWord(newWord);
    }
    return result;
}
