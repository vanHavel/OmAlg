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

