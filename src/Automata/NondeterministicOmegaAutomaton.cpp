#include "NondeterministicOmegaAutomaton.h"

namespace omalg {
  NondeterministicOmegaAutomaton::NondeterministicOmegaAutomaton(std::vector<std::vector<std::set<size_t> > > theTransitionRelation)
    : transitionRelation(theTransitionRelation) {}
  
  std::set<size_t> NondeterministicOmegaAutomaton::getTargets(size_t state, size_t letter) const {
    return this->transitionRelation[state][letter];
  }

  std::string NondeterministicOmegaAutomaton::description() const {
    std::string transitionList = "";
    std::vector<std::string> states = this->getStateNames();
    std::vector<std::string> letters = this->getAlphabet();
    for (auto outerIter = this->transitionRelation.begin(); outerIter != this->transitionRelation.end(); ++outerIter) {
      //Add newline after each origin state, but don't add empty lines
      if (outerIter != this->transitionRelation.begin() && !transitionList.empty() && transitionList.back() != '\n') {
        transitionList += ",\n";
      }
      for (auto middleIter = outerIter->begin(); middleIter != outerIter->end(); ++middleIter) {
        for (auto innerIter = middleIter->begin(); innerIter != middleIter->end(); ++innerIter) {
          //Add separator but not at beginning of new line
          if (transitionList.back() != '\n' && !transitionList.empty()) {
            transitionList += ",";
          }
          //Construct transition string
          std::string origin = states[outerIter - this->transitionRelation.begin()];
          std::string letter = letters[middleIter - outerIter->begin()];
          std::string target = states[*innerIter];
          std::string transition = "(" + origin + "," + letter + "," + target + ")";
          //Add to list
          transitionList += transition;
        }
      }
    }
    //Remove possible trailing newline and ','
    if (transitionList.back() == '\n') {
      transitionList.pop_back();
      transitionList.pop_back();
    }
    transitionList += ";";
    return transitionList;
  }
}
