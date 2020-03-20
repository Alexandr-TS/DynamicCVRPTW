#pragma once

#include "DataClasses.h"

class EventsHandler {
public:
	static bool UpdateOnRemoveTarget(
		ProblemSolution& solution, int target_id, double cur_time);
	static bool UpdateOnTimeWindowUpdate(
		ProblemSolution& solution, int target_id, double cur_time, double new_start, double new_end);
	static bool UpdateOnCoordinatesUpdate(
		ProblemSolution& solution, int target_id, double cur_time, double new_x, double new_y);
};

