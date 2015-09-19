#ifndef OMALG_OMEGA_AUTOMATON
#define OMALG_OMEGA_AUTOMATON

#include <vector>
#include <string>

#include "OmegaSemigroup.h"

namespace omalg {

  class OmegaAutomaton {
      
  private:
    int numberOfStates;
    std::vector<std::string> stateNames;
    
    int alphabetSize;
    std::vector<std::string> alphabet;
    
    int initialState;
    
  protected:
    OmegaAutomaton(int theNumberOfStates, std::vector<std::string> theStateNames,
                   int theAlphabetSize,   std::vector<std::string> theAlphabet,
                   int theInitialState);

  public:
      virtual OmegaSemigroup* toOmegaSemigroup() const = 0;
      virtual ~OmegaAutomaton() {};
  };
}

#endif
