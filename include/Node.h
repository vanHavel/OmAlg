#ifndef OMALG_NODE
#define	OMALG_NODE

#include <vector>
#include <exception>

namespace omalg {
  
  /**
   * Class for graphs with unique "root" and a finite number n of successors. 
   * The nodes store a value of type T, a pointer to their parent and a
   * unique node index.
   * The successor pointers are stored together with a boolean value which 
   * stores whether the associated successor node was "discovered" on this
   * path from the root. Used to store Cayley graphs of semigroups.
   */
  template<typename T> class Node {
  private:
    std::vector<std::pair<Node*, bool> > successors;
    T value;
    Node* parent;
    size_t index;
  public:
    /**
     * Constructor for node with given value. Successors are initialized with
     * 0 and have to be set later.
     * @param theValue Value of the node.
     * @param numberOfSuccessors The number of successors of the node.
     */
    Node(T theValue, size_t numberOfSuccessors, Node* theParent, size_t theIndex)
      : successors(numberOfSuccessors), value(theValue), parent(theParent), index(theIndex) {}
    /**
     * Compares a variable of type T with the value stored by the node.
     * @return true iff the two values are equal.
     */
    bool equalToValue(T const& candidate) const {
      return this->value == candidate;
    }
    /**
     * Get the stored value.
     * @return The value.
     */
    T getValue() const {
      return this->value;
    }
    /**
     * Returns number of successors of a node.
     * @return the number of successors.
     */
    size_t numberOfSuccessors() const {
      return this->successors.size();
    }
    /**
     * Returns successor of the node given its index. Bounds are checked.
     * @param index The successor index.
     * @return The successor at the given index.
     * @throw out_of_range if index out of range.
     */
    std::pair<Node*, bool> successor(size_t index) {
      return this->successors.at(index);
    }
    /**
     * Returns next successor labeled true after(including) given index.
     * @param index the index from which on to search.
     * @return index of the next true successor, or -1 if such a successor does not exist.
     */
    int nextTrueSucessor(size_t index) {
      for (size_t i = index; i < this->successors.size(); ++i) {
        if (this->successors[i].second) {
          return i;
        }
      }
      return -1;
    }
    /**
     * Returns successor of the node given its index. Bounds are  not checked.
     * @param index The successor index.
     * @return The successor at the given index.
     */
    std::pair<Node*, bool> operator[](size_t index) {
      return this->successors[index];
    }
    /**
     * Returns a node's parent.
     * @return the parent node.
     */
    Node* getParent() {
      return this->parent;
    }
    /**
     * Returns a node's index.
     * @return the node index.
     */
    size_t getIndex() {
      return this->index;
    }
    /**
     * Sets the successor at given index. Bounds are checked.
     * @param index Index of the successor.
     * @param successor The new successor to be installed.
     * @param novel Specifies whether the successor is added to the graph for the first time.
     * @throw out_of_range if out of range.
     */
    void setSuccessorChecked(size_t index, Node* successor, bool novel) {
      if (index < this->successors.size()) {
        this->successors[index] = std::make_pair(successor, novel);
      }
      else {
        throw std::out_of_range("Node: Successor index out of range.");
      }
    }
    /**
     * Sets the successor at given index. Bounds are not checked.
     * @param index Index of the successor.
     * @param successor The new successor to be installed.
     * @param novel Specifies whether the successor is added to the graph for the first time.
     */
    void setSuccessor(size_t index, Node* successor, bool novel) {
      this->successors[index] = std::make_pair(successor, novel);
    }
  };

}
#endif

