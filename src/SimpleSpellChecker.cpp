//
// Created by Olcay Taner Yıldız on 9.03.2019.
//

#include "SimpleSpellChecker.h"
#include "Language/TurkishLanguage.h"
#include <fstream>

const vector<string> SimpleSpellChecker::shortcuts = {"cc", "cm2", "cm", "gb", "ghz", "gr", "gram", "hz", "inc", "inch", "inç",
"kg", "kw", "kva", "litre", "lt", "m2", "m3", "mah", "mb", "metre", "mg", "mhz", "ml", "mm", "mp", "ms",
"mt", "mv", "tb", "tl", "va", "volt", "watt", "ah", "hp"};

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
vector<Candidate*> SimpleSpellChecker::generateCandidateList(string word) {
    string s = TurkishLanguage::LOWERCASE_LETTERS;
    vector<Candidate*> candidates;
    string englishCharacters = "wqx";
    for (int i = 0; i < Word::size(word); i++) {
        if (i < Word::size(word) - 1){
            string swapped = Word::substring(word, 0, i) + Word::charAt(word, i + 1) + Word::charAt(word, i) + Word::substring(word, i + 2);
            candidates.push_back(new Candidate(swapped, Operator::SPELL_CHECK));
        }
        if (TurkishLanguage::LETTERS.find(Word::charAt(word, i)) != -1 || englishCharacters.find(Word::charAt(word, i)) != -1){
            string deleted = Word::substring(word, 0, i) + Word::substring(word, i + 1);
            candidates.push_back(new Candidate(deleted, Operator::SPELL_CHECK));
            for (int j = 0; j < Word::size(s); j++) {
                string replaced = Word::substring(word, 0, i) + Word::charAt(s, j) + Word::substring(word, i + 1);
                candidates.push_back(new Candidate(replaced, Operator::SPELL_CHECK));
            }
            for (int j = 0; j < Word::size(s); j++) {
                string added = Word::substring(word, 0, i) + Word::charAt(s, j) + Word::substring(word, i);
                candidates.push_back(new Candidate(added, Operator::SPELL_CHECK));
            }
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
vector<Candidate*> SimpleSpellChecker::candidateList(Word *word) {
    vector<Candidate*> firstCandidates;
    vector<Candidate*> candidates;
    firstCandidates = generateCandidateList(word->getName());
    for (int i = 0; i < firstCandidates.size(); i++) {
        FsmParseList fsmParseList = fsm.morphologicalAnalysis(firstCandidates.at(i)->getName());
        if (fsmParseList.size() != 0) {
            candidates.emplace_back(firstCandidates.at(i));
        } else {
            string newCandidate = fsm.getDictionary().getCorrectForm(firstCandidates.at(i)->getName());
            if (!newCandidate.empty()){
                candidates.emplace_back(new Candidate(newCandidate, Operator::MISSPELLED_REPLACE));
            }
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
SimpleSpellChecker::SimpleSpellChecker(const FsmMorphologicalAnalyzer& fsm) {
    this->fsm = fsm;
    loadDictionaries();
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
    vector<Candidate*> candidates;
    auto* result = new Sentence();
    for (int i = 0; i < sentence->wordCount(); i++) {
        word = sentence->getWord(i);
        Word* nextWord = nullptr;
        Word* previousWord = nullptr;
        if (i > 0){
            previousWord = sentence->getWord(i - 1);
        }
        if (i < sentence->wordCount() - 1){
            nextWord = sentence->getWord(i + 1);
        }
        if (forcedMisspellCheck(word, result) || forcedBackwardMergeCheck(word, result, previousWord)){
            continue;
        }
        if (forcedForwardMergeCheck(word, result, nextWord)){
            i++;
            continue;
        }
        if (forcedSplitCheck(word, result) || forcedShortcutCheck(word, result, previousWord)){
            continue;
        }
        FsmParseList fsmParseList = fsm.morphologicalAnalysis(word->getName());
        if (fsmParseList.size() == 0) {
            candidates = candidateList(word);
            if (candidates.empty()) {
                vector<Candidate*> mergedCandidates = mergedCandidatesList(previousWord, word, nextWord);
                candidates.insert(candidates.end(), mergedCandidates.begin(), mergedCandidates.end());
            }
            if (candidates.empty()) {
                vector<Candidate*> splitCandidates = splitCandidatesList(word);
                candidates.insert(candidates.cend(), splitCandidates.begin(), splitCandidates.end());
            }
            if (!candidates.empty()) {
                randomCandidate = random() % candidates.size();
                newWord = new Word(candidates.at(randomCandidate)->getName());
                if (candidates[randomCandidate]->getOperator() == Operator::BACKWARD_MERGE){
                    result->replaceWord(i - 1, newWord);
                    continue;
                }
                if (candidates[randomCandidate]->getOperator() == Operator::FORWARD_MERGE){
                    i++;
                }
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

void SimpleSpellChecker::loadDictionaries() {
    ifstream inputStream;
    inputStream.open("merged.txt", ifstream::in);
    string line;
    while (inputStream.good()){
        getline(inputStream, line);
        vector<string> items = Word::split(line);
        mergedWords.emplace(items[0] + " " + items[1], items[2]);
    }
    inputStream.close();
    inputStream.open("split.txt", ifstream::in);
    while (inputStream.good()){
        getline(inputStream, line);
        vector<string> items = Word::split(line);
        splitWords.emplace(items[0], items[1] + " " + items[2]);
    }
    inputStream.close();
}

bool SimpleSpellChecker::forcedMisspellCheck(Word* word, Sentence* result) {
    string forcedReplacement = fsm.getDictionary().getCorrectForm(word->getName());
    if (!forcedReplacement.empty()){
        result->addWord(new Word(forcedReplacement));
        return true;
    }
    return false;
}

bool SimpleSpellChecker::forcedBackwardMergeCheck(Word* word, Sentence* result, Word* previousWord) {
    if (previousWord != nullptr){
        string forcedReplacement = getCorrectForm(result->getWord(result->wordCount() - 1)->getName() + " " + word->getName(), mergedWords);
        if (!forcedReplacement.empty()) {
            result->replaceWord(result->wordCount() - 1, new Word(forcedReplacement));
            return true;
        }
    }
    return false;
}

string SimpleSpellChecker::getCorrectForm(const string& wordName, unordered_map<string, string> dictionary) {
    if (dictionary.contains(wordName)){
        return dictionary.at(wordName);
    }
    return "";
}

bool SimpleSpellChecker::forcedForwardMergeCheck(Word *word, Sentence *result, Word *nextWord) {
    if (nextWord != nullptr){
        string forcedReplacement = getCorrectForm(word->getName() + " " + nextWord->getName(), mergedWords);
        if (!forcedReplacement.empty()) {
            result->addWord(new Word(forcedReplacement));
            return true;
        }
    }
    return false;
}

bool SimpleSpellChecker::forcedSplitCheck(Word* word, Sentence* result) {
    string forcedReplacement = getCorrectForm(word->getName(), splitWords);
    if (!forcedReplacement.empty()){
        result->addWord(new Word(forcedReplacement));
        return true;
    }
    return false;
}

bool SimpleSpellChecker::forcedShortcutCheck(Word* word, Sentence* result, Word* previousWord) {
    string forcedReplacement;
    string shortcutRegex = "[0-9]+(" + shortcuts[0];
    for (int i = 1; i < shortcuts.size(); i++){
        shortcutRegex += "|" + shortcuts[i];
    }
    shortcutRegex += ")";
    if (regex_search(word->getName(), regex(shortcutRegex))){
        pair<string, string> pair = getSplitPair(word);
        forcedReplacement = pair.first + " " + pair.second;
        result->addWord(new Word(forcedReplacement));
        return true;
    }
    return false;
}

vector<Candidate *> SimpleSpellChecker::mergedCandidatesList(Word *previousWord, Word *word, Word *nextWord) {
    vector<Candidate*> mergedCandidates;
    Candidate* backwardMergeCandidate = nullptr;
    Candidate* forwardMergeCandidate = nullptr;
    if (previousWord != nullptr){
        backwardMergeCandidate = new Candidate(previousWord->getName() + word->getName(), Operator::BACKWARD_MERGE);
        FsmParseList fsmParseList = fsm.morphologicalAnalysis(backwardMergeCandidate->getName());
        if (fsmParseList.size() != 0){
            mergedCandidates.emplace_back(backwardMergeCandidate);
        }
    }
    if (nextWord != nullptr){
        forwardMergeCandidate = new Candidate(word->getName() + nextWord->getName(), Operator::FORWARD_MERGE);
        if (backwardMergeCandidate == nullptr || backwardMergeCandidate->getName() != forwardMergeCandidate->getName()){
            FsmParseList fsmParseList = fsm.morphologicalAnalysis(forwardMergeCandidate->getName());
            if (fsmParseList.size() != 0){
                mergedCandidates.emplace_back(forwardMergeCandidate);
            }
        }
    }
    return mergedCandidates;
}

vector<Candidate *> SimpleSpellChecker::splitCandidatesList(Word *word) {
    vector<Candidate*> splitCandidates;
    for (int i = 4; i < word->getName().length() - 3; i++) {
        string firstPart = Word::substring(word->getName(), 0, i);
        string secondPart = Word::substring(word->getName(), i);
        FsmParseList fsmParseListFirst = fsm.morphologicalAnalysis(firstPart);
        FsmParseList fsmParseListSecond = fsm.morphologicalAnalysis(secondPart);
        if (fsmParseListFirst.size() > 0 && fsmParseListSecond.size() > 0){
            splitCandidates.emplace_back(new Candidate(firstPart + " " + secondPart, Operator::SPLIT));
        }
    }
    return splitCandidates;
}

pair<string, string> SimpleSpellChecker::getSplitPair(Word *word) {
    pair<string, string> pair;
    string first;
    int j;
    for (j = 0; j < word->getName().length(); j++){
        if (Word::charAt(word->getName(), j) >= "0" && Word::charAt(word->getName(), j) <= "9") {
            first += Word::charAt(word->getName(), j);
        } else {
            break;
        }
    }
    string second = Word::substring(word->getName(), j);
    pair.first = first;
    pair.second = second;
    return pair;
}

void SimpleSpellChecker::addSplitWords(string multiWord, Sentence *result) {
    vector<string> words = Word::split(multiWord);
    result->addWord(new Word(words[0]));
    result->addWord(new Word(words[1]));
}
