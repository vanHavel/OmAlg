#include <exception>

#include "Morphism.h"

namespace omalg {

  Morphism::Morphism(std::vector<size_t> theValues, std::vector<std::string> theAlphabet)
    : values(theValues), alphabet(theAlphabet) {}

  std::vector<std::string> Morphism::getAlphabet() const {
    return this->alphabet;
  }
  
  size_t Morphism::size() const {
    return this->values.size();
  }

  size_t Morphism::apply(size_t letter) const {
    return this->values.at(letter);
  }

  size_t Morphism::operator[](size_t letter) const {
    return this->values[letter];
  }
  
  std::string Morphism::letterName(size_t letter) const {
    return this->alphabet.at(letter);
  }

}
