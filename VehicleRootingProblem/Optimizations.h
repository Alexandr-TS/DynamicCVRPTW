#pragma once

#include "DataClasses.h"
#include "Utils.h"

bool LocalReverseOptimization(std::vector<int>& path, InputData& input, bool pathHasFirst0); 
bool StringCrossOptimization(std::vector<std::vector<int>>& paths, InputData& input, 
	ProblemMode problemMode, bool pathsHaveFirst0 = true);

bool GlobalSwapOptimization(std::vector<std::vector<int>>& paths, InputData& input);