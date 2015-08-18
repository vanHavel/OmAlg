#ifndef OMALG_OMEGA_AUTOMATON
#define OMALG_OMEGA_AUTOMATON

#include <vector>
#include <string>

namespace omalg {

  class OmegaAutomaton {
  private:
    int numberOfStates;
    std::vector<std::string> stateNames;
    
    int alphabetSize;
    std::vector<std::string> alphabet;
    
    int initialState;
    
  };
}

#endif