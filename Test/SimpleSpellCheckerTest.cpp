//
// Created by Olcay Taner YILDIZ on 5.01.2021.
//

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <FsmMorphologicalAnalyzer.h>
#include "catch.hpp"
#include "../src/SimpleSpellChecker.h"

TEST_CASE("SimpleSpellCheckerTest-testSpellCheck") {
    FsmMorphologicalAnalyzer fsm = FsmMorphologicalAnalyzer();
    SimpleSpellChecker simpleSpellChecker = SimpleSpellChecker(fsm);
    ifstream inputFile;
    string misspelled, corrected;
    inputFile.open("misspellings.txt", ifstream::in);
    while (inputFile.good()){
        inputFile >> misspelled;
        inputFile >> corrected;
        REQUIRE(corrected == simpleSpellChecker.spellCheck(new Sentence(misspelled))->to_string());
    }
    inputFile.close();
}