#include "OmegaAutomaton.h"

namespace omalg {

  OmegaAutomaton::OmegaAutomaton(int theNumberOfStates, std::vector<std::string> theStateNames,
                                 int theAlphabetSize,   std::vector<std::string> theAlphabet,
                                 int theInitialState)
    : numberOfStates(theNumberOfStates), stateNames(theStateNames), alphabetSize(theAlphabetSize),
      alphabet(theAlphabet), initialState(theInitialState) {}

}
