#ifndef OMALG_OMEGA_AUTOMATON
#define OMALG_OMEGA_AUTOMATON

#include <vector>
#include <string>

#include "OmegaSemigroup.h"
#include "TransitionProfile.h"
#include "TransformToOmegaSemigroup.tcc"

namespace omalg {

  class OmegaAutomaton {
      
  private:
    std::vector<std::string> stateNames;
    
    std::vector<std::string> alphabet;
    
    int initialState;
    
  protected:
    OmegaAutomaton(std::vector<std::string> theStateNames,
                   std::vector<std::string> theAlphabet,
                   size_t theInitialState);
    std::vector<std::string> getStateNames() const;

  public:
    /**
     * Returns number of states.
     * @return The number of states.
     */
    size_t numberOfStates() const;
    /**
     * Returns size of alphabet.
     * @return The alphabet size.
     */
    size_t alphabetSize() const;
    /**
     * Returns index of the initial state.
     * @return the initial state.
     */
    size_t getInitialState() const;
    /**
     * Returns the alphabet of the automaton.
     * @return the alphabet.
     */
    std::vector<std::string> getAlphabet() const;
    /**
     * Returns a textual description of the automaton in the OmAlg format
     * @return A string containing the description.
     */
    virtual std::string description() const;
    /**
     * Constructs an equivalent omega semigroup for the automaton.
     * @return The omega semigroup.
     */
    virtual OmegaSemigroup* toOmegaSemigroup() const = 0;
    /**
     * Returns transition profile associated with given letter.
     * @param letter The letter index.
     * @return The associated transition profile.
     */
    virtual ~OmegaAutomaton() {};
  };
}

#endif
