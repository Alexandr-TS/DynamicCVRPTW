#pragma once

#include "Utils.h"

enum EAlgorithms { DP, ClarkeWright, AntColony, Genetic, Greedy };

class SolverMain: public Solver {
public:
	static ProblemSolution Run(InputData input, ProblemMode problemMode, 
		EAlgorithms algorithm, std::vector<double> args);
};

