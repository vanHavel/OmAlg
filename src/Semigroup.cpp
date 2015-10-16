#include "Semigroup.h"

namespace omalg {

  Semigroup::Semigroup(std::vector<std::string> theElementNames, std::vector<std::vector<size_t> > theMultiplicationTable)
    : elementNames(theElementNames), multiplicationTable(theMultiplicationTable) {
    this->hClasses = 0;
    this->rClasses = 0;
    this->lClasses = 0;
    this->jClasses = 0;
    this->jOrder = 0;
  }

  size_t Semigroup::product(size_t lhs, size_t rhs) const {
    return this->multiplicationTable[lhs][rhs];
  }
  
  std::string Semigroup::elementName(size_t index) const {
    return this->elementNames.at(index);
  }
  
  std::string Semigroup::description() const {
    //Element names
    std::string elementList = "";
    for (auto iter = this->elementNames.begin(); iter != this->elementNames.end(); ++iter) {
      if (iter != this->elementNames.begin()) {
        elementList += ",";
      }
      elementList += *iter;
    }
    elementList += ";";
    
    //Multiplication table
    std::string table = "";
    for(auto outerIter = this->multiplicationTable.begin();
        outerIter != this->multiplicationTable.end();
        ++outerIter) {
      //Add newline, but not at beginning
      if (outerIter != this->multiplicationTable.begin()) {
        table += "\n";
      }
      //Write table line
      for(auto innerIter = outerIter->begin(); innerIter != outerIter->end(); ++innerIter) {
        //Add ',', but not at beginnning
        if (!table.empty() && table.back() != '\n') {
          table += ",";
        }
        table += this->elementNames[*innerIter];
      }
    }
    //Add final ';'
    table += ";";
    
    return elementList + "\n" + table;
  }
}
