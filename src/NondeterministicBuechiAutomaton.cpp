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
    return TransitionProfile<NondeterministicBuechiAutomaton>();
  }
}
