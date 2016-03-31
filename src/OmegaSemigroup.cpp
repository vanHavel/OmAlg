#include "OmegaSemigroup.h"
#include "DeterministicBuechiAutomaton.h"
#include "DeterministicCoBuechiAutomaton.h"
#include "DeterministicParityAutomaton.h"
#include "AutomatonConverter.h"

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
    this->sPlus.calculateROrder();
    auto linkedPairs = this->sPlus.linkedPairs();
    //Check all linked pairs (s,e) and (s,f) for the condition. This makes use of the list of linked pairs being sorted.
    for (auto iter1 = linkedPairs.begin(); iter1 != linkedPairs.end(); ++iter1) {
      for (auto iter2 = iter1; iter2 != linkedPairs.end() && iter2->first == iter1->first ; ++iter2) {
        if (iter2 == iter1) {
          continue;
        }
        auto e = iter1->second;
        auto f = iter2->second;
        auto s = iter1->first;
        auto eOm = this->omegaIterationTable[e];
        auto fOm = this->omegaIterationTable[f];
        //Check if e <=_r f, sf^w in P, se^w not in P. In this case the condition is violated.
        if (this->sPlus.r(e, f) && this->P[this->mixedProductTable[s][fOm]] && !this->P[this->mixedProductTable[s][eOm]]) {
          return false;
        }
        //Check the same for f and e switching roles.
        if (this->sPlus.r(f, e) && this->P[this->mixedProductTable[s][eOm]] && !this->P[this->mixedProductTable[s][fOm]]) {
          return false;
        }
      }
    }
    return true;
  }

  bool OmegaSemigroup::isDCRecognizable() const {
    OmegaSemigroup comp = *this;
    comp.invertP();
    return comp.isDBRecognizable();
  }

  bool OmegaSemigroup::isERecognizable() const {
    this->sPlus.calculateROrder();
    auto linkedPairs = this->sPlus.linkedPairs();
    //Check all linked pairs (s,e) and (t,f) for the condition.
    for (auto iter1 = linkedPairs.begin(); iter1 != linkedPairs.end(); ++iter1) {
      for (auto iter2 = linkedPairs.begin(); iter2 != linkedPairs.end(); ++iter2) {
        if (iter2 == iter1) {
          continue;
        }
        auto s = iter1->first;
        auto t = iter2->first;
        auto e = iter1->second;
        auto f = iter2->second;
        auto eOm = this->omegaIterationTable[e];
        auto fOm = this->omegaIterationTable[f];
        //check if s <=_r t, tf^w in P and se^w not in P. Then the condition is violated.
        if (this->sPlus.r(s,t) && this->P[this->mixedProductTable[t][fOm]] && !this->P[this->mixedProductTable[s][eOm]]) {
          return false;
        }
      }
    }
    return true;
  }

  bool OmegaSemigroup::isARecognizable() const {
    OmegaSemigroup comp = *this;
    comp.invertP();
    return comp.isERecognizable();
  }

  bool OmegaSemigroup::isWBRecognizable() const {
    return (this->isDBRecognizable() && this->isDCRecognizable());
  }

  DeterministicBuechiAutomaton OmegaSemigroup::toWeakBuechi() const {
    //Check if conversion is possible
    if (!this->isWBRecognizable()) {
      throw OperationNotApplicableException();
    }
    auto Converter = AutomatonConverter(*this);
    return Converter.convertToWeakBuechi();
  }

  DeterministicCoBuechiAutomaton OmegaSemigroup::toCoBuechi() const {
    if (!this->isDCRecognizable()) {
      throw OperationNotApplicableException();
    }
    auto Converter = AutomatonConverter(*this);
    return Converter.convertToCoBuechi();
  }

  DeterministicBuechiAutomaton OmegaSemigroup::toDetBuechi() const {
    if (!this->isDBRecognizable()) {
      throw OperationNotApplicableException();
    }
    auto Converter = AutomatonConverter(*this);
    return Converter.convertToDetBuechi();
  }

  DeterministicParityAutomaton OmegaSemigroup::toParity() const {
    auto Converter = AutomatonConverter(*this);
    return Converter.convertToParity();
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
