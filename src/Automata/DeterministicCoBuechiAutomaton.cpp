#include "DeterministicCoBuechiAutomaton.h"
#include "DeterministicBuechiAutomaton.h"

namespace omalg {
  DeterministicCoBuechiAutomaton::DeterministicCoBuechiAutomaton
  (std::vector<std::string> theStateNames,
   std::vector<std::string> theAlphabet,
   size_t theInitialState,
   std::vector<std::vector<size_t> > theTransitionTable,
   std::vector<bool>  theFinalStates)
    : OmegaAutomaton(theStateNames, theAlphabet, theInitialState),
      CoBuechiAutomaton(theFinalStates),
      DeterministicOmegaAutomaton(theTransitionTable) {}

  std::string DeterministicCoBuechiAutomaton::description() const {
    std::string description = "CoBuechi;\n";
    description += "Deterministic;\n";
    description += OmegaAutomaton::description() + "\n";
    description += DeterministicOmegaAutomaton::description() + "\n";
    description += CoBuechiAutomaton::description();
    return description;
  }
  
  OmegaSemigroup* DeterministicCoBuechiAutomaton::toOmegaSemigroup() const {
    return TransformToOmegaSemigroup(*this);
  }

  TransitionProfile<DeterministicCoBuechiAutomaton> DeterministicCoBuechiAutomaton::getTransitionProfileForLetter(size_t letter) const {
    std::vector<std::pair<size_t,bool> > newRepresentation(this->numberOfStates());
    for (size_t state = 0; state < newRepresentation.size(); ++state) {
      size_t target = this->getTarget(state, letter);
      bool finalVisited = this->isFinal(state) && this->isFinal(target);
      newRepresentation[state] = std::make_pair(target, finalVisited);
    }
    return TransitionProfile<DeterministicCoBuechiAutomaton>(newRepresentation);
  }
  
  TransitionProfile<DeterministicCoBuechiAutomaton> DeterministicCoBuechiAutomaton::getEpsilonProfile() const {
    std::vector<std::pair<size_t,bool> > newRepresentation(this->numberOfStates());
    for (size_t state = 0; state < newRepresentation.size(); ++state) {
      newRepresentation[state] = std::make_pair(state, this->isFinal(state));
    }
    return TransitionProfile<DeterministicCoBuechiAutomaton>(newRepresentation);
  }

  DeterministicBuechiAutomaton DeterministicCoBuechiAutomaton::dual() const {
    //invert final states
    auto inverted = this->getFinalStates();
    for (auto iter = inverted.begin(); iter != inverted.end(); ++iter) {
      *iter = !(*iter);
    }
    return DeterministicBuechiAutomaton(this->getStateNames(), this->getAlphabet(), this->getInitialState(), this->getTransitionTable(), inverted);
  }
}
