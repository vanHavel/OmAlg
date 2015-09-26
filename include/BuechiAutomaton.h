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
  public:
    virtual std::string description() const override;
    /**
     * Returns wheter given state is final. Bounds are not checked.
     * @param state Index of the state to consider.
     * @return true iff the state is final.
     */
    bool isFinal(size_t state) const;
  };
}

#endif
