#include "Utils.h"

using namespace std;

static std::mt19937 randGen;

int Math::GenInt(int l, int r) {
	std::uniform_int_distribution<int> uid(l, r);
	return uid(randGen);
}

double Math::GenDouble(double l, double r) {
	std::uniform_real_distribution<double> urd(l, r);
	return urd(randGen);
}

double Math::PowInt(double x, int alpha) {
	double ans = 1;
	for (int i = 0; i < alpha; ++i) {
		ans *= x;
	}
	return ans;
}

pair<double, double> GetVehicleCoords(ProblemSolution& problem_solution, int path_id, double time) {
	int cur_vertex = 0;
	if (path_id >= problem_solution.Paths.size() || path_id < 0) {
		throw runtime_error("Path index out of range");
	}
	auto path = problem_solution.Paths[path_id];
	if (path.empty()) {
		return problem_solution.Input.Points[0];
	}
	auto arrival_times = problem_solution.ArrivalTimes[path_id];
	arrival_times.push_back(arrival_times.back() + problem_solution.Input.Distance(path.back(), 0));
	arrival_times.insert(arrival_times.begin(), arrival_times[0] - problem_solution.Input.Distance(0, path[0]));
	path.push_back(0);
	path.insert(path.begin(), 0);
	if (time <= arrival_times[0] || time >= arrival_times.back()) {
		// still in depot or already in depot
		return problem_solution.Input.Points[0];
	}

	for (size_t i = 0; i < path.size(); ++i) {
		if (arrival_times[i] <= time && time <= arrival_times[i + 1]) {
			double real_dist = problem_solution.Input.Distance(path[i], path[i + 1]);
			// We add unloading time only for (v, u) where v!=0
			if (i) { 
				real_dist -= problem_solution.Input.UnloadingTime;
			}
			// time to stay in path[i]. Then move to path[i + 1]
			double unloading_and_wait_time = arrival_times[i + 1] - arrival_times[i] - real_dist;
			double time_leave_i = arrival_times[i] + unloading_and_wait_time;
			if (time <= time_leave_i) {
				return problem_solution.Input.Points[path[i]];
			}
			double coef = (time - time_leave_i) / (arrival_times[i + 1] - time_leave_i);

			auto point_1 = problem_solution.Input.Points[path[i]];
			auto point_2 = problem_solution.Input.Points[path[i + 1]];
			return {
				coef * point_2.first + (1 - coef) * point_1.first,
				coef * point_2.second + (1 - coef) * point_1.second
			};
		}
	}
	assert(false);
}
