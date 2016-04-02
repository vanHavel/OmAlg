#include <queue>

#include "Semigroup.h"

namespace omalg {

  Semigroup::Semigroup(std::vector<std::string> theElementNames, std::vector<std::vector<size_t> > theMultiplicationTable)
    : elementNames(theElementNames), multiplicationTable(theMultiplicationTable) { }

  Semigroup::Semigroup(Semigroup const& S)
    : elementNames(S.elementNames), multiplicationTable(S.multiplicationTable) {
    if (S.rOrder) {
      rOrder = new std::vector<std::vector<bool> >();
      *rOrder = *S.rOrder;
    }
    else {
      rOrder = 0;
    }
    if (S.lOrder) {
      lOrder = new std::vector<std::vector<bool> >();
      *lOrder = *S.lOrder;
    }
    else {
      lOrder = 0;
    }
    if (S.jOrder) {
      jOrder = new std::vector<std::vector<bool> >();
      *jOrder = *S.jOrder;
    }
    else {
      jOrder = 0;
    }
    if (S.jDepths) {
      jDepths = new std::vector<size_t>();
      *jDepths = *S.jDepths;
    }
    else {
      jDepths = 0;
    }
  }

  Semigroup& Semigroup::operator=(Semigroup const& S) {
    if (&S != this) {
      this->elementNames = S.elementNames;
      this->multiplicationTable = S.multiplicationTable;
      if (S.rOrder) {
        this->rOrder = new std::vector<std::vector<bool> >();
        *(this->rOrder) = *S.rOrder;
      }
      else {
        this->rOrder = 0;
      }
      if (S.lOrder) {
        this->lOrder = new std::vector<std::vector<bool> >();
        *(this->lOrder) = *S.lOrder;
      }
      else {
        this->lOrder = 0;
      }
      if (S.jOrder) {
        this->jOrder = new std::vector<std::vector<bool> >();
        *(this->jOrder) = *S.jOrder;
      }
      else {
        this->jOrder = 0;
      }
      if (S.jDepths) {
        this->jDepths = new std::vector<size_t>();
        *(this->jDepths) = *S.jDepths;
      }
      else {
        this->jDepths = 0;
      }
    }
    return *this;

  }

  size_t Semigroup::product(size_t lhs, size_t rhs) const {
    return this->multiplicationTable[lhs][rhs];
  }

  size_t Semigroup::size() const {
    return this->elementNames.size();
  }
  
  std::string Semigroup::elementName(size_t index) const {
    return this->elementNames.at(index);
  }

  std::vector<std::string> Semigroup::getElementNames() const {
    return this->elementNames;
  }
  
  void Semigroup::calculateGreenRelations() const {
    //The runtime is (O(n^3)).
    //calculate r order
    if (!this->rOrder) {
      this->calculateROrder();
    }
    if (!this->lOrder) {
      //initialize l order
      this->lOrder = new std::vector<std::vector<bool> >(this->elementNames.size(), std::vector<bool>(this->elementNames.size(), false));

      //calculate l order
      for (size_t i = 0; i < this->elementNames.size(); ++i) {
        for (size_t j = 0; j < this->elementNames.size(); ++j) {
          for (size_t k = 0; k < this->elementNames.size(); ++k) {
            if (this->multiplicationTable[k][j] == i) {
              (*(this->lOrder))[i][j] = true;
              break;
            }
          }
        }
      }
    }
    if(!this->jOrder) {
      //initialize j order
      this->jOrder = new std::vector<std::vector<bool> >(this->elementNames.size(), std::vector<bool>(this->elementNames.size(), false));

      //calculate j order
      for (size_t i = 0; i < this->elementNames.size(); ++i) {
        for (size_t j = 0; j < this->elementNames.size(); ++j) {
          for (size_t k = 0; k < this->elementNames.size(); ++k) {
            if ((*(this->lOrder))[k][j] && (*(this->rOrder))[i][k]) {
              (*(this->jOrder))[i][j] = true;
              break;
            }
          }
        }
      }
    }
  }

