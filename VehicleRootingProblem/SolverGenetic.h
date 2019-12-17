#pragma once
#include "Utils.h"

class SolverGenetic : public Solver {
public:
	static ProblemSolution Run(InputData input, std::vector<double> args);
};

