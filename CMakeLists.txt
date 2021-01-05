cmake_minimum_required(VERSION 3.12)
project(SpellChecker)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
link_directories(library)

add_library(SpellChecker SpellChecker.h SimpleSpellChecker.cpp SimpleSpellChecker.h NGramSpellChecker.cpp NGramSpellChecker.h)
add_executable(Test SpellChecker.h SimpleSpellChecker.cpp SimpleSpellChecker.h NGramSpellChecker.cpp NGramSpellChecker.h Test/SimpleSpellCheckerTest.cpp Test/NGramSpellCheckerTest.cpp)

target_link_libraries(SpellChecker MorphologicalAnalysis Dictionary XmlParser Corpus)
target_link_libraries(Test MorphologicalAnalysis Dictionary XmlParser Corpus)