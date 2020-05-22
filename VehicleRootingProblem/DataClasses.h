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
#include <exception>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <tuple>
#include <vector>

const double INF = 2000'000'000.0;
const double EPS = 1e-8;

typedef std::vector<std::vector<double> > MatrixDouble;
typedef std::vector<std::vector<int> > MatrixInt;

enum class EProblemSolutionCtorType { 
	CHECK_PRESENCE, 
	SKIP_PRESENCE 
};

enum class ETargetPathsChange {
	ENABLE,
	DISABLE
};


class ChangeVisitedVertexException : public std::exception {};
class NoValidSolutionException : public std::exception {};
class NoSuchTargetException : public std::exception {};

class InputData {
public:
	InputData();
	InputData(const InputData&) = default;
	InputData(InputData&&) = default;

	InputData& operator = (const InputData&) = default;
	InputData& operator = (InputData&&) = default;
	InputData(int dronsCnt, int targetsCnt, double maxDist, 
		std::vector<std::pair<double, double>> points, 
		std::vector<std::pair<double, double>> timeWindows);
	InputData(std::string inputFileName);
	~InputData();
public:
	double Distance(int i, int j);

public:
	int DronsCnt;
	int TargetsCnt;
	double MaxDist;
	int UnloadingTime;
	// Including depot (it's always {0, 0})
	std::vector<std::pair<double, double>> Points;
	std::vector<std::vector<double>> Distances;
	// Including depot (it's always {-INF, INF})
	std::vector<std::pair<double, double>> TimeWindows;
	std::string FileName;
};

class ProblemSolution {
public:
	ProblemSolution();
	ProblemSolution(InputData& input, MatrixInt paths, 
		EProblemSolutionCtorType type = EProblemSolutionCtorType::CHECK_PRESENCE);

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
	std::map<int, std::pair<double, double>> BrokenVehiclesCoords;
};