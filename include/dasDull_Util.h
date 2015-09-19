#ifndef DASDULL_UTIL
#define DASDULL_UTIL

#include <list>
#include <vector>
#include <string>

#include "dasDull_Util.tcc"

namespace dasdull {
  std::list<std::string> stringSplit(std::string str, char split, bool strip);
  template<class T> int vectorPos(std::vector<T> const &vec, T const &val);
}

#endif
