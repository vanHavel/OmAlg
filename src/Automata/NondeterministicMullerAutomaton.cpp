#include "NondeterministicMullerAutomaton.h"

namespace omalg {
  NondeterministicMullerAutomaton::NondeterministicMullerAutomaton
  (std::vector<std::string> theStateNames,
   std::vector<std::string> theAlphabet,
   size_t theInitialState,
   std::vector<std::vector<std::set<size_t> > > theTransitionRelation,
   std::set<std::set<size_t> > theTable)
    : OmegaAutomaton(theStateNames, theAlphabet, theInitialState),
      MullerAutomaton(theTable),
      NondeterministicOmegaAutomaton(theTransitionRelation) {}

  std::string NondeterministicMullerAutomaton::description() const {
    std::string description = "Muller;\n";
    description += "Nondeterministic;\n";
    description += OmegaAutomaton::description() + "\n";
    description += NondeterministicOmegaAutomaton::description() + "\n";
    description += MullerAutomaton::description();
    return description;
  }
  
  OmegaSemigroup* NondeterministicMullerAutomaton::toOmegaSemigroup() const {
    return TransformToOmegaSemigroup(*this);
  }
  
  TransitionProfile<NondeterministicMullerAutomaton> NondeterministicMullerAutomaton::getTransitionProfileForLetter(size_t letter) const {
    std::vector<std::set<std::pair<size_t,std::set<size_t> > > > newProfile;
    for (size_t state = 0; state < this->numberOfStates(); ++state) {
      std::set<size_t> targets = this->getTargets(state, letter);
      std::set<size_t>::const_iterator targetIter;
      for (targetIter = targets.begin(); targetIter != targets.end(); ++targetIter) {
        size_t targetState = *targetIter;
        std::set<size_t> visited = {state, targetState};
        std::pair<size_t, std::set<size_t> > newPair = std::make_pair(targetState, visited);
        newProfile[state].insert(newPair);
      }
    }
    return TransitionProfile<NondeterministicMullerAutomaton>(newProfile, this->getTablePointer());
  }
  
  TransitionProfile<NondeterministicMullerAutomaton> NondeterministicMullerAutomaton::getEpsilonProfile() const {
    std::vector<std::set<std::pair<size_t,std::set<size_t> > > > newProfile;
    for (size_t state = 0; state < this->numberOfStates(); ++state) {
      std::set<size_t> stateSingleton = {state};
      newProfile[state].insert(std::make_pair(state, stateSingleton));
    }
    return TransitionProfile<NondeterministicMullerAutomaton>(newProfile, this->getTablePointer());
  }
}
