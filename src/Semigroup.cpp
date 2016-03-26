#include "Semigroup.h"

namespace omalg {

  Semigroup::Semigroup(std::vector<std::string> theElementNames, std::vector<std::vector<size_t> > theMultiplicationTable)
    : elementNames(theElementNames), multiplicationTable(theMultiplicationTable) { }

  size_t Semigroup::product(size_t lhs, size_t rhs) const {
    return this->multiplicationTable[lhs][rhs];
  }
  
  std::string Semigroup::elementName(size_t index) const {
    return this->elementNames.at(index);
  }
  
  void Semigroup::calculateGreenRelations() const {
    //initialize
    this->jOrder = new std::vector<std::vector<bool> >(this->elementNames.size(), std::vector<bool>(this->elementNames.size(), true));
    this->rOrder = new std::vector<std::vector<bool> >(this->elementNames.size(), std::vector<bool>(this->elementNames.size(), true));
    this->lOrder = new std::vector<std::vector<bool> >(this->elementNames.size(), std::vector<bool>(this->elementNames.size(), true));

    //calculate rOrder and lOrder
    for (size_t i = 0; i < this->elementNames.size(); ++i) {
      for (size_t j = 0; j < this->elementNames.size(); ++j) {
        //rOrder
        if (std::find(this->multiplicationTable[j].begin(), this->multiplicationTable[j].end(), i)
            != this->multiplicationTable[j].end()) {
          (*(this->rOrder))[i][j] = 1;
        }
        //lOrder
        for (size_t k = 0; k < this->elementNames.size(); ++k) {
          if (this->multiplicationTable[k][j] == i) {
            (*(this->lOrder))[i][j] = 1;
            break;
          }
        }
      }
    }
    //calculate jOrder
    for (size_t i = 0; i < this->elementNames.size(); ++i) {
      for (size_t j = 0; j < this->elementNames.size(); ++j) {
        for (size_t k = 0; k < this->elementNames.size(); ++k) {
          if ((*(this->lOrder))[k][j] && (*(this->rOrder))[i][k]) {
            (*(this->jOrder))[i][j] = 1;
            break;
          }
        }
      }
    }
  }

  inline bool Semigroup::J(size_t lhs, size_t rhs) const {
    return (this->j(lhs, rhs) && this->j(rhs, lhs));
  }

  inline bool Semigroup::R(size_t lhs, size_t rhs) const {
    return (this->r(lhs, rhs) && this->r(rhs, lhs));
  }

  inline bool Semigroup::L(size_t lhs, size_t rhs) const {
    return (this->l(lhs, rhs) && this->l(rhs, lhs));
  }

  inline bool Semigroup::H(size_t lhs, size_t rhs) const {
    return (this->R(lhs, rhs) && this->L(lhs, rhs));
  }

  bool Semigroup::j(size_t lhs, size_t rhs) const {
    if (this->jOrder) {
      return (*(this->jOrder))[lhs][rhs];
    }
    else {
      for (size_t iter = 0; iter < this->elementNames.size(); ++iter) {
        if (this->l(iter, rhs) && this->r(lhs, iter)) {
          return true;
        }
      }
      return false;
    }
  }

  bool Semigroup::r(size_t lhs, size_t rhs) const {
    if (this->rOrder) {
      return (*(this->rOrder))[lhs][rhs];
    }
    else {
      return (std::find(this->multiplicationTable[rhs].begin(), this->multiplicationTable[rhs].end(), lhs)
              != this->multiplicationTable[rhs].end());
    }
  }

  bool Semigroup::l(size_t lhs, size_t rhs) const {
    if (this->lOrder) {
      return (*(this->lOrder))[lhs][rhs];
    }
    else {
      for (size_t iter = 0; iter < this->elementNames.size(); ++iter) {
        if (this->multiplicationTable[iter][rhs] == lhs) {
          return true;
        }
      }
      return false;
    }
  }

  inline bool Semigroup::h(size_t lhs, size_t rhs) const {
    return (this->r(lhs, rhs) && this->l(lhs, rhs));
  }

  std::list<size_t> Semigroup::idempotents() const {
    if (this->_idempotents.empty()) {
      for (size_t iter = 0; iter < this->elementNames.size(); ++iter) {
        if (this->multiplicationTable[iter][iter] == iter) {
          this->_idempotents.insert(this->_idempotents.end(), iter);
        }
      }
    }
    return this->_idempotents;
  }

  std::list<std::pair<size_t, size_t> > Semigroup::linkedPairs() const {
    if (this->_linkedPairs.empty()) {
      auto idem = this->idempotents();
      for (size_t s = 0; s < this->elementNames.size(); ++s) {
        for (auto iter = idem.begin(); iter != idem.end(); ++iter) {
          if (this->multiplicationTable[s][*iter] == s) {
            this->_linkedPairs.insert(this->_linkedPairs.end(), std::make_pair(s, *iter));
          }
        }
      }
    }
    return this->_linkedPairs;
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

  Semigroup::~Semigroup() {
    if (this->jOrder) {
      delete jOrder;
    }
    if (this->lOrder) {
      delete lOrder;
    }
    if (this->rOrder) {
      delete rOrder;
    }
  }
}

