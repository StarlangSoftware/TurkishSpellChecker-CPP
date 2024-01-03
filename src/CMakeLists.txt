cmake_minimum_required(VERSION 3.12)
project(SpellChecker)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3")

find_package(morphological_analysis REQUIRED)
find_package(n_gram REQUIRED)

add_library(SpellChecker SpellChecker.h SimpleSpellChecker.cpp SimpleSpellChecker.h NGramSpellChecker.cpp NGramSpellChecker.h Operator.h Candidate.cpp Candidate.h  SpellCheckerParameter.cpp SpellCheckerParameter.h)
target_link_libraries(SpellChecker morphological_analysis::morphological_analysis n_gram::n_gram)
