#include "OmegaAutomaton.h"

namespace omalg {

  OmegaAutomaton::OmegaAutomaton(std::vector<std::string> theStateNames,
                                 std::vector<std::string> theAlphabet,
                                 size_t theInitialState)
    : stateNames(theStateNames), alphabet(theAlphabet), initialState(theInitialState) {}

  std::string OmegaAutomaton::description() const {
    //State names
    std::string stateList = "";
    for (auto iter = this->stateNames.begin(); iter != this->stateNames.end(); ++iter) {
      if (iter != this->stateNames.begin()) {
        stateList += ",";
      }
      stateList += *iter;
    }
    stateList += ";";
    //Initial name
    std::string initialName = this->stateNames[this->initialState] + ";";
    //Alphabet
    std::string letterList = "";
    for (auto iter = this->alphabet.begin(); iter != this->alphabet.end(); ++iter) {
      if (iter != this->alphabet.begin()) {
        letterList += ",";
      }
      letterList += *iter;
    }
    letterList += ";";
    return stateList + "\n" + initialName + "\n" + letterList;
  }

  std::vector<std::string> OmegaAutomaton::getStateNames() const {
    return this->stateNames;
  }
  
  std::vector<std::string> OmegaAutomaton::getAlphabet() const {
    return this->alphabet;
  }
  
  size_t OmegaAutomaton::numberOfStates() const {
    return this->stateNames.size();
  }
  
  size_t OmegaAutomaton::alphabetSize() const {
    return this->alphabet.size();
  }
  
  size_t OmegaAutomaton::getInitialState() const {
    return this->initialState;
  }
}

