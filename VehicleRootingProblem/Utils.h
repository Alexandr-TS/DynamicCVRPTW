#pragma once

#include "DataClasses.h"

#include <random>

class DataGenerator {
public:
	DataGenerator() = default;
public:
	InputData GenInputData(int minDronsCnt, int maxDronsCnt, int minTargetsCnt,
		int maxTargetsCnt, double minMaxDist, double maxMaxDist, double minCoordinate, double maxCoordinate);
public:
	static int GenInt(int l, int r);
	static double GenDouble(double l, double r);
	static double PowInt(double x, int alpha);
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