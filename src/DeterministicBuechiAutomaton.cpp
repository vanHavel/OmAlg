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

  std::string DeterministicBuechiAutomaton::description() const {
    std::string description = "Buechi\n";
    description += "Deterministic\n";
    description += OmegaAutomaton::description() + "\n";
    description += DeterministicOmegaAutomaton::description() + "\n";
    description += BuechiAutomaton::description();
    return description;
  }

}
