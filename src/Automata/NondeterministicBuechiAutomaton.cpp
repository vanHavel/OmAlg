#include "NondeterministicBuechiAutomaton.h"

namespace omalg {
  NondeterministicBuechiAutomaton::NondeterministicBuechiAutomaton
  (std::vector<std::string> theStateNames,
   std::vector<std::string> theAlphabet,
   size_t theInitialState,
   std::vector<std::vector<std::set<size_t> > > theTransitionRelation,
   std::vector<bool>  theFinalStates)
    : OmegaAutomaton(theStateNames, theAlphabet, theInitialState),
      BuechiAutomaton(theFinalStates),
      NondeterministicOmegaAutomaton(theTransitionRelation) {}

  std::string NondeterministicBuechiAutomaton::description() const {
    std::string description = "Buechi;\n";
    description += "Nondeterministic;\n";
    description += OmegaAutomaton::description() + "\n";
    description += NondeterministicOmegaAutomaton::description() + "\n";
    description += BuechiAutomaton::description();
    return description;
  }
  
  OmegaSemigroup* NondeterministicBuechiAutomaton::toOmegaSemigroup() const {
    return TransformToOmegaSemigroup(*this);
  }
  
  TransitionProfile<NondeterministicBuechiAutomaton> NondeterministicBuechiAutomaton::getTransitionProfileForLetter(size_t letter) const {
    std::vector<std::set<std::pair<size_t,bool> > > newProfile(this->numberOfStates());
    for (size_t state = 0; state < this->numberOfStates(); ++state) {
      std::set<size_t> targets = this->getTargets(state, letter);
      std::set<size_t>::const_iterator targetIter;
      for (targetIter = targets.begin(); targetIter != targets.end(); ++targetIter) {
        size_t targetState = *targetIter;
        bool finalEdge = this->isFinal(state) || this->isFinal(targetState);
        std::pair<size_t, bool> newPair = std::make_pair(targetState, finalEdge);
        newProfile[state].insert(newPair);
      }
    }
    return TransitionProfile<NondeterministicBuechiAutomaton>(newProfile);
  }
  
  TransitionProfile<NondeterministicBuechiAutomaton> NondeterministicBuechiAutomaton::getEpsilonProfile() const {
    std::vector<std::set<std::pair<size_t,bool> > > newProfile(this->numberOfStates());
    for (size_t state = 0; state < this->numberOfStates(); ++state) {
      newProfile[state].insert(std::make_pair(state, this->isFinal(state)));
    }
    return newProfile;
  }
}
