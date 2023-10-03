//
// Created by Olcay Taner YILDIZ on 7.03.2023.
//

#include "ContextBasedSpellChecker.h"

/**
 * A constructor of ContextBasedSpellChecker class which takes a FsmMorphologicalAnalyzer, an NGram
 * and a SpellCheckerParameter as inputs. Then, calls its super class NGramSpellChecker with given inputs.
 *
 * @param fsm       FsmMorphologicalAnalyzer type input.
 * @param nGram     NGram type input.
 * @param parameter SpellCheckerParameter type input.
 */
ContextBasedSpellChecker::ContextBasedSpellChecker(const FsmMorphologicalAnalyzer &fsm, NGram<string> *nGram,
                                                   const SpellCheckerParameter &_parameter) : NGramSpellChecker(fsm,
                                                                                                                nGram,
                                                                                                                _parameter) {
    loadContextDictionaries();
}

/**
 * Another constructor of ContextBasedSpellChecker class which takes an FsmMorphologicalAnalyzer and
 * an NGram as inputs. Then, calls its super class NGramSpellChecker with given inputs.
 *
 * @param fsm   FsmMorphologicalAnalyzer type input.
 * @param nGram NGram type input.
 */
ContextBasedSpellChecker::ContextBasedSpellChecker(const FsmMorphologicalAnalyzer &fsm, NGram<string> *nGram)
        : NGramSpellChecker(fsm, nGram) {
    loadContextDictionaries();
}

/**
 * {@inheritDoc}
 * This method also loads context information from a file.
 */
void ContextBasedSpellChecker::loadContextDictionaries() {
    ifstream inputStream = getInputStream("context_list.txt");
    string line;
    while (inputStream.good()){
        getline(inputStream, line);
        vector<string> words = Word::split(line, "\t");
        string word = words[0];
        vector<string> otherWords = Word::split(words[1], " ");
        contextList[word] = otherWords;
    }
    inputStream.close();
}

/**
 * Calculates the Damerau-Levenshtein distance between two strings.
 * This method also allows for the transposition of adjacent characters,
 * which is not possible in a standard Levenshtein distance calculation.
 *
 * @param first  the first string
 * @param second the second string
 * @return the Damerau-Levenshtein distance between the two strings
 */
int ContextBasedSpellChecker::damerauLevenshteinDistance(const string& first, const string& second) {
    if (first.empty()) {
        return second.empty() ? 0 : Word::size(second);
    }
    if (second.empty()) {
        return Word::size(first);
    }
    int firstLength = Word::size(first);
    int secondLength = Word::size(second);
    int** distanceMatrix = new int*[firstLength + 1];
    for (int i = 0; i < firstLength + 1; i++){
        distanceMatrix[i] = new int[secondLength + 1];
    }
    for (int firstIndex = 0; firstIndex <= firstLength; firstIndex++) {
        distanceMatrix[firstIndex][0] = firstIndex;
    }
    for (int secondIndex = 0; secondIndex <= secondLength; secondIndex++) {
        distanceMatrix[0][secondIndex] = secondIndex;
    }
    for (int firstIndex = 1; firstIndex <= firstLength; firstIndex++) {
        for (int secondIndex = 1; secondIndex <= secondLength; secondIndex++) {
            int cost = Word::charAt(first, firstIndex - 1) == Word::charAt(second, secondIndex - 1) ? 0 : 1;
            distanceMatrix[firstIndex][secondIndex] = min(min(distanceMatrix[firstIndex - 1][secondIndex] + 1,
                                                                        distanceMatrix[firstIndex][secondIndex - 1] + 1), distanceMatrix[firstIndex - 1][secondIndex - 1] + cost);
            if (firstIndex == 1 || secondIndex == 1) {
                continue;
            }
            if (Word::charAt(first, firstIndex - 1) == Word::charAt(second, secondIndex - 2) && Word::charAt(first, firstIndex - 2) == Word::charAt(second, secondIndex - 1)) {
                distanceMatrix[firstIndex][secondIndex] = min(distanceMatrix[firstIndex][secondIndex], distanceMatrix[firstIndex - 2][secondIndex - 2] + cost);
            }
        }
    }
    int result = distanceMatrix[firstLength][secondLength];
    for (int i = 0; i < firstLength + 1; i++){
        delete distanceMatrix[i];
    }
    delete[] distanceMatrix;
    return result;
}

/**
 * Uses context information to generate candidates for a misspelled word.
 * The candidates are the words that are in the context of the neighbouring words of the misspelled word.
 * Uses the {@Link damerauLevenshteinDistance(String, String) method to calculate the distance between the misspelled word and
 * the candidates and to determine whether the candidates are valid.
 *
 * @param word     the misspelled word
 * @param sentence the sentence containing the misspelled word
 * @return an ArrayList of valid candidates for the misspelled word
 */
vector<Candidate *> ContextBasedSpellChecker::candidateList(Word *word, Sentence *sentence) {
    vector<Word*> words = sentence->getWords();
    unordered_set<Candidate*> candidates;
    vector<Candidate*> validCandidates;
    int i = 0;
    for (Word* w : words){
        if (w == word){
            words.erase(words.begin() + i);
            break;
        }
        i++;
    }
    for (const Word* w : words) {
        FsmParseList parses = fsm.morphologicalAnalysis(Word::toCapital(w->getName()));
        if (parses.size() > 0) {
            string root = parses.getParseWithLongestRootWord().getWord()->getName();
            if (contextList.contains(root)) {
                for (const string& s : contextList[root]) {
                    candidates.emplace(new Candidate(s, Operator::CONTEXT_BASED));
                }
            }
        }
    }
    for (Candidate* candidate : candidates) {
        int distance;
        if (Word::size(candidate->getName()) < 5) {
            distance = 1;
        } else {
            if (Word::size(candidate->getName()) < 7) {
                distance = 2;
            } else {
                distance = 3;
            }
        }
        if (damerauLevenshteinDistance(word->getName(), candidate->getName()) <= distance) {
            validCandidates.emplace_back(candidate);
        }
    }
    return validCandidates;
}
