#ifndef OMALG_BUECHI_AUTOMATON
#define OMALG_BUECHI_AUTOMATON

#include "OmegaAutomaton.h"

namespace omalg {

  class BuechiAutomaton: OmegaAutomaton {
  private:
    bool coBuechi;
    std::vector<int> finalStates;
  };

}

#endif