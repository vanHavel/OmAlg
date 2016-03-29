# OmAlg
Toolkit for the algebraic theory of omega-automata.

# Installation 
Build using CMake. Version 3.4 is required.

# Implemented features
* transform automata into equivalent omega semigroups (Büchi's construction). Supported types: 
    * Deterministic:    Buechi, coBuechi, parity, Muller
    * Nondeterministic: Buechi, coBuechi
* decide a language L's position in the Landweber hierarchy given an omega-semigroup for L
* transform omega-semigroups into equivalent automata (if applicable). Supported types:
    * weak Buechi automata

# Currently working on
- omega-semigroups to deterministic Büchi and co-Büchi automata
- omega-semigroups to parity automata

# Planned features
- user manual
