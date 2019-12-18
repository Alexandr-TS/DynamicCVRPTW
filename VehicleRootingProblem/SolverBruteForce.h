#pragma once

#include "Utils.h"

class SolverBruteForce: public Solver {
public:
	static ProblemSolution Run(InputData input, std::vector<double> args);
};

