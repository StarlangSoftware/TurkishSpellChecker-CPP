cmake_minimum_required(VERSION 3.12)
project(SpellChecker)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(SpellChecker src/SpellChecker.h src/SimpleSpellChecker.cpp src/SimpleSpellChecker.h src/NGramSpellChecker.cpp src/NGramSpellChecker.h src/Operator.h src/Candidate.cpp src/Candidate.h  src/SpellCheckerParameter.cpp src/SpellCheckerParameter.h)
add_executable(Test src/SpellChecker.h src/SimpleSpellChecker.cpp src/SimpleSpellChecker.h src/NGramSpellChecker.cpp src/NGramSpellChecker.h Test/SimpleSpellCheckerTest.cpp Test/NGramSpellCheckerTest.cpp src/Operator.h src/Candidate.cpp src/Candidate.h  src/SpellCheckerParameter.cpp src/SpellCheckerParameter.h)
target_link_libraries(SpellChecker ${CONAN_LIBS})
target_link_libraries(Test ${CONAN_LIBS})