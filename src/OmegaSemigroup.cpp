#include <list>

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

  //TODO Find a more efficient implementation
  void OmegaSemigroup::reduceToSyntactic() {
    //Reduce sPlus
    size_t pSize = this->sPlus.size();
    //list of all element ids yet to check
    auto toCheck = std::list<size_t>();
    for (size_t s = 0; s < pSize; ++s) {
      toCheck.push_back(s);
    }
    //list of representants
    auto representantList = std::list<size_t>();
    //vector assigning to each element the id of its equivalence class
    auto classID = std::vector<size_t>(pSize, 0);
    size_t currentClassID = 0;
    //main loop
    while(!toCheck.empty()) {
      //process first element
      size_t current = toCheck.front();
      toCheck.pop_front();
      classID[current] = currentClassID;
      representantList.push_back(current);
      //identify equivalent elements
      auto iter = toCheck.begin();
      while (iter != toCheck.end()) {
        if (this->sPlusEquivalent(current, *iter)) {
          classID[*iter] = currentClassID;
          iter = toCheck.erase(iter);
        }
        else {
          ++iter;
        }
      }
      //increase class ID
      ++currentClassID;
    }

    //Reduce sOmega
    size_t oSize = this->omegaElementNames.size();
    //list of all element ids yet to check
    toCheck = std::list<size_t>();
    for (size_t s = 0; s < oSize; ++s) {
      toCheck.push_back(s);
    }
    //list of representants
    auto omegaRepresentantList = std::list<size_t>();
    //vector assigning to each element the id of its equivalence class
    auto omegaClassID = std::vector<size_t>(oSize, 0);
    size_t omegaCurrentClassID = 0;
    //main loop
    while(!toCheck.empty()) {
      //process first element
      size_t current = toCheck.front();
      toCheck.pop_front();
      omegaClassID[current] = omegaCurrentClassID;
      omegaRepresentantList.push_back(current);
      //identify equivalent elements
      auto iter = toCheck.begin();
      while (iter != toCheck.end()) {
        if (this->sOmegaEquivalent(current, *iter)) {
          omegaClassID[*iter] = omegaCurrentClassID;
          iter = toCheck.erase(iter);
        }
        else {
          ++iter;
        }
      }
      //increase class ID
      ++omegaCurrentClassID;
    }

    //turn representants into vectors
    auto representants = std::vector<size_t>(representantList.begin(), representantList.end());
    auto omegaRepresentants = std::vector<size_t>(omegaRepresentantList.begin(), omegaRepresentantList.end());
    size_t pSizeReduced = representants.size();
    size_t oSizeReduced = representants.size();

    //Build new name vectors
    auto sPlusNamesReduced = std::vector<std::string>(pSizeReduced);
    auto sOmegaNamesReduced = std::vector<std::string>(oSizeReduced);
    for (size_t pClass = 0; pClass < pSizeReduced; ++pClass) {
      sPlusNamesReduced[pClass] = "[" + this->sPlus.elementName(representants[pClass]) + "]";
    }
    for (size_t oClass = 0; oClass < oSizeReduced; ++oClass) {
      sOmegaNamesReduced[oClass] = "[" + this->omegaElementNames[omegaRepresentants[oClass]] + "]";
    }

    //Build new finite product table
    auto productTableReduced = std::vector<std::vector<size_t> >(pSizeReduced, std::vector<size_t>(pSizeReduced, 0));
    for (size_t pClass = 0; pClass < pSizeReduced; ++pClass) {
      for (size_t pClass2 = 0; pClass2 < pSizeReduced; ++pClass2) {
        size_t pRep = representants[pClass];
        size_t pRep2 = representants[pClass2];
        productTableReduced[pClass][pClass2] = classID[this->sPlus.product(pRep, pRep2)];
      }
    }

    //Build new mixed product table
    auto mixedTableReduced = std::vector<std::vector<size_t> >(pSizeReduced, std::vector<size_t>(oSizeReduced, 0));
    for (size_t pClass = 0; pClass < pSizeReduced; ++pClass) {
      for (size_t oClass = 0; oClass < oSizeReduced; ++oClass) {
        size_t pRep = representants[pClass];
        size_t oRep = omegaRepresentants[oClass];
        mixedTableReduced[pClass][oClass] = omegaClassID[this->mixedProductTable[pRep][oRep]];
      }
    }

    //Build new omega iteration table
    auto omegaTableReduced = std::vector<size_t>(pSizeReduced, 0);
    for (size_t pClass = 0; pClass < pSizeReduced; ++pClass) {
      size_t pRep = representants[pClass];
      omegaTableReduced[pClass] = omegaClassID[this->omegaIterationTable[pRep]];
    }

    //Build new morphism
    auto imagesReduced = std::vector<size_t>(this->phi.size(), 0);
    for (size_t letter = 0; letter < this->phi.size(); ++letter) {
      imagesReduced[letter] = classID[this->phi[letter]];
    }
    auto phiReduced = Morphism(imagesReduced, this->phi.getAlphabet());

    //Build new P
    auto PReduced = std::vector<bool>(oSizeReduced, false);
    for (size_t oClass = 0; oClass < oSizeReduced; ++oClass) {
      PReduced[oClass] = this->P[omegaRepresentants[oClass]];
    }

    //Update all members
    this->sPlus = Semigroup(sPlusNamesReduced, productTableReduced);
    this->omegaElementNames = sOmegaNamesReduced;
    this->mixedProductTable = mixedTableReduced;
    this->omegaIterationTable = omegaTableReduced;
    this->phi = phiReduced;
    this->P = PReduced;
  }

  bool OmegaSemigroup::sPlusEquivalent(size_t lhs, size_t rhs) const {
    size_t sPlusSize = this->sPlus.size();
    //Check first condition
    for (size_t s = 0; s < sPlusSize; ++s) {
      for (size_t t = 0; t < sPlusSize; ++t) {
        for (size_t u = 0; u < sPlusSize; ++u) {
          //Compare s * lhs * t * u^w and s * rhs * t * u^w
          size_t leftProd = this->mixedProductTable[this->sPlus.product(s, this->sPlus.product(lhs, t))][this->omegaIterationTable[u]];
          size_t rightProd = this->mixedProductTable[this->sPlus.product(s, this->sPlus.product(rhs, t))][this->omegaIterationTable[u]];
          if (this->P[leftProd] != this->P[rightProd]) {
            return false;
          }
        }
      }
    }
    //Check second condition
    for (size_t s = 0; s < sPlusSize; ++s) {
      for (size_t t = 0; t < sPlusSize; ++t) {
        //Compare s(lhs * t)^w and s(rhs * t)^w
        size_t leftProd = this->mixedProductTable[s][this->omegaIterationTable[this->sPlus.product(lhs, t)]];
        size_t rightProd = this->mixedProductTable[s][this->omegaIterationTable[this->sPlus.product(rhs, t)]];
        if (this->P[leftProd] != this->P[rightProd]) {
          return false;
        }
      }
    }
    return true;
  }

  bool OmegaSemigroup::sOmegaEquivalent(size_t lhs, size_t rhs) const {
    for (size_t s = 0; s < this->sPlus.size(); ++s) {
      //Compare s * lhs and s * rhs
      size_t leftProd = this->mixedProductTable[s][lhs];
      size_t rightProd = this->mixedProductTable[s][rhs];
      if (this->P[leftProd] != this->P[rightProd]) {
        return false;
      }
    }
    return true;
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
