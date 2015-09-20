#include "NondeterministicBuechiAutomaton.h"

namespace omalg {
  NondeterministicBuechiAutomaton::NondeterministicBuechiAutomaton
  (std::vector<std::string> theStateNames,
   std::vector<std::string> theAlphabet,
   int theInitialState,
   std::vector<std::vector<std::set<int> > > theTransitionRelation,
   std::vector<bool>  theFinalStates)
    : OmegaAutomaton(theStateNames, theAlphabet, theInitialState),
      NondeterministicOmegaAutomaton(theTransitionRelation),
      BuechiAutomaton(theFinalStates) {}
  OmegaSemigroup* NondeterministicBuechiAutomaton::toOmegaSemigroup() const {
    return 0; //TODO
  }
}
