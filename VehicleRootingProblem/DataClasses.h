#pragma once

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <tuple>
#include <vector>

const double EPS = 1e-8;
const double INF = 2e9;

typedef std::vector<std::vector<double> > MatrixDouble;
typedef std::vector<std::vector<int> > MatrixInt;

class InputData {
public:
	InputData();
	InputData(int dronsCnt, int targetsCnt, double maxDist, 
		std::vector<std::pair<double, double>> timeWindows);
	InputData(int dronsCnt, int targetsCnt, double maxDist, 
		std::vector<std::pair<double, double>> points, std::vector<std::pair<double, double>> timeWindows);
	InputData(std::string inputFileName);
	~InputData();
public:
	double Distance(int i, int j);

public:
	int DronsCnt;
	int TargetsCnt;
	double MaxDist;
	// Including depot (it's always {0, 0})
	std::vector<std::pair<double, double>> Points;
	// Including depot (it's always {-INF, INF})
	std::vector<std::pair<double, double>> TimeWindows;
};

class ProblemSolution {
public:
	ProblemSolution();
	ProblemSolution(InputData& input, MatrixInt paths, MatrixDouble arrivalTimes);

public:
	void PrintIntoFile(std::string outputFileName);

public:
	InputData Input;
	// Sequences of indices in input targets (not all locations) for each dron
	MatrixInt Paths;
	// Times when the dron reaches targets
	MatrixDouble ArrivalTimes;
	double MaxPathLength;
	double SumOfPathLengths;
	bool SolutionExists;
};