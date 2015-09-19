#include "NondeterministicBuechiAutomaton.h"

namespace omalg {
  NondeterministicBuechiAutomaton::NondeterministicBuechiAutomaton
  (int theNumberOfStates, std::vector<std::string> theStateNames,
   int theAlphabetSize,   std::vector<std::string> theAlphabet,
   int theInitialState,   std::vector<std::set<int> > theTransitionRelation,
   std::vector<bool>  theFinalStates)
    : OmegaAutomaton(theNumberOfStates, theStateNames, theAlphabetSize, theAlphabet, theInitialState),
      NondeterministicOmegaAutomaton(theTransitionRelation), BuechiAutomaton(theFinalStates) {}
  OmegaSemigroup* NondeterministicBuechiAutomaton::toOmegaSemigroup() const {
    return 0; //TODO
  }
}
