#include "MullerAutomaton.h"

namespace omalg {
  MullerAutomaton::MullerAutomaton(std::set<std::set<size_t> > theTable)
    : table(theTable) {}
  
  std::set<std::set<size_t> > const* MullerAutomaton::getTablePointer() const {
    return &(this->table);
  }

  std::string MullerAutomaton::description() const {
    std::string tableList = "";
    for (auto outerIter = this->table.begin(); outerIter != this->table.end(); ++outerIter) {
      if (outerIter != this->table.begin()) {
        tableList += "\n";
      }
      tableList += "{";
      for (auto innerIter = outerIter->begin(); innerIter != outerIter->end(); ++innerIter) {
        if (innerIter != outerIter->begin()) {
          tableList += ",";
        }
        tableList += std::to_string(*innerIter);
      }
      tableList += "}";
    }
    return tableList;
  }
  
  bool MullerAutomaton::inTable(std::set<size_t> stateSet) const {
    return (this->table.find(stateSet) != this->table.end());
  }
}
