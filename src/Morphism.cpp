#include <exception>

#include "Morphism.h"

namespace omalg {

  Morphism::Morphism(std::vector<size_t> theValues, std::vector<std::string> theAlphabet)
    : values(theValues), alphabet(theAlphabet) {}

  size_t Morphism::apply(size_t letter) const {
    return this->values.at(letter);
  }

  inline size_t Morphism::operator[](size_t letter) const {
    return this->values[letter];
  }

}
