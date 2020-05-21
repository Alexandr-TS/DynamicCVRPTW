#pragma once

#include "DataClasses.h"

struct DistanceToChange {
	int first_vertex;
	int second_vertex;
	double new_distance;
};

class EventsHandler {
public:
	static bool UpdateOnRemoveTarget(
		ProblemSolution& solution, int target_id, double cur_time);
	static bool UpdateOnTimeWindowUpdate(
		ProblemSolution& solution, int target_id, double cur_time, double new_start, double new_end, ETargetPathsChange target_paths_change);
	static bool UpdateOnCoordinatesUpdate(
		ProblemSolution& solution, int target_id, double cur_time, double new_x, double new_y, ETargetPathsChange target_paths_change);
	static bool UpdateOnDistMatrixUpdate(
		ProblemSolution& solution, double cur_time, const std::vector<DistanceToChange>& upd_dists, ETargetPathsChange target_paths_change);
};

