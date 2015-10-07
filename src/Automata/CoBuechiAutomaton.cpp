#include "CoBuechiAutomaton.h"

namespace omalg {
  CoBuechiAutomaton::CoBuechiAutomaton(std::vector<bool> theFinalStates)
    : finalStates(theFinalStates) {}

  std::string CoBuechiAutomaton::description() const {
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
  
  bool CoBuechiAutomaton::isFinal(size_t state) const {
    return this->finalStates[state];
  }
}
