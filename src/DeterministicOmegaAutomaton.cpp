#include "DeterministicOmegaAutomaton.h"

namespace omalg {
  DeterministicOmegaAutomaton::DeterministicOmegaAutomaton(std::vector<std::vector<int> > theTransitionTable)
    : transitionTable(theTransitionTable) {}
}
