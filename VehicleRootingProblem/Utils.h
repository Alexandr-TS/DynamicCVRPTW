#pragma once

#include "DataClasses.h"

#include <random>


class Math {
private:
public:
	static int GenInt(int l, int r);
	static double GenDouble(double l, double r);
	static double PowInt(double x, int p);
};

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
