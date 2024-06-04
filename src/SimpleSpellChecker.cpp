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
 * and an vector candidates. Then, it loops i times where i ranges from 0 to the length of given word. It gets substring
 * from 0 to ith index and concatenates it with substring from i+1 to the last index as a new String called deleted. Then, adds
 * this String to the candidates vector. Secondly, it loops j times where j ranges from 0 to length of
 * lowercase letters String and adds the jth character of this String between substring of given word from 0 to ith index
 * and the substring from i+1 to the last index, then adds it to the candidates vector. Thirdly, it loops j
 * times where j ranges from 0 to length of lowercase letters String and adds the jth character of this String between
 * substring of given word from 0 to ith index and the substring from i to the last index, then adds it to the candidates vector.
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
 * The candidateList method takes a Word as an input and creates a candidates vector by calling generateCandidateList
 * method with given word. Then, it loop i times where i ranges from 0 to size of candidates vector and creates a
 * FsmParseList by calling morphologicalAnalysis with each item of candidates vector. If the size of
 * FsmParseList is 0, it then removes the ith item.
 *
 * @param word Word input.
 * @return candidates vector.
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
 * A constructor of SimpleSpellChecker class which takes a FsmMorphologicalAnalyzer as an input and
 * assigns it to the fsm variable.
 *
 * @param fsm FsmMorphologicalAnalyzer type input.
 */
SimpleSpellChecker::SimpleSpellChecker(const FsmMorphologicalAnalyzer& fsm) {
    this->fsm = fsm;
    this->parameter = SpellCheckerParameter();
    loadDictionaries();
}

/**
 * The spellCheck method takes a Sentence as an input and loops i times where i ranges from 0 to size of words in given sentence.
 * Then, it calls morphologicalAnalysis method with each word and assigns it to the FsmParseList, if the size of
 * FsmParseList is equal to the 0, it adds current word to the candidateList and assigns it to the candidates ArrayList.
 * if the size of candidates greater than 0, it generates a random number and selects an item from candidates ArrayList with
 * this random number and assign it as newWord. If the size of candidates is not greater than 0, it directly assigns the
 * current word as newWord. At the end, it adds the newWord to the result Sentence.
 *
 * @param sentence Sentence type input.
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
        if (forcedSplitCheck(word, result) || forcedShortcutCheck(word, result) || forcedDeDaSplitCheck(word, result) || forcedQuestionSuffixSplitCheck(word, result) || forcedSuffixSplitCheck(word, result)){
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

/**
 * Loads the merged and split lists from the specified files.
 */
