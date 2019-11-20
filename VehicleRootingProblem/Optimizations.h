#pragma once

#include "DataClasses.h"
#include "Utils.h"

ProblemSolution SwapOptimization(ProblemSolution solution, ProblemMode problemMode);
ProblemSolution Edges3Optimization(ProblemSolution solution, ProblemMode problemMode);
ProblemSolution ReverseOptimization(ProblemSolution solution, ProblemMode problemMode);
bool LocalReverseOptimization(std::vector<int>& path, InputData& input, bool pathHasFirst0); 
bool StringCrossOptimization(std::vector<std::vector<int>>& paths, InputData& input, 
	ProblemMode problemMode, bool pathsHaveFirst0 = true);