cmake_minimum_required(VERSION 3.12)
project(SpellChecker)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
link_directories(library)

add_library(SpellChecker SpellChecker.h SimpleSpellChecker.cpp SimpleSpellChecker.h NGramSpellChecker.cpp NGramSpellChecker.h)

target_link_libraries(SpellChecker MorphologicalAnalysis)
