#include "OmegaSemigroup.h"

namespace omalg {
  OmegaSemigroup::OmegaSemigroup(Semigroup theSemigroup, std::vector<std::string> theOmegaElementNames,
      std::vector<std::vector<size_t> > theMixedProductTable, std::vector<size_t> theOmegaIterationTable,
      std::vector<bool> theP, Morphism thePhi)
        : sPlus(theSemigroup), omegaElementNames(theOmegaElementNames),
          mixedProductTable(theMixedProductTable), omegaIterationTable(theOmegaIterationTable),
          P(theP), phi(thePhi) { }

  inline size_t OmegaSemigroup::product(size_t lhs, size_t rhs) const {
    return this->sPlus.product(lhs, rhs);
  }

  inline size_t OmegaSemigroup::mixedProduct(size_t plusElement, size_t omegaElement) const {
    return this->mixedProductTable[plusElement][omegaElement];
  }

  inline size_t OmegaSemigroup::omegaIteration(size_t element) const {
    return this->omegaIterationTable[element];
  }

  std::string OmegaSemigroup::description() const {
    //Finite part
    std::string sPlusDescription = this->sPlus.description();
    
    //Omega element names
    std::string omegaElementList = "";
    std::vector<std::string>::const_iterator iter;
    for (iter = this->omegaElementNames.begin(); iter != this->omegaElementNames.end(); ++iter) {
      if (iter != this->omegaElementNames.begin()) {
        omegaElementList += ",";
      }
      omegaElementList += *iter;
    }
    omegaElementList += ";";
    
    //Mixed product table
    std::string mixedTable = "";
    std::vector<std::vector<size_t> >::const_iterator outerIter;
    for(outerIter = this->mixedProductTable.begin(); outerIter != this->mixedProductTable.end(); ++outerIter) {
      //Add newline, but not at beginning
      if (outerIter != this->mixedProductTable.begin()) {
        mixedTable += "\n";
      }
      //Write table line
      std::vector<size_t>::const_iterator innerIter;
      for(innerIter = outerIter->begin(); innerIter != outerIter->end(); ++innerIter) {
        //Add ',', but not at beginnning
        if (!mixedTable.empty() && !mixedTable.back() == '\n') {
          mixedTable += ",";
        }
        mixedTable += this->omegaElementNames[*innerIter];
      }
    }
    //Add final ';'
    mixedTable += ";";
    
    //Omega iteration table
    std::string omegaTable = "";
    std::vector<size_t>::const_iterator iter2;
    for (iter2 = this->omegaIterationTable.begin(); iter2 != this->omegaIterationTable.end(); ++iter2) {
      if (iter2 != this->omegaIterationTable.begin()) {
        omegaTable += ",";
      }
      omegaTable += this->omegaElementNames[*iter2];
    }
    omegaTable += ";";
    
    return sPlusDescription + "\n" + omegaElementList + "\n" + 
           mixedTable +       "\n" + omegaTable;
  }

}
