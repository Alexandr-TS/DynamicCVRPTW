#pragma once

#include "DataClasses.h"

#include <random>

enum class ProblemMode {
	MINSUM,
	MINMAXLEN
};

class Solver {
public:
	Solver() = default;

public:
	static ProblemSolution Run(InputData input, ProblemMode problemMode) {}
};