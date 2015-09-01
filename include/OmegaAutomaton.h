#ifndef OMALG_OMEGA_AUTOMATON
#define OMALG_OMEGA_AUTOMATON

#include <vector>
#include <string>

#include "TransitionRelation.h"
#include "OmegaSemigroup.h"

namespace omalg {

  class OmegaAutomaton {
      
  private:
    int numberOfStates;
    std::vector<std::string> stateNames;
    
    int alphabetSize;
    std::vector<std::string> alphabet;
    
    int initialState;
    
  public:
      virtual OmegaSemigroup* toOmegaSemigroup() = 0;
      virtual ~OmegaAutomaton() { };
  };
}

#endif