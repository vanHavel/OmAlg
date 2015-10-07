#include "BuechiAutomaton.h"

namespace omalg {
  BuechiAutomaton::BuechiAutomaton(std::vector<bool> theFinalStates)
    : finalStates(theFinalStates) {}

  std::string BuechiAutomaton::description() const {
    std::string finalList = "";
    std::vector<std::string> states = this->getStateNames();
    std::vector<bool>::const_iterator iter;
    for (iter = this->finalStates.begin(); iter != this->finalStates.end(); ++iter) {
      if(*iter) {
        finalList += states[iter - this->finalStates.begin()];
        finalList += ",";
      }
    }
    //Remove final ','
    if (finalList.back() == ',') {
      finalList.pop_back();
    }
    finalList += ";";
    return finalList;
  }
  
  bool BuechiAutomaton::isFinal(size_t state) const {
    return this->finalStates[state];
  }
}
