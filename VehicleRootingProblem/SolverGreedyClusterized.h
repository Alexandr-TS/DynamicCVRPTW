#pragma once

#include "Utils.h"

class SolverGreedyClusterized: public Solver {
public:
	static ProblemSolution Run(InputData input, ProblemMode problemMode, std::vector<double> args);
};

