#ifndef OMALG_BUECHI_AUTOMATON
#define OMALG_BUECHI_AUTOMATON

#include <vector>

#include "OmegaAutomaton.h"

namespace omalg {

  class BuechiAutomaton: public virtual OmegaAutomaton {
  private:
    std::vector<bool> finalStates;

  protected:
    BuechiAutomaton(std::vector<bool> theFinalStates);

  };
}

#endif
