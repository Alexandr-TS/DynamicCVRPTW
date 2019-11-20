#include "DataClasses.h"

InputData::InputData() {
	DronsCnt = TargetsCnt = 0;
	MaxDist = 0;
}

InputData::InputData(int dronsCnt, int targetsCnt, double maxDist, 
		std::vector<std::pair<double, double>> timeWindows)
	: DronsCnt(dronsCnt)
	, TargetsCnt(targetsCnt)
	, MaxDist(maxDist)
	, TimeWindows(timeWindows)
{
}

InputData::InputData(int dronsCnt, int targetsCnt, double maxDist,
	std::vector<std::pair<double, double> > points, std::vector<std::pair<double, double>> timeWindows)
	: DronsCnt(dronsCnt)
	, TargetsCnt(targetsCnt)
	, MaxDist(maxDist)
	, Points(points)
	, TimeWindows(timeWindows)
{
}

InputData::InputData(std::string inputFileName) {
	std::ifstream fin(inputFileName.c_str(), std::ios::in);
	try {
		fin >> DronsCnt >> TargetsCnt;
		fin >> MaxDist;
		Points.resize(TargetsCnt + 1);
		Points[0] = { 0, 0 };
		for (int i = 0; i < TargetsCnt; i++) {
			fin >> Points[i + 1].first >> Points[i + 1].second;
		}
		TimeWindows.resize(TargetsCnt + 1);
		TimeWindows[0] = { -INF, INF };
		for (int i = 0; i < TargetsCnt; i++) {
			fin >> TimeWindows[i + 1].first >> TimeWindows[i + 1].second;
		}
	}
	catch(int error) {
		DronsCnt = TargetsCnt = MaxDist = 0;
	}
	fin.close();
}

InputData::~InputData() {
}

inline double InputData::Distance(const int i, const int j) {
	return hypot(Points[i].first - Points[j].first, Points[i].second - Points[j].second);
}

ProblemSolution::ProblemSolution() {
	Input = InputData();
	Paths = {};
	ArrivalTimes = {};
	SolutionExists = false;
	MaxPathLength = 0;
	SumOfPathLengths = 0;
}


// paths: {{1, 2, 3}, {6, 5, 4, 7}}. without 0. 0 is depot
ProblemSolution::ProblemSolution(InputData& input, MatrixInt paths, MatrixDouble arrivalTimes)
	: Input(input)
	, Paths(paths)
	, ArrivalTimes(arrivalTimes)
{
	MaxPathLength = 0;
	SumOfPathLengths = 0;

	// Check that solution is valid
	SolutionExists = true;

	std::vector<int> used(input.TargetsCnt, 0);

	assert(paths.size() == arrivalTimes.size());

	for (int pathInd = 0; pathInd < (int)paths.size(); ++pathInd) {
		auto path = paths[pathInd];
		if (path.size() == 0)
			continue;
		double currentLength = input.Distance(path.back(), 0);
		int lastIndex = 0;

		for (int i = 0; i < (int)path.size(); ++i) {
			auto index = path[i];
			assert(path.size() == arrivalTimes[pathInd].size());
			if (input.TimeWindows[index].first - EPS > arrivalTimes[pathInd][i] || 
				input.TimeWindows[index].second + EPS < arrivalTimes[pathInd][i]) {
				SolutionExists = false;
			}
			assert(index > 0 && index <= input.TargetsCnt);
			currentLength += input.Distance(lastIndex, index);
			lastIndex = index;
			used[index - 1]++;
		}

		MaxPathLength = std::max(MaxPathLength, currentLength);
		SumOfPathLengths += currentLength;
	}

	for (size_t i = 0; i < used.size(); i++)
		if (used[i] != 1)
			SolutionExists = false;

	if (MaxPathLength - EPS > input.MaxDist || paths.size() > input.DronsCnt) {
		SolutionExists = false;
	}
}

void ProblemSolution::PrintIntoFile(std::string outputFileName) {
	std::ofstream fout(outputFileName.c_str(), std::ios::out);

	if (!SolutionExists) {
		fout << "Solution not found" << std::endl;
		fout.close();
		return;
	}

	fout << "Number of drons: " << Input.DronsCnt << ", Number of targets: " << Input.TargetsCnt << std::endl;
	fout << "Max distance of drone: " << Input.MaxDist << std::endl;	fout << std::endl;
	fout << "Max Path Length: " << MaxPathLength << std::endl;
	fout << "Sum of lengths: " << SumOfPathLengths << std::endl;
	fout << "Paths: " << std::endl;
	for (const auto& path: Paths) {
		for (const auto& nums : path) {
			fout << nums << " ";
		}
		fout << std::endl;
	}
	fout << std::endl;
	fout << "Arrival times: " << std::endl;
	for (const auto& path: ArrivalTimes) {
		for (const auto& nums : path) {
			fout << nums << " ";
		}
		fout << std::endl;
	}
	fout.close();
}