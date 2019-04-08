#pragma once

#include "Utils.h"

enum EAlgorithms { DP, ClarkeWright };

class SolverMain: public Solver {
public:
	static ProblemSolution Run(InputData input, double timeLimit, ProblemMode problemMode, EAlgorithms algorithm);
};

