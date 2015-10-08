#include "UnionFind.h"

namespace vanHavel {
  
  UnionFind::UnionFind(size_t size)
    : parents(size), ranks(size, 0) {
    for (size_t i = 0; i < this->parents.size(); ++i) {
      this->parents[i] = i;
    }
  }
  
  size_t UnionFind::find(size_t member) {
    size_t current = member;
    while (this->parents[current] != current) {
      current = this->parents[current];
    }
    size_t root = current;
    //Path compression.
    current = member;
    while (this->parents[current] != current) {
      size_t temp = this->parents[current];
      this->parents[current] = root;
      current = temp;
    }
    return root;
  }
  
  void UnionFind::unite(size_t member1, size_t member2) {
    size_t root1 = this->find(member1);
    size_t root2 = this->find(member2);
    if (root1 != root2) {
      //Rank update.
      if (this->ranks[root1] == this->ranks[root2]) {
        this->parents[root1] = root2;
        ++(this->ranks[root2]);
      }
      else if (this->ranks[root1] > this->ranks[root2]) {
        this->parents[root2] = root1;
      }
      else {
        this->parents[root1] = root2;
      }
    }
  }
  
}