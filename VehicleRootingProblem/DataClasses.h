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
	InputData(int dronsCnt, int targetsCnt, double maxDist, MatrixDouble distanceMatrix, 
		std::vector<std::pair<double, double>> timeWindows);
	InputData(int dronsCnt, int targetsCnt, double maxDist, MatrixDouble distanceMatrix, 
		std::vector<std::pair<double, double>> points, std::vector<std::pair<double, double>> timeWindows);
	InputData(std::string inputFileName, bool isMatrix = true);
	~InputData();
public:
	double Distance(int i, int j);

public:
	int DronsCnt;
	int TargetsCnt;
	double MaxDist;
	MatrixDouble DistanceMatrix;
	std::vector<std::pair<double, double>> Points;
	std::vector<std::pair<double, double>> TimeWindows;
};

class ProblemSolution {
public:
	ProblemSolution() = default;
	ProblemSolution(InputData& input, MatrixInt paths);

public:
	void PrintIntoFile(std::string outputFileName);

public:
	InputData Input;
	// Sequences of indices in input targets (not all locations) for each dron
	MatrixInt Paths;
	double MaxPathLength;
	double SumOfPathLengths;
	bool SolutionExists;
};