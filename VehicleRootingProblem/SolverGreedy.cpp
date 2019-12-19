#include "SolverGreedy.h"

using namespace std;

ProblemSolution RunMinSum(InputData input) {
	double startTime = (double)clock() / CLOCKS_PER_SEC;

	input.DronsCnt = std::min(input.DronsCnt, input.TargetsCnt);
	auto init_input = input;

	int n = input.TargetsCnt;
	int drons = input.DronsCnt;

	std::vector<int> pointIndexOfDron(input.DronsCnt, 0);
	std::vector<double> doneDistance(input.DronsCnt, 0);
	std::vector<double> currentDronTime(input.DronsCnt, 0);

	MatrixInt paths(drons);
	MatrixDouble times(drons);

	assert(n + 1 == input.TimeWindows.size());
	assert(n + 1 == input.Points.size());
	std::vector<int> targetsPermutation;
	for (int i = 0; i < n + 1; ++i) {
		targetsPermutation.push_back(i);
	}

	for (int i = 0; i < n - 1; i++) {
		for (int j = 1; j < n - i; j++) {
			if (input.TimeWindows[j].first > input.TimeWindows[j + 1].first) {
				swap(targetsPermutation[j], targetsPermutation[j + 1]);
				swap(input.TimeWindows[j], input.TimeWindows[j + 1]);
				swap(input.Points[j], input.Points[j + 1]);
			}
		}
	}

	for (int targetIndex = 1; targetIndex < input.TargetsCnt + 1; ++targetIndex) {
		int dronIndex = -1;
		double minDistance = std::numeric_limits<double>::max();

		for (int i = 0; i < drons; ++i) {
			bool works = true;
			// cannot get in time
			if (currentDronTime[i] + input.Distance(pointIndexOfDron[i], targetIndex) > 
				input.TimeWindows[targetIndex].second) {
				works = false;
			}
			// path is too long 
			if (doneDistance[i] + input.Distance(pointIndexOfDron[i], targetIndex) + 
				input.Distance(targetIndex, 0) > input.MaxDist) {
				works = false;
			}
			if (!works) {
				continue;
			}

			double curDistance = input.Distance(pointIndexOfDron[i], targetIndex);
			if (minDistance > curDistance) {
				minDistance = curDistance;
				dronIndex = i;
			}
		}
		
		if (dronIndex == -1) {
			return ProblemSolution(init_input, paths);
		}

		doneDistance[dronIndex] += input.Distance(pointIndexOfDron[dronIndex], targetIndex);
		currentDronTime[dronIndex] = std::max(currentDronTime[dronIndex] + 
			input.Distance(pointIndexOfDron[dronIndex], targetIndex), input.TimeWindows[targetIndex].first);
		times[dronIndex].push_back(currentDronTime[dronIndex]);
		pointIndexOfDron[dronIndex] = targetIndex;
		paths[dronIndex].push_back(targetsPermutation[targetIndex]);
	}

	return ProblemSolution(init_input, paths);
}


ProblemSolution SolverGreedy::Run(InputData input, ProblemMode problemMode, std::vector<double> args) {
	if (problemMode == ProblemMode::MINSUM) {
		return RunMinSum(input);
	}
	else {
		// times when all drons should return to depot
		double tl = 0, tr = 0;
		for (int i = 1; i < (int)input.TimeWindows.size(); ++i) {
			tr = std::max(tr, input.TimeWindows[i].second + input.Distance(i, 0));
		}
		auto answer_solution = ProblemSolution();
		auto input_copy = input;
		for (int iter = 0; iter < 80; ++iter) {
			double tm = (tl + tr) / 2;
			bool works = true;
			for (int i = 1; i < (int)input.TimeWindows.size(); i++) {
				input_copy.TimeWindows[i].second = 
					std::min(input.TimeWindows[i].second, tm - input_copy.Distance(i, 0));
				if (input_copy.TimeWindows[i].first > input_copy.TimeWindows[i].second) {
					works = false;
				}
			}
			auto solution = RunMinSum(input_copy);
			if (!solution.SolutionExists) {
				works = false;
			}
			else {
				answer_solution = solution;
			}
			if (works) {
				tr = tm;
			}
			else {
				tl = tm;
			}
		}
		return ProblemSolution(input, answer_solution.Paths);
	}
}