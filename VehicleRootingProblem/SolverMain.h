#pragma once

#include "Utils.h"

class SolverMain: public Solver {
public:
	static ProblemSolution Run(InputData input, double timeLimit, ProblemMode problemMode);
};

