#include <vector>
#include <algorithm>

namespace dasdull {
  template<typename T> int vectorPos(std::vector<T> const &vec, T const &val) {
    typename std::vector<T>::const_iterator iter = std::find(vec.begin(), vec.end(), val);
    if (iter == vec.end()) {
      return -1;
    }
    else {
      return (iter - vec.begin());
    }
  }
}
