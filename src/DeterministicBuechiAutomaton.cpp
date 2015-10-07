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

  std::string DeterministicBuechiAutomaton::description() const {
    std::string description = "Buechi;\n";
    description += "Deterministic;\n";
    description += OmegaAutomaton::description() + "\n";
    description += DeterministicOmegaAutomaton::description() + "\n";
    description += BuechiAutomaton::description();
    return description;
  }
  
  OmegaSemigroup* DeterministicBuechiAutomaton::toOmegaSemigroup() const {
    return TransformToOmegaSemigroup(*this);
  }

  TransitionProfile<DeterministicBuechiAutomaton> DeterministicBuechiAutomaton::getTransitionProfileForLetter(size_t letter) const {
    std::vector<std::pair<size_t,bool> > newRepresentation(this->numberOfStates());
    for (size_t state = 0; state < newRepresentation.size(); ++state) {
      size_t target = this->getTarget(state, letter);
      bool finalVisited = this->isFinal(state) || this->isFinal(target);
      newRepresentation[state] = std::make_pair(target, finalVisited);
    }
    return TransitionProfile<DeterministicBuechiAutomaton>(newRepresentation);
  }
  
  TransitionProfile<DeterministicBuechiAutomaton> DeterministicBuechiAutomaton::getEpsilonProfile() const {
    std::vector<std::pair<size_t,bool> > newRepresentation(this->numberOfStates());
    for (size_t state = 0; state < newRepresentation.size(); ++state) {
      newRepresentation[state] = std::make_pair(state, this->isFinal(state));
    }
    return TransitionProfile<DeterministicBuechiAutomaton>(newRepresentation);
  }
}
