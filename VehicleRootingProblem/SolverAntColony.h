#pragma once

#include "Utils.h"

class SolverAntColony : public Solver {
public:
	static ProblemSolution Run(InputData input, std::vector<double> args);
};
