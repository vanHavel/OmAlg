cmake_minimum_required(VERSION 3.4)
project(a2os)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(SOURCE_FILES
    OptParser/OptParser.cpp
    src/Automata/BuechiAutomaton.cpp
    src/Automata/CoBuechiAutomaton.cpp
    src/Automata/DeterministicBuechiAutomaton.cpp
    src/Automata/DeterministicCoBuechiAutomaton.cpp
    src/Automata/DeterministicMullerAutomaton.cpp
    src/Automata/DeterministicOmegaAutomaton.cpp
    src/Automata/DeterministicParityAutomaton.cpp
    src/Automata/MullerAutomaton.cpp
    src/Automata/NondeterministicBuechiAutomaton.cpp
    src/Automata/NondeterministicCoBuechiAutomaton.cpp
    src/Automata/NondeterministicMullerAutomaton.cpp
    src/Automata/NondeterministicOmegaAutomaton.cpp
    src/Automata/NondeterministicParityAutomaton.cpp
    src/Automata/OmegaAutomaton.cpp
    src/Automata/ParityAutomaton.cpp
    src/IOHandler.cpp
    src/Morphism.cpp
    src/OmegaSemigroup.cpp
    src/Semigroup.cpp
    vanHavel/UnionFind.cpp
    vanHavel/vanHavel_Util.cpp)

include_directories(include)
include_directories(include/Automata)
include_directories(include/TransitionProfiles)
include_directories(OptParser)
include_directories(vanHavel)

add_executable(a2os ${SOURCE_FILES} AutomatonToOmegaSemigroup.cpp)
add_executable(oslh ${SOURCE_FILES} OmegaSemigroupLandweberHierarchy.cpp)
add_executable(os2w ${SOURCE_FILES} OmegaSemigroupToWeakBuechi.cpp)
add_executable(os2c ${SOURCE_FILES} OmegaSemigroupToCoBuechi.cpp)
add_executable(os2d ${SOURCE_FILES} OmegaSemigroupToDetBuechi.cpp)