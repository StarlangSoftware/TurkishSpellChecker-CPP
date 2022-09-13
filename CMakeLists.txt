cmake_minimum_required(VERSION 3.12)
project(SpellChecker)
set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
set(CONAN_INCLUDE_DIRS_MORPHOLOGICALANALYSIS "/Users/olcay/.conan/data/MorphologicalAnalysis/1.0.0/_/_/package/cc52375504dc650e490f805aefcdfe91296cd5b6/include")

add_library(SpellChecker src/SpellChecker.h src/SimpleSpellChecker.cpp src/SimpleSpellChecker.h src/NGramSpellChecker.cpp src/NGramSpellChecker.h)
add_executable(Test src/SpellChecker.h src/SimpleSpellChecker.cpp src/SimpleSpellChecker.h src/NGramSpellChecker.cpp src/NGramSpellChecker.h Test/SimpleSpellCheckerTest.cpp Test/NGramSpellCheckerTest.cpp)
target_link_libraries(SpellChecker ${CONAN_LIBS})
target_link_libraries(Test ${CONAN_LIBS})