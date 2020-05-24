#include "SolverGreedy.h"

using namespace std;

ProblemSolution RunMinSum(InputData input) {
	input.DronsCnt = min(input.DronsCnt, input.TargetsCnt);
	auto init_input = input;

	int n = input.TargetsCnt;
	int drons = input.DronsCnt;

	vector<int> point_index_of_vehicle(input.DronsCnt, 0);
	vector<double> done_distance(input.DronsCnt, 0);
	vector<double> current_vehicle_time(input.DronsCnt, 0);

	MatrixInt paths(drons);
	MatrixDouble times(drons);

	assert(n + 1 == input.TimeWindows.size());
	assert(n + 1 == input.Points.size());
	vector<int> targets_permutation;
	for (int i = 0; i < n + 1; ++i) {
		targets_permutation.push_back(i);
	}

	for (int i = 0; i < n - 1; ++i) {
		for (int j = 1; j < n - i; ++j) {
			if (input.TimeWindows[j].first > input.TimeWindows[j + 1].first) {
				swap(targets_permutation[j], targets_permutation[j + 1]);
				swap(input.TimeWindows[j], input.TimeWindows[j + 1]);
				swap(input.Points[j], input.Points[j + 1]);
			}
		}
	}

	for (int target_index = 1; target_index < input.TargetsCnt + 1; ++target_index) {
		int dron_index = -1;
		double min_distance = numeric_limits<double>::max();

		for (int i = 0; i < drons; ++i) {
			bool works = true;
			// cannot get in time
			if (current_vehicle_time[i] + input.Distance(point_index_of_vehicle[i], target_index) > 
				input.TimeWindows[target_index].second) {
				works = false;
			}
			// path is too long 
			if (done_distance[i] + input.Distance(point_index_of_vehicle[i], target_index) + 
				input.Distance(target_index, 0) > input.MaxDist) {
				works = false;
			}
			if (!works) {
				continue;
			}

			double curDistance = input.Distance(point_index_of_vehicle[i], target_index);
			if (min_distance > curDistance) {
				min_distance = curDistance;
				dron_index = i;
			}
		}
		
		if (dron_index == -1) {
			return ProblemSolution(init_input, paths);
		}

		done_distance[dron_index] += input.Distance(point_index_of_vehicle[dron_index], target_index);
		current_vehicle_time[dron_index] = max(current_vehicle_time[dron_index] + 
			input.Distance(point_index_of_vehicle[dron_index], target_index), 
			input.TimeWindows[target_index].first);
		times[dron_index].push_back(current_vehicle_time[dron_index]);
		point_index_of_vehicle[dron_index] = target_index;
		paths[dron_index].push_back(targets_permutation[target_index]);
	}

	return ProblemSolution(init_input, paths);
}


ProblemSolution SolverGreedy::Run(InputData input, ProblemMode problemMode, vector<double> args) {
	if (problemMode == ProblemMode::MINSUM) {
		return RunMinSum(input);
	}
	else {
		// times when all drons should return to depot
		double tl = 0, tr = 0;
		for (int i = 1; i < (int)input.TimeWindows.size(); ++i) {
			tr = max(tr, input.TimeWindows[i].second + input.Distance(i, 0));
		}
		auto answer_solution = ProblemSolution();
		auto input_copy = input;
		for (int iter = 0; iter < 80; ++iter) {
			double tm = (tl + tr) / 2;
			bool works = true;
			for (int i = 1; i < (int)input.TimeWindows.size(); ++i) {
				input_copy.TimeWindows[i].second = 
					min(input.TimeWindows[i].second, tm - input_copy.Distance(i, 0));
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