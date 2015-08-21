#ifndef OMALG_OMEGA_SEMIGROUP
#define OMALG_OMEGA_SEMIGROUP

#include <vector>
#include <string>

#include "Semigroup.h"

namespace omalg {

  class OmegaSemigroup {
  private:
  	Semigroup sPlus;
  	int numberOfOmegaElements;
	std::vector<std::string> omegaElementNames;
	std::vector<std::vector<int> > mixedProductTable;
	std::vector<int> omegaIterationTable;
  };

}

#endif