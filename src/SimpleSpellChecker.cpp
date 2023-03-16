//
// Created by Olcay Taner Yıldız on 9.03.2019.
//

#include "SimpleSpellChecker.h"
#include "Language/TurkishLanguage.h"
#include <fstream>

const vector<string> SimpleSpellChecker::shortcuts = {"cc", "cm2", "cm", "gb", "ghz", "gr", "gram", "hz", "inc", "inch",
                                                      "inç", "kg", "kw", "kva", "litre", "lt", "m2", "m3", "mah", "mb",
                                                      "metre", "mg", "mhz", "ml", "mm", "mp", "ms", "mt", "mv", "tb",
                                                      "tl", "va", "volt", "watt", "ah", "hp", "oz", "rpm", "dpi", "ppm",
                                                      "ohm", "kwh", "kcal", "kbit", "mbit", "gbit", "bit", "byte", "mbps",
                                                      "gbps", "cm3", "mm2", "mm3", "khz", "ft", "db", "sn"};

const vector<string> SimpleSpellChecker::conditionalShortcuts = {"g", "v", "m", "l", "w", "s"};

const vector<string> SimpleSpellChecker::questionSuffixList = {"mi", "mı", "mu", "mü", "miyim", "misin", "miyiz", "midir",
                                                               "miydi", "mıyım", "mısın", "mıyız", "mıdır", "mıydı", "muyum",
                                                               "musun", "muyuz", "mudur", "muydu", "müyüm", "müsün", "müyüz",
                                                               "müdür", "müydü", "miydim", "miydin", "miydik", "miymiş",
                                                               "mıydım", "mıydın", "mıydık", "mıymış", "muydum", "muydun",
                                                               "muyduk", "muymuş", "müydüm", "müydün", "müydük", "müymüş",
                                                               "misiniz", "mısınız", "musunuz", "müsünüz", "miyimdir",
                                                               "misindir", "miyizdir", "miydiniz", "miydiler", "miymişim",
                                                               "miymişiz", "mıyımdır", "mısındır", "mıyızdır", "mıydınız",
                                                               "mıydılar", "mıymışım", "mıymışız", "muyumdur", "musundur",
                                                               "muyuzdur", "muydunuz", "muydular", "muymuşum", "muymuşuz",
                                                               "müyümdür", "müsündür", "müyüzdür", "müydünüz", "müydüler",
                                                               "müymüşüm", "müymüşüz", "miymişsin", "miymişler", "mıymışsın",
                                                               "mıymışlar", "muymuşsun", "muymuşlar", "müymüşsün", "müymüşler",
                                                               "misinizdir", "mısınızdır", "musunuzdur", "müsünüzdür"};

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
vector<Candidate*> SimpleSpellChecker::generateCandidateList(const string& word) const{
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
vector<Candidate*> SimpleSpellChecker::candidateList(Word *word, Sentence* sentence){
    vector<Candidate*> firstCandidates;
    vector<Candidate*> candidates;
    TxtDictionary* dictionary = fsm.getDictionary();
    firstCandidates = generateCandidateList(word->getName());
    for (auto & firstCandidate : firstCandidates) {
        FsmParseList fsmParseList = fsm.morphologicalAnalysis(firstCandidate->getName());
        if (fsmParseList.size() != 0) {
            candidates.emplace_back(firstCandidate);
        } else {
            string newCandidate = dictionary->getCorrectForm(firstCandidate->getName());
            if (!newCandidate.empty() && fsm.morphologicalAnalysis(newCandidate).size() > 0){
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
        if (forcedMisspellCheck(word, result) || forcedBackwardMergeCheck(word, result, previousWord) || forcedSuffixMergeCheck(word, result, previousWord)){
            continue;
        }
        if (forcedForwardMergeCheck(word, result, nextWord) || forcedHyphenMergeCheck(word, result, previousWord, nextWord)){
            i++;
            continue;
        }
        if (forcedSplitCheck(word, result) || forcedShortcutCheck(word, result) || forcedDeDaSplitCheck(word, result) || forcedQuestionSuffixSplitCheck(word, result)){
            continue;
        }
        FsmParseList fsmParseList = fsm.morphologicalAnalysis(word->getName());
        FsmParseList upperCaseFsmParseList = fsm.morphologicalAnalysis(Word::toCapital(word->getName()));
        if (fsmParseList.size() == 0 && upperCaseFsmParseList.size() == 0) {
            candidates = mergedCandidatesList(previousWord, word, nextWord);
            if (candidates.empty()) {
                candidates = candidateList(word, sentence);
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
                if (candidates[randomCandidate]->getOperator() == Operator::SPLIT){
                    addSplitWords(candidates[randomCandidate]->getName(), result);
                    continue;
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
        string result = items[1];
        for (int i = 2; i < items.size(); i++){
            result += " " + items[i];
        }
        splitWords.emplace(items[0], result);
    }
    inputStream.close();
}

bool SimpleSpellChecker::forcedMisspellCheck(Word* word, Sentence* result) const{
    string forcedReplacement = fsm.getDictionary()->getCorrectForm(word->getName());
    if (!forcedReplacement.empty()){
        result->addWord(new Word(forcedReplacement));
        return true;
    }
    return false;
}

bool SimpleSpellChecker::forcedBackwardMergeCheck(Word* word, Sentence* result, Word* previousWord) const{
    if (previousWord != nullptr){
        string forcedReplacement = getCorrectForm(result->getWord(result->wordCount() - 1)->getName() + " " + word->getName(), mergedWords);
        if (!forcedReplacement.empty()) {
            result->replaceWord(result->wordCount() - 1, new Word(forcedReplacement));
            return true;
        }
    }
    return false;
}

string SimpleSpellChecker::getCorrectForm(const string& wordName, const unordered_map<string, string>& dictionary) const{
    if (dictionary.contains(wordName)){
        return dictionary.at(wordName);
    }
    return "";
}

bool SimpleSpellChecker::forcedForwardMergeCheck(Word *word, Sentence *result, Word *nextWord) const{
    if (nextWord != nullptr){
        string forcedReplacement = getCorrectForm(word->getName() + " " + nextWord->getName(), mergedWords);
        if (!forcedReplacement.empty()) {
            result->addWord(new Word(forcedReplacement));
            return true;
        }
    }
    return false;
}

bool SimpleSpellChecker::forcedSplitCheck(Word* word, Sentence* result) const{
    string forcedReplacement = getCorrectForm(word->getName(), splitWords);
    if (!forcedReplacement.empty()){
        addSplitWords(forcedReplacement, result);
        return true;
    }
    return false;
}

bool SimpleSpellChecker::forcedShortcutCheck(Word* word, Sentence* result) const{
    string shortcutRegex = "^(([1-9][0-9]*)|[0])(([.]|[,])[0-9]*)?(" + shortcuts[0];
    for (int i = 1; i < shortcuts.size(); i++){
        shortcutRegex += "|" + shortcuts[i];
    }
    shortcutRegex += ")$";
    string conditionalShortcutRegex = "^(([1-9][0-9]{0,2})|[0])(([.]|[,])[0-9]*)?(" + conditionalShortcuts[0];
    for (int i = 1; i < conditionalShortcuts.size(); i++){
        conditionalShortcutRegex += "|" + conditionalShortcuts[i];
    }
    conditionalShortcutRegex += ")$";
    if (regex_search(word->getName(), regex(shortcutRegex)) || regex_search(word->getName(), regex(conditionalShortcutRegex))){
        pair<string, string> pair = getSplitPair(word);
        result->addWord(new Word(pair.first));
        result->addWord(new Word(pair.second));
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
    for (int i = 4; i < Word::size(word->getName()) - 3; i++) {
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

pair<string, string> SimpleSpellChecker::getSplitPair(Word *word) const{
    pair<string, string> pair;
    string first;
    int j;
    for (j = 0; j < Word::size(word->getName()); j++){
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

void SimpleSpellChecker::addSplitWords(const string& multiWord, Sentence *result) const{
    vector<string> words = Word::split(multiWord);
    for (auto& word : words){
        result->addWord(new Word(word));
    }
}

bool SimpleSpellChecker::forcedDeDaSplitCheck(Word *word, Sentence *result) {
    string wordName = word->getName();
    string capitalizedWordName = Word::toCapital(wordName);
    TxtWord* txtWord = nullptr;
    if (wordName.ends_with("da") || wordName.ends_with("de")) {
        if (fsm.morphologicalAnalysis(wordName).size() == 0 && fsm.morphologicalAnalysis(capitalizedWordName).size() == 0){
            string newWordName = Word::substring(wordName, 0, Word::size(wordName) - 2);
            FsmParseList fsmParseList = fsm.morphologicalAnalysis(newWordName);
            TxtWord* txtNewWord = (TxtWord*) fsm.getDictionary()->getWord(Word::toLowerCase(newWordName));
            if (txtNewWord != nullptr && txtNewWord->isProperNoun()) {
                if (fsm.morphologicalAnalysis(newWordName + "'" + "da").size() > 0) {
                    result->addWord(new Word(newWordName + "'" + "da"));
                } else {
                    result->addWord(new Word(newWordName + " " + "de"));
                }
                return true;
            }
            if (fsmParseList.size() > 0) {
                txtWord = (TxtWord*) fsm.getDictionary()->getWord(fsmParseList.getParseWithLongestRootWord().getWord()->getName());
            }
            if (txtWord != nullptr && !txtWord->isCode()) {
                result->addWord(new Word(newWordName));
                if (TurkishLanguage::isBackVowel(Word::lastVowel(newWordName))) {
                    if (txtWord->notObeysVowelHarmonyDuringAgglutination()) {
                        result->addWord(new Word("de"));
                    } else {
                        result->addWord(new Word("da"));
                    }
                } else {
                    if (txtWord->notObeysVowelHarmonyDuringAgglutination()) {
                        result->addWord(new Word("da"));
                    } else {
                        result->addWord(new Word("de"));
                    }
                }
                return true;
            }
        }
    }
    return false;
}

bool SimpleSpellChecker::forcedSuffixMergeCheck(Word *word, Sentence *result, Word *previousWord) {
    vector<string> liList = {"li", "lı", "lu", "lü"};
    vector<string> likList = {"lik", "lık", "luk", "lük"};
    if (find(liList.begin(), liList.end(), word->getName()) != liList.end() ||
    find(likList.begin(), likList.end(), word->getName()) != likList.end()){
        if (previousWord != nullptr && regex_search(previousWord->getName(), regex("\\d+"))) {
            for (const string& suffix : liList) {
                if (Word::size(word->getName()) == 2 &&
                    fsm.morphologicalAnalysis(previousWord->getName() + "'" + suffix).size() > 0) {
                    result->replaceWord(result->wordCount() - 1, new Word(previousWord->getName() + "'" + suffix));
                    return true;
                }
            }
            for (const string& suffix : likList) {
                if (Word::size(word->getName()) == 3 &&
                    fsm.morphologicalAnalysis(previousWord->getName() + "'" + suffix).size() > 0) {
                    result->replaceWord(result->wordCount() - 1, new Word(previousWord->getName() + "'" + suffix));
                    return true;
                }
            }
        }
    }
    return false;
}

bool SimpleSpellChecker::forcedHyphenMergeCheck(Word *word, Sentence *result, Word *previousWord, Word *nextWord) {
    if (word->getName() == "-" || word->getName() == "–" || word->getName() == "—") {
        if (previousWord != nullptr && nextWord != nullptr && regex_search(previousWord->getName(), regex("^[a-zA-ZçöğüşıÇÖĞÜŞİ]+$"))
            && regex_search(nextWord->getName(), regex("^[a-zA-ZçöğüşıÇÖĞÜŞİ]+$"))) {
            string newWordName = previousWord->getName() + "-" + nextWord->getName();
            if (fsm.morphologicalAnalysis(newWordName).size() > 0) {
                result->replaceWord(result->wordCount() - 1, new Word(newWordName));
                return true;
            }
        }
    }
    return false;
}

bool SimpleSpellChecker::forcedQuestionSuffixSplitCheck(Word *word, Sentence *result) {
    string wordName = word->getName();
    if (fsm.morphologicalAnalysis(wordName).size() > 0){
        return false;
    }
    for (const string& questionSuffix : questionSuffixList) {
        if (wordName.ends_with(questionSuffix)) {
            string newWordName = Word::substring(wordName, 0, wordName.find_last_of(questionSuffix));
            auto* txtWord = (TxtWord*) fsm.getDictionary()->getWord(newWordName);
            if (fsm.morphologicalAnalysis(newWordName).size() > 0 && txtWord != nullptr && !txtWord->isCode()) {
                result->addWord(new Word(newWordName));
                result->addWord(new Word(questionSuffix));
                return true;
            }
        }
    }
    return false;
}
