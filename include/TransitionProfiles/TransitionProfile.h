#ifndef OMALG_TRANSITIONPROFILE
#define	OMALG_TRANSITIONPROFILE

#include <vector>

namespace omalg {
  
  /**
   * Class used as S_Omega element in the transition profile construction.
   */
  class OmegaProfile { 
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