  void Semigroup::calculateROrder() const {
    if (!this->rOrder) {
      //initialize
      this->rOrder = new std::vector<std::vector<bool> >(this->elementNames.size(), std::vector<bool>(this->elementNames.size(), false));
      //calculate rOrder
      for (size_t i = 0; i < this->elementNames.size(); ++i) {
        for (size_t j = 0; j < this->elementNames.size(); ++j) {
          if (std::find(this->multiplicationTable[j].begin(), this->multiplicationTable[j].end(), i)
          != this->multiplicationTable[j].end()) {
            (*(this->rOrder))[i][j] = true;
          }
        }
      }
    }
  }

  void Semigroup::calculateJDepths() const {
    //Calculate jOrder first
    if (!this->jOrder) {
      this->calculateGreenRelations();
    }
    size_t size = this->elementNames.size();
    //Allocate memory
    this->jDepths = new std::vector<size_t>(size, 0);

    //calculate strict j order
    auto strictJOrder = std::vector<std::vector<bool>>(size, std::vector<bool>(size, false));
    for (size_t i = 0; i < size; ++i) {
      for (size_t k = 0; k < size; ++k) {
        strictJOrder[i][k] = this->j(i,k) && !this->j(k,i);
      }
    }

    //similar to sat checking for horn formulas, we store for each index i the number count[i]
    //of stictly higher j elements, and the indices lower[i] for which i is a strictly higher j
    //element. If a depth is assigned to i, the count for all indices in lower[i] is decreased.
    //If it hits 0, a new j depth can be assigned. This is O(n^2) if green relations are calculated.
    auto count = std::vector<size_t>(size, 0);
    auto lower = std::vector<std::list<size_t> >(size, std::list<size_t>());
    auto higher = std::vector<std::list<size_t> >(size, std::list<size_t>());
    auto Q = std::queue<size_t>();

    //Determine count and lower sets and higher sets
    for (size_t i = 0; i < size; ++i) {
      for (size_t k = 0; k < size; ++k) {
        if (strictJOrder[i][k]) {
          count[i]++;
          lower[k].push_back(i);
          higher[i].push_back(k);
        }
      }
      //push those elements with count 0 to Q
      if (count[i] == 0) {
        Q.push(i);
      }
    }

    //Iterate over elements in Q
    while (!Q.empty()) {
      size_t cur = Q.front();
      Q.pop();
      //assign j depth to maximum depth of higher elements + 1
      for (auto iter = higher[cur].begin(); iter != higher[cur].end(); ++iter) {
        if ((*(this->jDepths))[cur] == 0 || (*(this->jDepths))[cur] < (*(this->jDepths))[*iter] + 1) {
          (*(this->jDepths))[cur] = (*(this->jDepths))[*iter] + 1;
        }
      }
      //Special case: depth 1. There are no higher elements.
      if((*(this->jDepths))[cur] == 0) {
        (*(this->jDepths))[cur] = 1;
      }
      //Decrease count for all lower elements.
      for (auto iter = lower[cur].begin(); iter != lower[cur].end(); ++iter) {
        count[*iter]--;
        //push to Q
        if (count[*iter] == 0) {
          Q.push(*iter);
        }
      }
    }

  }

  bool Semigroup::J(size_t lhs, size_t rhs) const {
    return (this->j(lhs, rhs) && this->j(rhs, lhs));
  }

  bool Semigroup::R(size_t lhs, size_t rhs) const {
    return (this->r(lhs, rhs) && this->r(rhs, lhs));
  }

  bool Semigroup::L(size_t lhs, size_t rhs) const {
    return (this->l(lhs, rhs) && this->l(rhs, lhs));
  }

  bool Semigroup::H(size_t lhs, size_t rhs) const {
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

  size_t Semigroup::jDepth(size_t index) const {
    if (!this->jDepths) {
      this->calculateJDepths();
    }
    return (*(this->jDepths))[index];
  }

  std::list<size_t> Semigroup::idempotents() const {
    if (this->_idempotents.empty()) {
      for (size_t e = 0; e < this->elementNames.size(); ++e) {
        if (this->multiplicationTable[e][e] == e) {
          this->_idempotents.insert(this->_idempotents.end(), e);
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

