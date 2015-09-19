#include "DeterministicBuechiAutomaton.h"

namespace omalg {
  DeterministicBuechiAutomaton::DeterministicBuechiAutomaton
  (int theNumberOfStates, std::vector<std::string> theStateNames,
   int theAlphabetSize,   std::vector<std::string> theAlphabet,
   int theInitialState,   std::vector<std::vector<int> > theTransitionTable,
   std::vector<bool>  theFinalStates)
    : OmegaAutomaton(theNumberOfStates, theStateNames, theAlphabetSize, theAlphabet, theInitialState),
      DeterministicOmegaAutomaton(theTransitionTable), BuechiAutomaton(theFinalStates) {}

  OmegaSemigroup* DeterministicBuechiAutomaton::toOmegaSemigroup() const {
    return 0;
  }

}
