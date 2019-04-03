#pragma once

#include "Utils.h"

class SolverClarkeWright: public Solver {
public:
	static ProblemSolution Run(InputData& input, double timeLimit, ProblemMode problemMode);
	static ProblemSolution RunMinSum(InputData& input, double timeLimit, ProblemMode problemMode);
	static ProblemSolution RunMinMaxLen(InputData& input, double timeLimit);
};

