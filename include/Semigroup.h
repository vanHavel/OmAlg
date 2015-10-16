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
    vanHavel::UnionFind* rClasses;
    vanHavel::UnionFind* lClasses;
    vanHavel::UnionFind* jClasses;
    vanHavel::UnionFind* hClasses;
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
    size_t product(size_t lhs, size_t rhs) const;
    /**
     * Returns name of element at index. Bounds are checked.
     * @param index index of the element to consider.
     * @return The elements name.
     */
    std::string elementName(size_t index) const;
    /**
     * Calculates the R, L, J and H-classes.
     */
    void calculateGreenRelations() const;
    /**
     * Calculates the complete <=_J-order.
     */
    void calculateJOrder() const;
    /**
     * Returns textual description of semigroup in OmAlg format.
     * @return The description as a string.
     */
    std::string description() const;
  };

}

#endif
