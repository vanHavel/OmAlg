#ifndef OMALG_TRANSITIONPROFILE
#define	OMALG_TRANSITIONPROFILE

#include <vector>

namespace omalg {
  
  //Forward declaration.
  struct OmegaProfileHash;
  
  /**
   * Class used as S_Omega element in the transition profile construction.
   */
  class OmegaProfile { 
  friend OmegaProfileHash;
  private:
    std::vector<bool> values; 
  public:
    /**
     * Constructor with given values.
     * @param theValues Binary vector of values.
     */
    OmegaProfile(std::vector<bool> theValues)
      : values(theValues) { }
    /**
     * Checked member access.
     * @param index Index to access.
     * @return Element at index.
     * @throw out_of_range if out of range.
     */
    bool getValue(size_t index) const{
      return this->values.at(index);
    }
    /**
     * Unchecked member access.
     * @param index Index to access.
     * @return Element at index.
     */
    bool operator[](size_t index) const{
      return this->values[index];
    }
    /**
     * Comparison operator.
     * @param lhs The left hand side.
     * @param rhs the right hand side.
     * @return true iff the two profiles are equal.
     */
    friend bool operator==(OmegaProfile const& lhs, OmegaProfile const& rhs);
  };
  
  inline bool operator==(OmegaProfile const& lhs, OmegaProfile const& rhs) {
    return lhs.values == rhs.values;
  }
  
  struct OmegaProfileHash {
    size_t operator()(OmegaProfile const& profile) const {
      return std::hash<std::vector<bool> >()(profile.values);
    }
  };

  /**
   * General transition profile class. Specialized versions exist for 
   * non abstract automata classes.
   */
  template<class T> class TransitionProfile {
  public:
    TransitionProfile concat(TransitionProfile const& rhs) const;
    OmegaProfile mixedProduct(OmegaProfile const& rhs) const;
    OmegaProfile omegaIteration() const;
  };

}
#endif	
