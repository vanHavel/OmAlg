cmake_minimum_required(VERSION 3.4)
project(a2os)
project(oslh)
project(os2w)
project(os2c)
project(os2d)
project(os2p)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(OMALG_SOURCES
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

include_directories(
        include
        include/Automata
        include/TransitionProfiles
        OptParser
        vanHavel)

add_library(OmAlgLib ${OMALG_SOURCES})

add_executable(a2os AutomatonToOmegaSemigroup.cpp)
add_executable(oslh OmegaSemigroupLandweberHierarchy.cpp)
add_executable(os2w OmegaSemigroupToWeakBuechi.cpp)
add_executable(os2c OmegaSemigroupToCoBuechi.cpp)
add_executable(os2d OmegaSemigroupToDetBuechi.cpp)
add_executable(os2p OmegaSemigroupToParity.cpp)

target_link_libraries(a2os OmAlgLib)
target_link_libraries(oslh OmAlgLib)
target_link_libraries(os2w OmAlgLib)
target_link_libraries(os2c OmAlgLib)
target_link_libraries(os2d OmAlgLib)
target_link_libraries(os2p OmAlgLib)