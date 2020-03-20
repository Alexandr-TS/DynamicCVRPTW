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
		int i = static_cast<int>(it - path.begin());
		if (!i && cur_time > 0) {
			throw ChangeVisitedVertexException();
		}
		if (i && solution.ArrivalTimes[path_id][i - 1] < cur_time) {
			throw ChangeVisitedVertexException();
		}
		path.erase(it);
		bool improved = true;
		while (improved) {
			improved = false;
			improved |= OptSingleStringExchange(path, solution.Input, cur_time);
			improved |= OptSingleStringRelocation(path, solution.Input, cur_time);
		}
		return true;
	}
	throw NoSuchTargetException();
}

bool EventsHandler::UpdateOnTimeWindowUpdate(ProblemSolution& solution, 
	int target_id, double cur_time, double new_start, double new_end) {
	int path_id = -1;
	for (auto& path : solution.Paths) {
		path_id++;
		auto it = find(path.begin(), path.end(), target_id);
		if (it == path.end()) {
			continue;
		}
		int i = static_cast<int>(it - path.begin());
		if (!i && cur_time > 0) {
			throw ChangeVisitedVertexException();
		}
		if (i && solution.ArrivalTimes[path_id][i - 1] < cur_time) {
			throw ChangeVisitedVertexException();
		}
		auto new_solution = solution;
		new_solution.Input.TimeWindows[path[i]] = { new_start, new_end };

		MultiOptimization(new_solution.Paths, new_solution.Input, cur_time);

		new_solution = ProblemSolution(new_solution.Input, 
			new_solution.Paths, EProblemSolutionCtorType::SKIP_PRESENCE);
		if (new_solution.SolutionExists) {
			solution = new_solution;
			return true;
		}
		throw NoValidSolutionException();
	}
	throw NoSuchTargetException();
}

bool EventsHandler::UpdateOnCoordinatesUpdate(ProblemSolution& solution, int target_id, 
	double cur_time, double new_x, double new_y) {
	int path_id = -1;
	for (auto& path : solution.Paths) {
		path_id++;
		auto it = find(path.begin(), path.end(), target_id);
		if (it == path.end()) {
			continue;
		}
		int i = static_cast<int>(it - path.begin());
		if (!i && cur_time > 0) {
			throw ChangeVisitedVertexException();
		}
		if (i && solution.ArrivalTimes[path_id][i - 1] < cur_time) {
			throw ChangeVisitedVertexException();
		}
		auto new_solution = solution;
		new_solution.Input.Points[path[i]] = { new_x, new_y };

		MultiOptimization(new_solution.Paths, new_solution.Input, cur_time);

		new_solution = ProblemSolution(new_solution.Input, new_solution.Paths, 
			EProblemSolutionCtorType::SKIP_PRESENCE);
		if (new_solution.SolutionExists) {
			solution = new_solution;
			return true;
		}
		throw NoValidSolutionException();
	}
	throw NoSuchTargetException();
}
