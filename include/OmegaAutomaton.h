#ifndef OMALG_OMEGA_AUTOMATON
#define OMALG_OMEGA_AUTOMATON

#include <vector>
#include <string>

#include "OmegaSemigroup.h"

namespace omalg {

  class OmegaAutomaton {
      
  private:
    std::vector<std::string> stateNames;
    
    std::vector<std::string> alphabet;
    
    int initialState;
    
  protected:
    OmegaAutomaton(std::vector<std::string> theStateNames,
                   std::vector<std::string> theAlphabet,
                   size_t theInitialState);
    std::vector<std::string> getStateNames() const;
    std::vector<std::string> getAlphabet() const;

  public:
    /**
     * Returns a textual description of the automaton in the OmAlg format
     * @return A string containing the description.
     */
    virtual std::string description() const;
    /**
     * Constructs an equivalent omega semigroup for the automaton.
     * @return The omega semigroup.
     */
    virtual OmegaSemigroup* toOmegaSemigroup() const = 0;
    /**
     * Virtual destructor.
     */
    virtual ~OmegaAutomaton() {};
  };
}

#endif
