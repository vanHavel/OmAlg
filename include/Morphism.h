#ifndef OMALG_MORPHISM
#define OMALG_MORPHISM

#include <string>
#include <vector>

namespace omalg {

  class Morphism {
  private:
    std::vector<size_t> values;
    std::vector<std::string> alphabet;
  public:
    /**
     * Constructor for morphism.
     * @param theValues Indices of the semigroup images.
     * @param theAlphabet Alphabet(Domain) of the morphism.
     */
    Morphism(std::vector<size_t> theValues, std::vector<std::string> theAlphabet);

    /**
     * Getter for alphabet.
     * @return the alphabet.
     */
    std::vector<std::string> getAlphabet() const;
    /**
     * Returns the size of the alphabet - the morphism's domain.
     * @return alphabet size.
     */
    size_t size() const;
    /**
     * Safe application of morphism to letter.
     * @param letter ID of the letter in the used alphabet.
     * @return ID of the image in the semigroup.
     * @throw out_of_range if letter index is too large.
     */
    size_t apply(size_t letter) const;
    /**
     * Unsafe application of morphism to letter(No check of bounds).
     * @param letter ID of the letter in the used alphabet.
     * @return ID of the image in the semigroup.
     */
    size_t operator[](size_t letter) const;
    /**
     * Returns name of letter with given index. Bounds are checked.
     * @param letter ID of the letter in the used alphabet.
     * @return the name of the letter.
     * @throw out_of_range if out of range.
     */   
    std::string letterName(size_t letter) const;
  };

}

#endif
