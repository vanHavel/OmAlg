#include "NondeterministicOmegaAutomaton.h"

namespace omalg {
  NondeterministicOmegaAutomaton::NondeterministicOmegaAutomaton(std::vector<std::vector<std::set<size_t> > > theTransitionRelation)
    : transitionRelation(theTransitionRelation) {}

  std::string NondeterministicOmegaAutomaton::description() const {
    std::string transitionList = "";
    std::vector<std::vector<std::set<size_t> > >::const_iterator outerIter;
    std::vector<std::string> states = this->getStateNames();
    std::vector<std::string> letters = this->getAlphabet();
    for (outerIter = this->transitionRelation.begin(); outerIter != this->transitionRelation.end(); ++outerIter) {
      //Add newline after each origin state, but don't add empty lines
      if (outerIter != this->transitionRelation.begin() && !transitionList.empty() && transitionList.back() != '\n') {
        transitionList += "\n";
      }
      std::vector<std::set<size_t> >::const_iterator middleIter;
      for (middleIter = outerIter->begin(); middleIter != outerIter->end(); ++middleIter) {
        std::set<size_t>::const_iterator innerIter;
        for (innerIter = middleIter->begin(); innerIter != middleIter->end(); ++innerIter) {
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
    transitionList += ";";
    return transitionList;
  }
}
