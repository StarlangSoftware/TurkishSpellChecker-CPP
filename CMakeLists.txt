cmake_minimum_required(VERSION 3.12)
project(SpellChecker)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3")

find_package(morphological_analysis REQUIRED)
find_package(n_gram REQUIRED)

add_library(SpellChecker src/SpellChecker.h src/SimpleSpellChecker.cpp src/SimpleSpellChecker.h src/NGramSpellChecker.cpp src/NGramSpellChecker.h src/Operator.h src/Candidate.cpp src/Candidate.h src/SpellCheckerParameter.cpp src/SpellCheckerParameter.h src/SpellCheckerTrieNode.cpp src/SpellCheckerTrieNode.h src/SpellCheckerTrie.cpp src/SpellCheckerTrie.h src/TrieBasedSpellChecker.cpp src/TrieBasedSpellChecker.h src/ContextBasedSpellChecker.cpp src/ContextBasedSpellChecker.h src/TrieCandidate.cpp src/TrieCandidate.h)
add_executable(Test src/SpellChecker.h src/SimpleSpellChecker.cpp src/SimpleSpellChecker.h src/NGramSpellChecker.cpp src/NGramSpellChecker.h Test/SimpleSpellCheckerTest.cpp Test/NGramSpellCheckerTest.cpp src/Operator.h src/Candidate.cpp src/Candidate.h src/SpellCheckerParameter.cpp src/SpellCheckerParameter.h src/SpellCheckerTrieNode.cpp src/SpellCheckerTrieNode.h src/SpellCheckerTrie.cpp src/SpellCheckerTrie.h src/TrieBasedSpellChecker.cpp src/TrieBasedSpellChecker.h src/ContextBasedSpellChecker.cpp src/ContextBasedSpellChecker.h src/TrieCandidate.cpp src/TrieCandidate.h Test/TrieBasedSpellCheckerTest.cpp Test/ContextBasedSpellCheckerTest.cpp)
target_link_libraries(SpellChecker morphological_analysis::morphological_analysis n_gram::n_gram)
target_link_libraries(Test morphological_analysis::morphological_analysis n_gram::n_gram)