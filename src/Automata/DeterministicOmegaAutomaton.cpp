#include "DeterministicOmegaAutomaton.h"

namespace omalg {
  DeterministicOmegaAutomaton::DeterministicOmegaAutomaton(std::vector<std::vector<size_t> > theTransitionTable)
    : transitionTable(theTransitionTable) {}

  std::string DeterministicOmegaAutomaton::description() const {
    std::string transitionList = "";
    std::vector<std::string> states = this->getStateNames();
    std::vector<std::string> letters = this->getAlphabet();
    for (auto outerIter = this->transitionTable.begin(); outerIter != this->transitionTable.end(); ++outerIter) {
      //Add newline after each origin state
      if (outerIter != this->transitionTable.begin()) {
        transitionList += ",\n";
      }
      for (auto innerIter = outerIter->begin(); innerIter != outerIter->end(); ++innerIter) {
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
  
  size_t DeterministicOmegaAutomaton::getTarget(size_t state, size_t transition) const{
    return this->transitionTable[state][transition];
  }

  std::vector<std::vector<size_t> > DeterministicOmegaAutomaton::getTransitionTable() const {
    return this->transitionTable;
  }
}
