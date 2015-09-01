#ifndef OMALG_NONDETERMINISTIC_BUECHI_AUTOMATON
#define OMALG_NONDETERMINISTIC_BUECHI_AUTOMATON

#include <vector>
#include <set>

#include "BuechiAutomaton.h"
#include "NondeterministicOmegaAutomaton.h"

namespace omalg {

  class NondeterministicBuechiAutomaton: public BuechiAutomaton, public NondeterministicOmegaAutomaton {
		  
  };
}

#endif