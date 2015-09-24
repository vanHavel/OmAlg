#include <exception>

#include "Morphism.h"

namespace omalg {

  Morphism::Morphism(std::vector<size_t> theValues, std::vector<std::string> theAlphabet)
    : values(theValues), alphabet(theAlphabet) {}

  size_t Morphism::apply(size_t letter) const {
    if (letter <= this->values.size()) {
      return this->values[letter];
    }
    else {
      throw std::out_of_range("Index " + std::to_string(letter) + " out of range for morphism(size " + std::to_string(this->values.size()) + ").");
    }
  }

  size_t Morphism::operator[](size_t letter) const {
    return this->values[letter];
  }

}
