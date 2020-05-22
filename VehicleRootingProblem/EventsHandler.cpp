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
	int target_id, double cur_time, double new_start, double new_end, ETargetPathsChange target_paths_change) {
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

		MultiOptimization(new_solution.Paths, new_solution.Input, cur_time, target_paths_change);

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
	double cur_time, double new_x, double new_y, ETargetPathsChange target_paths_change) {
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

		/* use for test mode to change distances after changing coordinates
		{
			unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
			mt19937 gen(seed);
			int basic_speed = 60;
			int add_minutes = 3;
			exponential_distribution<> e_d_gen(1.5);
			for (size_t j = 0; j < new_solution.Input.Distances.size(); ++j) {
				double dist_m = hypot(new_solution.Input.Points[path[i]].first - new_solution.Input.Points[j].first,
					new_solution.Input.Points[path[i]].second - new_solution.Input.Points[j].second);
				double dist_minutes = dist_m / 1000. * 60.0 / (basic_speed / (1.0 + e_d_gen(gen))) + (path[i] != j ? add_minutes : 0);
				new_solution.Input.Distances[path[i]][j] = static_cast<int>(dist_minutes * 100) / 100.;
				dist_minutes = dist_m / 1000. * 60.0 / (basic_speed / (1.0 + e_d_gen(gen))) + (path[i] != j ? add_minutes : 0);
				new_solution.Input.Distances[j][path[i]] = static_cast<int>(dist_minutes * 100) / 100.;
			}

			for (size_t k = 0; k < new_solution.Input.Distances.size(); ++k) {
				for (size_t i = 0; i < new_solution.Input.Distances.size(); ++i) {
					for (size_t j = 0; j < new_solution.Input.Distances.size(); ++j) {
						new_solution.Input.Distances[i][j] = min(
							new_solution.Input.Distances[i][j], 
							new_solution.Input.Distances[i][k] + new_solution.Input.Distances[k][j]
						);
					}
				}
			}
		}
		*/

		MultiOptimization(new_solution.Paths, new_solution.Input, cur_time, target_paths_change);

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

bool EventsHandler::UpdateOnDistMatrixUpdate(ProblemSolution& solution, double cur_time,
	const vector<DistanceToChange>& upd_dists, ETargetPathsChange target_paths_change) {

	auto new_solution = solution;

	for (const auto& el : upd_dists) {
		new_solution.Input.Distances[el.first_vertex][el.second_vertex] = el.new_distance;
	}

	MultiOptimization(new_solution.Paths, new_solution.Input, cur_time, target_paths_change);

	new_solution = ProblemSolution(new_solution.Input, new_solution.Paths, 
		EProblemSolutionCtorType::SKIP_PRESENCE);
	if (new_solution.SolutionExists) {
		solution = new_solution;
		return true;
	}
	throw NoValidSolutionException();
}


bool EventsHandler::UpdateOnVehicleBreakdown(ProblemSolution& solution, int vehicle_id, 
	double cur_time, ETargetPathsChange target_paths_change) {

	auto& path = solution.Paths[vehicle_id];
	vector<int> not_visited_vertices;
	size_t i = 0;
	for (; i < path.size(); ++i) {
		if (solution.ArrivalTimes[vehicle_id][i] > cur_time) {
			break;
		}
	}
	for (; i < path.size(); ++i) {
		not_visited_vertices.push_back(path[i]);
	}

	if (target_paths_change == ETargetPathsChange::ENABLE) {
		// dummy node
		not_visited_vertices.insert(not_visited_vertices.begin(), solution.Input.TargetsCnt + 1);

		auto new_solution = solution;
		new_solution.Input.TargetsCnt++;
		new_solution.Input.Points.push_back(new_solution.Input.Points[0]);
		new_solution.Input.TimeWindows.push_back({ cur_time, cur_time + EPS });
		new_solution.Paths.push_back(not_visited_vertices);
		for (size_t i = 0; i + 1 < not_visited_vertices.size(); ++i) {
			new_solution.Paths[vehicle_id].pop_back();
		}

		new_solution.Input.Distances.push_back(new_solution.Input.Distances[0]);
		for (size_t i = 0; i < new_solution.Input.Distances.size(); ++i) {
			new_solution.Input.Distances[i].push_back(new_solution.Input.Distances[i][0]);
		}

		//new_solution.BrokenVehiclesCoords[vehicle_id] = broken_coords;

		MultiOptimization(new_solution.Paths, new_solution.Input, cur_time, target_paths_change);

		new_solution = ProblemSolution(new_solution.Input,
			new_solution.Paths, EProblemSolutionCtorType::SKIP_PRESENCE);
		if (new_solution.SolutionExists) {
			solution = new_solution;
			return true;
		}
		throw NoValidSolutionException();
	}
	else {
		throw runtime_error("not implemented yet");
	}
}
