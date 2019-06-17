#pragma once

#include "DataClasses.h"
#include "Utils.h"

ProblemSolution SwapOptimization(ProblemSolution solution, ProblemMode problemMode);
ProblemSolution Edges3Optimization(ProblemSolution solution, ProblemMode problemMode);
ProblemSolution ReverseOptimization(ProblemSolution solution, ProblemMode problemMode);
void LocalReverseOptimization(std::vector<int>& path, InputData& input, bool pathHasFirst0); 
void StringCrossOptimization(std::vector<std::vector<int>>& paths, InputData& input, ProblemMode problemMode);