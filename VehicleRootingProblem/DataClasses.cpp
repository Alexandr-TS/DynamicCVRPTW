#include "DataClasses.h"

using namespace std;

InputData::InputData() 
	: DronsCnt(0)
	, TargetsCnt(0)
	, MaxDist(0)
	, UnloadingTime(0)
{
}

InputData::InputData(int dronsCnt, int targetsCnt, double maxDist,
	vector<pair<double, double> > points, vector<pair<double, double>> timeWindows)
	: DronsCnt(dronsCnt)
	, TargetsCnt(targetsCnt)
	, MaxDist(maxDist)
	, Points(points)
	, TimeWindows(timeWindows)
	, UnloadingTime(0)
{
	Distances.resize(TargetsCnt + 1);
	for (int i = 0; i <= TargetsCnt; ++i) {
		Distances[i].resize(TargetsCnt + 1);
		for (int j = 0; j <= TargetsCnt; ++j) {
			Distances[i][j] = hypot(points[i].first - points[j].first, points[i].second - points[j].second);
		}
	}
}

InputData::InputData(string inputFileName) {
	FileName = inputFileName;
	ifstream fin(inputFileName.c_str(), ios::in);
	try {
		fin >> DronsCnt >> TargetsCnt;
		fin >> MaxDist;
		fin >> UnloadingTime;
		Points.resize(TargetsCnt + 1);
		Points[0] = { 0, 0 };
		for (int i = 0; i < TargetsCnt; i++) {
			fin >> Points[i + 1].first >> Points[i + 1].second;
		}

		Distances.resize(TargetsCnt + 1);
		for (int i = 0; i <= TargetsCnt; ++i) {
			Distances[i].resize(TargetsCnt + 1);
			for (int j = 0; j <= TargetsCnt; ++j) {
				fin >> Distances[i][j];
				// Adding unloading time to outgoing edge: (v, u) = length + [unloading in v]
				if (i != j && i) {
					Distances[i][j] += UnloadingTime;
				}
			}
		}

		TimeWindows.resize(TargetsCnt + 1);
		TimeWindows[0] = { -INF, INF };
		for (int i = 0; i < TargetsCnt; i++) {
			fin >> TimeWindows[i + 1].first >> TimeWindows[i + 1].second;
		}
	}
	catch(...) {
		DronsCnt = TargetsCnt = 0;
		MaxDist = 0.0;
	}
	fin.close();
}

inline double InputData::Distance(const int i, const int j) {
	return Distances[i][j];
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

ProblemSolution::ProblemSolution(InputData& input, MatrixInt paths, 
	EProblemSolutionCtorType type, map<int, double> broken_vehicle_time_by_id)
	: Input(input)
	, BrokenVehicleTimeById(broken_vehicle_time_by_id)
{
	Paths = {};
	for (auto path : paths) {
		if (!path.empty()) {
			Paths.push_back(path);
		}
	}
	MaxPathLength = 0;
	SumOfPathLengths = 0;
	ArrivalTimes = {};

	// Check that solution is valid
	SolutionExists = true;

	vector<int> used(input.TargetsCnt, 0);

	for (int pathInd = 0; pathInd < (int)Paths.size(); ++pathInd) {
		ArrivalTimes.push_back({});
		double cur_time = 0;

		auto path = Paths[pathInd];
		if (path.size() == 0) {
			continue;
		}
		double currentLength = input.Distance(path.back(), 0);
		int lastIndex = 0;

		for (int i = 0; i < (int)path.size(); ++i) {
			auto index = path[i];
			assert(index > 0 && index <= input.TargetsCnt);
			double this_dist = input.Distance(lastIndex, index);
			cur_time = max(cur_time + this_dist, input.TimeWindows[index].first);
			ArrivalTimes.back().push_back(cur_time);
			if (cur_time > input.TimeWindows[index].second + EPS) {
				SolutionExists = false;
				cout << "ProblemSolution constructor. Solution is not valid because " <<
					"of time windows" << endl;
			}
			currentLength += this_dist;
			lastIndex = index;
			used[index - 1]++;
		}

		MaxPathLength = max(MaxPathLength, currentLength);
		SumOfPathLengths += currentLength;
	}

	if (type == EProblemSolutionCtorType::CHECK_PRESENCE) {
		for (size_t i = 0; i < used.size(); i++) {
			if (used[i] != 1) {
				SolutionExists = false;
				cout << "ProblemSolution constructor. Solution is not valid because not " <<
					"all the vertices were visited" << endl;
			}
		}
	}

	if (MaxPathLength - EPS > input.MaxDist) { 
		SolutionExists = false;
		cout << "ProblemSolution constructor. Solution is not valid because one of the " << 
			"paths is longer than MaxDist" << endl;
	}
	if (static_cast<int>(Paths.size()) > input.DronsCnt) {
		SolutionExists = false;
		cout << "ProblemSolution constructor. Solution is not valid because there are " << 
			"more paths than vehicles" << endl;
	}
}


void ProblemSolution::PrintIntoFile(string outputFileName) {
	ofstream fout(outputFileName.c_str(), ios::out);

	if (!SolutionExists) {
		fout << "Solution not found" << endl;
		fout.close();
		return;
	}

	fout << "Number of drons: " << Input.DronsCnt << ", Number of targets: " << 
		Input.TargetsCnt << endl;
	fout << "Max distance of drone: " << Input.MaxDist << endl;	
	fout << endl;
	fout << "Max Path Length: " << MaxPathLength << endl;
	fout << "Sum of lengths: " << SumOfPathLengths << endl;
	fout << "Paths: " << endl;
	for (const auto& path: Paths) {
		for (const auto& nums : path) {
			fout << nums << " ";
		}
		fout << endl;
	}
	fout << endl;
	fout << "Arrival times: " << endl;
	for (const auto& path: ArrivalTimes) {
		for (const auto& nums : path) {
			fout << nums << " ";
		}
		fout << endl;
	}
	fout.close();
}