#ifndef OMALG_COBUECHI_AUTOMATON
#define OMALG_COBUECHI_AUTOMATON

#include <vector>

#include "OmegaAutomaton.h"

namespace omalg {

  class CoBuechiAutomaton: public virtual OmegaAutomaton {
  private:
    std::vector<bool> finalStates;
  protected:
    CoBuechiAutomaton(std::vector<bool> theFinalStates);
    std::vector<bool> getFinalStates() const;
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
