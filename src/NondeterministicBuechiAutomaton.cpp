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

  std::string NondeterministicBuechiAutomaton::description() const {
    std::string description = "Buechi\n";
    description += "Nondeterministic\n";
    description += OmegaAutomaton::description() + "\n";
    description += NondeterministicOmegaAutomaton::description() + "\n";
    description += BuechiAutomaton::description();
    return description;
  }
}
