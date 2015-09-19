#include "NondeterministicOmegaAutomaton.h"

namespace omalg {
  NondeterministicOmegaAutomaton::NondeterministicOmegaAutomaton(std::vector<std::set<int> > theTransitionRelation)
    : transitionRelation(theTransitionRelation) {}
}
