#pragma once

#include "Utils.h"

class SolverAntColony : public Solver {
public:
	static ProblemSolution Run(InputData input, double timeLimit, ProblemMode problemMode, std::vector<double> args);
};


