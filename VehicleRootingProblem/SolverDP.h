#pragma once

#include "Utils.h"

class SolverDP: public Solver {
public:
	static ProblemSolution Run(InputData input, double timeLimit, ProblemMode problemMode);
};

