#ifndef OMALG_SEMIGROUP
#define OMALG_SEMIGROUP

#include <vector>
#include <string>

#include "UnionFind.h"

namespace omalg {

  class Semigroup {
  private:
    std::vector<std::string> elementNames;
    std::vector<std::vector<size_t> > multiplicationTable;
    UnionFind* rClasses;
    UnionFind* lClasses;
    UnionFind* jClasses;
    UnionFind* hClasses;
    std::vector<std::vector<bool> >* jOrder;
  public:
    /**
     * Semigroup constructor.
     * @param elementNames Names of the semigroup elements.
     * @param multiplicationTable Semigroup product table.
     */
    Semigroup(std::vector<std::string> theElementNames, std::vector<std::vector<size_t> > theMultiplicationTable);
    /**
     * Calculates the semigroup product, given two element indices(No check of bounds).
     * @param lhs Left factor.
     * @param rhs Right factor.
     * @return The index of the product element.
     */
    size_t product(const size_t lhs, const size_t rhs) const;
    /**
     * Calculates the R, L, J and H-classes.
     */
    void calculateGreenRelations() const;
    /**
     * Calculates the complete <=_J-order.
     */
    void calculateJOrder() const;
  };

}

#endif
