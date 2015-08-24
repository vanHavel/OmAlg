#ifndef OMALG_BUECHI_AUTOMATON
#define OMALG_BUECHI_AUTOMATON

#include "OmegaAutomaton.h"
#include "OmegaSemigroup.h"

namespace omalg {

  class BuechiAutomaton: public OmegaAutomaton {
  protected:
    bool coBuechi;
    std::vector<int> finalStates;

  public:
      OmegaSemigroup toOmegaSemigroup();
      ~BuechiAutomaton();
  };
}

#endif