#pragma once

#include "DataClasses.h"
#include "Utils.h"

void DrawPaths(System::Drawing::Graphics^ graphics, ProblemSolution solution, 
	int height, int width, int cur_time, bool update_only_progress);