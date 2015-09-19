#include "dasDull_Util.h"

namespace dasdull {

  std::list<std::string> stringSplit(std::string str, char split, bool strip) {
    std::list<std::string> result;
    //strip spaces
    if(strip) {
      std::string::iterator lastPos = std::remove(str.begin(), str.end(), ' ');
      str.erase(lastPos, str.end());
    }
    std::string::iterator segmentBegin = str.begin();
    std::string::iterator segmentEnd;
    while ((segmentEnd = std::find(segmentBegin, str.end(), split)) != str.end()) {
      result.push_back(std::string(segmentBegin, segmentEnd));
      segmentBegin = ++segmentEnd;
    }
    if (segmentBegin != str.end()) {
      result.push_back(std::string(segmentBegin, str.end()));
    }
    return result;
  }

}
