#include "NondeterministicParityAutomaton.h"

namespace omalg {
  NondeterministicParityAutomaton::NondeterministicParityAutomaton
  (std::vector<std::string> theStateNames,
   std::vector<std::string> theAlphabet,
   size_t theInitialState,
   std::vector<std::vector<std::set<size_t> > > theTransitionRelation,
   std::vector<size_t>  thePriorities)
    : OmegaAutomaton(theStateNames, theAlphabet, theInitialState),
      ParityAutomaton(thePriorities),
      NondeterministicOmegaAutomaton(theTransitionRelation) {}

  std::string NondeterministicParityAutomaton::description() const {
    std::string description = "Parity;\n";
    description += "Nondeterministic;\n";
    description += OmegaAutomaton::description() + "\n";
    description += NondeterministicOmegaAutomaton::description() + "\n";
    description += ParityAutomaton::description();
    return description;
  }
  
  OmegaSemigroup* NondeterministicParityAutomaton::toOmegaSemigroup() const {
    return TransformToOmegaSemigroup(*this);
  }
  
  TransitionProfile<NondeterministicParityAutomaton> NondeterministicParityAutomaton::getTransitionProfileForLetter(size_t letter) const {
    std::map<size_t, std::set<std::pair<size_t,size_t> > > newProfile;
    for (size_t state = 0; state < this->numberOfStates(); ++state) {
      std::set<size_t> targets = this->getTargets(state, letter);
      std::set<size_t>::const_iterator targetIter;
      for (targetIter = targets.begin(); targetIter != targets.end(); ++targetIter) {
        size_t targetState = *targetIter;
        size_t edgePriority = std::max(this->priority(state), this->priority(targetState));
        std::pair<size_t, size_t> newPair = std::make_pair(targetState, edgePriority);
        newProfile[state].insert(newPair);
      }
    }
    return TransitionProfile<NondeterministicParityAutomaton>(newProfile);
  }
  
  TransitionProfile<NondeterministicParityAutomaton> NondeterministicParityAutomaton::getEpsilonProfile() const {
    std::map<size_t, std::set<std::pair<size_t, size_t> > > newProfile;
    for (size_t state = 0; state < this->numberOfStates(); ++state) {
      newProfile[state].insert(std::make_pair(state, this->priority(state)));
    }
    return newProfile;
  }
}
