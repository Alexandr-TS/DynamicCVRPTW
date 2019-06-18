#include "DataClasses.h"

InputData::InputData()
{
	DronsCnt = TargetsCnt = 0;
	MaxDist = 0;
}

InputData::InputData(int dronsCnt, int targetsCnt, double maxDist, MatrixDouble distanceMatrix)
	: DronsCnt(dronsCnt)
	, TargetsCnt(targetsCnt)
	, MaxDist(maxDist)
	, DistanceMatrix(distanceMatrix)
{
}

InputData::InputData(int dronsCnt, int targetsCnt, double maxDist, MatrixDouble distanceMatrix, std::vector<std::pair<double, double> > points)
	: DronsCnt(dronsCnt)
	, TargetsCnt(targetsCnt)
	, MaxDist(maxDist)
	, DistanceMatrix(distanceMatrix)
	, Points(points)
{
}

InputData::InputData(std::string inputFileName, bool isMatrix) {
	std::ifstream fin(inputFileName.c_str(), std::ios::in);
	try {
		fin >> DronsCnt >> TargetsCnt;
		fin >> MaxDist;

		DistanceMatrix.resize(1 + TargetsCnt, std::vector<double>(1 + TargetsCnt));
		if (isMatrix) {
			for (int i = 0; i < 1 + TargetsCnt; ++i) {
				for (int j = 0; j < 1 + TargetsCnt; ++j) {
					fin >> DistanceMatrix[i][j];
					assert(DistanceMatrix[i][j] >= -EPS);
				}
			}
		} else {
			std::vector<std::pair<double, double> > coords(1 + TargetsCnt);
			for (int i = 0; i < 1 + TargetsCnt; ++i) {
				fin >> coords[i].first >> coords[i].second;
			}
			for (int i = 0; i < 1 + TargetsCnt; ++i)
				for (int j = 0; j < 1 + TargetsCnt; ++j)
					DistanceMatrix[i][j] = hypot(coords[i].first - coords[j].first, coords[i].second - coords[j].second);
			Points = coords;
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
	return DistanceMatrix[i][j];
}

ProblemSolution::ProblemSolution() {
	Input = InputData();
	Paths = {};
	SolutionExists = false;
	MaxPathLength = 0;
	SumOfPathLengths = 0;
}


// paths: {{1, 2, 3}, {6, 5, 4, 7}}. without 0. 0 is depot
ProblemSolution::ProblemSolution(InputData& input, MatrixInt paths)
	: Input(input)
	, Paths(paths)
{
	MaxPathLength = 0;
	SumOfPathLengths = 0;

	std::vector<int> used(input.TargetsCnt, 0);

	for (const auto& path: paths) {
		if (path.size() == 0)
			continue;
		double currentLength = input.Distance(path.back(), 0);
		int lastIndex = 0;

		for (auto index: path) {
			assert(index > 0 && index <= input.TargetsCnt);
			currentLength += input.Distance(lastIndex, index);
			lastIndex = index;
			used[index - 1]++;
		}

		MaxPathLength = std::max(MaxPathLength, currentLength);
		SumOfPathLengths += currentLength;
	}

	// Check that solution is valid
	SolutionExists = true;

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
	fout << "Max distance of drone: " << Input.MaxDist << std::endl;
	for (const auto& line : Input.DistanceMatrix) {
		for (auto number: line) {
			fout << std::setw(8) << std::fixed << std::setprecision(2) << number << " ";
		}
		fout << std::endl;
	}
	fout << std::endl;
	fout << "Max Path Length: " << MaxPathLength << std::endl;
	fout << "Sum of lengths: " << SumOfPathLengths << std::endl;
	for (const auto& path: Paths) {
		for (const auto& nums : path) {
			fout << nums << " ";
		}
		fout << std::endl;
	}
	fout.close();
}