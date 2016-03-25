#ifndef OMALG_SEMIGROUP
#define OMALG_SEMIGROUP

#include <vector>
#include <string>

namespace omalg {

  class Semigroup {
  private:
    std::vector<std::string> elementNames;
    std::vector<std::vector<size_t> > multiplicationTable;

    std::vector<std::vector<bool> >* jOrder;
    std::vector<std::vector<bool> >* rOrder;
    std::vector<std::vector<bool> >* lOrder;
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
     * Calculates the R, L, J and orders.
     */
    void calculateGreenRelations() const;

    /**
     * Functions for Green equivalence relations. Faster if calculateGreenRelations was executed beforehand.
     */
    bool J(size_t lhs, size_t rhs) const;
    bool R(size_t lhs, size_t rhs) const;
    bool L(size_t lhs, size_t rhs) const;
    bool H(size_t lhs, size_t rhs) const;

    /**
     * Functions for Green order relations. Faster if calculateGreenRelations was executed beforehand.
     */
    bool j(size_t lhs, size_t rhs) const;
    bool r(size_t lhs, size_t rhs) const;
    bool l(size_t lhs, size_t rhs) const;
    bool h(size_t lhs, size_t rhs) const;

    /**
     * Returns textual description of semigroup in OmAlg format.
     * @return The description as a string.
     */
    std::string description() const;
  };

}

#endif
