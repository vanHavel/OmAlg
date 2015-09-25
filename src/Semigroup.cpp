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

  inline size_t Semigroup::product(const size_t lhs, const size_t rhs) const {
    return this->multiplicationTable[lhs][rhs];
  }
  
  std::string Semigroup::description() const {
    //Element names
    std::string elementList = "";
    std::vector<std::string>::const_iterator iter;
    for (iter = this->elementNames.begin(); iter != this->elementNames.end(); ++iter) {
      if (iter != this->elementNames.begin()) {
        elementList += ",";
      }
      elementList += *iter;
    }
    elementList += ";";
    
    //Multiplication table
    std::string table = "";
    std::vector<std::vector<size_t> >::const_iterator outerIter;
    for(outerIter = this->multiplicationTable.begin(); outerIter != this->multiplicationTable.end(); ++outerIter) {
      //Add newline, but not at beginning
      if (outerIter != this->multiplicationTable.begin()) {
        table += "\n";
      }
      //Write table line
      std::vector<size_t>::const_iterator innerIter;
      for(innerIter = outerIter->begin(); innerIter != outerIter->end(); ++innerIter) {
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