void SimpleSpellChecker::loadDictionaries() {
    ifstream inputStream;
    inputStream = getInputStream("merged.txt");
    string line;
    while (inputStream.good()){
        getline(inputStream, line);
        vector<string> items = Word::split(line);
        mergedWords.emplace(items[0] + " " + items[1], items[2]);
    }
    inputStream.close();
    inputStream = getInputStream("split.txt");
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

/**
 * Checks if the given word is a misspelled word according to the misspellings list,
 * and if it is, then replaces it with its correct form in the given sentence.
 *
 * @param word   the Word to check for misspelling
 * @param result the Sentence that the word belongs to
 * @return true if the word was corrected, false otherwise
 */
bool SimpleSpellChecker::forcedMisspellCheck(Word* word, Sentence* result) const{
    string forcedReplacement = fsm.getDictionary()->getCorrectForm(word->getName());
    if (!forcedReplacement.empty()){
        result->addWord(new Word(forcedReplacement));
        return true;
    }
    return false;
}

/**
 * Checks if the given word and its preceding word need to be merged according to the merged list.
 * If the merge is needed, the word and its preceding word are replaced with their merged form in the given sentence.
 *
 * @param word         the Word to check for merge
 * @param result       the Sentence that the word belongs to
 * @param previousWord the preceding Word of the given Word
 * @return true if the word was merged, false otherwise
 */
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

/**
 * Returns the correct form of a given word by looking it up in the provided dictionary.
 *
 * @param wordName   the name of the word to look up in the dictionary
 * @param dictionary the dictionary to use for looking up the word
 * @return the correct form of the word, as stored in the dictionary, or null if the word is not found
 */
string SimpleSpellChecker::getCorrectForm(const string& wordName, const unordered_map<string, string>& dictionary) const{
    if (dictionary.contains(wordName)){
        return dictionary.at(wordName);
    }
    return "";
}

/**
 * Checks if the given word and its next word need to be merged according to the merged list.
 * If the merge is needed, the word and its next word are replaced with their merged form in the given sentence.
 *
 * @param word     the Word to check for merge
 * @param result   the Sentence that the word belongs to
 * @param nextWord the next Word of the given Word
 * @return true if the word was merged, false otherwise
 */
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

/**
 * Checks if the given word needs to be split according to the split list.
 * If the split is needed, the word is replaced with its split form in the given sentence.
 *
 * @param word   the Word to check for split
 * @param result the Sentence that the word belongs to
 * @return true if the word was split, false otherwise
 */
bool SimpleSpellChecker::forcedSplitCheck(Word* word, Sentence* result) const{
    string forcedReplacement = getCorrectForm(word->getName(), splitWords);
    if (!forcedReplacement.empty()){
        addSplitWords(forcedReplacement, result);
        return true;
    }
    return false;
}

/**
 * Checks if the given word is a shortcut form, such as "5kg" or "2.5km".
 * If it is, it splits the word into its number and unit form and adds them to the given sentence.
 *
 * @param word   the Word to check for shortcut split
 * @param result the Sentence that the word belongs to
 * @return true if the word was split, false otherwise
 */
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

/**
 * Generates a list of merged candidates for the word and previous and next words.
 *
 * @param previousWord The previous Word in the sentence.
 * @param word         The Word currently being checked.
 * @param nextWord     The next Word in the sentence.
 * @return A list of merged candidates.
 */
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

/**
 * Generates a list of split candidates for the given word.
 *
 * @param word The Word currently being checked.
 * @return A list of split candidates.
 */
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

/**
 * Splits a word into two parts, a key and a value, based on the first non-numeric/non-punctuation character.
 *
 * @param word the Word object to split
 * @return an {@link AbstractMap.SimpleEntry} object containing the key (numeric/punctuation characters) and the value (remaining characters)
 */
pair<string, string> SimpleSpellChecker::getSplitPair(Word *word) const{
    pair<string, string> pair;
    string first;
    int j;
    for (j = 0; j < Word::size(word->getName()); j++){
        if (Word::charAt(word->getName(), j) >= "0" && Word::charAt(word->getName(), j) <= "9" || Word::charAt(word->getName(), j) == "." || Word::charAt(word->getName(), j) == ",") {
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

/**
 * Given a multiword form, splits it and adds it to the given sentence.
 *
 * @param multiWord multiword form to split
 * @param result    the Sentence to add the split words to
 */
void SimpleSpellChecker::addSplitWords(const string& multiWord, Sentence *result) const{
    vector<string> words = Word::split(multiWord);
    for (auto& word : words){
        result->addWord(new Word(word));
    }
}

/**
 * Checks if the given word has a "da" or "de" suffix that needs to be split according to a predefined set of rules.
 * If the split is needed, the word is replaced with its bare form and "da" or "de" in the given sentence.
 *
 * @param word   the Word to check for "da" or "de" split
 * @param result the Sentence that the word belongs to
 * @return true if the word was split, false otherwise
 */
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
                string newWordNameCapitalized = Word::toCapital(newWordName);
                if (fsm.morphologicalAnalysis(newWordNameCapitalized + "'" + "da").size() > 0) {
                    result->addWord(new Word(newWordNameCapitalized + "'" + "da"));
                } else {
                    result->addWord(new Word(newWordNameCapitalized + "'" + "de"));
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

/**
 * Checks if the given word is a suffix like 'li' or 'lik' that needs to be merged with its preceding word which is a number.
 * If the merge is needed, the word and its preceding word are replaced with their merged form in the given sentence.
 *
 * @param word         the Word to check for merge
 * @param sentence     the Sentence that the word belongs to
 * @param previousWord the preceding Word of the given Word
 * @return true if the word was merged, false otherwise
 */
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

/**
 * Checks whether the next word and the previous word can be merged if the current word is a hyphen,
 * an en-dash or an em-dash.
 * If the previous word and the next word exist and they are valid words,
 * it merges the previous word and the next word into a single word and add the new word to the sentence
 * If the merge is valid, it returns true.
 *
 * @param word         current Word
 * @param result       the Sentence that the word belongs to
 * @param previousWord the Word before current word
 * @param nextWord     the Word after current word
 * @return true if merge is valid, false otherwise
 */
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

/**
 * Checks whether the current word ends with a valid question suffix and split it if it does.
 * It splits the word with the question suffix and adds the two new words to the sentence.
 * If the split is valid, it returns true.
 *
 * @param word   current Word
 * @param result the Sentence that the word belongs to
 * @return true if split is valid, false otherwise
 */
bool SimpleSpellChecker::forcedQuestionSuffixSplitCheck(Word *word, Sentence *result) {
    string wordName = word->getName();
    if (fsm.morphologicalAnalysis(wordName).size() > 0){
        return false;
    }
    for (const string& questionSuffix : questionSuffixList) {
        if (wordName.ends_with(questionSuffix)) {
            string splitWordName = wordName.substr(0, wordName.find(questionSuffix));
            if (fsm.morphologicalAnalysis(splitWordName).size() < 1) {
                return false;
            }
            auto* splitWordRoot = (TxtWord*) fsm.getDictionary()->getWord(fsm.morphologicalAnalysis(splitWordName).getParseWithLongestRootWord().getWord()->getName());
            if (fsm.morphologicalAnalysis(splitWordName).size() > 0 && splitWordRoot != nullptr && !splitWordRoot->isCode()) {
                result->addWord(new Word(splitWordName));
                result->addWord(new Word(questionSuffix));
                return true;
            }
        }
    }
    return false;
}

/**
 * Another constructor of SimpleSpellChecker class which takes an FsmMorphologicalAnalyzer and a
 * SpellCheckerParameter as inputs, assigns FsmMorphologicalAnalyzer to the fsm variable and
 * SpellCheckerParameter to the parameter variable. Then, it calls the loadDictionaries method.
 *
 * @param fsm       FsmMorphologicalAnalyzer type input.
 * @param parameter SpellCheckerParameter type input.
 */
SimpleSpellChecker::SimpleSpellChecker(const FsmMorphologicalAnalyzer &fsm, const SpellCheckerParameter& parameter) {
    this->fsm = fsm;
    this->parameter = parameter;
    loadDictionaries();
}

/**
 * Opens and returns a file reader of a given file name.
 * @param fileName File to read
 * @return File reader of the given file.
 */
ifstream SimpleSpellChecker::getInputStream(const string& fileName) {
    ifstream inputStream;
    if (parameter.getDomain().empty()){
        inputStream.open(fileName, ifstream::in);
    } else {
        inputStream.open(parameter.getDomain() + "_" + fileName, ifstream::in);
    }
    return inputStream;
}

/**
 * Checks whether the given Word can be split into a proper noun and a suffix, with an apostrophe in between
 * and adds the split result to the Sentence if it's valid.
 *
 * @param word the Word to check for forced suffix split.
 * @param result the Sentence that the word belongs to
 * @return true if the split is successful, false otherwise.
 */
bool SimpleSpellChecker::forcedSuffixSplitCheck(Word *word, Sentence *result) {
    string wordName = word->getName();
    if (fsm.morphologicalAnalysis(wordName).size() > 0){
        return false;
    }
    for (int i = 1; i < Word::size(wordName); i++){
        string probableProperNoun = Word::substring(Word::toCapital(wordName), 0, i);
        string probableSuffix = Word::substring(wordName, i);
        string apostropheWord = probableProperNoun + "'" + probableSuffix;
        TxtWord* txtWord = (TxtWord*) fsm.getDictionary()->getWord(Word::toLowerCase(probableProperNoun));
        if (txtWord != nullptr && txtWord->isProperNoun() && fsm.morphologicalAnalysis(apostropheWord).size() > 0){
            result->addWord(new Word(apostropheWord));
            return true;
        }
    }
    return false;
}
