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

  void OmegaSemigroup::invertP() {
    for (auto iter = this->P.begin(); iter != this->P.end(); ++iter) {
      *iter = !*iter;
    }
  }

  bool OmegaSemigroup::isDBRecognizable() const {
    return true; //TODO
  }

  bool OmegaSemigroup::isDCRecognizable() const {
      return true; //TODO
    }

  bool OmegaSemigroup::isERecognizable() const {
      return true; //TODO
    }

  bool OmegaSemigroup::isARecognizable() const {
      return true; //TODO
    }

  bool OmegaSemigroup::isWBRecognizable() const {
      return true; //TODO
    }

  std::string OmegaSemigroup::description() const {
    //Finite part
    std::string sPlusDescription = this->sPlus.description();
    
    //Omega element names
    std::string omegaElementList = "";
    for (auto iter = this->omegaElementNames.begin(); iter != this->omegaElementNames.end(); ++iter) {
      if (iter != this->omegaElementNames.begin()) {
        omegaElementList += ",";
      }
      omegaElementList += *iter;
    }
    omegaElementList += ";";
    
    //Mixed product table
    std::string mixedTable = "";
    for(auto outerIter = this->mixedProductTable.begin(); outerIter != this->mixedProductTable.end(); ++outerIter) {
      //Add newline, but not at beginning
      if (outerIter != this->mixedProductTable.begin()) {
        mixedTable += "\n";
      }
      //Write table line
      for(auto innerIter = outerIter->begin(); innerIter != outerIter->end(); ++innerIter) {
        //Add ',', but not at beginnning
        if (!mixedTable.empty() && mixedTable.back() != '\n') {
          mixedTable += ",";
        }
        mixedTable += this->omegaElementNames[*innerIter];
      }
    }
    //Add final ';'
    mixedTable += ";";
    
    //Omega iteration table
    std::string omegaTable = "";
    for (auto iter = this->omegaIterationTable.begin(); iter != this->omegaIterationTable.end(); ++iter) {
      if (iter != this->omegaIterationTable.begin()) {
        omegaTable += ",";
      }
      omegaTable += this->omegaElementNames[*iter];
    }
    omegaTable += ";";
    
    //Alphabet and Morphism.
    std::string alphabet = "";
    std::string targetList = "";
    for (size_t letter = 0; letter < this->phi.size(); ++letter) {
      if (letter != 0) {
        alphabet += ",";
        targetList += ",";
      }
      alphabet += this->phi.letterName(letter);
      targetList += this->sPlus.elementName(this->phi[letter]);
    }
    alphabet += ";";
    targetList += ";";
    
    //Final set.
    std::string PList = "";
    for (auto iter = this->P.begin(); iter != this->P.end(); ++iter) {
      if (*iter) {
        PList += this->omegaElementNames[iter - P.begin()];
        PList += ",";
      }
    }
    //Remove final ',' and set ';' instead.
    PList.pop_back();
    PList += ";";
    
    return sPlusDescription + "\n" + omegaElementList + "\n" + 
           mixedTable       + "\n" + omegaTable       + "\n" +
           alphabet         + "\n" + targetList       + "\n" +
           PList;
  }

}
