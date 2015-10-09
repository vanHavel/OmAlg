#include "DeterministicParityAutomaton.h"

namespace omalg {
  DeterministicParityAutomaton::DeterministicParityAutomaton
  (std::vector<std::string> theStateNames,
   std::vector<std::string> theAlphabet,
   size_t theInitialState,
   std::vector<std::vector<size_t> > theTransitionTable,
   std::vector<size_t>  thePriorities)
    : OmegaAutomaton(theStateNames, theAlphabet, theInitialState),
      ParityAutomaton(thePriorities),
      DeterministicOmegaAutomaton(theTransitionTable) {}

  std::string DeterministicParityAutomaton::description() const {
    std::string description = "Parity;\n";
    description += "Deterministic;\n";
    description += OmegaAutomaton::description() + "\n";
    description += DeterministicOmegaAutomaton::description() + "\n";
    description += ParityAutomaton::description();
    return description;
  }
  
  OmegaSemigroup* DeterministicParityAutomaton::toOmegaSemigroup() const {
    return TransformToOmegaSemigroup(*this);
  }

  TransitionProfile<DeterministicParityAutomaton> DeterministicParityAutomaton::getTransitionProfileForLetter(size_t letter) const {
    std::vector<std::pair<size_t,size_t> > newRepresentation(this->numberOfStates());
    for (size_t state = 0; state < newRepresentation.size(); ++state) {
      size_t target = this->getTarget(state, letter);
      size_t edgePriority = std::max(this->priority(state), this->priority(target));
      newRepresentation[state] = std::make_pair(target, edgePriority);
    }
    return TransitionProfile<DeterministicParityAutomaton>(newRepresentation);
  }
  
  TransitionProfile<DeterministicParityAutomaton> DeterministicParityAutomaton::getEpsilonProfile() const {
    std::vector<std::pair<size_t,size_t> > newRepresentation(this->numberOfStates());
    for (size_t state = 0; state < newRepresentation.size(); ++state) {
      newRepresentation[state] = std::make_pair(state, this->priority(state));
    }
    return TransitionProfile<DeterministicParityAutomaton>(newRepresentation);
  }
}
