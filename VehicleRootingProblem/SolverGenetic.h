#pragma once
#include "Utils.h"

std::vector<std::vector<int>> SplitPaths(std::vector<int> path, InputData& input);

class SolverGenetic : public Solver {
public:
	static ProblemSolution Run(InputData input, std::vector<double> args);
};

