#pragma once

#include "Utils.h"

class SolverClarkeWright: public Solver {
public:
	static ProblemSolution Run(InputData& input, ProblemMode problemMode, std::vector<double> args);
	static ProblemSolution RunMinSum(InputData& input, ProblemMode problemMode, std::vector<double> args);
	static ProblemSolution RunMinMaxLen(InputData& input, std::vector<double> args);
};

