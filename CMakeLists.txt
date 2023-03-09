cmake_minimum_required(VERSION 3.12)
project(SpellChecker)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
set(CONAN_INCLUDE_DIRS_MORPHOLOGICALANALYSIS "/Users/olcay/.conan/data/MorphologicalAnalysis/1.0.0/_/_/package/cc52375504dc650e490f805aefcdfe91296cd5b6/include")

add_library(SpellChecker src/SpellChecker.h src/SimpleSpellChecker.cpp src/SimpleSpellChecker.h src/NGramSpellChecker.cpp src/NGramSpellChecker.h src/Operator.h src/Candidate.cpp src/Candidate.h src/SpellCheckerParameter.cpp src/SpellCheckerParameter.h src/SpellCheckerTrieNode.cpp src/SpellCheckerTrieNode.h src/SpellCheckerTrie.cpp src/SpellCheckerTrie.h src/TrieBasedSpellChecker.cpp src/TrieBasedSpellChecker.h src/ContextBasedSpellChecker.cpp src/ContextBasedSpellChecker.h src/TrieCandidate.cpp src/TrieCandidate.h)
add_executable(Test src/SpellChecker.h src/SimpleSpellChecker.cpp src/SimpleSpellChecker.h src/NGramSpellChecker.cpp src/NGramSpellChecker.h Test/SimpleSpellCheckerTest.cpp Test/NGramSpellCheckerTest.cpp src/Operator.h src/Candidate.cpp src/Candidate.h src/SpellCheckerParameter.cpp src/SpellCheckerParameter.h src/SpellCheckerTrieNode.cpp src/SpellCheckerTrieNode.h src/SpellCheckerTrie.cpp src/SpellCheckerTrie.h src/TrieBasedSpellChecker.cpp src/TrieBasedSpellChecker.h src/ContextBasedSpellChecker.cpp src/ContextBasedSpellChecker.h src/TrieCandidate.cpp src/TrieCandidate.h Test/TrieBasedSpellCheckerTest.cpp Test/ContextBasedSpellCheckerTest.cpp)
target_link_libraries(SpellChecker ${CONAN_LIBS})
target_link_libraries(Test ${CONAN_LIBS})