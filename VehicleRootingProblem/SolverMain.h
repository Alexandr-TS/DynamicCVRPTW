#pragma once

#include "Utils.h"

enum EAlgorithms { DP, ClarkeWright, AntColony };

class SolverMain: public Solver {
public:
	static ProblemSolution Run(InputData input, double timeLimit, ProblemMode problemMode, EAlgorithms algorithm);
	static ProblemSolution Run(InputData input, double timeLimit, ProblemMode problemMode, EAlgorithms algorithm, std::vector<double> args);
};

