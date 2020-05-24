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
	static ProblemSolution Run(InputData input) {}
};

std::pair<double, double> GetVehicleCoords(ProblemSolution& problem_solution, int path_id, double cur_time);


double PointDistance(std::pair<double, double> p1, std::pair<double, double> p2);
