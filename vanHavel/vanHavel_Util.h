#ifndef VANHAVEL_UTIL
#define VANHAVEL_UTIL

#include <list>
#include <vector>
#include <string>

namespace dasdull {
  
  std::list<std::string> stringSplit(std::string str, char split, bool strip);
  template<typename T> int vectorPos(std::vector<T> const &vec, T const &val);
  
}

#include "vanHavel_Util.tcc"

#endif
