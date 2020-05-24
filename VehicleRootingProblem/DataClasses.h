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

typedef std::vector<std::vector<double>> MatrixDouble;
typedef std::vector<std::vector<int>> MatrixInt;

const double EPS = 1e-8;
const double INF = static_cast<double>(2e9);

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

struct InputData {
	InputData();

	InputData(int dronsCnt, int targetsCnt, double maxDist, 
		std::vector<std::pair<double, double>> points, 
		std::vector<std::pair<double, double>> timeWindows);
	InputData(std::string inputFileName);

	double Distance(int i, int j);

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

struct ProblemSolution {
	ProblemSolution();
	ProblemSolution(InputData& input, MatrixInt paths, 
		EProblemSolutionCtorType type = EProblemSolutionCtorType::CHECK_PRESENCE, 
		std::map<int, double> broken_vehicle_time_by_id = {});

	void PrintIntoFile(std::string outputFileName);

	InputData Input;
	// Sequences of indices in input targets (not all locations) for each dron
	MatrixInt Paths;
	// Times when the dron reaches targets
	MatrixDouble ArrivalTimes;
	double MaxPathLength;
	double SumOfPathLengths;
	bool SolutionExists;
	// Numeration from 0 - not like in the UI
	std::map<int, double> BrokenVehicleTimeById;
};