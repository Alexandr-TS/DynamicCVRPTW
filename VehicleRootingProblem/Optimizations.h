#pragma once

#include "DataClasses.h"
#include "Utils.h"

bool LocalReverseOptimization(std::vector<int>& path, InputData& input, bool pathHasFirst0); 
bool StringCrossOptimization(MatrixInt& paths, InputData& input, 
	ProblemMode problemMode, bool pathsHaveFirst0 = true);

bool GlobalSwapOptimization(MatrixInt& paths, InputData& input);
bool GlobalInsertOptimization(MatrixInt& paths, InputData& input);
bool LocalSwapOptimization(std::vector<int>& path, InputData& input);