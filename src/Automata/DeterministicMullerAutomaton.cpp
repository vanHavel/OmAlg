#include "DeterministicMullerAutomaton.h"

namespace omalg {
  DeterministicMullerAutomaton::DeterministicMullerAutomaton
  (std::vector<std::string> theStateNames,
   std::vector<std::string> theAlphabet,
   size_t theInitialState,
   std::vector<std::vector<size_t> > theTransitionTable,
   std::set<std::set<size_t> > theTable)
    : OmegaAutomaton(theStateNames, theAlphabet, theInitialState),
      MullerAutomaton(theTable),
      DeterministicOmegaAutomaton(theTransitionTable) {}

  std::string DeterministicMullerAutomaton::description() const {
    std::string description = "Muller;\n";
    description += "Deterministic;\n";
    description += OmegaAutomaton::description() + "\n";
    description += DeterministicOmegaAutomaton::description() + "\n";
    description += MullerAutomaton::description();
    return description;
  }
  
  OmegaSemigroup* DeterministicMullerAutomaton::toOmegaSemigroup() const {
    return TransformToOmegaSemigroup(*this);
  }

  TransitionProfile<DeterministicMullerAutomaton> DeterministicMullerAutomaton::getTransitionProfileForLetter(size_t letter) const {
    std::vector<std::pair<size_t,std::set<size_t> > > newRepresentation(this->numberOfStates());
    for (size_t state = 0; state < this->numberOfStates(); ++state) {
      size_t target = this->getTarget(state, letter);
      std::set<size_t> visited = {state, target};
      newRepresentation[state] = std::make_pair(target, visited);
    }
    return TransitionProfile<DeterministicMullerAutomaton>(newRepresentation, this->getTablePointer());
  }
  
  TransitionProfile<DeterministicMullerAutomaton> DeterministicMullerAutomaton::getEpsilonProfile() const {
    std::vector<std::pair<size_t,std::set<size_t> > > newRepresentation(this->numberOfStates());
    for (size_t state = 0; state < this->numberOfStates(); ++state) {
      std::set<size_t> stateSingleton = {state};
      newRepresentation[state] = std::make_pair(state, stateSingleton);
    }
    return TransitionProfile<DeterministicMullerAutomaton>(newRepresentation, this->getTablePointer());
  }
}
