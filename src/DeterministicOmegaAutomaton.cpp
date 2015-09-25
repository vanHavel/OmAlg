#include "DeterministicOmegaAutomaton.h"

namespace omalg {
  DeterministicOmegaAutomaton::DeterministicOmegaAutomaton(std::vector<std::vector<size_t> > theTransitionTable)
    : transitionTable(theTransitionTable) {}

  std::string DeterministicOmegaAutomaton::description() const {
    std::string transitionList = "";
    std::vector<std::vector<size_t> >::const_iterator outerIter;
    std::vector<std::string> states = this->getStateNames();
    std::vector<std::string> letters = this->getAlphabet();
    for (outerIter = this->transitionTable.begin(); outerIter != this->transitionTable.end(); ++outerIter) {
      //Add newline after each origin state
      if (outerIter != this->transitionTable.begin()) {
        transitionList += ",\n";
      }
      std::vector<size_t>::const_iterator innerIter;
      for (innerIter = outerIter->begin(); innerIter != outerIter->end(); ++innerIter) {
        //Add separator but not at beginning of line
        if (innerIter != outerIter->begin()) {
          transitionList += ",";
        }
        //Construct transition string
        std::string origin = states[outerIter - this->transitionTable.begin()];
        std::string letter = letters[innerIter - outerIter->begin()];
        std::string target = states[*innerIter];
        std::string transition = "(" + origin + "," + letter + "," + target + ")";
        //Add to list
        transitionList += transition;
      }
    }
    return transitionList;
  }
}
