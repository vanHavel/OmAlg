#ifndef OMALG_SEMIGROUP
#define OMALG_SEMIGROUP

#include <vector>
#include <string>
#include <list>

namespace omalg {

  class Semigroup {
  private:
    std::vector<std::string> elementNames;
    std::vector<std::vector<size_t> > multiplicationTable;

    //Allow to store pre-computed green relations to increase efficiency in computations.
    //This is achieved by calling calculateGreenRelations.
    mutable std::vector<std::vector<bool> >* jOrder = 0;
    mutable std::vector<std::vector<bool> >* rOrder = 0;
    mutable std::vector<std::vector<bool> >* lOrder = 0;

    //Allow to store pre-computed j depths to increase efficiency in computations.
    //This is achieved by calling calculateJDepths.
    mutable std::vector<size_t>* jDepths = 0;

    //Lists of idempotents and linked pairs are cached once calculated.
    //These can not be empty once calculated as every finite semigroup contains an idempotent.
    mutable std::list<std::pair<size_t, size_t> > _linkedPairs = std::list<std::pair<size_t, size_t> >();
    mutable std::list<size_t> _idempotents = std::list<size_t>();

  public:
    /**
     * Semigroup constructor.
     * @param elementNames Names of the semigroup elements.
     * @param multiplicationTable Semigroup product table.
     */
    Semigroup(std::vector<std::string> theElementNames, std::vector<std::vector<size_t> > theMultiplicationTable);

    /**
     * Copy constructor
     * @param S Semigroup to be copied
     */
    Semigroup(Semigroup const& S);

    /**
     * Copy assignment operator
     * @param S Semigroup to be copied
     * @return A copy of the semigroup
     */
    Semigroup& operator=(Semigroup const& S);

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
     * Getter for element names vector.
     * @return the vector of element names.
     */
    std::vector<std::string> getElementNames() const;

    /**
     * Calculates the R, L, J and orders.
     */
    void calculateGreenRelations() const;

    /**
     * Calculates only the R order.
     */
    void calculateROrder() const;

    /**
     * Calculates the J depth function of the semigroup.
     */
    void calculateJDepths() const;

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
     * Returns the J depth of a given element.
     * @param index index of the element.
     * @return J depth of the element.
     */
    size_t jDepth(size_t index) const;

    /**
     * Returns all linked pairs of the semigroup. A linked pair is a tuple (s,e) such that se = s and ee = e.
     * The list is sorted by increasing element id of s. Where s is identical, it is sorted by increasing element id of e.
     * @return A list of all linked pairs.
     */
    std::list<std::pair<size_t, size_t> > linkedPairs() const;

    /**
     * Returns a list of all idempotents in the semigroup.
     * The list is sorted by increasing element id.
     * @return A list of all idempotents.
     */
    std::list<size_t> idempotents() const;


    /**
     * Returns textual description of semigroup in OmAlg format.
     * @return The description as a string.
     */
    std::string description() const;

    /**
     * Destructor, removing matrices for green orders
     */
    ~Semigroup();
  };

}

#endif
