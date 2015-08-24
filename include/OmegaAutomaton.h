#ifndef OMALG_OMEGA_AUTOMATON
#define OMALG_OMEGA_AUTOMATON

#include <vector>
#include <string>

#include "TransitionRelation.h"
#include "OmegaSemigroup.h"

namespace omalg {

  class OmegaAutomaton {
      
  protected:
    int numberOfStates;
    std::vector<std::string> stateNames;
    
    int alphabetSize;
    std::vector<std::string> alphabet;
    
    int initialState;
    
    TransitionRelation transitionRelation;
    
  public:
      virtual OmegaSemigroup toOmegaSemigroup() = 0;
      virtual ~OmegaAutomaton() { };
  };
}

#endif