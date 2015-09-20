#include "DeterministicBuechiAutomaton.h"

namespace omalg {
  DeterministicBuechiAutomaton::DeterministicBuechiAutomaton
  (std::vector<std::string> theStateNames,
   std::vector<std::string> theAlphabet,
   int theInitialState,
   std::vector<std::vector<int> > theTransitionTable,
   std::vector<bool>  theFinalStates)
    : OmegaAutomaton(theStateNames, theAlphabet, theInitialState),
      DeterministicOmegaAutomaton(theTransitionTable),
      BuechiAutomaton(theFinalStates) {}

  OmegaSemigroup* DeterministicBuechiAutomaton::toOmegaSemigroup() const {
    return 0;
  }

}
