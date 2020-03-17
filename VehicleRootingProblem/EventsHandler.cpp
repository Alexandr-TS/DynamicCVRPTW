#include "EventsHandler.h"
#include "Optimizations.h"
#include <algorithm>

using namespace std;

bool EventsHandler::UpdateOnRemoveTarget(ProblemSolution& solution, int target_id, double cur_time) {
	int path_id = -1;
	for (auto& path : solution.Paths) {
		path_id++;
		auto it = find(path.begin(), path.end(), target_id);
		if (it == path.end()) {
			continue;
		}
		int i = it - path.begin();
		if (!i && cur_time > 0) {
			return false;
		}
		if (i && solution.ArrivalTimes[path_id][i - 1] < cur_time) {
			return false;
		}
		path.erase(it);
		OptSingleStringExchange(path, solution.Input, cur_time);
		OptSingleStringRelocation(path, solution.Input, cur_time);
		return true;
	}
	return false;
}

bool EventsHandler::UpdateOnTimeWindowUpdate(ProblemSolution& solution, int target_id, double cur_time, double new_start, double new_end) {
	int path_id = -1;
	for (auto& path : solution.Paths) {
		path_id++;
		auto it = find(path.begin(), path.end(), target_id);
		if (it == path.end()) {
			continue;
		}
		int i = it - path.begin();
		if (!i && cur_time > 0) {
			return false;
		}
		if (i && solution.ArrivalTimes[path_id][i - 1] < cur_time) {
			return false;
		}
		solution.Input.TimeWindows[path[i]] = { new_start, new_end };
		OptSingleStringExchange(path, solution.Input, cur_time);
		OptSingleStringRelocation(path, solution.Input, cur_time);
		OptStringCross(solution.Paths, solution.Input, cur_time);
		OptStringExchange(solution.Paths, solution.Input, cur_time);
		ProblemSolution new_solution = ProblemSolution(solution.Input, solution.Paths);
		if (new_solution.SolutionExists) {
			solution = new_solution;
			return true;
		}
		return false;
	}
	return false;
}

bool EventsHandler::UpdateOnCoordinatesUpdate(ProblemSolution& solution, int target_id, double cur_time, double new_x, double new_y) {
	int path_id = -1;
	for (auto& path : solution.Paths) {
		path_id++;
		auto it = find(path.begin(), path.end(), target_id);
		if (it == path.end()) {
			continue;
		}
		int i = it - path.begin();
		if (!i && cur_time > 0) {
			return false;
		}
		if (i && solution.ArrivalTimes[path_id][i - 1] < cur_time) {
			return false;
		}
		solution.Input.Points[path[i]] = { new_x, new_y };
		OptSingleStringExchange(path, solution.Input, cur_time);
		OptSingleStringRelocation(path, solution.Input, cur_time);
		OptStringCross(solution.Paths, solution.Input, cur_time);
		OptStringExchange(solution.Paths, solution.Input, cur_time);
		ProblemSolution new_solution = ProblemSolution(solution.Input, solution.Paths);
		if (new_solution.SolutionExists) {
			solution = new_solution;
			return true;
		}
		return false;
	}
	return false;

}
