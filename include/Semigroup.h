#ifndef OMALG_SEMIGROUP
#define OMALG_SEMIGROUP

#include <vector>
#include <string>

#include "UnionFind.h"

namespace omalg {

  class Semigroup {
  private:
    int numberOfElements;
    std::vector<std::string> elementNames;
    std::vector<std::vector<int> > multiplicationTable;
    UnionFind* rClasses;
    UnionFind* lClasses;
    UnionFind* jClasses;
    UnionFind* hClasses;
    std::vector<std::vector<bool> >* jOrder;
  public:
    void calculateGreenRelations();
    void calculateJOrder();
  };

}

#endif