#include "ParityAutomaton.h"

namespace omalg {
  ParityAutomaton::ParityAutomaton(std::vector<size_t> thePriorities)
    : priorities(thePriorities) {}

  std::string ParityAutomaton::description() const {
    std::string parityList = "";
    std::vector<size_t>::const_iterator iter;
    for (iter = this->priorities.begin(); iter != this->priorities.end(); ++iter) {
      if (iter != this->priorities.begin()) {
        parityList += ",";
      }
      parityList += std::to_string(*iter);
    }
    parityList += ";";
    return parityList;
  }
  
  size_t ParityAutomaton::priority(size_t state) const {
    return this->priorities[state];
  }
}
