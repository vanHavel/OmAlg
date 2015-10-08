#ifndef VANHAVEL_UNION_FIND
#define	VANHAVEL_UNION_FIND

namespace vanHavel {
  
  class UnionFind {
  private:
    std::vector<size_t> parents;
    std::vector<size_t> ranks;
  public:
    UnionFind(size_t size);
    size_t find(size_t member);
    void unite(size_t member1, size_t member2);
  };

}

#endif

