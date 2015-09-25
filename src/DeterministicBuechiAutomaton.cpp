#include "DeterministicBuechiAutomaton.h"

namespace omalg {
  DeterministicBuechiAutomaton::DeterministicBuechiAutomaton
  (std::vector<std::string> theStateNames,
   std::vector<std::string> theAlphabet,
   size_t theInitialState,
   std::vector<std::vector<size_t> > theTransitionTable,
   std::vector<bool>  theFinalStates)
    : OmegaAutomaton(theStateNames, theAlphabet, theInitialState),
      BuechiAutomaton(theFinalStates),
      DeterministicOmegaAutomaton(theTransitionTable) {}

  OmegaSemigroup* DeterministicBuechiAutomaton::toOmegaSemigroup() const {
    return 0;
  }

  std::string DeterministicBuechiAutomaton::description() const {
    std::string description = "Buechi;\n";
    description += "Deterministic;\n";
    description += OmegaAutomaton::description() + "\n";
    description += DeterministicOmegaAutomaton::description() + "\n";
    description += BuechiAutomaton::description();
    return description;
  }

}
